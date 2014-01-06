//
//  aTrackGui.cpp
//  animationMachine
//
//  Created by bob white III on 11/29/13.
//
//

#include "aTrackGui.h"

aTrackGui* aTrackGui::_instance = NULL;

aTrackGui* aTrackGui::Instance() {
    if (_instance == NULL) {
        _instance = new aTrackGui();
    }
    return _instance;
}


aTrackGui::aTrackGui() {
    doReset = guiAlloc = false;
}

void aTrackGui::setup(){
    //TO-DO
    //-- populate the matieral vector with content.
}


void aTrackGui::reset() {
    
    setGUI();
    
    gui->loadSettings("GUI/gui_Track.xml");
    
    gui->setVisible(false);
    
    guiAlloc = true;
    doReset = false;
    
}

void aTrackGui::update() {
    
}

void aTrackGui::draw() {
    
}

void aTrackGui::exit() {
    gui->saveSettings("GUI/gui_Track.xml");
    delete gui;
}

void aTrackGui::guiEvent(ofxUIEventArgs &e) {
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    
    //TO-DO
    //--setup all the listeners for the matierial sliders.
    //--need to set this up in a loop. too many.
    
    /*

     //Material 1
} else if (name == "MAT SHINE 1") {
    ofxUISlider *slider = (ofxUISlider *)e.widget;
    myLights2->material1.setShininess(slider->getScaledValue());
    
    //material diffuse/ambient colour
} else if (name == "MDR1") {
    ofColor c = myLights2->material1.getDiffuseColor();
    ofxUISlider *slider = (ofxUISlider *)e.widget;
    c.r = slider->getScaledValue();
    setWSlider(matDiffSlider, c, slider->getScaledValue());
    myLights2->material1.setDiffuseColor(c);
    myLights2->material1.setAmbientColor(c);
} else if (name == "MDG1") {
    ofColor c = myLights2->material1.getDiffuseColor();
    ofxUISlider *slider = (ofxUISlider *)e.widget;
    c.g = slider->getScaledValue();
    setWSlider(matDiffSlider, c, slider->getScaledValue());
    myLights2->material1.setDiffuseColor(c);
    myLights2->material1.setAmbientColor(c);
} else if (name == "MDB1") {
    ofColor c = myLights2->material1.getDiffuseColor();
    ofxUISlider *slider = (ofxUISlider *)e.widget;
    c.b = slider->getScaledValue();
    setWSlider(matDiffSlider, c, slider->getScaledValue());
    myLights2->material1.setDiffuseColor(c);
    myLights2->material1.setAmbientColor(c);
} else if (name == "MDA1") {
    ofColor c = myLights2->material1.getDiffuseColor();
    ofxUISlider *slider = (ofxUISlider *)e.widget;
    c.a = slider->getScaledValue();
    myLights2->material1.setDiffuseColor(c);
    myLights2->material1.setAmbientColor(c);
    
    //material emissive colour
} else if (name == "MER1") {
    ofColor c = myLights2->material1.getEmissiveColor();
    ofxUISlider *slider = (ofxUISlider *)e.widget;
    c.r = slider->getScaledValue();
    setWSlider(matEmSlider, c, slider->getScaledValue());
    myLights2->material1.setEmissiveColor(c);
} else if (name == "MEG1") {
    ofColor c = myLights2->material1.getEmissiveColor();
    ofxUISlider *slider = (ofxUISlider *)e.widget;
    c.g = slider->getScaledValue();
    setWSlider(matEmSlider, c, slider->getScaledValue());
    myLights2->material1.setEmissiveColor(c);
} else if (name == "MEB1") {
    ofColor c = myLights2->material1.getEmissiveColor();
    ofxUISlider *slider = (ofxUISlider *)e.widget;
    c.b = slider->getScaledValue();
    setWSlider(matEmSlider, c, slider->getScaledValue());
    myLights2->material1.setEmissiveColor(c);
} else if (name == "MEA1") {
    ofColor c = myLights2->material1.getEmissiveColor();
    ofxUISlider *slider = (ofxUISlider *)e.widget;
    c.a = slider->getScaledValue();
    myLights2->material1.setEmissiveColor(c);
    
    //material specular colour
} else if (name == "MSR1") {
    ofColor c = myLights2->material1.getSpecularColor();
    ofxUISlider *slider = (ofxUISlider *)e.widget;
    c.r = slider->getScaledValue();
    setWSlider(matSpecSlider, c, slider->getScaledValue());
    myLights2->material1.setSpecularColor(c);
    
} else if (name == "MSG1") {
    ofColor c = myLights2->material1.getSpecularColor();
    ofxUISlider *slider = (ofxUISlider *)e.widget;
    c.g = slider->getScaledValue();
    setWSlider(matSpecSlider, c, slider->getScaledValue());
    myLights2->material1.setSpecularColor(c);
} else if (name == "MSB1") {
    ofColor c = myLights2->material1.getSpecularColor();
    ofxUISlider *slider = (ofxUISlider *)e.widget;
    c.b = slider->getScaledValue();
    setWSlider(matSpecSlider, c, slider->getScaledValue());
    myLights2->material1.setSpecularColor(c);
} else if (name == "MSA1") {
    ofColor c = myLights2->material1.getSpecularColor();
    ofxUISlider *slider = (ofxUISlider *)e.widget;
    c.a = slider->getScaledValue();
    myLights2->material1.setSpecularColor(c);
    
    */
    
}

