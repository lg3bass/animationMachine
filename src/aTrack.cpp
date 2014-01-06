//
//  aTrack.cpp
//  lights_camera_alembic
//
//  Created by bob white III on 9/11/13.
//
//

#include "aTrack.h"


aTrack::aTrack(std::string _behavior, bool _playNote, int _myID){
    
    
    myID = _myID;
    behavior = _behavior;
    playNote = _playNote;
    
    isInit = false;
    
}

void aTrack::getInfo() {
    cout << "TRACK: " << myID+1 << ",type:" << behavior << ",note:" << playNote << "," << "[";
    for(int i=0; i<myLdrs.size();i++){
        cout << "(" << myLdrs[i].x << "," << myLdrs[i].y << ")";        
    }
    cout << "]" << endl;
}

void aTrack::whatIsPlaying(int _ldrID) {
    cout << "SLECTED TO PLAY (index:" << myLdrs[_ldrID].x << ",note:" << myLdrs[_ldrID].y << ",seg:" << segments << ")" << endl;
}