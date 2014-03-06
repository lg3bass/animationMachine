#include "testApp.h"

/*
 * Starter file for Alembic Media Player
 *
 *
 *
 */


#include "ofxAlembic.h"


//TO-DO: I think I want to do 20 per screen.
const int numOfABC = 28;

//--------------------------------------------------------------
void testApp::setup(){

    
    doneBuilding = false;
    
    //turn this on to show lots of output data.
    //ofSetLogLevel(OF_LOG_VERBOSE);
    
    //setup Midi and set input source to (0=IAC, 1=Network Midi)
    setupMidi(0);
    
    //OSC
    // listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);
    current_msg_string = 0;
    
    
    
    //SYPHON
    //much nicer than processing's vs.:)
    ofSetWindowTitle("Lights Camera Alembic Syphon");
    mainOutputSyphonServer.setName("Screen Output");
    
 	mClient.setup();
    
    mClient.setApplicationName("Simple Server");
    mClient.setServerName("");
    //END SYPHON
    
    
    ofBackground(0);
    //ofSetFrameRate(30);
    ofEnableAntiAliasing();
    //ofDisableAntiAliasing();
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    ofSetGlobalAmbientColor(ofColor(0, 0, 0));
    ofSetSmoothLighting(true);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    //used in the buttons at the top.
    currentScene = 1;
    
    //LIGHTS
    myLights = aLights::Instance();
    
    //GUI
    myGui = aGui::Instance();
    myGui->setup(numOfABC);//set the nmber of loader slots
    
    //aTrackGui
    myTrackGui = aTrackGui::Instance();
    myTrackGui->setup();
    
    gui_loader_Alloc = false;
    
    //reset lights and lights gui.
    reset();
    
    saveCam.setup(&cam,"xml"); // add you ofeasycam and the folder where the xmls are
    //saveCam.enableSave(); // by defaul the listion is on you can actival with enableSave;
    //saveCam.disableSave(); // or disable key save wtih this
    
    saveCam.isSettingCam = false;//flag to display the press 'n' to save message.
    
    
}



void testApp::reset()
{
    myLights->reset();
    myGui->reset();
    myTrackGui->reset();
    
    cam.reset();
    
    
    
    //*****NOT NEEDED, switched to ofNode()
    //this sets the distance from the origin.
    //in Maya this translates to +30 Z-units 
    //cam.setDistance(1500);
    
    
    
    //set up the default gui_loader
    setGUI_loader(numOfABC);
    
    //set this up to read from the button at the top.
    loadScene(currentScene); //run this on initial setup.
    
    //start with all the gui's turned off
    myGui->gui->setVisible(false);
    myGui->gui2->setVisible(false);
    gui_loader->setVisible(false);
    myTrackGui->gui->setVisible(false);
}

void testApp::loadScene(int sceneIndex){
    
    string filename = "GUI/gui_loader_Settings_" + ofToString(sceneIndex) + ".xml";
    
    //load the settings
    gui_loader->loadSettings(filename);
    
    gui_loader_Alloc = true;
    
    //Load Alembic Models. gather data from gui_loader and load the models.
    setupABCLoaders(numOfABC);
    
    doneBuilding = true;
    
}

void testApp::clearScene(int sceneIndex){

    clearParamsInABCloaders(numOfABC);
    
    
    
    //string filename = "GUI/gui_loader_Settings_" + ofToString(sceneIndex) + ".xml";
    
    //load the settings
    //gui_loader->loadSettings(filename);
    
    //gui_loader_Alloc = true;
    
    //Load Alembic Models. gather data from gui_loader and load the models.
    //setupABCLoaders(numOfABC);
    
    //doneBuilding = true;
    
}


void testApp::saveScene(int sceneIndex){
    
    gui_loader_Alloc = false;
    doneBuilding = false;
    
    string filename = "GUI/gui_loader_Settings_" + ofToString(sceneIndex) + ".xml";
    
    gui_loader->saveSettings(filename);
    
    gui_loader_Alloc = true;
    
    //re-setup loaders and commit changes.
    setupABCLoaders(numOfABC);
    
    doneBuilding = true;
    
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    
    ofSetWindowTitle("size:"+ofToString(ofGetWidth())+","+ofToString(ofGetHeight())+", port: "+ofToString(midiIn.getPort())+", fps: "+ofToString(ofGetFrameRate()));
    
    
    //OSC
    // hide old messages
	for(int i = 0; i < NUM_MSG_STRINGS; i++){
		if(timers[i] < ofGetElapsedTimef()){
			msg_strings[i] = "";
		}
	}

    // check for waiting messages
	while(receiver.hasWaitingMessages()){
        // get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
        // unrecognized message: display on the bottom of the screen
        string msg_string;
        msg_string = m.getAddress();
        msg_string += ": ";
        for(int i = 0; i < m.getNumArgs(); i++){
            // get the argument type
            msg_string += m.getArgTypeName(i);
            msg_string += ":";
            // display the argument - make sure we get the right type
            if(m.getArgType(i) == OFXOSC_TYPE_INT32){
                msg_string += ofToString(m.getArgAsInt32(i));
            }
            else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
                msg_string += ofToString(m.getArgAsFloat(i));
            }
            else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
                msg_string += m.getArgAsString(i);
            }
            else{
                msg_string += "unknown";
            }
        }
        // add to the list of strings to display
        msg_strings[current_msg_string] = msg_string;
        timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
        current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
        // clear the next line
        msg_strings[current_msg_string] = "";
        
        
    }
    
    
    //KEEP
    //loop through all the abc files and calculate the time in each.
    for(int i = 0; i < abcModels.size(); i++){
        //animTime is passed but not used.
        abcModels[i].calcTime(myGui->animTime);
        //abcModels[i].update();
    }
    
    if (myGui->doReset)
        reset();
    
    
    //this is the center point around which the light rotates.
    //TO-DO: use this same point as the world space position of the axis.
    ofVec3f center = ofVec3f(0);
    if (myGui->autoRotatePoint) {
        myLights->point.setPosition(cos(ofGetElapsedTimef()*.6f) * myGui->radius * 2 + center.x,sin(ofGetElapsedTimef()*.8f) * myGui->radius * 2 + center.y,-cos(ofGetElapsedTimef()*.8f) * myGui->radius * 2 + center.z);
    }
    
    
    saveCam.update();
    

    
}


