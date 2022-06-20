//
//  ofxKaleidoscopeExtended.h
//  kaleidoscope-dev
//
//  Created by Adam Hinshaw on 6/4/22.
//
//  Wrapper of Base ofxKaleidoscope, but adds animation things.
//
//  Note: This has dependecy on this repo / file: https://github.com/radamchin/ofxBBCUtils/blob/dev/src/animation/Oscillator.h
//

#ifndef ofxKaleidoscopeExtended_h
#define ofxKaleidoscopeExtended_h

#include "ofxKaleidoscope.h"
#include "Oscillator.h"

//--------------------------------------------------------------
class ofxKaleidoscopeExtended: public ofxKaleidoscope {

    public:

    // Animation parameters
    bbc::utils::Oscillator oscillateSegments;
    bbc::utils::Oscillator oscillateOffset;
    bbc::utils::Oscillator oscillateRotation;
    bbc::utils::Oscillator oscillateHeight;

    ofParameter<int> height;
    ofParameter<float> alpha{"Alpha", 1.0, 0.0, 1.0};
    ofParameter<float> rotation{"Rotation", 0, 0, TWO_PI};

    //--------------------------------------------------------------
    ofxKaleidoscopeExtended() {
        
    }
    
    ~ofxKaleidoscopeExtended() {
        
    }

    //--------------------------------------------------------------
    void setup( int w, int h ) {
        
       //  ofxKaleidoscope::setup( w, h );
        
        oscillateSegments.setup("Segments");
        oscillateSegments.setSpeed(0.02);
        oscillateSegments.setSpeedRange(0.001, 0.05);
        oscillateSegments.setOutputRange(8, 64);
        
        oscillateOffset.setup("Offset", 0.01, 0.001, 0.05);

        oscillateRotation.setup("Rotation", 0.01, 0.001, 0.05);
        oscillateRotation.setOutputRange(0, TWO_PI);

        oscillateHeight.setup("Height", 0.01, 0.001, 0.05);

        
        oscillateSegments.enabled.addListener(this, &ofxKaleidoscopeExtended::segmentOscillateEnabled);
        oscillateOffset.enabled.addListener(this, &ofxKaleidoscopeExtended::offsetOscillateEnabled);
        oscillateRotation.enabled.addListener(this, &ofxKaleidoscopeExtended::rotationOscillateEnabled);
        oscillateHeight.enabled.addListener(this, &ofxKaleidoscopeExtended::heightOscillateEnabled);
        
        updateDimensions(w, h);
        
    }
    
    // Map the oscillator step values to start where the current item value is at.
    //--------------------------------------------------------------
    void segmentOscillateEnabled( bool & b) {
        if(b) oscillateSegments.mapStepTo( segments.get() );
    }
    
    //--------------------------------------------------------------
    void offsetOscillateEnabled( bool & b) {
        if(b) oscillateOffset.mapStepTo( offset.get() );
    }
    
    //--------------------------------------------------------------
    void rotationOscillateEnabled( bool & b) {
        if(b) oscillateRotation.mapStepTo( rotation.get() );
    }
    
    //--------------------------------------------------------------
    void heightOscillateEnabled( bool & b) {
        if(b) oscillateHeight.mapStepTo( height.get() );
    }
    
    //--------------------------------------------------------------
    void updateDimensions( int w, int h ) {
        
        ofxKaleidoscope::size( w, h );
        
        float ww = w; //ofGetWidth();
        float wh = h; // ofGetHeight();
        // float ratio = ww / hh;
        float fill_diameter = sqrt( ww * ww  +  wh * wh ); // Scale to fill full frame.

        oscillateHeight.setOutputRange( ofGetHeight() * .4, fill_diameter );
        
        height.set("Height", h, 8, fill_diameter);
    }
    
    //--------------------------------------------------------------
    void update() {
        
        if(oscillateSegments.enabled) {
            this->setSegments( (int)oscillateSegments.update() );
        }

        if(oscillateOffset.enabled) {
            this->setOffset( oscillateOffset.update() );
        }

        if(oscillateRotation.enabled) {
            rotation.set( oscillateRotation.update() );
        }

        if(oscillateHeight.enabled) {
            height.set( oscillateHeight.update() );
        }

    }

    //--------------------------------------------------------------
    void draw(int x = 0, int y = 0, float global_alpha = 1.0, ofBlendMode blendMode = OF_BLENDMODE_ALPHA) {

        ofPushMatrix();
        ofPushStyle();
        
        ofEnableBlendMode( blendMode );
        
        float a = global_alpha * alpha * 255;
        if(a > 0 ) {
            ofSetColor(255, global_alpha * alpha * 255);
            ofxKaleidoscope::draw( x, y, height.get(), rotation.get() );
        }
        
        ofPopStyle();
        ofPopMatrix();
    }

    //--------------------------------------------------------------
    
    
};

#endif /* ofxKaleidoscopeExtended_h */
