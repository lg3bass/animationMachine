#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxSyphon.h"
#include "ofxMidi.h"
#include "ofxCameraMove.h"
#include "abcModel.h"
#include "aGui.h"
#include "aLights.h"
#include "aTrack.h"
#include "aTrackGui.h"



class testApp : public ofBaseApp, public ofxMidiListener {
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void reset();
    
    //MIDI
    void setupMidi(int input);
    void toggleMidiPort();
    
    //SCENES
    int currentScene;
    void clearScene(int sceneIndex);
    void loadScene(int sceneIndex);
    void saveScene(int sceneIndex);
    
    
    //TRACKS
    void createTracks(int num);
    void setParamsInABCloaders(int num);
    void clearParamsInABCloaders(int num);
    vector<aTrack> tracks;
    int pickRandomLoader(int ch);
    
    //TRACK GUI
    aTrackGui *myTrackGui;
    
    //CAM
    ofEasyCam cam;
    ofxCameraMove saveCam;
    
    //GUI
    aGui *myGui;
    bool modkey = false;
    bool showTrack = false;
    bool showLights = false;
    bool showLdr = false;
    
    //GUI LOADER
    ofPoint dragPt;//drag and drop
    bool gui_loader_Alloc;
    bool doneBuilding;
    ofxUICanvas *gui_loader;//new loader gui
    void setGUI_loader(int num);
    void LoaderGuiEvent(ofxUIEventArgs &e);
    void resetAnimation(int num);
    
    
    int selectedLoaderRow;
    
    
    //int currentPlayingIndex;
    
    //Lights
    aLights *myLights;
    
    
    //MIDI
    //void fireNote(); //removed 20130920
    void noteIn();
    void newMidiMessage(ofxMidiMessage& eventArgs);
    
    ofxMidiIn midiIn;
	ofxMidiMessage midiMessage;
    
    
    //SYPHON
    ofxSyphonServer mainOutputSyphonServer;
    ofxSyphonClient mClient;
    
    //ALEMBIC
    void setupABCLoaders(int num);
    
    
    vector<ofxAlembic::Reader *> myABCs;
    vector<abcModel> abcModels;
    
     
    
};