//--------------------------------------------------------------
void testApp::draw(){

    //register the cursor position.
    float dx = dragPt.x;
	float dy = dragPt.y;
    
    // Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (myGui->cull)
        glEnable(GL_CULL_FACE);
    
    cam.begin();
    
    if (myGui->showAxis){
        //draw x/y/z axis
        ofSetLineWidth(5);
        ofSetColor(255, 0, 0);
        ofLine(0, 0, 200, 0);
        ofSetColor(0, 255, 0);
        ofLine(0, 0, 0, 200);
        ofSetColor(0, 0, 255);
        ofLine(0, 0, 0, 0, 0, 200);
    }
    
    if (myGui->useAmb)
        myLights->amb.enable();
    else
        myLights->amb.disable();
    
    if (myGui->useDir)
        myLights->dir.enable();
    else
        myLights->dir.disable();
    
    if (myGui->useSpot)
        myLights->spot.enable();
    else
        myLights->spot.disable();
    
    if (myGui->usePoint)
        myLights->point.enable();
    else
        myLights->point.disable();
    
    //enabling lights above will call ofEnableLighting() which will
    //enable GL_COLOR_MATERIAL. This which means calls to ofSetColor()
    //would change material properties, which we dont want here. We
    //want it controlled by the sliders, so we turn it off then turn
    //on our material
    glDisable(GL_COLOR_MATERIAL);
     
    //we could enable GL_COLOR_MATERIAL set the color here
    //    ofSetColor(255);
    
    //THIS IS WHERE THE ANIMATION HAPPENS.
    glPushMatrix();
    
    //old method
    /*
     for(int i = 0; i < abcModels.size(); i++){
     //glRotatef(ofGetElapsedTimef()*.8f,0,0,1); //rotating the models.
     abcModels[i].draw();
     }
     */
    
    for(int t=0;t<tracks.size();t++){
        if(tracks[t].myLdrs.size()>0){
            for(int i=0; i<tracks[t].myLdrs.size();i++){
                
                //this actually needs to be an array of materials.  Let also move the materials out of the aLights class.
                
                //TO-DO:
                //--currently this references materials in the aLights class. Need to move to the aTrackGui
                
                if(tracks[t].myLdrs[i].x<4){
                    myLights->material1.begin();
                } else {
                    myLights->material2.begin();
                }
                
                if(!showLdr){
                    //myLights->("material"+ofToString(i)).begin();
                    if(abcModels[tracks[t].myLdrs[i].x].isActive) {
                        
                        abcModels[tracks[t].myLdrs[i].x].draw();
                    
                    }
                }
                //myLights->("material"+ofToString(i)).end();
                
                if(tracks[t].myLdrs[i].x<4){
                    myLights->material1.end();
                } else {
                    myLights->material2.end();
                }
            }
            
        }
        
    }
    
    //myLights->material1.end();
    
    glPopMatrix();
    
    myLights->amb.disable();
    myLights->dir.disable();
    myLights->spot.disable();
    myLights->point.disable();
        
    ofDisableLighting();
    
    if (myGui->useSpot && myGui->showSpotSource) {
        ofSetColor(myLights->spot.getDiffuseColor());
        myLights->spot.draw();
    }
    
    if (myGui->usePoint && myGui->showPointSource) {
        ofSetColor(myLights->point.getDiffuseColor());
        myLights->point.draw();
    }
    
    if (myGui->useDir && myGui->showDirSource) {
        ofSetColor(myLights->dir.getDiffuseColor());
        myLights->dir.draw();
    }
    
    cam.end();
    
    if (myGui->cull)
        glDisable(GL_CULL_FACE);
    
    ofSetColor(255, 255, 255);
    
    ofEnableAlphaBlending();
    
    mClient.draw(50, 50);
    
	mainOutputSyphonServer.publishScreen();
    

    saveCam.information();
    
    //OSC TEST
    string buf;
	buf = "listening for osc messages on port" + ofToString(PORT);
	ofDrawBitmapString(buf, 10, 46);
    
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
		ofDrawBitmapString(msg_strings[i], 10, 58 + 15 * i);
	}
    
    
}// end draw


//--------------------------------------------------------------
void testApp::exit() {
    
	myGui->exit();//save the gui XML
    myTrackGui->exit();
    
    //gui_loader->saveSettings("GUI/gui_loader_Settings.xml");//take out because I have a load button now
	delete gui_loader;
    
	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
}


