#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(30);
    ofBackground(ofColor::black);
    
    showUi = true;
    
    vidPlayer.load( "19bn17jNjbs-480p.mov" );
    
    vidPlayer.play();
    vidPlayer.setVolume(0);
    vidPlayer.setLoopState(OF_LOOP_NORMAL);
    
    kScope.setup( vidPlayer.getWidth(), vidPlayer.getHeight() );
    
    gui.setup("Kaleidoscope");
    
    gui.add( kScope.segments) ;
    gui.add( kScope.offset ) ;
    gui.add( kheight.set("Height", ofGetHeight(), 8, ofGetHeight() * 4) );
    
    gui.setPosition( ofGetWidth()-235, 5 );
    
}

//--------------------------------------------------------------
void ofApp::update( ){
    
    vidPlayer.update();
    
    if(vidPlayer.isFrameNew()){
       kScope.update( vidPlayer.getPixels() );
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
        
    kScope.draw( kheight.get() );
    
    if(showUi) gui.draw();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == 'h' ){
        showUi = !showUi;
    }
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
