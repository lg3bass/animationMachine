//
//  aTrack.cpp
//  lights_camera_alembic
//
//  Created by bob white III on 9/11/13.
//
//

#include "aTrack.h"


aTrack::aTrack(std::string _behavior, bool _playNote, int _myID){
    
    //myLdr is a vector that contains. This is here for fast reference.
    //1. x = index of the loader. (~28 total)
    //2. y = midi note as assigned in the gui.
    
    myID = _myID;
    behavior = _behavior;
    playNote = _playNote;
    
    isInit = false;
    
}

void aTrack::getInfo() {
    cout << "TRACK: " << myID+1 << ",type:" << behavior << ",note:" << playNote << "," << "[";
    for(int i=0; i<myLdrs.size();i++){
        //loop through all the loaders in the track and out put the 1. x=abcModel 2. y=midi note.
        cout << "(" << myLdrs[i].x << "," << myLdrs[i].y << ")";
    }
    cout << "]" << endl;
}

void aTrack::whatIsPlaying(int _ldrID) {
    cout << "SLECTED TO PLAY (index:" << myLdrs[_ldrID].x << ",note:" << myLdrs[_ldrID].y << ",seg:" << segments << ")" << endl;
}