//--------------------------------------------------------------
void testApp::setupABCLoaders(int num) {
    
    myABCs.clear();
    abcModels.clear();
    
    cout << "myABCs:" << myABCs.size() << "-" << "abcModels:" << abcModels.size() << endl;
    
    //Set up all the ABCLoaders.
    for(int i = 0; i < num; i++){
        
        //create abcReader
        ofxAlembic::Reader *newabc = new ofxAlembic::Reader;
        myABCs.push_back(newabc);
        
        //get the contents of the Text Input field in gui_loader. This contains the file path.
        ofxUITextInput *loader_label = (ofxUITextInput *)gui_loader->getWidget(ofToString(i)+"_TRK_READER");
        
        //add blank abcModels 
        abcModels.push_back(abcModel(i)); //QUESTION: Why is the asterick needed on myABCs?
        
        
        //setup the abcModel(s)
        //syntax: init(file,divisions,reader,id)
        abcModels[i].init(loader_label->getTextString(),0,*myABCs[i]);
        
        //componsate for the Maya cm->m
        abcModels[i].setScale(25.0);
        
    }
    
    tracks.clear();
    
    //Set up all the Tracks.
    createTracks(num);
    
    //Set the attributes in the loaders based on what is currently loaded in the GUI.
    setParamsInABCloaders(num);
    
    
    // OUTPUT/TRACE
    //proof is in the pudding. output what we go so far.
    cout << "TRACKS:" << "-----------------------------" << endl;
    
    for(int t=0;t<tracks.size();t++){
        if(tracks[t].myLdrs.size()>0){
            tracks[t].getInfo();
        }
        for(int i=0; i<tracks[t].myLdrs.size();i++){
            abcModels[tracks[t].myLdrs[i].x].INFOlaunchingToPlay();
        }
    }
    
    
}

//--------------------------------------------------------------
void testApp::createTracks(int num){
    //var num is not being used right now.
    
    
    //create 10 empty tracks
    for(int t=0;t<10;t++){
        aTrack::aTrack *newTrack = new aTrack::aTrack("empty", false, t);
        tracks.push_back(*newTrack);
    }
    
    //loop through the loader instances and pull the data from the UI.
    for(int i = 0; i < abcModels.size(); i++){
        string behavior = "";
        int midiChannel = 0;
        bool playsOnNote = false;
        
        
        //midi track/channel
        ofxUINumberDialer *midiDialer = (ofxUINumberDialer *)gui_loader->getWidget(ofToString(i)+"_TRK_MIDI");
        midiChannel = floor(midiDialer->getValue());
        
        //note number
        
        ofxUINumberDialer *noteDialer = (ofxUINumberDialer *)gui_loader->getWidget(ofToString(i)+"_TRK_NOTE");
        int midiNote = noteDialer->getValue();
        if(midiNote > 0) {
            playsOnNote = true;
        }
        
        //behavior
        ofxUIRadio *butType = (ofxUIRadio *)gui_loader->getWidget(ofToString(i)+"_TRK_TYPE");
        vector<ofxUIToggle*> toggles = butType->getToggles();
        for(int j = 0; j < toggles.size(); j++) {
            if(toggles[j]->getValue() == 0) {
                behavior = "random";
            } else {
                behavior = "linear";
            }
        }
        
        //segments
        ofxUINumberDialer *segDialer = (ofxUINumberDialer *)gui_loader->getWidget(ofToString(i)+"_TRK_SEGMENTS");
        int seg = segDialer->getValue();
        
        
        //What loaders are in each track.
        for(int l=0;l<tracks.size();l++){
            if(l+1 == midiChannel){
                if(tracks[l].behavior == "empty"){
                    tracks[l].behavior = behavior;
                    tracks[l].playNote = playsOnNote;
                    tracks[l].segments = seg;
                }
                ofVec2f id;
                id.set(i,midiNote);
                tracks[l].myLdrs.push_back(id);
            }// if
        }// for l
    }//for i
    
}

//--------------------------------------------------------------
void testApp::setParamsInABCloaders(int num) {
    
    /*
     //params to set
     //public
     float segments;
     float clipSpeedMod;
     float midiChannel;
     float midiNote;
     int ldrType; // 0=random, 1=linear
     int trackMode; // 0=noteOn, 1=noteOn/noteOff
     float segLength;
     int currentSegment;
     
     */
    
    
    // loop through the loaders and set the params.
    for(int i = 0; i < num; i++){
        
        
        ofxUISlider *trackSlider = (ofxUISlider *)gui_loader->getWidget(ofToString(i)+"_TRK_SPD");
        trackSlider->setLabelVisible(false);
        
        //track speed
        ofxUINumberDialer *trackDialer = (ofxUINumberDialer *)gui_loader->getWidget(ofToString(i)+"_TRK_SPEED");
        abcModels[i].clipSpeedMod = trackDialer->getValue();
        
        
        //track midi
        ofxUINumberDialer *midiDialer = (ofxUINumberDialer *)gui_loader->getWidget(ofToString(i)+"_TRK_MIDI");
        abcModels[i].midiChannel = midiDialer->getValue();
        
        //track note
        ofxUINumberDialer *noteDialer = (ofxUINumberDialer *)gui_loader->getWidget(ofToString(i)+"_TRK_NOTE");
        abcModels[i].midiNote = noteDialer->getValue();
        
        //track type
        ofxUIRadio *butType = (ofxUIRadio *)gui_loader->getWidget(ofToString(i)+"_TRK_TYPE");
        vector<ofxUIToggle*> type_toggles = butType->getToggles();
        if(type_toggles[0]->getValue()) {
            abcModels[i].ldrType = 0;//reporting[1,0] then set "random:0"
        } else {
            abcModels[i].ldrType = 1;
        }
        
        
        //track mode
        ofxUIRadio *butMode = (ofxUIRadio *)gui_loader->getWidget(ofToString(i)+"_TRK_MODE");
        vector<ofxUIToggle*> mode_toggles = butMode->getToggles();
        if(mode_toggles[0]->getValue()) {
            abcModels[i].trackMode = 0;//reporting[1,0] then set "noteOn:0"
        } else {
            abcModels[i].trackMode = 1;
        }
        
        //segments
        ofxUINumberDialer *segDialer = (ofxUINumberDialer *)gui_loader->getWidget(ofToString(i)+"_TRK_SEGMENTS");
        abcModels[i].segments = segDialer->getValue();
        
        //seg len
        ofxUINumberDialer *segLnDialer = (ofxUINumberDialer *)gui_loader->getWidget(ofToString(i)+"_TRK_SEGLN");
        abcModels[i].segLength = segLnDialer->getValue();
        
        
        abcModels[i].setClipMarkers(0);
       
        
    }
    
}

