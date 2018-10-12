#pragma once

#include "ofMain.h"
#include "ofxPubSubOsc.h"

class ofxMyoEngine{
public:
    enum class State{
        kStop,
        kPlaying,
        kRecording,
        kPlayback
    };
    ofxMyoEngine();
    ~ofxMyoEngine();
    
    void setup(const int port=12345,std::string address="/myo",const int moving_average_num=300);
    void listen(const int port, std::string address);
    void save();
    void clear();
    void stop();
    void play();
    void record(const int sec_record_time);
    void setSavedPath(std::string file_path_);
    void emgReceived(ofxOscMessage& m);
    std::vector<float>& getEmg(int channel);
    int getNumChannel();
    int getRecordDataSize();
    int getSampleRate();
    void draw(float centerX, float centerY, float w, float h);
    void drawSMA(float centerX, float centerY, float w, float h);
    float calculateSMA();
    float& getSMA();
    void loadFile();
    
    
    
    
private:
    State state = State::kStop;
    std::vector<float> data;
    std::vector<float> record_data[8];
    std::string file_path;
    std::vector<float> sma_data;
    int num_channel {8};
    int data_num_moving {300};
    float sma_result{0};
    const int sample_rate {200};
    int buffer_counter {0};
};
