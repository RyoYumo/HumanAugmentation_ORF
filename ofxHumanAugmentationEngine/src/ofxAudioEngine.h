#pragma once
#include "ofMain.h"
#include "ofxCsv.h"
#include "ofxFindPeaks.h"

class ofxAudioEngine: public ofBaseSoundInput{
public:
    enum class State{
        kStop,
        kPlaying,
        kRecording,
        kPlayback
    };
    ofxAudioEngine();
    ~ofxAudioEngine();
    void setup(const int buffer_size=256, const int sample_rate=44100);
    void stop();
    void play();
    void playback();
    void record(const int sec_record_time);
    void save();
    void clear();
    void setSavedPath(std::string file_path_);
    void audioIn(float* input, int bufferSize, int nChannels);
    void loadFile();
    vector<float>& getAudio();
    int getBufferSize();
    int getSampleRate();
    int getRecordDataSize();
    void draw(float centerX, float centerY, float w, float h);
    void drawPlayback(float centerX, float centerY, float w, float h);
    void nomalization2();
    void nomalization();
    State getState();
    
private:
    std::vector<float> buffer_data;
    std::vector<float> record_data;
    std::vector<float> playback_data;
    std::vector<int> peak_index;
    std::string file_path;
    ofSoundStream stream;
    int buffer_counter;
    State state;

};
