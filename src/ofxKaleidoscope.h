//
//  Kaleidoscope.h
//  ofxKaleidoscope
//
//  Created by Adam Hinshaw on 15/3/22.
//
//  Based on code from Anthropocene -> https://github.com/HellicarAndLewis/Anthropocene
//

#ifndef ofxKaleidoscope_h
#define ofxKaleidoscope_h

#include "ofMain.h"

#define MAX_KS_UI_SEGMENTS 64

class ofxKaleidoscope {

public:
    
    // Note: ofParameters can be externally added to ofxGui for manipulation
    ofParameter<int> segments;
    ofParameter<float> offset;
    
    ofParameter<bool> debug;
    
    ofParameter<float> innerRadius; // A hole in the centre
    
    bool forceEvenSegments = false;
	
	ofFbo buffer;
	
    //--------------------------------------------------
    ofxKaleidoscope() {
        
        segments.set("Segments", 16, 3, MAX_KS_UI_SEGMENTS);
        offset.set("Offset", .5, 0.0, 2.0);
        
        innerRadius.set("Inner Radius", 0, 0, 1);
        
        debug.set("Debug", false);
            
        segments.addListener(this, &ofxKaleidoscope::segmentsChanged);
    }
    
    //--------------------------------------------------
    void setup( int w, int h ) {
        size(w, h);
    }
    
    //--------------------------------------------------
    void size( int w, int h  ) {
        		
		ofFboSettings settings;
		settings.width = w;
		settings.height = h;
		settings.textureTarget = GL_TEXTURE_RECTANGLE_ARB; // GL_TEXTURE_2D;
		buffer.allocate( settings );
		
    }
	
	//--------------------------------------------------
	void updateBuffer( const ofTexture & tex ) {
		
		buffer.begin();
			tex.draw(0,0);
		buffer.end();
		
	}
	
    //--------------------------------------------------
    void draw( int height = 0, float rotation = 0 ) {
        draw( ofGetWidth()/2.0, ofGetHeight()/2.0, buffer.getTexture(), height, rotation );
    }
	
	//--------------------------------------------------
    void draw( const ofTexture & tex, int height = 0, float rotation = 0 ) {
         draw( ofGetWidth()/2.0, ofGetHeight()/2.0, tex, height, rotation );
    }
	
	//--------------------------------------------------
    void draw( float cx, float cy, int height = 0, float rotation = 0 ) {
        draw( cx, cy, buffer.getTexture(), height, rotation );
    }
	
	//--------------------------------------------------
	void draw( float cx, float cy, const ofTexture & tex, int height = 0, float rotation = 0 ) {
		
		if(!tex.isAllocated()) return;
		
		// Use actual texture height if not specified
		height = (height < 1) ? tex.getHeight() : height;
		
		float angle = 360.f / segments; // 8 sides to start
		 
		ofMesh mesh;
		ofVec3f vec(0, 0, 0);
		mesh.addVertex(vec);
		vec.x += height / 2;
		 
		for (int i = 0; i < segments; i++) {
			mesh.addVertex(vec);
			vec.rotate(angle, ofVec3f(0, 0, 1));
		}
		
		// Close the loop
		mesh.addVertex(vec);
		 
		/*
		  __________________
		  |   \        /   |
		  |    \      /    |
		  |     \    /     |
		  |      \  /      |
		  |       \/       |
		  +----------------+
		  
		  A v shape out of the centre of the camera texture
		*/
		 
		// Half-width of the V shape in pixels
		float halfWidth = abs(tex.getHeight() * tan(ofDegToRad(angle) * 0.5f));
		
		// The x position (in pixels) of the centre of the V
		float realOffset = ofMap(offset, 0, 1, halfWidth, tex.getWidth() - halfWidth);
		
		// This is the point at the bottom of the triangle - our centre for the triangle fan
		mesh.addTexCoord(ofVec2f(realOffset, tex.getHeight()));
		
		ofVec2f ta(realOffset - halfWidth, 0);
		ofVec2f tb(realOffset + halfWidth, 0);

		for (int i = 0; i <= segments; i++) {
			mesh.addTexCoord(i % 2 == 0 ? ta : tb);
		}

		ofPushMatrix();
		ofTranslate(cx, cy, 0);
		if (rotation != 0) ofRotateRad(rotation);
		
		mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
		
		tex.bind();
		mesh.draw();
		tex.unbind();
		
		if (debug) {
			ofPushStyle();
			ofSetColor(ofColor::red, 128);
			mesh.drawWireframe();
			ofSetColor(ofColor::green);
			mesh.drawVertices();
			ofPopStyle();
		}
		
		ofPopMatrix();
	}
	
	
    //--------------------------------------------------
    void setOffset( float f ) {
        offset.set(f);        
    }
    
    //--------------------------------------------------
    void segmentsChanged( int & val ) {
        
        if(forceEvenSegments) {
            if(val % 2 != 0) {
                segments.set(val+1);
            }
        }
        
    }
    
    //--------------------------------------------------
    int setSegments( int n, bool force_even = true ) {
        
        if(force_even) {
            if(n % 2 != 0) {
                n++;
            }
        }
        segments.set(n);
        return segments.get();
    }
    
    //--------------------------------------------------
    
};

#endif /* ofxKaleidoscope_h */