//--------------------------------------------------------------
void testApp::clearParamsInABCloaders(int num) {
    
    
    // loop through the loaders and clear the params.
    for(int i = 0; i < num; i++){
        
        ofxUITextInput *uitrack = (ofxUITextInput *)gui_loader->getWidget(ofToString(i)+"_TRK_READER");
        uitrack->setTextString("empty");
        
        //track speed
        ofxUINumberDialer *trackDialer = (ofxUINumberDialer *)gui_loader->getWidget(ofToString(i)+"_TRK_SPEED");
        trackDialer->setValue(0.07);
        
        
        //track midi
        ofxUINumberDialer *midiDialer = (ofxUINumberDialer *)gui_loader->getWidget(ofToString(i)+"_TRK_MIDI");
        midiDialer->setValue(1);
        
        //track note
        ofxUINumberDialer *noteDialer = (ofxUINumberDialer *)gui_loader->getWidget(ofToString(i)+"_TRK_NOTE");
        noteDialer->setValue(0);
        
        //track type
        ofxUIRadio *butType = (ofxUIRadio *)gui_loader->getWidget(ofToString(i)+"_TRK_TYPE");
        butType->activateToggle(ofToString(i)+"_random");
        
        
        //track mode
        ofxUIRadio *butMode = (ofxUIRadio *)gui_loader->getWidget(ofToString(i)+"_TRK_MODE");
        butMode->activateToggle(ofToString(i)+"_note_Off");
        
        
        //segments
        ofxUINumberDialer *segDialer = (ofxUINumberDialer *)gui_loader->getWidget(ofToString(i)+"_TRK_SEGMENTS");
        segDialer->setValue(0);
        
        //seg len
        ofxUINumberDialer *segLnDialer = (ofxUINumberDialer *)gui_loader->getWidget(ofToString(i)+"_TRK_SEGLN");
        segLnDialer->setValue(0);
        
        
        abcModels[i].setClipMarkers(0);
    }
    
}

//--------------------------------------------------------------
void testApp::resetAnimation(int num){
    // loop through the loaders and move the play head back to the beginning.
    for(int i = 0; i < num; i++){
     
        abcModels[i].currentSegment = -1;
        
    }
}

//--------------------------------------------------------------
void testApp::setupMidi(int input) {
	// print input ports to console
	midiIn.listPorts(); // via instance
	//ofxMidiIn::listPorts(); // via static as well
	
	// open port by number (you may need to change this)
	midiIn.openPort(input);
	//midiIn.openPort("IAC Driver Bus 1");	// by name
	//midiIn.openVirtualPort("ofxMidiIn Input");	// open a virtual port
	
	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, false, false);
	
	// add testApp as a listener
	midiIn.addListener(this);
	
	// print received messages to the console
	midiIn.setVerbose(false);
    
}


//--------------------------------------------------------------
void testApp::toggleMidiPort() {
    
    //test if the existing port is open
    if(midiIn.isOpen()){
        //get the existing port
        int port = midiIn.getPort();
        
        //close the existing port
        midiIn.closePort();
        
        //open the new port
        if (port == 0){
            midiIn.openPort(1);
        } else {
            midiIn.openPort(0);
        }
     }
}


//--------------------------------------------------------------
void testApp::newMidiMessage(ofxMidiMessage& msg) {
    
	// make a copy of the latest message
	midiMessage = msg;
    
    //trigger animation
    noteIn();
}

