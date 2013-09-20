#pragma once

#include "ofMain.h"

class ofxTeensyOcto
{
public:
    
	void setup(int _ledWidth, int _ledHeight);
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
    ofColor * colors;
    ofPixels pix;
    int ledWidth;
    int ledHeight;

};
