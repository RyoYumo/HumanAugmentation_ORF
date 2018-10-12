#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);
    ofSetVerticalSync(true);
    ofSetLineWidth(2.0);
    hm.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    hm.autoRecord();
}

//--------------------------------------------------------------
void ofApp::draw(){
    hm.drawState(ofGetWidth()/2., 50);
    hm.drawAudio(ofGetWidth()/2., 350, ofGetWidth() - 200, 500);
    hm.drawMyo(ofGetWidth()/2., 900, ofGetWidth() - 200, 400);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 's'){
        hm.stop();
    }else if (key == 'p'){
        hm.play();
    }else if(key == 'r'){
        hm.record(10);
    }else if(key == 'l'){
        hm.playback();
    }
    
    if(key == 't'){
        ofToggleFullscreen();
    }
}


