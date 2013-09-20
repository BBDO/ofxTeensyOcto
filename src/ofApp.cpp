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
    counterShape = 0.0f;
    waveSpeed = 0.05f;
    brightness = 255;   // LED brightness
    hue = 0;

}

//--------------------------------------------------------------
void ofApp::update(){
    
    teensy.update();
    updateScroller();
}

//--------------------------------------------------------------
void ofApp::updateScroller(){
    
    // hue scroller
    // openFrameworks hsb is 0-255, not 0-360 !!
    hue++;
    if (hue > 255)
    {
        hue = 0;
    }
    
    // counters
    counterShape = counterShape + waveSpeed;
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // draw a simple wave animation
    drawWaves();
    
    // brightness draw
    ofSetColor(255, 255, 255);
    ofDrawBitmapString("brightness (up/down) == " + ofToString(brightness), 20, ofGetHeight()-40);
    ofDrawBitmapString("waveSpeed (left/right) == " + ofToString(waveSpeed), 20, ofGetHeight()-20);
    
}

//--------------------------------------------------------------
void ofApp::drawWaves() {
    
    // back layer
    float k = 0.0;
    for(int i = 0; i < teensy.ledWidth; i+=3)
    {
        ofSetColor(ofColor::fromHsb(hue, 255, brightness));
        ofRect(i, 8, 3, -3 * (sin(counterShape-k)+1.0) - 2);
        k+=0.5;
    }
    
    // front layer
    float kk = 0.0;
    for(int i = 0; i < teensy.ledWidth; i+=3)
    {
        ofSetColor(ofColor::fromHsb(hue, 255, brightness*0.25));
        ofRect(i, 10, 3, -3 * (sin(counterShape-kk)+1.0) - 2);
        kk+=0.5;
    }
    
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
