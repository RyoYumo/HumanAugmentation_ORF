#include "ofxHumanAugmentationEngine.h"



void ofxHumanAugmentationEngine::setup()
{
    audio.setup();
    myo1.setup(12345,"/myo",8);
    myo2.setup(12346,"/myo",8);
    myo3.setup(12347,"/myo",8);
    myo4.setup(12348,"/myo",8);
    
    ofTrueTypeFont::setGlobalDpi(72);
    font.load("Arial.ttf",30);
    font.setLineHeight(30);
    font.setLetterSpacing(1.0);
    
}

void ofxHumanAugmentationEngine::drawAudio(float centerX, float centerY, float w, float h)
{
    if(audio.getState() == ofxAudioEngine::State::kPlayback){
        audio.drawPlayback(centerX, centerY, w, h);
    }else{
        audio.draw(centerX, centerY, w, h);
    }
}

void ofxHumanAugmentationEngine::drawMyo(float centerX, float centerY, float w, float h)
{
    ofNoFill();
    myo1.drawSMA(centerX - w/4., centerY - h/4., w/2., h/2.);
    myo2.drawSMA(centerX - w/4., centerY + h/4. , w/2., h/2.);
    myo3.drawSMA(centerX + w/4., centerY - h/4., w/2., h/2.);
    myo4.drawSMA(centerX + w/4., centerY + h/4., w/2., h/2.);
}

void ofxHumanAugmentationEngine::stop()
{
    audio.stop();
    myo1.stop();
    myo2.stop();
    myo3.stop();
    myo4.stop();
}

void ofxHumanAugmentationEngine::play()
{
    audio.play();
    myo1.play();
    myo2.play();
    myo3.play();
    myo4.play();
}

void ofxHumanAugmentationEngine::record(const int sec_record_time)
{
    audio.record(sec_record_time);
    myo1.record(10);
//    myo2.record();
//    myo3.record();
//    myo4.record();
}

void ofxHumanAugmentationEngine::playback()
{
    audio.playback();
}

void ofxHumanAugmentationEngine::autoRecord(float audio_threshold,const int sec_record_time)
{
    // this function must be called in ofApp::update() function.
    
    if(audio.getState() != ofxAudioEngine::State::kPlaying){
        return;
    }else{
        auto data = audio.getAudio();
        for(auto& a: data){
            if(a > audio_threshold){
                audio.record(sec_record_time);
            }
        }
    }
    
    
}

void ofxHumanAugmentationEngine::drawState(float centerX, float centerY)
{
    auto state = audio.getState();
    std::string state_msg = "";
    if(state == ofxAudioEngine::State::kStop)
    {
        
        state_msg = "STOP";
        font.drawStringCentered(state_msg, centerX, centerY);
    }else if(state == ofxAudioEngine::State::kRecording)
    {
        ofPushStyle();
        ofFill();
        ofSetColor(255, 0, 0);
        state_msg = "REC";
        ofDrawCircle(centerX - 50, centerY, 10);
        font.drawStringCentered(state_msg, centerX, centerY);
        ofPopStyle();
        
    }else if(state == ofxAudioEngine::State::kPlayback){
        
        state_msg = "Playback";
        font.drawStringCentered(state_msg, centerX, centerY);
    }else if(state == ofxAudioEngine::State::kPlaying){
        
        state_msg = "Play";
        font.drawStringCentered(state_msg, centerX, centerY);
    }
}

