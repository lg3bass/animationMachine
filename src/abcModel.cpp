//
//  abcModel.cpp
//  lights_camera_alembic
//
//  Created by bob white III on 8/11/13.
//
//

#include "abcModel.h"
#include "ofxAlembic.h"



//New abcModel(file,divisions,
//--------------------------------------------------------------
abcModel::abcModel(std::string file, float divisions, ofxAlembic::Reader r,int _id){
    
   
    
    
    
    this_id = _id;    
    aReader = r;  //Question: do I need to dispose of this object?        
    string path = file;    
    
    if(path == "empty"){
        isActive = false;
    } else {
        isActive = true;
        //open the file if active
        aReader.open(path);
    }
     // show all drawable names
	r.dumpNames();
    
    //params
    //how many segments
    segments = divisions;
    //track speed
    clipSpeedMod = 0.07;
    //what midi channel/track
    midiChannel = 0;
    //what specific note
    midiNote = 0;
    //track type(0:random,1:linear)
    ldrType = 0;
    //track mode(0:note On,1:note Off)
    trackMode = 0;
    //how big are the segments
    segLength = 0;
    //which segment is currently playing.
    currentSegment = 0;
    
    isDemo = false;
    isAnimating = false;
    isHolding = false;
    isFinishing = false;
    
    full_unit = 1.0;
    half_unit = 0.5;
    
    
    vbo_mesh.setMode(OF_PRIMITIVE_TRIANGLES);

}




void abcModel::load() {
    //ofxUINumberDialer *trackDialer = (ofxUINumberDialer *)myGuiRef->gui_loader->getWidget(ofToString(this_id)+"_TRK_SPEED");
    //clipSpeedMod = trackDialer->getValue();
}


// ???? Dont think this is needed anymore. 
void abcModel::launch() {
    clipTime = 0.0;
    isHolding = false;
    stageHold = false;
    stageEnd = false;
}


//-----------------------------------------------------
void abcModel::playSegment(int _type){
    if(currentSegment == segments-1){
        currentSegment = 0;
    } else {
        if(_type == 0){//if random
            currentSegment = floor(ofRandom(0, segments));
        } else {//if linear
            currentSegment++;
        }
    }
       //set the markers
    setClipMarkers(currentSegment);
    
   //set the flags.
    isFinishing = false;
    isHolding = false;
    isAnimating = true; //may be redundant. 
    
    //finally output what is slated to play. Uncomment to see what is playing
    //INFOlaunchingToPlay();
}

//-----------------------------------------------------

//TO-DO:  You need to calculate markers for aReader.getMaxTime() somehow. That way you can simplify the calcTime procedure.
void abcModel::setClipMarkers(int _currentSegment){
    
    full_unit = (segLength/30)/segments;
    half_unit = full_unit/2;
    
    clipTime = _currentSegment*full_unit;
    midTime = clipTime+half_unit;
    endTime = clipTime+full_unit;
}



//-----------------------------------------------------
void abcModel::calcTime(float t){
    //set the position in this Alembic model
    if(isDemo){
        //fed from slider
        aReader.setTime(t);
    } else {    
        if(isAnimating) {
                if(trackMode == 1){
                    if(clipTime < midTime) {
                        clipTime += clipSpeedMod;
                        if(clipTime > midTime){
                            clipTime = midTime;
                        }
                        //cout << "clipTime:" << clipTime << endl;
                    } else {
                        //cout << "midTime:" << clipTime << endl;
                    }
                } else {
                    if(clipTime < endTime) {
                        clipTime += clipSpeedMod;
                        if(clipTime > endTime){
                            clipTime = endTime;
                        }
                    } else {
                        isAnimating = false;
                    }
                }        
        } else {
            if(isHolding) {
                finalizeAnimation();
            } else {
                
            }
        }//end isAnimating
        
        aReader.setTime(clipTime);
        
    } // end isDemo

}

bool abcModel::holdAnimation(){
    
    clipTime = aReader.getMaxTime()/segments;
    //isHolding = true;
    return true;
}

bool abcModel::finalizeAnimation(){
    //isAnimating = true;
    if(clipTime < endTime) {
        clipTime += clipSpeedMod;
        if(clipTime > endTime){
            clipTime = endTime;
        }
        //cout << "endTime:" << clipTime << endl;
    } else {
        isHolding = false;
        isAnimating = false;
    }    
    return true;
}

void abcModel::update(){
    for (int i = 0; i < aReader.size(); i++)
	{
		ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_TRIANGLES);
        
        
		if (aReader.get(i, mesh))
		{
			//mesh.draw();
            vbo_mesh = mesh;
            //vbo_mesh.draw();
            
		}
	}
    
    
}


void abcModel::draw(){
    
    //tried to move to update but it was only rendering one mesh per track.
    //vbo_mesh.draw();
    
    
    for (int i = 0; i < aReader.size(); i++)
	{
		//ofMesh mesh;
        //mesh.setMode(OF_PRIMITIVE_TRIANGLES);
        
        
		if (aReader.get(i, vbo_mesh))
		{
			//mesh.draw();
            //vbo_mesh = mesh;
            vbo_mesh.draw();
            
		}
	}
    
    
}


void abcModel::report() {
    // show all drawable names
	aReader.dumpNames();
}

void abcModel::INFOlaunchingToPlay() {
    cout    << "----->LDR (index:" << this_id    
            << ",speed:" << clipSpeedMod
            << ",ch:" << midiChannel
            << ",note:" << midiNote
            << ",type:" << ldrType
            << ",mode:" << trackMode
            << ",segments:" << segments
            << ",segment Length:" << segLength
            << ",currentSegment:" << currentSegment
            << ",TIME:" << clipTime << "-" << midTime << "-" << endTime
            << ")" << endl;
}