//--------------------------------------------------------------
void testApp::noteIn() {
    
    if (ofxMidiMessage::getStatusString(midiMessage.status) == "Note On") {
        
        bool specificNote = false;
        int currentPlayingIndex = 0;
        
        //cout << ofxMidiMessage::getStatusString(midiMessage.status) << " -- value:" << midiMessage.value << " pitch: " << midiMessage.pitch << " delta: " << midiMessage.deltatime << " ch: " << midiMessage.channel << endl;
        
        //make sure the track is not empty. Does the track have content.
        if(tracks[midiMessage.channel-1].myLdrs.size()>0){
            
            //loop through the loader and look for one that has a specific note.
            for(int i=0; i<tracks[midiMessage.channel-1].myLdrs.size();i++) {
                if(tracks[midiMessage.channel-1].myLdrs[i].y > 0){
                    //play the specific note
                    if(midiMessage.pitch == tracks[midiMessage.channel-1].myLdrs[i].y) {
                        currentPlayingIndex = tracks[midiMessage.channel-1].myLdrs[i].x;
                        if(tracks[midiMessage.channel-1].behavior == "random"){
                            abcModels[currentPlayingIndex].playSegment(0);
                        } else {
                            abcModels[currentPlayingIndex].playSegment(1);
                        }
                        
                        //abcModels[currentPlayingIndex].isAnimating = true;// this is already in playSegment
                        specificNote = true;
                        //goto jmp;// works but dont like it.
                    }
                }
            }//end for
            
            if(!specificNote) {
                //exclude the tracks that are assigned to a specific note.
                int availLdrs = 0;
                for(int j=0; j<tracks[midiMessage.channel-1].myLdrs.size();j++){
                    
                    if(tracks[midiMessage.channel-1].myLdrs[j].y == 0 ){
                        if(!abcModels[tracks[midiMessage.channel-1].myLdrs[j].x].isAnimating) {
                            availLdrs++;
                        }
                    }
                }
                
                
                // check if there are any loaders avail. if not dont play.
                if(availLdrs > 0) {
                    //RANDOM
                    if(tracks[midiMessage.channel-1].behavior == "random"){
                        do {
                            currentPlayingIndex = pickRandomLoader(midiMessage.channel-1);//pickRandomLoader: applicable for linear mode???
                        } while (abcModels[currentPlayingIndex].isAnimating);
                        
                        //PLAY THE MODEL IN RANDOM MODE
                        abcModels[currentPlayingIndex].playSegment(0);//0 = play random
                        
                    }
                    //LINEAR
                    if(tracks[midiMessage.channel-1].behavior == "linear") {
                        for(int i=0; i<tracks[midiMessage.channel-1].myLdrs.size();i++) {
                            if(!abcModels[tracks[midiMessage.channel-1].myLdrs[i].x].isAnimating) {
                                currentPlayingIndex = tracks[midiMessage.channel-1].myLdrs[i].x;
                                abcModels[currentPlayingIndex].playSegment(1); //1 = play linear.
                            }
                        }
                    }//end linear
                }//end availLdrs
            }//if specificNote
            
        }//if has content.
        
        //jmp://end jmp point. Not used but a good example.
        
        //cout << "Note ON - currentPlayingIndex:" << currentPlayingIndex << endl;
        
        
    } else if (ofxMidiMessage::getStatusString(midiMessage.status) == "Note Off"){
        //I think i have to mimic the note on behavior.
        
        //cout << ofxMidiMessage::getStatusString(midiMessage.status) << " -- value:" << midiMessage.value << " pitch: " << midiMessage.pitch << " delta: " << midiMessage.deltatime << " ch: " << midiMessage.channel << endl;
        
        //make sure the track is not empty. Does the track have content.
        if(tracks[midiMessage.channel-1].myLdrs.size()>0){
            
            //loop through the loaders.
            for(int i=0; i<tracks[midiMessage.channel-1].myLdrs.size();i++) {
                
                //logic: there should only be one that is animating at a time in the track. Is the loader Animating?
                if(abcModels[tracks[midiMessage.channel-1].myLdrs[i].x].isAnimating) {
                    
                    //if it is animating, match against which midi note is playing.
                    if(tracks[midiMessage.channel-1].myLdrs[i].y == midiMessage.pitch){
                     
                        //only actually do this if the trackMode is 1.
                        //reminder: trackMode = NoteOn or NoteOff enabled.
                        //trackMode 0 = animate on noteOn only.
                        //trackMode 1 = animate noteOn then finish animating on noteOff message.
                        if(abcModels[tracks[midiMessage.channel-1].myLdrs[i].x].trackMode == 1) {
                            
                            abcModels[tracks[midiMessage.channel-1].myLdrs[i].x].isAnimating = false;
                            abcModels[tracks[midiMessage.channel-1].myLdrs[i].x].isHolding = true;
                            
                            //cout << "Note OFF - match currentPlayingIndex:" << tracks[midiMessage.channel-1].myLdrs[i].x << endl;
                            
                        }//end if trackMode
                    }//end match against midi note.
                    
                    
                }//end if animating
            }//end for
        }//end if midiChannel
    }//end elseIf
    
    
}


//-------------------------------------------------------------
int testApp::pickRandomLoader(int ch){
    
    int selector = 0;
    //pick a random loader within the track.
    
    do{
        selector = ofRandom(tracks[ch].myLdrs.size());
    } while (tracks[ch].myLdrs[selector].y > 0);
    
    
    int abc_index = tracks[ch].myLdrs[selector].x;
    
    
    
    
    //output: Show what is selected to play
    //tracks[ch].whatIsPlaying(selector);
    
    return abc_index;
    
}


