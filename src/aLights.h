//
//  aLights.h
//  lights_camera_alembic
//
//  Created by bob white III on 8/17/13.
//
//

#ifndef __lights_camera_alembic__aLights__
#define __lights_camera_alembic__aLights__

#include <iostream>
#include "ofMain.h"

class aLights {
    
public:
    static aLights* Instance();
    
    //methods
    void reset();
    void update();
    void draw();
    void setLightOri(ofLight &light, ofVec3f rot);
 
    //variables
    ofEasyCam cam;
    ofLight spot;
    ofLight point;
    ofLight dir;
    ofLight amb;
    
    //turn this into an array.
    //************MOVING TO aTrackGui
    ofMaterial material1;
    ofMaterial material2;
    ofMaterial material3;
    ofMaterial material4;
    //*****************************
    
    ofVec3f dir_rot;
    ofVec3f spot_rot;

protected:
    aLights();
    
private:
    static aLights* _instance;
    
    
};

#endif /* defined(__lights_camera_alembic__aLights__) */
