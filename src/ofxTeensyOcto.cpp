//
//      OF Teensy/OCTO class by Jason Walters @ BBDO ...
//      Original P5 code by Paul Stoffregen/PJRC.COM
//
//      Last revision by Jason Walters on September 19th, 2013
//      Compatible with openFrameworks 0.80
//
///////////////////////////////////////////////////////////////

#include "ofxTeensyOcto.h"

//--------------------------------------------------------------
void ofxTeensyOcto::setup(int _ledWidth, int _ledHeight){
    
    // LED variables
    ledWidth = _ledWidth;      // LED max width
    ledHeight = _ledHeight;      // LED max height
    errorCount = 0;
    numPorts = 0;       // default teensy ports
    framerate = 30.0f;
    maxPorts = 24;      // max teensy ports
    
    ledSerial = new ofSerial[maxPorts];
    ledArea = new ofRectangle[maxPorts];
    ledLayout = new bool[maxPorts];
    ledImage = new ofImage[maxPorts];
    colors = new ofColor[ledWidth*ledHeight];
    
    // let's list our serial devices
    ledSerial[numPorts].listDevices();
    vector <ofSerialDeviceInfo> deviceList = ledSerial[numPorts].getDeviceList();
    ofSleepMillis(20);
    
    if (numPorts >= maxPorts) {
        cout << "too many serial ports, please increase maxPorts" << endl;
        errorCount++;
        return;
    }
    else {
        int baud = 9600;
        ledSerial[numPorts].setup(0, baud);     // grab the first one in the list
        ledSerial[numPorts].writeByte('?');     // send an initial character
    }
    
    // setup our LED array
    ledImage[numPorts].allocate(ledWidth, ledHeight, OF_IMAGE_COLOR);
    ledArea[numPorts].set(0, 0, 100, 100);
    ledLayout[numPorts] = 0 == 0;
    numPorts++;
    
    /*
     // only store the info and increase numPorts if Teensy responds properly
     ledImage[numPorts].allocate(ofToInt(param[0]), ofToInt(param[1]), OF_IMAGE_COLOR);
     ledArea[numPorts].set(ofToInt(param[5]), ofToInt(param[6]), ofToInt(param[7]), ofToInt(param[8]));
     ledLayout[numPorts] = (ofToInt(param[5]) == 0);
     numPorts++;
     */
    
}

// translate the 24 bit color from RGB to the actual
// order used by the LED wiring.  GRB is the most common.
int colorWiring(int c) {
    //return c;
    return ((c & 0xFF0000) >> 8) | ((c & 0x00FF00) << 8) | (c & 0x0000FF); // GRB - most common wiring
}

// image2data converts an image to OctoWS2811's raw data format.
// The number of vertical pixels in the image must be a multiple
// of 8.  The data array must be the proper size for the image.
void ofxTeensyOcto::image2data(ofImage image, unsigned char * data, bool layout) {
    
    int offset = 3;
    int x, y, xbegin, xend, xinc, mask;
    int linesPerPin = image.getHeight() / 8;
    int * pixel = new int[8];
    
    // get the copied image pixels
    pix = image.getPixelsRef();
    
    // 2d array of our pixel colors
    for (int x = 0; x < ledWidth; x++)
    {
        for (int y = 0; y < ledHeight; y++)
        {
            int loc = x + y * ledWidth;
            colors[loc] = pix.getColor(x, y);
        }
    }
    
    for (y = 0; y < linesPerPin; y++) {
        
        if ((y & 1) == (layout ? 0 : 1)) {
            // even numbered rows are left to right
            xbegin = 0;
            xend = image.getWidth();
            xinc = 1;
        } else {
            // odd numbered rows are right to left
            xbegin = image.getWidth() - 1;
            xend = -1;
            xinc = -1;
        }
        
        for (x = xbegin; x != xend; x += xinc) {
            for (int i=0; i < 8; i++) {
                
                int temploc = x + (i * ledWidth);
                
                pixel[i] = colors[temploc].getHex();
                pixel[i] = colorWiring(pixel[i]);
                //cout << "x = " << x << " colors[" << temploc << "] " << colors[temploc].getHex() << endl;     // color numbers
                //cout << "x = " << x << " pixel[" << i << "] " << ofToHex(pixel[i]) << endl;                   // hex values
                //cout << "pixel[" << i << "] " << pixel[i] << endl;                                            // color numbers
            }
            
            // convert 8 pixels to 24 bytes
            for (mask = 0x800000; mask != 0; mask >>= 1) {
                unsigned char b = 0;
                for (int i=0; i < 8; i++) {
                    if ((pixel[i] & mask) != 0) b |= (1 << i);
                }
                data[offset++] = b;
                //cout << "x = " << x << " data[" << offset << "] " << (int)data[offset] << endl;   // data view
            }
        }
    }
}

// convert an integer from 0 to 100 to a float percentage
// from 0.0 to 1.0.  Special cases for 1/3, 1/6, 1/7, etc
// are handled automatically to fix integer rounding.
double percentageFloat(int percent) {
    if (percent == 33) return 1.0 / 3.0;
    if (percent == 17) return 1.0 / 6.0;
    if (percent == 14) return 1.0 / 7.0;
    if (percent == 13) return 1.0 / 8.0;
    if (percent == 11) return 1.0 / 9.0;
    if (percent ==  9) return 1.0 / 11.0;
    if (percent ==  8) return 1.0 / 12.0;
    return (double)percent / 100.0;
}

// scale a number by a percentage, from 0 to 100
int percentage(int num, int percent) {
    double mult = percentageFloat(percent);
    double output = num * mult;
    return (int)output;
}

// scale a number by the inverse of a percentage, from 0 to 100
int percentageInverse(int num, int percent) {
    double div = percentageFloat(percent);
    double output = num / div;
    return (int)output;
}

//--------------------------------------------------------------
void ofxTeensyOcto::update(){
    
    for (int i=0; i < numPorts; i++) {
        
        // copy a portion of the movie's image to the LED image
        int xoffset = percentage(ledWidth, ledArea[i].x);
        int yoffset = percentage(ledHeight, ledArea[i].y);
        int xwidth =  percentage(ledWidth, ledArea[i].getWidth());
        int yheight = percentage(ledHeight, ledArea[i].getHeight());
        
        ledImage[i].grabScreen(xoffset, yoffset, xwidth, yheight);
        
        // convert the LED image to raw data
        unsigned char *ledData = new unsigned char[((int)ledImage[i].getWidth() * (int)ledImage[i].getHeight() * 3) + 3];
        
        image2data(ledImage[i], ledData, ledLayout[i]);
        
        if (i == 0)
        {
            ledData[0] = '*';  // first Teensy is the frame sync master
            int usec = (int)((1000000.0 / framerate) * 0.75);
            ledData[1] = (unsigned char)(usec);   // request the frame sync pulse
            ledData[2] = (unsigned char)(usec >> 8); // at 75% of the frame time
        }
        else
        {
            ledData[0] = '%';  // others sync to the master board
            ledData[1] = 0;
            ledData[2] = 0;
        }
        
        int dataSize = ((ledWidth*ledHeight) * 3) + 3;
        // send the raw data to the LEDs  :-)
        for (int j = 0; j < dataSize; j++) {
            ledSerial[i].writeByte(ledData[j]);
            //cout << "ledData[" << j << "] " << (int)ledData[j] << endl;   // serial data
        }
        ledSerial[i].drain();   // this prevents massive flickering!
    }

}