void aTrackGui::setGUI() {
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float w = 1000 - xInit * 2;
    float vertH = 40;
    float h = 8;
    
    
    if (guiAlloc) {
        ofRemoveListener(gui->newGUIEvent, this, &aTrackGui::guiEvent);
        
        //will need to add the sliders.
        
        delete gui;
    }
    
    
    gui = new ofxUICanvas(0, 0, w + xInit * 2, ofGetHeight());
    gui->setFont("GUI/HelveticaNeueLTStd-Bd.otf");
    gui->setFontSize(OFX_UI_FONT_SMALL, 6);
    /*
    gui->addWidgetRight(new ofxUILabelButton(50,false,"LOAD",OFX_UI_FONT_SMALL));
    gui->addWidgetRight(new ofxUILabelButton(50,false,"SAVE",OFX_UI_FONT_SMALL));
    */

    gui->addWidgetDown(new ofxUILabel("Material 1", OFX_UI_FONT_MEDIUM));
    gui->addSlider("MAT SHINE 1", 0, 128, 255, 230, h);
    
    gui->addSpacer(230,15);
    
    
    matDiffSlider.push_back(gui->addSlider("DR", 0, 255, 100, h, vertH));
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    matDiffSlider.push_back(gui->addSlider("DG", 0, 255, 100, h, vertH));
    matDiffSlider.push_back(gui->addSlider("DB", 0, 255, 100, h, vertH));
    
    gui->addSpacer(2, vertH+10);
    
    matEmSlider.push_back(gui->addSlider("ER", 0, 255, 100, h, vertH));
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    matEmSlider.push_back(gui->addSlider("EG", 0, 255, 100, h, vertH));
    matEmSlider.push_back(gui->addSlider("EB", 0, 255, 100, h, vertH));
    
    gui->addSpacer(2, vertH+10);
    
    matSpecSlider.push_back(gui->addSlider("SR", 0, 255, 100, h, vertH));
    matSpecSlider.push_back(gui->addSlider("SG", 0, 255, 100, h, vertH));
    matSpecSlider.push_back(gui->addSlider("SB", 0, 255, 100, h, vertH));
    
    gui->addWidgetNorthOf(new ofxUILabel("Diffuse",OFX_UI_FONT_SMALL),"DR");
    gui->addWidgetNorthOf(new ofxUILabel("Emissive",OFX_UI_FONT_SMALL),"ER");
    gui->addWidgetNorthOf(new ofxUILabel("Specular",OFX_UI_FONT_SMALL),"SR");
    
    //TO-DO
    //ADD THE MATIERIAL.
    //--need to put in a loop.
    //--set up the sliders with labels "northOf"
    
    /*
     
     //Material Control 1
     gui2->addSpacer(w, 2);
     gui2->addWidgetDown(new ofxUILabel("Material 1", OFX_UI_FONT_MEDIUM));
     gui2->addSlider("MAT SHINE 1", 0, 128, myLights2->material1.getShininess(), w, h);
     gui2->addWidgetDown(new ofxUILabel("Material Diffuse/Emissive/Specular Color", OFX_UI_FONT_SMALL));
     
     c = myLights2->material1.getDiffuseColor();
     matDiffSlider.push_back(gui2->addSlider("MDR1", 0, 255, c.r, h, vertH));
     gui2->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
     matDiffSlider.push_back(gui2->addSlider("MDG1", 0, 255, c.g, h, vertH));
     matDiffSlider.push_back(gui2->addSlider("MDB1", 0, 255, c.b, h, vertH));
     //    gui2->addSlider("MDA", 0, 255, c.a, h, vertH);
     
     gui2->addSpacer(2, vertH+10);
     c = myLights2->material1.getEmissiveColor();
     matEmSlider.push_back(gui2->addSlider("MER1", 0, 255, c.r, h, vertH));
     gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
     matEmSlider.push_back(gui2->addSlider("MEG1", 0, 255, c.g, h, vertH));
     matEmSlider.push_back(gui2->addSlider("MEB1", 0, 255, c.b, h, vertH));
     //    gui2->addSlider("MDA", 0, 255, c.a, h, vertH);
     
     gui2->addSpacer(2, vertH+10);
     c = myLights2->material1.getSpecularColor();
     matSpecSlider.push_back(gui2->addSlider("MSR1", 0, 255, c.r, h, vertH));
     matSpecSlider.push_back(gui2->addSlider("MSG1", 0, 255, c.g, h, vertH));
     matSpecSlider.push_back(gui2->addSlider("MSB1", 0, 255, c.b, h, vertH));
     //    gui2->addSlider("MSA", 0, 255, c.a, h, vertH);
     
     */
    
    gui->setWidgetColor(OFX_UI_WIDGET_COLOR_BACK, ofColor(120,200));
    
    ofAddListener(gui->newGUIEvent, this, &aTrackGui::guiEvent);
    
}

void aTrackGui::setWSlider(vector<ofxUISlider *>sliders, ofColor &c, float v){
    if (wModActive) {
        c.r = c.b = c.g = v;
        for (int i = 0; i < sliders.size(); i++)
            sliders.at(i)->setValue(v);
        
    }
    
}