//--------------------------------------------------------------
void testApp::LoaderGuiEvent(ofxUIEventArgs &e)
{
    
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    
    //get the track number from the name.
    vector<string> result=ofSplitString(name, "_");
    int row = ofToInt(result[0]);
    selectedLoaderRow = row;
    
    
    if(doneBuilding){ //only if the loaders are done.
        
        //if current row eq last selected row on file then
        
        if(ofIsStringInString(name, "CLEAR")){
            ofxUILabelButton *clearBut = (ofxUILabelButton *)gui_loader->getWidget("CLEAR");
            if(clearBut->getValue()){
                cout << "Clear Scene: " << currentScene << endl;
                clearScene(currentScene);
            }
        } else if (ofIsStringInString(name, "LOAD")){
            ofxUILabelButton *loadBut = (ofxUILabelButton *)gui_loader->getWidget("LOAD");
            if(loadBut->getValue()){
                cout << "load Scene: " << currentScene << endl;
                loadScene(currentScene);
            }
        } else if (ofIsStringInString(name, "SAVE")){
            ofxUILabelButton *saveBut = (ofxUILabelButton *)gui_loader->getWidget("SAVE");
            if(saveBut->getValue()){
                cout << "save Scene: " << currentScene << endl;
                saveScene(currentScene);
            }
        } else if (ofIsStringInString(name, "BANKS")){
            ofxUIToggleMatrix *tbanks = (ofxUIToggleMatrix *)gui_loader->getWidget("BANKS");
            vector<ofxUIToggle*> saveTog = tbanks->getToggles();
            for(int i=0; i<saveTog.size();i++){
                if(saveTog[i]->getValue() == 1){
                    //ofxUISetColor(ofColor::red);
                    currentScene = i+1;
                    cout << "currentScene:" << currentScene << endl;
                    
                }
                
            }
        } else if (ofIsStringInString(name, "TRK_PLAY")){
            ofxUIImageToggle *playBut = (ofxUIImageToggle *)gui_loader->getWidget(ofToString(row)+"_TRK_PLAY");
            
            if(playBut->getValue() == 1){
                abcModels[row].isDemo = true;
            }
            if(playBut->getValue() == 0){
                abcModels[row].isDemo = false;
            }
            
            cout << ofToString(row) << "_TRK_PLAY" << ":ofxUIImageToggle (Play Button Toggle) >" << playBut->getValue() << endl;
        } else if (ofIsStringInString(name, "TRK_SPEED")){
            ofxUINumberDialer *trackDialer = (ofxUINumberDialer *)gui_loader->getWidget(ofToString(row)+"_TRK_SPEED");
            abcModels[row].clipSpeedMod = trackDialer->getValue();//abcModel.cpp
            
            cout << ofToString(row) << "_TRK_SPEED" << ":ofxUINumberDialer (Clip speed modifier) >" << trackDialer->getValue() << endl;
            
        } else if (ofIsStringInString(name, "TRK_MIDI")){
            ofxUINumberDialer *midiDialer = (ofxUINumberDialer *)gui_loader->getWidget(ofToString(row)+"_TRK_MIDI");
            abcModels[row].midiChannel = midiDialer->getValue();
            cout << ofToString(row) << "_TRK_MIDI" << ":ofxUINumberDialer (Midi Channel/Track) >" << midiDialer->getValue() << endl;
            
        } else if (ofIsStringInString(name, "TRK_NOTE")){
            ofxUINumberDialer *noteDialer = (ofxUINumberDialer *)gui_loader->getWidget(ofToString(row)+"_TRK_NOTE");
            abcModels[row].midiNote = noteDialer->getValue();
            if(noteDialer->getValue() > 0){
                tracks[abcModels[row].midiChannel-1].playNote = true;
            } else {
                tracks[abcModels[row].midiChannel-1].playNote = false;
            }
            
            cout << ofToString(row) << "_TRK_NOTE" << ":ofxUINumberDialer (Midi Note Number) >" << noteDialer->getValue() << endl;
            
        } else if (ofIsStringInString(name, "random")){
            ofxUIRadio *butType = (ofxUIRadio *)gui_loader->getWidget(ofToString(row)+"_TRK_TYPE");
            vector<ofxUIToggle*> toggles = butType->getToggles();
            cout << ofToString(row) << "_TRK_TYPE" << ":UIRadio (Random On) >" << "[";
            for(int i = 0; i < toggles.size(); i++) {
                cout << toggles[i]->getValue() << (i<toggles.size()-1?",":"]");
                if(toggles[i]->getValue()) {
                    //cout << "RANDOM - pass 0 to abcModel::abcModel" << endl;
                    //pass ldrType to the loader
                    abcModels[row].ldrType = 0;
                    tracks[abcModels[row].midiChannel-1].behavior = "random";
                }
            }
            cout << endl;
            //cout << "Random Button : " << result[0] << "-" << butRandom->getValue() << endl;
        } else if (ofIsStringInString(name, "seq")){
            ofxUIRadio *butType = (ofxUIRadio *)gui_loader->getWidget(ofToString(row)+"_TRK_TYPE");
            vector<ofxUIToggle*> toggles = butType->getToggles();
            cout << ofToString(row) << "_TRK_TYPE" << ":UIRadio (Sequence On) >"  << "[";
            for(int i = 0; i < toggles.size(); i++) {
                cout << toggles[i]->getValue() << (i<toggles.size()-1?",":"]");
                if(toggles[i]->getValue()) {
                    //cout << "LINEAR - pass 1 to abcModel::abcModel" << endl;
                    //pass ldrType to the loader
                    abcModels[row].ldrType = 1;
                    tracks[abcModels[row].midiChannel-1].behavior = "linear";
                }
            }
            cout << endl;
        } else if (ofIsStringInString(name, "note_On")){
            ofxUIRadio *butMode = (ofxUIRadio *)gui_loader->getWidget(ofToString(row)+"_TRK_MODE");
            vector<ofxUIToggle*> toggles = butMode->getToggles();
            cout << ofToString(row) << "_TRK_MODE" << ":UIRadio (Trigger on Note On) >" << "[";
            for(int i = 0; i < toggles.size(); i++) {
                cout << toggles[i]->getValue() << (i<toggles.size()-1?",":"]");
                if(toggles[i]->getValue()) {
                    abcModels[row].trackMode = 0;
                }
            }
            cout << endl;
        } else if (ofIsStringInString(name, "note_Off")){
            ofxUIRadio *butMode = (ofxUIRadio *)gui_loader->getWidget(ofToString(row)+"_TRK_MODE");
            vector<ofxUIToggle*> toggles = butMode->getToggles();
            cout << ofToString(row) << "_TRK_MODE" << ":UIRadio (Trigger on Note On & Note Off) >"  << "[";
            for(int i = 0; i < toggles.size(); i++) {
                cout << toggles[i]->getValue() << (i<toggles.size()-1?",":"]");
                if(toggles[i]->getValue()) {
                    abcModels[row].trackMode = 1;
                }
            }
            cout << endl;
        } else if (ofIsStringInString(name, "TRK_SEGMENTS")){
            ofxUINumberDialer *segDialer = (ofxUINumberDialer *)gui_loader->getWidget(ofToString(row)+"_TRK_SEGMENTS");
            abcModels[row].segments = segDialer->getValue();
            tracks[abcModels[row].midiChannel-1].segments = segDialer->getValue();
            cout << ofToString(row) << "_TRK_NOTE" << ":ofxUINumberDialer (ABC sections) >" << segDialer->getValue() << endl;
            
        } else if (ofIsStringInString(name, "TRK_SEGLN")){
            ofxUINumberDialer *segLnDialer = (ofxUINumberDialer *)gui_loader->getWidget(ofToString(row)+"_TRK_SEGLN");
            abcModels[row].segLength = segLnDialer->getValue();
            cout << ofToString(row) << "_TRK_SEGLN" << ":ofxUINumberDialer (ABC segment length[time]) >" << segLnDialer->getValue() << endl;
        }
        
    }
    
    
}

