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
    
    ofImage canvasImg;
    
    //--------------------------------------------------
    ofxKaleidoscope() {
        
        segments.set("Segments", 8, 3, MAX_KS_UI_SEGMENTS);
        offset.set("Offset", .5, 0.0, 2.0);
        
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
    
    void draw( int height ) {
            
        ofPushStyle();

        bool usingNormTexCoords = ofGetUsingNormalizedTexCoords();
                     
        if(!usingNormTexCoords) {
            ofEnableNormalizedTexCoords();
        }
         
        canvasImg.getTexture().bind();
        
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
        float dist = abs( (float)canvasImg.getHeight() * tan(ofDegToRad(angle)*0.5) ) / (float)canvasImg.getHeight();
             
        // realOffset is where the (normalized) middle of the 'V' is on the x-axis
        float realOffset = ofMap(offset, 0, 1, dist, 1-dist);
         
        // This is the point at the bottom of the triangle - our centre for the triangle fan
        mesh.addTexCoord(ofVec2f(realOffset, 1));
     
        ofVec2f ta(realOffset-dist, 0);
        ofVec2f tb(realOffset+dist, 0);

        for(int i = 0; i <= segments; i++) {
            mesh.addTexCoord( i%2==0 ? ta : tb);
        }

        ofPushMatrix();
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0);
        
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);        
        mesh.draw();

        // Debug
        //mesh.drawWireframe();
        //ofSetColor(ofColor::red);
        //mesh.drawVertices();

        ofPopMatrix();

        canvasImg.getTexture().unbind();

        if(!usingNormTexCoords) {
            ofDisableNormalizedTexCoords();
        }

        ofPopStyle();
        
    }
    
};

#endif /* ofxKaleidoscope_h */