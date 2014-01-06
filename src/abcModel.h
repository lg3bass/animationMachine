//
//  abcModel.h
//  lights_camera_alembic
//
//  Created by bob white III on 8/11/13.
//
//

#ifndef __lights_camera_alembic__abcModel__
#define __lights_camera_alembic__abcModel__


#include <iostream>
#include "ofMain.h"
#include "ofxAlembic.h"
#include "aGui.h"


class abcModel{
    
public:

    aGui * myGuiRef;// not used I think???
    
    abcModel(std::string file, float divisions, ofxAlembic::Reader r, int _id);
    void load();
    void calcTime(float t);
    void update();
    void draw();
    void report();
    void launch();// dont think this is needed anymore.
    bool holdAnimation();
    bool finalizeAnimation();
    void setClipMarkers(int _currentSegment);
    void playSegment(int type);
    void INFOlaunchingToPlay();
    
    int this_id;  // dont think this is public.
    
    //public
    float segments;
    float clipSpeedMod;
    float midiChannel;
    float midiNote;
    int ldrType; // 0=random, 1=linear
    int trackMode; // 0=noteOn, 1=noteOn/noteOff
    float segLength;
    int currentSegment;
    
    float half_unit;
    float full_unit;
        
    float clipTime;//where are you in the animation
    float midTime;//where the clip holds on noteOn
    float endTime;//in segment mode. where to stop.
    
    bool isDemo; //controlled by the play button.
    bool isAnimating;//is the model animating
    bool isHolding;//is the model holding with a noteOn/noteOff
    bool isFinishing;//
    bool isActive;//anything in here or an empty clip.
    
    bool stageHold;
    bool stageEnd;//are we finalizing the animation run
    
    ofVboMesh vbo_mesh;
        
    ofxAlembic::Reader aReader;
};




#endif /* defined(__lights_camera_alembic__abcModel__) */