void testApp::setGUI_loader(int num){
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float w = 1100 - xInit * 2;
    float vertH = 40;
    float h = 8;
    
    if (gui_loader_Alloc) {
        ofRemoveListener(gui_loader->newGUIEvent, this, &testApp::LoaderGuiEvent);
        delete gui_loader;
    }
    
    gui_loader = new ofxUICanvas(0, 0, w + xInit * 2, ofGetHeight());
    gui_loader->setFont("GUI/HelveticaNeueLTStd-Bd.otf");
    gui_loader->setFontSize(OFX_UI_FONT_SMALL, 6);
    
    
    gui_loader->addWidgetRight(new ofxUILabelButton(50,false,"CLEAR",OFX_UI_FONT_SMALL));
    gui_loader->addWidgetRight(new ofxUILabelButton(50,false,"LOAD",OFX_UI_FONT_SMALL));
    gui_loader->addWidgetRight(new ofxUILabelButton(50,false,"SAVE",OFX_UI_FONT_SMALL));
    gui_loader->addWidgetRight(new ofxUIToggleMatrix(18,18,1,6,"BANKS",OFX_UI_FONT_SMALL));
    ofxUIToggleMatrix* mtx = (ofxUIToggleMatrix *) gui_loader->getWidget("BANKS");
    mtx->setAllowMultiple(false);
    
    
    gui_loader->addSpacer(240,2);
    
    for(int i = 0; i < num; i++)
    {
        string counter = ofToString(i);
        
        gui_loader->addWidgetDown(new ofxUIImageToggle(20,20,false,"GUI/play.png",ofToString(i)+"_TRK_PLAY"));
        //gui_loader->addWidgetRight(new ofxUILabelButton(50,false,"LOAD",OFX_UI_FONT_SMALL));
        gui_loader->addWidgetRight(new ofxUITextInput(300, ofToString(i)+"_TRK_READER", "empty", OFX_UI_FONT_SMALL));
        
        gui_loader->addWidgetRight(new ofxUISlider(ofToString(i)+"_TRK_SPD", 0.01f, 0.12f, 0.05, 100, 18));
        
        gui_loader->addWidgetRight(new ofxUINumberDialer(0, 0.2, 0.0, 2, ofToString(i)+"_TRK_SPEED", OFX_UI_FONT_SMALL));
        gui_loader->addWidgetRight(new ofxUINumberDialer(1, 10, 10, 0, ofToString(i)+"_TRK_MIDI", OFX_UI_FONT_SMALL));
        gui_loader->addWidgetRight(new ofxUINumberDialer(0, 88, 0.0, 0, ofToString(i)+"_TRK_NOTE", OFX_UI_FONT_SMALL));
        vector<string> htype; htype.push_back(ofToString(i)+"_random"); htype.push_back(ofToString(i)+"_seq");
        gui_loader->addWidgetRight(new ofxUIRadio(ofToString(i)+"_TRK_TYPE", htype,OFX_UI_ORIENTATION_HORIZONTAL,20,20));
        
        
        vector<string> hmode; hmode.push_back(ofToString(i)+"_note_On"); hmode.push_back(ofToString(i)+"_note_Off");
        gui_loader->addWidgetRight(new ofxUIRadio(ofToString(i)+"_TRK_MODE", hmode,OFX_UI_ORIENTATION_HORIZONTAL,20,20));
        gui_loader->addWidgetRight(new ofxUINumberDialer(1, 50, 1.0, 0,ofToString(i)+ "_TRK_SEGMENTS", OFX_UI_FONT_SMALL));
        gui_loader->addWidgetRight(new ofxUINumberDialer(0, 600, 30, 0,ofToString(i)+ "_TRK_SEGLN", OFX_UI_FONT_SMALL));
        //gui_loader->addWidgetRight(new ofxUIToggle("random",false,20,20));
        
    }
    // set the labels over the top row
    gui_loader->addWidgetNorthOf(new ofxUISpacer(40,2,"speed_spacer"),"0_TRK_SPEED");
    gui_loader->addWidgetNorthOf(new ofxUILabel("SPEED",OFX_UI_FONT_SMALL),"speed_spacer");
    
    gui_loader->addWidgetNorthOf(new ofxUISpacer(30,2,"midi_spacer"),"0_TRK_MIDI");
    gui_loader->addWidgetNorthOf(new ofxUILabel("MIDI",OFX_UI_FONT_SMALL),"midi_spacer");
    
    gui_loader->addWidgetNorthOf(new ofxUISpacer(30,2,"note_spacer"),"0_TRK_NOTE");
    gui_loader->addWidgetNorthOf(new ofxUILabel("NOTE",OFX_UI_FONT_SMALL),"note_spacer");
    
    gui_loader->addWidgetNorthOf(new ofxUISpacer(130,2,"type_spacer"),"0_TRK_TYPE");
    gui_loader->addWidgetNorthOf(new ofxUILabel("TYPE",OFX_UI_FONT_SMALL),"type_spacer");
    
    gui_loader->addWidgetNorthOf(new ofxUISpacer(120,2,"mode_spacer"),"0_TRK_MODE");
    gui_loader->addWidgetNorthOf(new ofxUILabel("MODE",OFX_UI_FONT_SMALL),"mode_spacer");
    
    gui_loader->addWidgetNorthOf(new ofxUISpacer(30,2,"seg_spacer"),"0_TRK_SEGMENTS");
    gui_loader->addWidgetNorthOf(new ofxUILabel("SEG",OFX_UI_FONT_SMALL),"seg_spacer");
    
    gui_loader->addWidgetNorthOf(new ofxUISpacer(45,2,"segln_spacer"),"0_TRK_SEGLN");
    gui_loader->addWidgetNorthOf(new ofxUILabel("LENGTH",OFX_UI_FONT_SMALL),"segln_spacer");
    
    //gui_loader->setColorBack(ofColor(255,100));
    gui_loader->setWidgetColor(OFX_UI_WIDGET_COLOR_BACK, ofColor(120,200));
    
    ofAddListener(gui_loader->newGUIEvent, this, &testApp::LoaderGuiEvent);
    
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
	if(key & OF_KEY_MODIFIER){
		if(key >= OF_KEY_F1 && key <= OF_KEY_F12){
            cout << "F KEY:" << key << endl;
            }else{

                switch(key){
                case OF_KEY_LEFT_SUPER:
                    cout << "Left Apple Pressed" << endl;
                    modkey = true;
                    break;

                }
		}
	}else{
        switch (key) {
            case ' ':
                myGui->wModActive = true;
                cout << "space out" << endl;
                
                resetAnimation(numOfABC);
                break;
            case 'a':
                myGui->showAxis = true;
                break;
            case '1':
                if(modkey) loadScene(1);
                break;
            case '2':
                if(modkey) loadScene(2);
                break;
            case '3':
                if(modkey) loadScene(3);
                break;
            case '4':
                if(modkey) loadScene(4);
                break;
            case '5':
                if(modkey) loadScene(5);
                break;
            case '6':
                if(modkey) loadScene(6);
                break;
            case '7':
                break;
            case '8':
                break;
            case '9':
                break;
            case 'l':
                //loader screens
                if(modkey) {
                    cout << "l key pressed" << endl;
                    gui_loader->toggleVisible();
                    showLdr = !showLdr;
                    if(showLights) {
                        myGui->gui->toggleVisible();
                        myGui->gui2->toggleVisible();
                        showLights = false;
                    }
                    if(showTrack) {
                        myTrackGui->gui->toggleVisible();
                        showTrack = false;
                    }
                }
                break;
            case 'g':
                //gui screens
               if(modkey){
                    myGui->gui->toggleVisible();
                    myGui->gui2->toggleVisible();
                    showLights = !showLights;
                    if(showLdr) {
                        gui_loader->toggleVisible();
                        showLdr = false;
                    }
                   if(showTrack){
                       myTrackGui->gui->toggleVisible();
                       showTrack = false;
                   }
                 
                }
                break;
            case 'm':
                //midi port
                
                toggleMidiPort();
              
                break;
            case 't':
                //gui screens
                if(modkey){
                    myTrackGui->gui->toggleVisible();
                    showTrack = !showTrack;
                    if(showLdr) {
                        gui_loader->toggleVisible();
                        showLdr = false;
                    }
                    if(showLights) {
                        myGui->gui->toggleVisible();
                        myGui->gui2->toggleVisible();
                        showLights = false;
                    }
                }
                break;
            case 'o':
                if(modkey) {
                    if(currentScene < 6){
                        currentScene+=1;
                    } else {
                        currentScene = 1;
                    }
                    cout << "cycle loadScene: "<< currentScene << endl;
                    
                    loadScene(currentScene);
                }
                
                break;
            //saveCam
            case ',':
                saveCam.prevView(4.0);
                break;
            case '.':
                saveCam.nextView(4.0);
                break;
            case 's':
                saveCam.updateView(-1);//-1= current view
                break;
            case 'n':
                saveCam.newView();
                break;
                
            default:
                break;
        }
	}
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if(key & OF_KEY_MODIFIER){
		if(key >= OF_KEY_F1 && key <= OF_KEY_F12){
        // if the f keys are pressed.
        
        }else{
           switch(key){
            case OF_KEY_LEFT_SUPER:
                    cout << "Left Apple Released" << endl;
                   modkey = false;
            break;
           }
        }
    }else{
            if (key == ' '){
                myGui->wModActive = false;
                myTrackGui->wModActive = false;
            }
            if (key == 'a'){
                myGui->showAxis = false;
            }
    }
    

}
 


