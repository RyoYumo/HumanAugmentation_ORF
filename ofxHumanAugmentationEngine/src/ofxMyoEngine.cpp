#include "ofxMyoEngine.h"

ofxMyoEngine::ofxMyoEngine()
{
    
}

ofxMyoEngine::~ofxMyoEngine()
{
    
}

void ofxMyoEngine::setup(const int port, std::string address,const int moving_average_num)
{
    sma_data.assign(512, 0.f);
    data_num_moving = moving_average_num;
    data.assign(data_num_moving, 0.f);
    
    ofxPublishOsc("127.0.0.1", 9400, "/moving_average", sma_result);
    
    ofxSubscribeOsc(port, address, [&](ofxOscMessage& m){
        if(m.getNumArgs() != num_channel){
            return;
        }else{
            emgReceived(m);
            calculateSMA();
        }
    });
}

void ofxMyoEngine::listen(const int port, std::string address)
{
    
}


void ofxMyoEngine::emgReceived(ofxOscMessage& m)
{
    switch (state) {
        case State::kStop:
            return;
        case State::kPlaying:
        {
            data.push_back(m.getArgAsFloat(0));
            if(data.size() > data_num_moving){
                data.erase(data.begin());
            }
            break;
        }
        case State::kRecording:
        {
            data.push_back(m.getArgAsFloat(0));
            if(data.size() > data_num_moving){
                data.erase(data.begin());
            }
            for(auto i = 0; i < num_channel;++i){
                record_data[i][buffer_counter] = m.getArgAsFloat(i);
            }
            
            if(buffer_counter == record_data[0].size() - 1){
                stop();
                save();
            }else{
                ++buffer_counter;
            }
            break;
        }
        case State::kPlayback:
            return;
    }
}


void ofxMyoEngine::stop()
{
    state = State::kStop;
}

void ofxMyoEngine::play()
{
    state = State::kPlaying;
}


void ofxMyoEngine::record(const int sec_record_time)
{
    for(auto i = 0; i < num_channel; ++i){
        record_data[i].resize(sample_rate * sec_record_time);
    }
    state = State::kRecording;
}

void ofxMyoEngine::setSavedPath(std::string file_path_)
{
    file_path = file_path_;
}


void ofxMyoEngine::clear()
{
    data.clear();
    for(auto i = 0; i < num_channel;++i){
        record_data[i].clear();
    }
}

int ofxMyoEngine::getNumChannel()
{
    return num_channel;
}

int ofxMyoEngine::getRecordDataSize()
{
    return record_data[0].size();
}

int ofxMyoEngine::getSampleRate(){
    return sample_rate;
}

std::vector<float>& ofxMyoEngine::getEmg(int channel)
{
    return record_data[channel];
}

void ofxMyoEngine::draw(float centerX, float centerY, float w, float h)
{
    ofPolyline poly;
    auto x = centerX - w * 0.5;
    auto y = centerY - h * 0.5;
    
    
    for(auto i = 0; i < data.size();++i){
        poly.addVertex(i * w / data.size(),h/2. -data[i] * h/2.);
    }
    
    ofNoFill();
    ofPushMatrix();
        ofTranslate(x, y);
        ofSetColor(255);
        ofDrawRectangle(0, 0, w, h);
        poly.draw();
    ofPopMatrix();
}

void ofxMyoEngine::drawSMA(float centerX, float centerY, float w, float h)
{

    auto x = centerX - w * 0.5;
    auto y = centerY - h * 0.5;
    
    ofPushStyle();
    ofPushMatrix();
        ofTranslate(x, y);
        ofDrawRectangle(0, 0, w, h);
        ofFill();
        ofBeginShape();
        for(auto i = 0 ; i < sma_data.size();++i){
            auto x = i * w / sma_data.size();
            auto y = h - sma_data[i] * h;
            if(i == 0) ofVertex(i, h);
            ofVertex(x, y);
            if( i == sma_data.size() -1) ofVertex(i * w/sma_data.size(), h);
        }
        ofEndShape(false);
    ofPopMatrix();
    ofPopStyle();
}

float ofxMyoEngine::calculateSMA()
{
    if(data.size() != data_num_moving){
        return;
    }else{
        auto sum = 0.f;
        for(auto& a :data){
            sum += a * a ;
        }
        sma_result = sqrt(sum / data_num_moving);
        sma_data.push_back(sma_result);
        if(sma_data.size() > 512){
            sma_data.erase(sma_data.begin());
        }
    }
}

float& ofxMyoEngine::getSMA(){
    return sma_result;
}

void ofxMyoEngine::loadFile()
{
    
}

void ofxMyoEngine::save()
{
    ofFile file(ofGetTimestampString() + ".txt",ofFile::WriteOnly);
    for(auto i = 0 ; i < num_channel;++i){
        for(auto j = 0; j < record_data[i].size();++j){
            file << record_data[i][j];
            if(i == num_channel - 1 && j == record_data[i].size() -1){
                file << endl;
            }else{
                file << ",";
            }
        }
    }
    clear();
}



