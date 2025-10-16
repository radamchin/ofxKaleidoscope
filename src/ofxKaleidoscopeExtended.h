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
    bbc::utils::Oscillator oscillate_segments;
    bbc::utils::Oscillator oscillate_offset;
    bbc::utils::Oscillator oscillate_rotation;
    bbc::utils::Oscillator oscillate_height;

    ofParameter<int> height;
    ofParameter<float> alpha{"Alpha", 1.0, 0.0, 1.0};
    ofParameter<float> rotation{"Rotation", 0, 0, TWO_PI};
	
	ofParameterGroup oscillator_params;

    //--------------------------------------------------------------
    ofxKaleidoscopeExtended() {
        
    }
    
	//--------------------------------------------------------------
    ~ofxKaleidoscopeExtended() {
        
    }

    //--------------------------------------------------------------
    void setup( int w, int h ) {
        
       //  ofxKaleidoscope::setup( w, h );
        
        oscillate_segments.setup("Segments");
        oscillate_segments.setSpeed(0.02);
        oscillate_segments.setSpeedRange(0.001, 0.05);
        oscillate_segments.setOutputRange(8, 64);
        
        oscillate_offset.setup("Offset", 0.01, 0.001, 0.05);

        oscillate_rotation.setup("Rotation", 0.01, 0.001, 0.05);
        oscillate_rotation.setOutputRange(0, TWO_PI);

        oscillate_height.setup("Height", 0.01, 0.001, 0.05);

        oscillate_segments.enabled.addListener(this, &ofxKaleidoscopeExtended::segmentOscillateEnabled);
        oscillate_offset.enabled.addListener(this, &ofxKaleidoscopeExtended::offsetOscillateEnabled);
        oscillate_rotation.enabled.addListener(this, &ofxKaleidoscopeExtended::rotationOscillateEnabled);
        oscillate_height.enabled.addListener(this, &ofxKaleidoscopeExtended::heightOscillateEnabled);
        
		oscillator_params.setName("Oscillators");
		oscillator_params.add(oscillate_segments.params);
		oscillator_params.add(oscillate_offset.params);
		oscillator_params.add(oscillate_rotation.params);
		oscillator_params.add(oscillate_height.params);
		
        updateDimensions(w, h);
        
    }
    
    // Map the oscillator step values to start where the current item value is at.
    //--------------------------------------------------------------
    void segmentOscillateEnabled( bool & b) {
        if(b) oscillate_segments.mapStepTo( segments.get() );
    }
    
    //--------------------------------------------------------------
    void offsetOscillateEnabled( bool & b) {
        if(b) oscillate_offset.mapStepTo( offset.get() );
    }
    
    //--------------------------------------------------------------
    void rotationOscillateEnabled( bool & b) {
        if(b) oscillate_rotation.mapStepTo( rotation.get() );
    }
    
    //--------------------------------------------------------------
    void heightOscillateEnabled( bool & b) {
        if(b) oscillate_height.mapStepTo( height.get() );
    }
    
    //--------------------------------------------------------------
    void updateDimensions( int w, int h ) {
        
        ofxKaleidoscope::size( w, h );
        
        float ww = w; //ofGetWidth();
        float wh = h; // ofGetHeight();
        // float ratio = ww / hh;
        float fill_diameter = sqrt( ww * ww  +  wh * wh ); // Scale to fill full frame.

        oscillate_height.setOutputRange( ofGetHeight() * .4, fill_diameter );
        
        height.set("Height", h, 8, fill_diameter);
    }
    
    //--------------------------------------------------------------
    void update() {
        
        if(oscillate_segments.enabled) {
            this->setSegments( (int)oscillate_segments.update() );
        }

        if(oscillate_offset.enabled) {
            this->setOffset( oscillate_offset.update() );
        }

        if(oscillate_rotation.enabled) {
            rotation.set( oscillate_rotation.update() );
        }

        if(oscillate_height.enabled) {
            height.set( oscillate_height.update() );
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
