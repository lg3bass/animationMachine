//
//  aLights.cpp
//  lights_camera_alembic
//
//  Created by bob white III on 8/17/13.
//
//

#include "aLights.h"

// Implementation
aLights* aLights::_instance = NULL;


aLights* aLights::Instance() {
	
	// checks if instance already exists:
	if (_instance == NULL) {
        _instance = new aLights();
    }
	return _instance;
	
	// from OF forum:
	/*
	 static Singleton inst;
	 return &inst;
	 */
}


aLights::aLights(){
    
    
}

void aLights::setLightOri(ofLight &light, ofVec3f rot)
{
    ofVec3f xax(1, 0, 0);
    ofVec3f yax(0, 1, 0);
    ofVec3f zax(0, 0, 1);
    ofQuaternion q;
    q.makeRotate(rot.x, xax, rot.y, yax, rot.z, zax);
    light.setOrientation(q);
}

void aLights::reset() {
    
    spot.setDiffuseColor(ofFloatColor(255.0, 0.0, 0.0f));
    spot.setSpecularColor(ofColor(0, 0, 255));
    spot.setSpotlight();
    spot.setSpotConcentration(3);
    spot.setSpotlightCutOff(10);
    spot_rot = ofVec3f(0, 0, 0);
    setLightOri(spot, spot_rot);
    
    spot.setPosition(0, 0, 300);
    
    point.setDiffuseColor(ofColor(0.0, 255.0, 0.0));
    
    point.setPointLight();
    
    dir.setDiffuseColor(ofColor(0.0f, 0.0f, 255.0f));
    dir.setSpecularColor(ofColor(255.0f, 0.0f, 0.0f));
    
    dir.setDirectional();
    dir_rot = ofVec3f(0, 0, 0);
    setLightOri(dir, dir_rot);
    
    amb.setAmbientColor(ofColor(20.0, 20.0, 20.0, 20.0));
    //*************MOVING TO aTrackGui
    material1.setShininess(128);
    
    material1.setSpecularColor(ofColor(255, 255, 255, 255));
    material1.setEmissiveColor(ofColor(0, 0, 0, 255));
    material1.setDiffuseColor(ofColor(255, 255, 255, 255));
    material1.setAmbientColor(ofColor(255, 255, 255, 255));
    
    material2.setShininess(128);
    
    material2.setSpecularColor(ofColor(255, 255, 255, 255));
    material2.setEmissiveColor(ofColor(0, 0, 0, 255));
    material2.setDiffuseColor(ofColor(255, 255, 255, 255));
    material2.setAmbientColor(ofColor(255, 255, 255, 255));
    //************MOVING TO aTrackGUI

}



void aLights::draw() {
    
    
}