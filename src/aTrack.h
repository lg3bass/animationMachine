//
//  aTrack.h
//  lights_camera_alembic
//
//  Created by bob white III on 9/11/13.
//
//

#ifndef __lights_camera_alembic__aTrack__
#define __lights_camera_alembic__aTrack__

#include <iostream>
#include "ofMain.h"

#endif /* defined(__lights_camera_alembic__aTrack__) */


class aTrack  {
    
    public:
        aTrack(std::string _behavior, bool _playNote, int _myID);
    
        int myID;
        std::string behavior;
        bool playNote;
        bool isInit;
        vector <ofVec2f> myLdrs;//this is a vector holding 1. loader index and 2. midi note (if any).
        int segments;
    
        void getInfo();
        void whatIsPlaying(int _ldrID);

};