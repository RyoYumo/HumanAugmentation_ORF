#pragma once

#include "ofMain.h"
#include "ofxAudioEngine.h"
#include "ofxMyoEngine.h"
#include "ofxCenteredTrueTypeFont.h"

class ofxHumanAugmentationEngine{
public:
    ofxHumanAugmentationEngine(){
        
    }
    ofxAudioEngine audio;
    ofxMyoEngine myo1;
    ofxMyoEngine myo2;
    ofxMyoEngine myo3;
    ofxMyoEngine myo4;
    ofxCenteredTrueTypeFont font;

    void setup();
    void drawAudio(float centerX,float centerY,float w, float h);
    void drawMyo(float centerX, float centerY, float w, float h);
    void drawPlaybackAudio(float centerX, float centerY, float w, float h);
    void drawState(float centerX, float centerY);
    void stop();
    void play();
    void record(const int sec_record_time);
    void playback();
    void autoRecord(float audio_threshold=0.5,const int sec_record_time=10);
    int moving_average_num;

};

