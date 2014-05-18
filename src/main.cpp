#include "ofMain.h"
#include "ofApp.h"

//#include "ofAppGlutWindow.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
    // there are couple wayz of doing this. The issue I'm having is that my modifier keys are registering under ofAppGlutWindow.
    // But here are the 3 different methods.
    
    //ofAppGlutWindow Method
    //======================
    /*
    ofAppGlutWindow window;
    
    //REFERENCE:
    //https://sites.google.com/site/ofauckland/examples/smoothing-edges-anti-aliasing
    
    //window.setGlutDisplayString("rgba double samples>=4");//sometimes this doesn't work
    window.setGlutDisplayString("rgba double depth samples>=4");//This was my intial setting
    //window.setGlutDisplayString("rgba double depth alpha samples>=4");
    
    ofSetupOpenGL(&window, 1280, 720, OF_WINDOW);// <-------- setup the GL context
    */
    
    
    //ofAppGLFWWindow Method
    //======================
    
    ofAppGLFWWindow window;
    window.setNumSamples(4);
    //window.setNumSamples(16); // I definately see a difference.  Need to do a better test though.
    
    ofSetupOpenGL(&window, 1280,720,OF_WINDOW);
   
    
    //Default Method
    //======================
    // REMINDER: de-include ofAppGLFW and ofAppGlut
    
    //ofSetupOpenGL(720,360, OF_WINDOW);
    


	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
