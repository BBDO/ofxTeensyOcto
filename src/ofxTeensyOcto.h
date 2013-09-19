#pragma once

#include "ofMain.h"

class ofxTeensyOcto
{
public:
    
	void setup();
    void update();
    void image2data(ofImage image, unsigned char * data, bool layout);
    
    // variables
    int errorCount;
    int numPorts;
    float framerate;
    int maxPorts;
    ofSerial * ledSerial;
    ofRectangle * ledArea;
    bool * ledLayout;
    ofImage * ledImage;
//    ofColor colors[480];
    ofColor * colors;
    ofPixels pix;
    int ledWidth;
    int ledHeight;

};
