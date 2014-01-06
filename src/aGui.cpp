//
//  aGui.cpp
//  lights_camera_alembic
//
//  Created by bob white III on 8/17/13.
//
//

#include "aGui.h"

aGui* aGui::_instance = NULL;


aGui* aGui::Instance() {
    
    
	// checks if instance already exists:
	if (_instance == NULL) {
        _instance = new aGui();
    }
	return _instance;
	
	// from OF forum:
	/*
	 static Singleton inst;
	 return &inst;
	 */
    
    
}

aGui::aGui() {
    
    doReset = guiAlloc = false;
    wModActive = false;
    
    myLights2 = aLights::Instance();
    
}

void aGui::setup(int num){
    numOfLoaders = num;
    
    //setup variables
    

}


void aGui::reset(){

    radius = 300;
    animTime = 0;
    
    cull = false;
    useSpot = false;
    usePoint = true;
    useAmb = true;
    useDir = true;
    autoRotatePoint = true;
    showDirSource = false;
    showSpotSource = false;
    showPointSource = true;

    setGUI();


    gui->loadSettings("GUI/gui_Settings.xml");
    gui2->loadSettings("GUI/gui_Settings2.xml");
    
    gui->setVisible(false);
    gui2->setVisible(false);
    
    guiAlloc = true;
    
    doReset = false;
    
    
}

void aGui::update(){
    
}

void aGui::draw(){
    
    
}

void aGui::exit(){
    gui->saveSettings("GUI/gui_Settings.xml");
    gui2->saveSettings("GUI/gui_Settings2.xml");
    //gui_loader->saveSettings("GUI/gui_loader_Settings.xml");
	//delete gui_loader;
    
    delete gui;
	delete gui2;
    
    
}


void aGui::setWSlider(vector<ofxUISlider *> sliders, ofColor &c, float v)
{
    if (wModActive) {
        c.r = c.b = c.g = v;
        for (int i = 0; i < sliders.size(); i++)
            sliders.at(i)->setValue(v);
        
    }
}

