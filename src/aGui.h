//
//  aGui.h
//  lights_camera_alembic
//
//  Created by bob white III on 8/17/13.
//
//

#ifndef __lights_camera_alembic__aGui__
#define __lights_camera_alembic__aGui__

#include <iostream>
#include "ofMain.h"
#include "ofxUI.h"
#include "aLights.h"


class aGui{
    
public:
    
    static aGui* Instance();// pointer to itself
    
    
    /*
    {
        //doReset = guiAlloc = false;
        //wModActive = false;
    };
    */
    
    aLights * myLights2;
    
    
    // methods
    void setup(int num);//new
    void reset();
    void update();
    void draw();
    void exit();//new

    //GUI methods
    void guiEvent(ofxUIEventArgs &);
    void setGUI();//sets gui and gui2
    void setWSlider(vector<ofxUISlider *>sliders, ofColor &c, float v);
    
    // variables
    ofxUICanvas *gui;
    ofxUICanvas *gui2;
    //ofxUICanvas *gui_loader;//new loader gui
    
    bool guiAlloc;

    float radius;
    bool cull;
    bool useSpot;
    bool usePoint;
    bool autoRotatePoint;
    bool useAmb;
    bool useDir;
    bool showDirSource;
    bool showSpotSource;
    bool showPointSource;

    bool wModActive;
    bool showAxis;
    bool doReset;

    float animTime;

    int numOfLoaders;
    
    float speed;
    
    
    
    //gui sliders
    vector<ofxUISlider *> ambslider;
    vector<ofxUISlider *> spotspecslider;
    vector<ofxUISlider *> spotDiffSlider;
    vector<ofxUISlider *> dirspecslider;
    vector<ofxUISlider *> dirDiffSlider;
    vector<ofxUISlider *> pointspecslider;
    vector<ofxUISlider *> pointDiffSlider;
    vector<ofxUISlider *> matDiffSlider;
    vector<ofxUISlider *> matSpecSlider;
    vector<ofxUISlider *> matEmSlider;

protected:
    aGui();
    
private:
    static aGui* _instance;
    


};


#endif /* defined(__lights_camera_alembic__aGui__) */