//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    saveCam.isSettingCam = true;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    if (myGui->gui->isHit(x, y)) {
        cam.disableMouseInput();
    }
    if (myGui->gui2->isHit(x, y)) {
        cam.disableMouseInput();
    }
    if (gui_loader->isHit(x,y)) {
        cam.disableMouseInput();
    }
    if (myTrackGui->gui->isHit(x,y)) {
        cam.disableMouseInput();
    }
    
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    cam.enableMouseInput();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}



//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo info){
	if( info.files.size() > 0 ){
		
        string abcFile = "";
        bool isABC = false;
        
        dragPt = info.position;
		
        vector<string> paths = ofSplitString(info.files[0], "/");
        for(int i=0; i<paths.size()-1;i++){
            //get the position of the "abc" directory
            if(paths[i] == "abc") {
                isABC = true;
                abcFile += paths[i] + "/";
            } else {
                if(isABC){
                    abcFile += paths[i] + "/";
                } else {
                    //cout << "select an alembic file within the data/abc/ directory." << endl;
                }
            }
        }
        if(isABC){
            abcFile += paths[paths.size()-1];
        }
        

        //loop through the loader text fields, check if drop is over one of them.
        for(int i = 0; i < numOfABC; i++){
        
            //get an temporary pointer to the text field
            ofxUITextInput *text1 = (ofxUITextInput *)gui_loader->getWidget(ofToString(i)+"_TRK_READER");
            ofxUIRectangle *rect2 = text1->getRect();
            
            // is the drop point within the text field
            if(rect2->inside(dragPt)){
                
                if(isABC){
                    cout << "dropped inside " << i << "_TRK_READER" << endl;
                    text1->setTextString(abcFile);
                } else {
                    //cout << "watch where you're dropping!" << endl;
                }
            }
            
        }//end for

        
	}
    
    
    
    
}