void aGui::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
    
	if (name == "Radius") {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        radius = slider->getScaledValue();
    } else if (name == "ANIM") {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        animTime = slider->getScaledValue();
    } else if (name == "Spot Source") {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        showSpotSource = toggle->getValue();
    } else if (name == "Cutoff") {
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        
        myLights2->spot.setSpotlightCutOff(slider->getScaledValue());
    } else if (name == "Cons") {
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        myLights2->spot.setSpotConcentration(slider->getScaledValue());
    } else if (name == "SX_POS") {
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        ofVec3f curpos = myLights2->spot.getPosition();
        curpos.x = slider->getScaledValue();
        myLights2->spot.setPosition(curpos);
    } else if (name == "SY_POS") {
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        ofVec3f curpos = myLights2->spot.getPosition();
        curpos.y = slider->getScaledValue();
        myLights2->spot.setPosition(curpos);
    } else if (name == "SZ_POS") {
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        ofVec3f curpos = myLights2->spot.getPosition();
        curpos.z = slider->getScaledValue();
        myLights2->spot.setPosition(curpos);
        
        /* spot orientation */
    } else if (name == "SX_ORI") {
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        myLights2->spot_rot.x = slider->getScaledValue();
        myLights2->setLightOri(myLights2->spot, myLights2->spot_rot);
    } else if (name == "SY_ORI") {
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        myLights2->spot_rot.y = slider->getScaledValue();
        myLights2->setLightOri(myLights2->spot, myLights2->spot_rot);
    } else if (name == "SZ_ORI") {
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        myLights2->spot_rot.z = slider->getScaledValue();
        myLights2->setLightOri(myLights2->spot, myLights2->spot_rot);
        
        /* spot diffuse colour */
    } else if (name == "SDR") {
        ofColor c = myLights2->spot.getDiffuseColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.r = slider->getScaledValue();
        setWSlider(spotDiffSlider, c, slider->getScaledValue());
        myLights2->spot.setDiffuseColor(c);
    } else if (name == "SDG") {
        ofColor c = myLights2->spot.getDiffuseColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.g = slider->getScaledValue();
        setWSlider(spotDiffSlider, c, slider->getScaledValue());
        myLights2->spot.setDiffuseColor(c);
    } else if (name == "SDB") {
        ofColor c = myLights2->spot.getDiffuseColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.b = slider->getScaledValue();
        setWSlider(spotDiffSlider, c, slider->getScaledValue());
        myLights2->spot.setDiffuseColor(c);
    } else if (name == "SDA") {
        ofColor c = myLights2->spot.getDiffuseColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.a = slider->getScaledValue();
        myLights2->spot.setDiffuseColor(c);
        
        /* spot specular colour */
    } else if (name == "SSR") {
        ofColor c = myLights2->spot.getSpecularColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.r = slider->getScaledValue();
        setWSlider(spotspecslider, c, slider->getScaledValue());
        myLights2->spot.setSpecularColor(c);
    } else if (name == "SSG") {
        ofColor c = myLights2->spot.getSpecularColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.g = slider->getScaledValue();
        setWSlider(spotspecslider, c, slider->getScaledValue());
        myLights2->spot.setSpecularColor(c);
    } else if (name == "SSB") {
        ofColor c = myLights2->spot.getSpecularColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.b = slider->getScaledValue();
        setWSlider(spotspecslider, c, slider->getScaledValue());
        myLights2->spot.setSpecularColor(c);
    } else if (name == "SSA") {
        ofColor c = myLights2->spot.getSpecularColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.a = slider->getScaledValue();
        myLights2->spot.setSpecularColor(c);
        
        /* directional light parms */
    } else if (name == "Dir Source") {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        showDirSource = toggle->getValue();
    } else if (name == "DIR_XORI") {
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        myLights2->dir_rot.x = slider->getScaledValue();
        myLights2->setLightOri(myLights2->dir, myLights2->dir_rot);
    } else if (name == "DIR_YORI") {
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        myLights2->dir_rot.y = slider->getScaledValue();
        myLights2->setLightOri(myLights2->dir, myLights2->dir_rot);
    } else if (name == "DIR_ZORI") {
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        myLights2->dir_rot.z = slider->getScaledValue();
        myLights2->setLightOri(myLights2->dir, myLights2->dir_rot);
        
        /* directional diffuse colour */
    } else if (name == "DDR") {
        ofColor c = myLights2->dir.getDiffuseColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.r = slider->getScaledValue();
        setWSlider(dirDiffSlider, c, slider->getScaledValue());
        myLights2->dir.setDiffuseColor(c);
    } else if (name == "DDG") {
        ofColor c = myLights2->dir.getDiffuseColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.g = slider->getScaledValue();
        setWSlider(dirDiffSlider, c, slider->getScaledValue());
        myLights2->dir.setDiffuseColor(c);
    } else if (name == "DDB") {
        ofColor c = myLights2->dir.getDiffuseColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.b = slider->getScaledValue();
        setWSlider(dirDiffSlider, c, slider->getScaledValue());
        myLights2->dir.setDiffuseColor(c);
    } else if (name == "DDA") {
        ofColor c = myLights2->dir.getDiffuseColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.a = slider->getScaledValue();
        myLights2->dir.setDiffuseColor(c);
        
        /* directional specular colour */
    } else if (name == "DSR") {
        ofColor c = myLights2->dir.getSpecularColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.r = slider->getScaledValue();
        setWSlider(dirspecslider, c, slider->getScaledValue());
        myLights2->dir.setSpecularColor(c);
    } else if (name == "DSG") {
        ofColor c = myLights2->dir.getSpecularColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.g = slider->getScaledValue();
        setWSlider(dirspecslider, c, slider->getScaledValue());
        myLights2->dir.setSpecularColor(c);
    } else if (name == "DSB") {
        ofColor c = myLights2->dir.getSpecularColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.b = slider->getScaledValue();
        setWSlider(dirspecslider, c, slider->getScaledValue());
        myLights2->dir.setSpecularColor(c);
    } else if (name == "DSA") {
        ofColor c = myLights2->dir.getSpecularColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.a = slider->getScaledValue();
        myLights2->dir.setSpecularColor(c);
        
        /* point diffuse colour */
    } else if (name == "Point Source") {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        showPointSource = toggle->getValue();
        
    } else if (name == "PDR") {
        ofColor c = myLights2->point.getDiffuseColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.r = slider->getScaledValue();
        setWSlider(pointDiffSlider, c, slider->getScaledValue());
        myLights2->point.setDiffuseColor(c);
    } else if (name == "PDG") {
        ofColor c = myLights2->point.getDiffuseColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.g = slider->getScaledValue();
        setWSlider(pointDiffSlider, c, slider->getScaledValue());
        myLights2->point.setDiffuseColor(c);
    } else if (name == "PDB") {
        ofColor c = myLights2->point.getDiffuseColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.b = slider->getScaledValue();
        setWSlider(pointDiffSlider, c, slider->getScaledValue());
        myLights2->point.setDiffuseColor(c);
    } else if (name == "PDA") {
        ofColor c = myLights2->point.getDiffuseColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.a = slider->getScaledValue();
        myLights2->point.setDiffuseColor(c);
        
        /* point specular colour */
    } else if (name == "PSR") {
        ofColor c = myLights2->point.getSpecularColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.r = slider->getScaledValue();
        setWSlider(pointspecslider, c, slider->getScaledValue());
        myLights2->point.setSpecularColor(c);
    } else if (name == "PSG") {
        ofColor c = myLights2->point.getSpecularColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.g = slider->getScaledValue();
        setWSlider(pointspecslider, c, slider->getScaledValue());
        myLights2->point.setSpecularColor(c);
    } else if (name == "PSB") {
        ofColor c = myLights2->point.getSpecularColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.b = slider->getScaledValue();
        setWSlider(pointspecslider, c, slider->getScaledValue());
        myLights2->point.setSpecularColor(c);
    } else if (name == "PSA") {
        ofColor c = myLights2->point.getSpecularColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.a = slider->getScaledValue();
        myLights2->point.setSpecularColor(c);
        
        /* point auto move */
    } else if (name == "Point Light Auto Move") {
        ofxUIToggle *toggle = (ofxUIToggle *)e.widget;
        autoRotatePoint = toggle->getValue();
        
        /* misc toggles */
    } else if (name == "cull") {
        ofxUIToggle *toggle = (ofxUIToggle *)e.widget;
        cull = toggle->getValue();
    } else if (name == "point") {
        ofxUIToggle *toggle = (ofxUIToggle *)e.widget;
        usePoint = toggle->getValue();
    } else if (name == "spot") {
        ofxUIToggle *toggle = (ofxUIToggle *)e.widget;
        useSpot = toggle->getValue();
    } else if (name == "dir") {
        ofxUIToggle *toggle = (ofxUIToggle *)e.widget;
        useDir = toggle->getValue();
 
        
//Material 1
        /* material shininess */
    } else if (name == "MAT SHINE 1") {
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        myLights2->material1.setShininess(slider->getScaledValue());
        
        /* material diffuse/ambient colour */
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
        
        /* material emissive colour */
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
        
        /* material specular colour */
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

 
        
//Material 2
        /* material shininess */
    } else if (name == "MAT SHINE 2") {
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        myLights2->material2.setShininess(slider->getScaledValue());
        
        /* material diffuse/ambient colour */
    } else if (name == "MDR2") {
        ofColor c = myLights2->material2.getDiffuseColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.r = slider->getScaledValue();
        setWSlider(matDiffSlider, c, slider->getScaledValue());
        myLights2->material2.setDiffuseColor(c);
        myLights2->material2.setAmbientColor(c);
    } else if (name == "MDG2") {
        ofColor c = myLights2->material2.getDiffuseColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.g = slider->getScaledValue();
        setWSlider(matDiffSlider, c, slider->getScaledValue());
        myLights2->material2.setDiffuseColor(c);
        myLights2->material2.setAmbientColor(c);
    } else if (name == "MDB2") {
        ofColor c = myLights2->material2.getDiffuseColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.b = slider->getScaledValue();
        setWSlider(matDiffSlider, c, slider->getScaledValue());
        myLights2->material2.setDiffuseColor(c);
        myLights2->material2.setAmbientColor(c);
    } else if (name == "MDA2") {
        ofColor c = myLights2->material2.getDiffuseColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.a = slider->getScaledValue();
        myLights2->material2.setDiffuseColor(c);
        myLights2->material2.setAmbientColor(c);
        
        /* material emissive colour */
    } else if (name == "MER2") {
        ofColor c = myLights2->material2.getEmissiveColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.r = slider->getScaledValue();
        setWSlider(matEmSlider, c, slider->getScaledValue());
        myLights2->material2.setEmissiveColor(c);
    } else if (name == "MEG2") {
        ofColor c = myLights2->material2.getEmissiveColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.g = slider->getScaledValue();
        setWSlider(matEmSlider, c, slider->getScaledValue());
        myLights2->material2.setEmissiveColor(c);
    } else if (name == "MEB2") {
        ofColor c = myLights2->material2.getEmissiveColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.b = slider->getScaledValue();
        setWSlider(matEmSlider, c, slider->getScaledValue());
        myLights2->material2.setEmissiveColor(c);
    } else if (name == "MEA2") {
        ofColor c = myLights2->material2.getEmissiveColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.a = slider->getScaledValue();
        myLights2->material2.setEmissiveColor(c);
        
        /* material specular colour */
    } else if (name == "MSR2") {
        ofColor c = myLights2->material2.getSpecularColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.r = slider->getScaledValue();
        setWSlider(matSpecSlider, c, slider->getScaledValue());
        myLights2->material2.setSpecularColor(c);
        
    } else if (name == "MSG2") {
        ofColor c = myLights2->material2.getSpecularColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.g = slider->getScaledValue();
        setWSlider(matSpecSlider, c, slider->getScaledValue());
        myLights2->material2.setSpecularColor(c);
    } else if (name == "MSB2") {
        ofColor c = myLights2->material2.getSpecularColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.b = slider->getScaledValue();
        setWSlider(matSpecSlider, c, slider->getScaledValue());
        myLights2->material2.setSpecularColor(c);
    } else if (name == "MSA2") {
        ofColor c = myLights2->material2.getSpecularColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.a = slider->getScaledValue();
        myLights2->material2.setSpecularColor(c);
        
        
        /* ambient light stuff */
    } else if (name == "ambient") {
        ofxUIToggle *toggle = (ofxUIToggle *)e.widget;
        useAmb = toggle->getValue();
    } else if (name == "AR") {
        ofColor c = myLights2->amb.getAmbientColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.r = slider->getScaledValue();
        setWSlider(ambslider, c, slider->getScaledValue());
        myLights2->amb.setAmbientColor(c);
        
    } else if (name == "AG") {
        ofColor c = myLights2->amb.getAmbientColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.g = slider->getScaledValue();
        setWSlider(ambslider, c, slider->getScaledValue());
        myLights2->amb.setAmbientColor(c);
    } else if (name == "AB") {
        ofColor c = myLights2->amb.getAmbientColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.b = slider->getScaledValue();
        setWSlider(ambslider, c, slider->getScaledValue());
        myLights2->amb.setAmbientColor(c);
    } else if (name == "AA") {
        ofColor c = myLights2->amb.getAmbientColor();
        ofxUISlider *slider = (ofxUISlider *)e.widget;
        c.a = slider->getScaledValue();
        myLights2->amb.setAmbientColor(c);
        
    } else if (name == "RESET") {
        doReset = true;
    }

    
}

