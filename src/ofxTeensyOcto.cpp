//
//      OF Teensy/OCTO class by Jason Walters @ BBDO ...
//      Original P5 code by Paul Stoffregen/PJRC.COM
//
//      Last revision by Jason Walters on September 25th, 2013
//      Compatible with openFrameworks 0.80
//
///////////////////////////////////////////////////////////////

#include "ofxTeensyOcto.h"

//--------------------------------------------------------------
void ofxTeensyOcto::setup(int _ledWidth, int _ledHeight)
{    
    // LED variables
    ledWidth = _ledWidth;        // LED max width
    ledHeight = _ledHeight;      // LED max height
    numPorts = 0;                // default teensy ports
    maxPorts = 24;               // max teensy ports
    counterShape = 0;
    
    // LED arrays
    ledSerial = new ofSerial[maxPorts];
    ledArea = new ofRectangle[maxPorts];
    ledLayout = new bool[maxPorts];
    ledImage = new ofImage[maxPorts];
    colors = new ofColor[ledWidth*ledHeight];
    
    // let's list our serial devices
    ledSerial[numPorts].listDevices();
    vector <ofSerialDeviceInfo> deviceList = ledSerial[numPorts].getDeviceList();
    ofSleepMillis(20);
}

//--------------------------------------------------------------
void ofxTeensyOcto::serialConfigure(string portName, int _ledWidth, int _ledHeight, int _xoffset, int _yoffset, int _portWidth, int _portHeight, int _direction)
{
    int baud = 9600;
    ledSerial[numPorts].setup(portName, baud);
    ledSerial[numPorts].writeByte('?');         // send an initial character
    ofSleepMillis(50);
    
    // only store the info and increase numPorts if Teensy responds properly
    ledImage[numPorts].allocate(_ledWidth, _ledHeight, OF_IMAGE_COLOR);
    ledArea[numPorts].set(_xoffset, _yoffset, _portWidth, _portHeight);
    ledLayout[numPorts] = _direction == 0; // affects layout > pixel direction
    numPorts++;
}

// image2data converts an image to OctoWS2811's raw data format.
// The number of vertical pixels in the image must be a multiple
// of 8.  The data array must be the proper size for the image.
//--------------------------------------------------------------
void ofxTeensyOcto::image2data(ofImage image, unsigned char * data, bool layout)
{    
    int offset = 3;
    int x, y, xbegin, xend, xinc, mask;
    int linesPerPin = image.getHeight() / 8;
    int * pixel = new int[8];
    
    // get the copied image pixels
    pixels2 = image.getPixelsRef();
    
    // 2d array of our pixel colors
    for (int x = 0; x < ledWidth; x++)
    {
        for (int y = 0; y < ledHeight; y++)
        {
            int loc = x + y * ledWidth;
            colors[loc] = pixels2.getColor(x, y);
        }
    }
    
    for (y = 0; y < linesPerPin; y++)
    {
        if ((y & 1) == (layout ? 0 : 1))
        {
            // even numbered rows are left to right
            xbegin = 0;
            xend = image.getWidth();
            xinc = 1;
        }
        else
        {
            // odd numbered rows are right to left
            xbegin = image.getWidth() - 1;
            xend = -1;
            xinc = -1;
        }
        
        //for (x = 0; x < ledWidth; x++)
        for (x = xbegin; x != xend; x += xinc)
        {
            for (int i=0; i < 8; i++)
            {
                int temploc = x + (i * ledWidth);
                pixel[i] = colors[temploc].getHex();
                pixel[i] = colorWiring(pixel[i]);
            }
            
            // convert 8 pixels to 24 bytes
            for (mask = 0x800000; mask != 0; mask >>= 1)
            {
                unsigned char b = 0;
                for (int i=0; i < 8; i++)
                {
                    if ((pixel[i] & mask) != 0) b |= (1 << i);
                }
                data[offset++] = b;
            }
        }
    }
}

//--------------------------------------------------------------
void ofxTeensyOcto::update()
{
    serialWrite();  // send our data via serial
}

