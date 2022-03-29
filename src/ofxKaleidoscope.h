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
    
    ofImage canvasImg;
    
    //--------------------------------------------------
    ofxKaleidoscope() {
        
        segments.set("Segments", 8, 3, MAX_KS_UI_SEGMENTS);
        offset.set("Offset", .5, 0.0, 2.0);
        
        debug.set("Debug", false);
        
    }
    
    void setup( int w, int h ) {
        
        size(w, h);
        
    }
    
    void size( int w, int h  ) {
        
        canvasImg.allocate(w, h, OF_IMAGE_COLOR);
        // canvasImg.setUseTexture(true);
        
    }
    
    void update( const ofPixels & pixels ) {
        
        canvasImg.setFromPixels(pixels);
        //canvasImg.setFromPixels(pixels.getData(), canvasImg.getWidth(), canvasImg.getHeight());
        //canvasImg.updateTexture();
    }
    
    void draw( int height = 0 ) {
        draw( ofGetWidth()/2.0, ofGetHeight()/2.0, canvasImg.getTexture(), height );
    }
    
    void draw( const ofTexture & tex, int height = 0 ) {
         draw( ofGetWidth()/2.0, ofGetHeight()/2.0, tex, height );
    }
    
    void draw( float cx, float cy, int height = 0 ) {
        draw( cx, cy, canvasImg.getTexture(), height );
    }
        
    void draw( float cx, float cy, const ofTexture & tex, int height = 0 ) {
        
        if(!tex.isAllocated()) return;
        
        ofPushStyle();

        bool usingNormTexCoords = ofGetUsingNormalizedTexCoords();
                     
        if(!usingNormTexCoords) {
            ofEnableNormalizedTexCoords();
        }
        
        height = (height < 1) ? tex.getHeight() : height;
        
        tex.bind();
        
        float angle = 360.f/segments; //8 sides to start
         
        ofMesh mesh;
         
        ofVec3f vec(0,0,0);
        mesh.addVertex(vec);
        vec.x += height/2;
         
        for(int i = 0; i < segments; i++) {
            mesh.addVertex(vec);
            vec.rotate(angle, ofVec3f(0,0,1));
        }
         
        // close the loop
        mesh.addVertex(vec);
         
        // Work out the texcoords
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
         
        // Normalized distance from the centre (half the width of the above 'V')
        float dist = abs( (float)tex.getHeight() * tan(ofDegToRad(angle)*0.5) ) / (float)tex.getHeight();
             
        // realOffset is where the (normalized) middle of the 'V' is on the x-axis
        float realOffset = ofMap( offset, 0, 1, dist, 1-dist );
         
        // This is the point at the bottom of the triangle - our centre for the triangle fan
        mesh.addTexCoord( ofVec2f(realOffset, 1) );
     
        ofVec2f ta( realOffset-dist, 0 );
        ofVec2f tb( realOffset+dist, 0 );

        for(int i = 0; i <= segments; i++) {
            mesh.addTexCoord( i%2==0 ? ta : tb);
        }

        ofPushMatrix();
        ofTranslate(cx, cy, 0);
        
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);        
        mesh.draw();
        
        tex.unbind();
        
        if(debug) {
            ofSetColor(ofColor::red, 128);
            mesh.drawWireframe();
            ofSetColor(ofColor::green);
            mesh.drawVertices();
        }
        
        ofPopMatrix();
        
        if(!usingNormTexCoords) {
            ofDisableNormalizedTexCoords();
        }

        ofPopStyle();
        
    }
    
    void setOffset( float f ) {
        offset.set(f);        
    }
    
    int setSegments( int n, bool force_even = true ) {
        
        if(force_even) {
            if(n % 2 != 0) {
                n++;
            }
        }
        segments.set(n);
        return segments.get();
    }
    
};

#endif /* ofxKaleidoscope_h */
