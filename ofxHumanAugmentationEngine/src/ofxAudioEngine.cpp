
#include "ofxAudioEngine.h"

ofxAudioEngine::ofxAudioEngine():buffer_counter{0},state{State::kStop}
{}

ofxAudioEngine::~ofxAudioEngine(){
    stream.close();
}


void ofxAudioEngine::setup(const int buffer_size, const int sample_rate)
{
    buffer_data.assign(buffer_size,0.f);
    auto device_list = stream.getDeviceList();
    std::string device_name = "";
    for(auto&& str:device_list)
        device_name += ofToString(str) + "\n";
    auto device_id = ofToInt(ofSystemTextBoxDialog(device_name));
    stream.setDeviceID(device_id);
    stream.setup(0, 1, sample_rate, buffer_size, 2);
    stream.setInput(this);
}



void ofxAudioEngine::audioIn(float* input, int bufferSize, int nChannels)
{
    //std::copyの方が早い？
    switch (state) {
        case State::kStop:
            return;
        case State::kPlaying:
            for(auto i = 0 ; i < bufferSize; ++i){
                buffer_data[i] = input[i];
            }
            break;
        case State::kRecording:
        {
            auto last_buffer = record_data.size() % bufferSize;
            if(buffer_counter * bufferSize + buffer_counter > record_data.size()){
                for(auto i = 0; i < last_buffer; ++i){
                    record_data[i + buffer_counter * bufferSize] = input[i];
                }
                stop();
                save();
            }else{
                for(auto i = 0; i < bufferSize; ++i){
                    buffer_data[i] = input[i];
                    record_data[i + buffer_counter * bufferSize] = input[i];
                }
                
            }
            ++buffer_counter;
        }
            break;
        case State::kPlayback:
            break;
    }
}

void ofxAudioEngine::draw(float centerX, float centerY, float w, float h)
{
    ofPolyline poly;
    auto x = centerX - w * 0.5 ;
    auto y = centerY - h * 0.5 ;
    for (auto i=0; i < buffer_data.size(); ++i) {
        poly.addVertex(i * w / buffer_data.size(), h/2.- buffer_data[i] * h/2.);
    }
    
    std::string state_message;
    if (state == State::kRecording) {
        state_message = "recording";
    }else{
        state_message = "input audio";
    }
    
    ofNoFill();
        ofPushMatrix();
        ofTranslate(x, y);
        ofDrawBitmapString(state_message, 4, 18);
        ofDrawRectangle(0, 0, w, h);
        ofSetColor(255, 255, 255);
        poly.draw();
    ofPopMatrix();
}

void ofxAudioEngine::drawPlayback(float centerX, float centerY, float w,float h)
{
    ofPolyline poly;
    auto x = centerX - w * 0.5 ;
    auto y = centerY - h * 0.5 ;
    for (auto i=0; i < playback_data.size(); ++i) {
        poly.addVertex(i * w / playback_data.size(), h/2.- playback_data[i] * h/2.);
        for(auto& index:peak_index){
            if(i == index){
                ofPushMatrix();
                ofPushStyle();
                ofTranslate(x, y);
                ofSetColor(255, 0, 0);
                ofDrawCircle(i * w / playback_data.size(), h/2.- playback_data[i] * h/2., 4);
                ofPopStyle();
                ofPopMatrix();
            }
        }
    }
    ofNoFill();
    ofPushMatrix();
    ofTranslate(x, y);
    ofSetColor(255);
    ofDrawBitmapString("playback data", 4, 18);
    ofDrawRectangle(0, 0, w, h);
    poly.draw();
    ofPopMatrix();
}



int ofxAudioEngine::getBufferSize()
{
    return stream.getBufferSize();
}

int ofxAudioEngine::getSampleRate()
{
    return stream.getSampleRate();
}

int ofxAudioEngine::getRecordDataSize()
{
    return record_data.size();
}

std::vector<float>& ofxAudioEngine::getAudio()
{
    return buffer_data;
}


void ofxAudioEngine::stop()
{
    state = State::kStop;
}

void ofxAudioEngine::play()
{
    state = State::kPlaying;
}

void ofxAudioEngine::record(const int sec_record_time)
{
    auto fs = stream.getSampleRate();
    record_data.resize(fs * sec_record_time);
    state = State::kRecording;
    
}

void ofxAudioEngine::playback()
{
    loadFile();
    state = State::kPlayback;
}


void ofxAudioEngine::clear()
{
    buffer_data.assign(buffer_data.size(), 0.f);
    record_data.clear();
    buffer_counter = 0;
}

void ofxAudioEngine::save()
{
    ofFile file(ofGetTimestampString() + ".txt",ofFile::WriteOnly);
    std::cout << "export line size=" << record_data.size() << std::endl;
    for(auto i = 0; i < record_data.size();++i){
        file << record_data[i];
        if(i == record_data.size() -1){
            file << endl;
        }else{
            file << ",";
        }
    }
    clear();
}

void ofxAudioEngine::setSavedPath(std::string file_path_)
{
    
}

void ofxAudioEngine::loadFile()
{
    auto select_file = ofSystemLoadDialog();
    auto load_file_path = select_file.getPath();
    ofxCsv csv;
    csv.load(load_file_path, ",");
    auto data = csv.getRow(0);
    playback_data.resize(data.size());
    for(auto i = 0; i < data.size();++i){
        playback_data[i] = data.getFloat(i);
    }
    findPeaks(playback_data, peak_index);
    nomalization();
}

void ofxAudioEngine::nomalization()
{
    float max_value = *std::max_element(playback_data.begin(), playback_data.end());
    float min_value = *std::min_element(playback_data.begin(), playback_data.end());
    for(auto i =0 ;i < playback_data.size();++i){
        float nomalized_data = (playback_data[i] - min_value) / (max_value - min_value) * (1. - (-1.)) + (-1.);
        playback_data[i] = nomalized_data;
    }
}





ofxAudioEngine::State ofxAudioEngine::getState()
{
    return state;
}


