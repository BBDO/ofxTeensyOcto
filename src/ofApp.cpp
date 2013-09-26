//
//      Example app w/ OF Teensy/OCTO class
//      by Jason Walters @ BBDO ...
//      Original P5 code by Paul Stoffregen/PJRC.COM
//
//      Last revision by Jason Walters on September 26th, 2013
//      Compatible with openFrameworks 0.80
//
///////////////////////////////////////////////////////////////

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofBackground(0, 0, 0);    // default background to black / LEDs off
    ofDisableAntiAliasing();  // we need our graphics sharp for the LEDs
    
    int stripWidth = 60;      // pixel width of strip
    int stripHeight = 1;      // pixel height of strip
    int stripsPerPort = 8;    // total number of strips per port
    int numPorts = 2;         // total number of teensy ports?
    
    teensy.setup(stripWidth, stripHeight*stripsPerPort*numPorts);
    
    // (portName, stripWidth, stripHeight*stripsPerPort, xOffset, yOffset, width%, height%, direction)
    teensy.serialConfigure("tty.usbmodem14761", stripWidth, stripHeight*stripsPerPort, 0, 0, 100, 50, 0);
    teensy.serialConfigure("tty.usbmodem14781", stripWidth, stripHeight*stripsPerPort, 0, 50, 100, 50, 0);
    
    // allocate our pixels & fbo
    teensy.pixels1.allocate(stripWidth, stripHeight*stripsPerPort*numPorts, OF_PIXELS_RGB);
    fbo.allocate(stripWidth, stripHeight*stripsPerPort*numPorts, GL_RGB);
    
    waveSpeed = 0.05f;  // wave speed
    brightness = 2;   // LED brightness

}

//--------------------------------------------------------------
void ofApp::update(){
    
    teensy.update();    // update our serial to teensy stuff
    updateFbo();        // update our Fbo functions
}

//--------------------------------------------------------------
void ofApp::updateFbo()
{    
    fbo.begin();
    
    ofClear(0,0,0);    // refreshes fbo, removes artifacts
    
    // teensy demo draw modes...
    //teensy.drawDebug(brightness, debugScroll);
    //teensy.drawRainbowH(brightness);
    //teensy.drawRainbowV(brightness);
    teensy.drawWaves(brightness, waveSpeed);
    
    fbo.end();
    
    fbo.readToPixels(teensy.pixels1);    // send fbo pixels to teensy
}

//--------------------------------------------------------------
void ofApp::draw()
{
    // brightness draw
    ofSetColor(255, 255, 255);
    ofDrawBitmapString("brightness (up/down) == " + ofToString(brightness), 20, ofGetHeight()-40);
    ofDrawBitmapString("waveSpeed (left/right) == " + ofToString(waveSpeed), 20, ofGetHeight()-20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key)
    {
        case OF_KEY_UP:
            brightness+=10;
            if (brightness > 255) brightness = 255;
            break;
            
        case OF_KEY_DOWN:
            brightness-=10;
            if (brightness < 0) brightness = 0;
            break;
            
        case OF_KEY_RIGHT:
            waveSpeed+=0.01f;
            if (waveSpeed > 1.0f) waveSpeed = 1.0f;
            break;
            
        case OF_KEY_LEFT:
            waveSpeed-=0.01f;
            if (waveSpeed < 0.05f) waveSpeed = 0.05f;
            break;
            
        case '.':
            debugScroll++;
            if (debugScroll > 15) debugScroll = 0;
            cout << "debugScroll = " << debugScroll << endl;
            break;
            
        case ',':
            debugScroll--;
            if (debugScroll < 0) debugScroll = 15;
            cout << "debugScroll = " << debugScroll << endl;
            break;
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