void aGui::setGUI()
{
    
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float w = 400 - xInit * 2;
    float vertH = 40;
    float h = 8;
    
    if (guiAlloc) {
        ofRemoveListener(gui->newGUIEvent, this, &aGui::guiEvent);
        ofRemoveListener(gui2->newGUIEvent, this, &aGui::guiEvent);
        ambslider.clear();
        spotspecslider.clear();
        spotDiffSlider.clear();
        dirspecslider.clear();
        dirDiffSlider.clear();
        pointspecslider.clear();
        pointDiffSlider.clear();
        matDiffSlider.clear();
        matSpecSlider.clear();
        matEmSlider.clear();
        
        delete gui;
        delete gui2;
    }
    
    gui = new ofxUICanvas(0, 0, w + xInit * 2, ofGetHeight());
    gui->setFont("GUI/HelveticaNeueLTStd-Bd.otf");
    gui->setFontSize(OFX_UI_FONT_SMALL, 6);
    
    gui->addWidgetDown(new ofxUILabel("LIGHT CONTROL", OFX_UI_FONT_MEDIUM));
    
    gui->addSlider("Radius", 5, 1000, radius, w, h);
    
    gui->addSpacer(w, 2);
    gui->addWidgetDown(new ofxUILabel("Spot Light Control", OFX_UI_FONT_MEDIUM));
    gui->addWidgetRight(new ofxUIToggle("Spot Source", showSpotSource, 10, 15));
    gui->addWidgetDown(new ofxUILabel("Spot Location", OFX_UI_FONT_SMALL));
    
    gui->addSlider("SX_POS", -500, 500, myLights2->spot.getPosition().x, w, h);
    gui->addSlider("SY_POS", -500, 500, myLights2->spot.getPosition().y, w, h);
    gui->addSlider("SZ_POS", -500, 500, myLights2->spot.getPosition().z, w, h);
    
    gui->addWidgetDown(new ofxUILabel("Spot Orientation", OFX_UI_FONT_SMALL));
    gui->addSlider("SX_ORI", -180, 180, myLights2->spot.getOrientationEuler().x, w, h);
    gui->addSlider("SY_ORI", -180, 180, myLights2->spot.getOrientationEuler().y, w, h);
    gui->addSlider("SZ_ORI", -180, 180, myLights2->spot.getOrientationEuler().z, w, h);
    
    gui->addWidgetDown(new ofxUILabel("Spot Param", OFX_UI_FONT_SMALL));
    gui->addSlider("Cutoff", 0.0, 90.0, myLights2->spot.getSpotlightCutOff(), w, h);
    gui->addSlider("Cons", 0.0, 128.0, myLights2->spot.getSpotConcentration(), w, h);
    
    gui->addWidgetDown(new ofxUILabel("Spot Diffuse/Specular Color", OFX_UI_FONT_SMALL));
    ofColor c = myLights2->spot.getDiffuseColor();
    spotDiffSlider.push_back(gui->addSlider("SDR", 0, 255, c.r, h, vertH));
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    spotDiffSlider.push_back(gui->addSlider("SDG", 0, 255, c.g, h, vertH));
    spotDiffSlider.push_back(gui->addSlider("SDB", 0, 255, c.b, h, vertH));
    //    gui->addSlider("SDA", 0, 255, c.a, h, vertH);
    
    gui->addSpacer(2, vertH+10);
    
    c = myLights2->spot.getSpecularColor();
    spotspecslider.push_back(gui->addSlider("SSR", 0, 255, c.r, h, vertH));
    spotspecslider.push_back(gui->addSlider("SSG", 0, 255, c.g, h, vertH));
    spotspecslider.push_back(gui->addSlider("SSB", 0, 255, c.b, h, vertH));
    //    spotspecslider.push_back(gui->addSlider("SSA", 0, 255, c.a, h, vertH));
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSpacer(w, 2);
    gui->addWidgetDown(new ofxUILabel("Directional Light Control", OFX_UI_FONT_MEDIUM));
    gui->addWidgetRight(new ofxUIToggle("Dir Source", showDirSource, 10, 15));
    
    gui->addWidgetDown(new ofxUILabel("Directional Orientation", OFX_UI_FONT_SMALL));
    
    gui->addSlider("DIR_XORI", -180, 180, myLights2->dir.getOrientationEuler().x, w, h);
    gui->addSlider("DIR_YORI", -180, 180, myLights2->dir.getOrientationEuler().y, w, h);
    gui->addSlider("DIR_ZORI", -180, 180, myLights2->dir.getOrientationEuler().z, w, h);
    
    gui->addWidgetDown(new ofxUILabel("Directional Diffuse/Specular Color", OFX_UI_FONT_SMALL));
    c = myLights2->dir.getDiffuseColor();
    dirDiffSlider.push_back(gui->addSlider("DDR", 0, 255, c.r, h, vertH));
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    dirDiffSlider.push_back(gui->addSlider("DDG", 0, 255, c.g, h, vertH));
    dirDiffSlider.push_back(gui->addSlider("DDB", 0, 255, c.b, h, vertH));
    
    gui->addSpacer(2, vertH+10);
    c = myLights2->dir.getSpecularColor();
    dirspecslider.push_back(gui->addSlider("DSR", 0, 255, c.r, h, vertH));
    dirspecslider.push_back(gui->addSlider("DSG", 0, 255, c.g, h, vertH));
    dirspecslider.push_back(gui->addSlider("DSB", 0, 255, c.b, h, vertH));
    //    dirspecslider.push_back(gui->addSlider("DSA", 0, 255, c.a, h, vertH));
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSpacer(w, 2);
    gui->addWidgetDown(new ofxUILabel("Point Light Control", OFX_UI_FONT_MEDIUM));
    gui->addWidgetRight(new ofxUIToggle("Point Source", showPointSource, 10, 15));
    gui->addWidgetDown(new ofxUILabel("Point Diffuse/Specular Color", OFX_UI_FONT_SMALL));
    c = myLights2->point.getDiffuseColor();
    pointDiffSlider.push_back(gui->addSlider("PDR", 0, 255, c.r, h, vertH));
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    pointDiffSlider.push_back(gui->addSlider("PDG", 0, 255, c.g, h, vertH));
    pointDiffSlider.push_back(gui->addSlider("PDB", 0, 255, c.b, h, vertH));
    //    gui->addSlider("PDA", 0, 255, c.a, h, vertH);
    
    gui->addSpacer(2, vertH+10);
    c = myLights2->point.getSpecularColor();
    pointspecslider.push_back(gui->addSlider("PSR", 0, 255, c.r, h, vertH));
    pointspecslider.push_back(gui->addSlider("PSG", 0, 255, c.g, h, vertH));
    pointspecslider.push_back(gui->addSlider("PSB", 0, 255, c.b, h, vertH));
    //    pointspecslider.push_back(gui->addSlider("PSA", 0, 255, c.a, h, vertH));
    
    
    
    
    
    
    ofAddListener(gui->newGUIEvent, this, &aGui::guiEvent);
    //guiAlloc = true;
    
    
    gui2 = new ofxUICanvas(800, 0, w + xInit * 2, ofGetHeight());
    gui2->setFont("GUI/HelveticaNeueLTStd-Bd.otf");
    gui2->setFontSize(OFX_UI_FONT_SMALL, 6);
    
    //gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui2->addToggle("Point Light Auto Move", autoRotatePoint);
    
    //ambient Control
    
    
    gui2->addSpacer(w, 2);
    gui2->addWidgetDown(new ofxUILabel("Ambient Light Control", OFX_UI_FONT_MEDIUM));
    gui2->addWidgetDown(new ofxUILabel("Ambient Light Color", OFX_UI_FONT_SMALL));
    
    
    c = myLights2->amb.getAmbientColor();
    ambslider.push_back(gui2->addSlider("AR", 0, 255.0, c.r, h, vertH));
    gui2->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    ambslider.push_back(gui2->addSlider("AG", 0, 255.0, c.g, h, vertH));
    ambslider.push_back(gui2->addSlider("AB", 0, 255.0, c.b, h, vertH));
    //    ambslider.push_back(gui2->addSlider("AA", 0, 255, c.a, h, vertH));
    gui2->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    
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

    gui2->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    //Material Control 2
    gui2->addSpacer(w, 2);
    gui2->addWidgetDown(new ofxUILabel("Material 2", OFX_UI_FONT_MEDIUM));
    gui2->addSlider("MAT SHINE 2", 0, 128, myLights2->material2.getShininess(), w, h);
    gui2->addWidgetDown(new ofxUILabel("Material Diffuse/Emissive/Specular Color", OFX_UI_FONT_SMALL));
    
    c = myLights2->material2.getDiffuseColor();
    matDiffSlider.push_back(gui2->addSlider("MDR2", 0, 255, c.r, h, vertH));
    gui2->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    matDiffSlider.push_back(gui2->addSlider("MDG2", 0, 255, c.g, h, vertH));
    matDiffSlider.push_back(gui2->addSlider("MDB2", 0, 255, c.b, h, vertH));
    //    gui2->addSlider("MDA", 0, 255, c.a, h, vertH);
    
    gui2->addSpacer(2, vertH+10);
    c = myLights2->material2.getEmissiveColor();
    matEmSlider.push_back(gui2->addSlider("MER2", 0, 255, c.r, h, vertH));
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    matEmSlider.push_back(gui2->addSlider("MEG2", 0, 255, c.g, h, vertH));
    matEmSlider.push_back(gui2->addSlider("MEB2", 0, 255, c.b, h, vertH));
    //    gui2->addSlider("MDA", 0, 255, c.a, h, vertH);
    
    gui2->addSpacer(2, vertH+10);
    c = myLights2->material2.getSpecularColor();
    matSpecSlider.push_back(gui2->addSlider("MSR2", 0, 255, c.r, h, vertH));
    matSpecSlider.push_back(gui2->addSlider("MSG2", 0, 255, c.g, h, vertH));
    matSpecSlider.push_back(gui2->addSlider("MSB2", 0, 255, c.b, h, vertH));
    //    gui2->addSlider("MSA", 0, 255, c.a, h, vertH);
    
    
    //turn on an off spotlights.
    gui2->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui2->addSpacer(w, 2);
    gui2->addToggle("cull", cull);
    gui2->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui2->addToggle("spot", useSpot);
    gui2->addToggle("point", usePoint);
    gui2->addToggle("dir", useDir);
    gui2->addToggle("amb", useAmb);
    
    
    gui2->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    
    gui2->addSpacer(w, 2);
    gui2->addLabelButton("RESET", false);
    
    gui2->addSpacer(w, 2);
    gui2->addWidgetDown(new ofxUILabel("Animation Control", OFX_UI_FONT_MEDIUM));
    gui2->addSlider("ANIM", 0.0, 1.0, animTime, w, h);
    
    ofAddListener(gui2->newGUIEvent, this, &aGui::guiEvent);
    
    
    
    //guiAlloc = true; //--moved to reset();
    
}// end setGUI()


