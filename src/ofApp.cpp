//
//      Example app w/ OF Teensy/OCTO class
//      by Jason Walters @ BBDO ...
//      Original P5 code by Paul Stoffregen/PJRC.COM
//
//      Last revision by Jason Walters on September 19th, 2013
//      Compatible with openFrameworks 0.80
//
///////////////////////////////////////////////////////////////

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0, 0, 0);
    ofDisableAntiAliasing();  // we need our graphics sharp for the LEDs
    
    teensy.setup(LWIDTH, LHEIGHT);  // setup and include led width & height
    
    // wave graphics sin + speed
    waveSpeed = 0.05f;
    brightness = 255;   // LED brightness

}

//--------------------------------------------------------------
void ofApp::update(){
    
    teensy.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //teensy.drawRainbow(brightness);
    teensy.drawWaves(brightness);
    
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
