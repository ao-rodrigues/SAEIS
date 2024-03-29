#include "ofApp.h"

const float ofApp::WINDOW_WIDTH = 1280.0;
const float ofApp::WINDOW_HEIGHT = 860.0;

//--------------------------------------------------------------
void ofApp::setup(){
    videoLibrary = std::make_unique<VideoLibrary>();
}

//--------------------------------------------------------------
void ofApp::update(){
    videoLibrary->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    videoLibrary->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    videoLibrary->key_pressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
