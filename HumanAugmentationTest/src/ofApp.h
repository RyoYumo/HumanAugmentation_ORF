#pragma once

#include "ofMain.h"
#include "ofxHumanAugmentationEngine.h"
#include "ofxPubSubOsc.h"


class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    ofxHumanAugmentationEngine hm;
};