//--------------------------------------------------------------
void ofxTeensyOcto::serialWrite()
{    
    for (int i=0; i < numPorts; i++)
    {
        // copy a portion of the movie's image to the LED image
        int xoffset = percentage(ledWidth, ledArea[i].x);
        int yoffset = percentage(ledHeight, ledArea[i].y);
        int xwidth =  percentage(ledWidth, ledArea[i].getWidth());
        int yheight = percentage(ledHeight, ledArea[i].getHeight());
        
        // grabs the screen so we can convert to pixels
        ledImage[i].setFromPixels(pixels1);
        ledImage[i].crop(xoffset, yoffset, xwidth, yheight);
        
        // convert the LED image to raw data
        unsigned char *ledData = new unsigned char[((int)ledImage[i].getWidth() * (int)ledImage[i].getHeight() * 3) + 3];
        
        image2data(ledImage[i], ledData, ledLayout[i]);
        
        ledData[0] = '*';  // first Teensy is the frame sync master
        
        int dataSize = ((ledWidth*8) * 3) + 3;  // height for each port is 8
        // send the raw data to the LEDs  :-)
        for (int j = 0; j < dataSize; j++)
        {
            ledSerial[i].writeByte(ledData[j]);
        }
        ledSerial[i].drain();   // this prevents massive flickering!
    }
}

//--------------------------------------------------------------
// DEMO DRAW FUNCTIONS BELOW !!! //
//--------------------------------------------------------------

// debugger
//--------------------------------------------------------------
void ofxTeensyOcto::drawDebug(int _brightness, int _debugScroll)
{    
    // white debug... 
    for (int i = 0; i < ledHeight; i++)
    {
        ofSetColor(ofColor::fromHsb(i*10, 0, _brightness));
        ofRect(0, i, ledWidth, 1);
    }
    
    // black line (LEDs OFF) -- use "," or "." to traverse the rows...
    ofSetColor(ofColor::fromHsb(0, 0, 0));
    ofRect(0, _debugScroll, ledWidth, 1);
    
}

// rainbow - horizontal
//--------------------------------------------------------------
void ofxTeensyOcto::drawRainbowH(int _brightness)
{    
    // vertical strips of rainbow goodness
    for (int i = 0; i < ledWidth; i++)
    {
        int huemap = ofMap(i, 0, ledWidth-1, 0, 255);
        ofSetColor(ofColor::fromHsb(huemap, 255, _brightness));
        ofRect(i, 0, 1, ledHeight);
    }
}

// rainbow - vertical
//--------------------------------------------------------------
void ofxTeensyOcto::drawRainbowV(int _brightness)
{    
    // horizontal strips of rainbow goodness
    for (int i = 0; i < ledHeight; i++)
    {
        int huemap = ofMap(i, 0, ledHeight-1, 0, 255);
        ofSetColor(ofColor::fromHsb(huemap, 255, _brightness));
        ofRect(0, i, ledWidth, 1);
    }
}

// wave animation
//--------------------------------------------------------------
void ofxTeensyOcto::drawWaves(int _brightness, float _waveSpeed)
{
    
    // sin wave
    counterShape = counterShape + _waveSpeed;
    
    // color scroller
    hue++;
    if (hue > 255) hue = 0;
    
    // back layer
    float k = 0.0;
    for(int i = 0; i < ledWidth; i+=5)
    {
        ofSetColor(ofColor::fromHsb(hue, 255, _brightness));
        ofRect(i, ledHeight, 5, -ledHeight/4 * (sin(counterShape-k)+1.0) - ledHeight/4);
        k+=0.5;
    }
    
    // front layer
    float kk = 0.0;
    for(int i = 0; i < ledWidth; i+=5)
    {
        ofSetColor(ofColor::fromHsb(hue, 255, _brightness*0.25));
        ofRect(i, ledHeight+2, 5, -ledHeight/4 * (sin(counterShape-kk)+1.0) - ledHeight/4);
        kk+=0.5;
    }
}
