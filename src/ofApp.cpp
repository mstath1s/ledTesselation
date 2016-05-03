#include "ofApp.h"
/*
std::string exec(char* cmd)
{
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe))
    {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}
*/
//--------------------------------------------------------------
void ofApp::setup()
{

    printFontHeader.load("GUI/DroidSans.ttf", 12, true, true, true, 0);
    printFontText.load("GUI/DroidSans.ttf", 8, true, true, true, 0);
    //guiFont.load("GUI/DroidSans.ttf", 10, true, true, true, 0);
    
    logoITU.load("GUI/logoITU.jpg");
    logoITU.resize(220, 220*logoITU.getHeight()/logoITU.getWidth());
    logoKADK.load("GUI/logoKADK.png");
    logoKADK.resize(100, 100*logoKADK.getHeight()/logoKADK.getWidth());

    loadedFileName = "unsaved preset";

    hideGUI = false;
    kelvinCold = 6500;
    kelvinWarm = 4200;

    kelvinWarmRangeDefault = kelvinWarm * 1.0;
    kelvinWarmRange = kelvinWarmRangeDefault;
    kelvinColdRangeDefault = kelvinCold * 1.0;
    kelvinColdRange = kelvinWarmRangeDefault; // fade up from warm
    temperatureSpeedDefault = 0.42;
    temperatureSpeed = temperatureSpeedDefault;
    temperatureSpreadDefault = 0.1;
    temperatureSpread = 0;
    temperatureTime = 0;

    brightnessRangeFromDefault = 0.0;
    brightnessRangeFrom = brightnessRangeFromDefault;
    brightnessRangeToDefault = 1.0;
    brightnessRangeTo = brightnessRangeFromDefault; // fade up from black
    brightnessSpeedDefault = 0.4;
    brightnessSpeed = brightnessSpeedDefault;
    brightnessSpreadDefault = 0.15;
    brightnessSpread = 0.25;
    brightnessTime = 0;

    lastTemperatureManipulationSeconds = -manipulationTimeoutSeconds*0.75;
    lastBrightnessManipulationSeconds = -manipulationTimeoutSeconds;
    
    ofSerial serial;
    
    auto devices = serial.getDeviceList();
    
    string searchString = "tty.usbserial-EN";
    
    for(auto d : devices){
        if(d.getDeviceName().find(searchString) != string::npos){
            dmx.connect(d.getDeviceName(), 512);
            break;
        }
    }
    
    // sourceCamera.connect();
    // cameraController.load("cameraSettings.xml");

    cam.setTranslationKey(' ');
    cam.enableOrtho();

    int tsqSize = 30;

    int tesselationHeight = 6;

    // construct tesselations

    ofVec3f origin;

    /** such is the mapping of dmx channels to the ofVec3f objects

             /     \
            |\  0  /|
            | \   / |
            |  \ /  |
            \ 1 | 2 /
             \  |  /

    **/


    ofVec3f * tesselation1map[] =
    {
        NULL,               NULL,               NULL,               NULL,               NULL,               NULL,
        new ofVec3f(0,0,53),new ofVec3f(),      NULL,               NULL,               NULL,               NULL,
        new ofVec3f(),      new ofVec3f(),      new ofVec3f(0,0,49),new ofVec3f(0,0,33),new ofVec3f(0,0,5), NULL,
        new ofVec3f(),      new ofVec3f(0,61,0),new ofVec3f(57,0,0),new ofVec3f(0,0,37),new ofVec3f(0,0,13),new ofVec3f(0,0,1),
        new ofVec3f(0,0,45),new ofVec3f(),      new ofVec3f(0,0,41),new ofVec3f(0,0,29),new ofVec3f(0,0,21),new ofVec3f(0,0,9),
        NULL,               new ofVec3f(),      new ofVec3f(),      NULL,               new ofVec3f(0,0,25),new ofVec3f(0,0,17)
    };
    int tesselation1width = 6;
    origin = addTesselation(origin, tsqSize, tesselation1width, tesselationHeight, tesselation1map);

    ofVec3f * tesselation2map[] =
    {
        NULL,               NULL,               NULL,               NULL,               NULL,               NULL,
        NULL,               new ofVec3f(),      new ofVec3f(0,0,69),NULL,               NULL,               NULL,
        new ofVec3f(0,0,93),new ofVec3f(),      new ofVec3f(),      new ofVec3f(),      new ofVec3f(0,0,65),NULL,
        new ofVec3f(),      new ofVec3f(0,89,0),new ofVec3f(81,0,0),new ofVec3f(0,0,77),new ofVec3f(),      new ofVec3f(),
        new ofVec3f(),      new ofVec3f(),      new ofVec3f(0,0,85),new ofVec3f(),      new ofVec3f(0,0,73),NULL,
        new ofVec3f(),      NULL,               new ofVec3f(),      new ofVec3f(),      new ofVec3f(),      NULL
    };
    int tesselation2width = 6;
    origin*=1.35;
    origin += addTesselation(origin, tsqSize, tesselation2width, tesselationHeight, tesselation2map);

    ofVec3f * tesselation3map[] =
    {
        //  1                   2                   3                   4                   5                   6                   7                   8                   9
        NULL,               NULL,               NULL,               new ofVec3f(),      NULL,               NULL,               NULL,               NULL,               NULL,
        NULL,               NULL,               new ofVec3f(),      new ofVec3f(),      new ofVec3f(),      NULL,               new ofVec3f(),      NULL,               NULL,
        new ofVec3f(0,0,121),new ofVec3f(),     new ofVec3f(0,0,109),new ofVec3f(0,113,0),new ofVec3f(),    new ofVec3f(),      new ofVec3f(),      new ofVec3f(),      new ofVec3f(),
        new ofVec3f(),      new ofVec3f(),      new ofVec3f(0,125,0),new ofVec3f(105,0,0),new ofVec3f(0,0,117),new ofVec3f(0,97,0),new ofVec3f(),   new ofVec3f(),      new ofVec3f(),
        new ofVec3f(),      new ofVec3f(),      new ofVec3f(),      new ofVec3f(),      new ofVec3f(),      new ofVec3f(101,0,0),new ofVec3f(),     new ofVec3f(),      new ofVec3f(),
        NULL,               NULL,               NULL,               new ofVec3f(),      new ofVec3f(),      new ofVec3f(),      NULL,               NULL,               NULL
    };

    int tesselation3width = 9;
    origin*=1.125;
    origin += addTesselation(origin, tsqSize, tesselation3width, tesselationHeight, tesselation3map);

    origin*=1.15;

    ofQuaternion rot = ofQuaternion(45, ofVec3f(0,0,1), 90, ofVec3f(1,0,0), 33, ofVec3f(1,0,0));
    ofMatrix4x4 mat;
    rot.get(mat);

    for(std::vector<TesselationSquare*>::iterator it = tesselation.begin(); it != tesselation.end(); ++it)
    {
        TesselationSquare* t = *(it);
        t->setGlobalPosition(t->getPosition()-(origin*.5));
        ofQuaternion rotQuat = t->getOrientationQuat();
        t->rotateAround(rot, ofVec3f(0,0,0));
        rotQuat *= rot;
        t->setOrientation(rotQuat);
//        t->setGlobalPosition(t->getPosition()+ofVec3f(0,3*tsqSize, 0));
        tesselationRect.growToInclude(t->getPosition());
    }

    perlinNoiseImage.allocate(floor(tesselationRect.getWidth()*0.25), floor(tesselationRect.getHeight()*0.25),  OF_IMAGE_COLOR);

    ofSetFrameRate(30);
    
    setGUI();

}

ofVec3f ofApp::addTesselation(ofVec3f _origin, int _size, int _width, int _height, ofVec3f** _tesselationMap)
{

    ofVec3f offset, firstOffset;

    for (int i = 0; i < _height*_width ; i++)
    {
        int x = i%_width;
        int y = i/_width;

        ofColor c;

        offset = ofVec3f(-x*_size,(x+y)*_size,y*_size);

        if(y < 1) firstOffset = offset;

        ofVec3f* adresses = _tesselationMap[((_width-1)-x)+(_width*y)];

        if(adresses != NULL)
        {

            c = ofColor(255,255,255,255);

            TesselationSquare * tsq = new TesselationSquare();
            tsq->set(_size,_size, 2, 2);
            tsq->setGlobalPosition(ofVec3f(0, 0, -_size/2) + _origin + offset);
            tsq->setOrientation(ofVec3f(0,0,0));
            tsq->setup(floor(adresses->x));
            tsq->kelvinCold = kelvinCold;
            tsq->kelvinWarm = kelvinWarm;
            tsq->setColor((adresses->x > 0)? c : ofColor(255,255) );
            tesselation.push_back(tsq);

            tsq = new TesselationSquare();
            tsq->set(_size,_size,2 ,2);
            tsq->setGlobalPosition(ofVec3f(0,_size/2,0) + _origin + offset);
            tsq->setOrientation(ofVec3f(90,0,0));
            tsq->setup(floor(adresses->y));
            tsq->kelvinCold = kelvinCold;
            tsq->kelvinWarm = kelvinWarm;
            tsq->setColor((adresses->y > 0)? c : ofColor(255*0.9) );
            tesselation.push_back(tsq);

            tsq = new TesselationSquare();
            tsq->set(_size,_size,2,2);
            tsq->setGlobalPosition(ofVec3f(_size/2,0,0) + _origin + offset);
            tsq->setOrientation(ofVec3f(0,90,0));
            tsq->setup(floor(adresses->z));
            tsq->kelvinCold = kelvinCold;
            tsq->kelvinWarm = kelvinWarm;
            tsq->setColor((adresses->z > 0)? c : ofColor(255*0.8) );
            tesselation.push_back(tsq);

        }
    }
    return firstOffset;
}

void ofApp::setGUI()
{

    gui = new ofxUISuperCanvas("");
//    gui->addLabel("Press 'h' to Hide GUIs", OFX_UI_FONT_SMALL);
    gui->setWidth(ofGetWidth()/3.);
    gui->setPadding(8);
    gui->setGlobalSliderHeight(45);
    gui->setPosition(10,0);
    gui->setFontSize(OFX_UI_FONT_LARGE, 10);
    gui->setFontSize(OFX_UI_FONT_MEDIUM, 8);
    gui->setFontSize(OFX_UI_FONT_SMALL, 6);
    gui->setFont("GUI/DroidSans.ttf");
    //gui->addLabel("");
    temperatureLabel = gui->addLabel("TEMPERATURE", OFX_UI_FONT_LARGE);
    gui->addSpacer();
    gui->addLabel("Range", OFX_UI_FONT_SMALL);
    gui->addRangeSlider("tRange", kelvinWarm, kelvinCold, &kelvinWarmRange, &kelvinColdRange)->setColorBack(ofColor(48,48,48));
    gui->addSpacer();
    gui->addLabel("Speed", OFX_UI_FONT_SMALL);
    gui->addSlider("tSpeed",0.3,0.6,&temperatureSpeed)->setColorBack(ofColor(48,48,48));
    gui->addSpacer();
    gui->addLabel("Spread", OFX_UI_FONT_SMALL);
    gui->addSlider("tSpread",0,0.33,&temperatureSpread)->setColorBack(ofColor(48,48,48));
    gui->addSpacer();
    gui->addLabel("");
    brightnessLabel = gui->addLabel("BRIGHTNESS", OFX_UI_FONT_LARGE);
    gui->addSpacer();
    gui->addLabel("Range", OFX_UI_FONT_SMALL);
    gui->addRangeSlider("bRange", 0, 1, &brightnessRangeFrom, &brightnessRangeTo)->setColorBack(ofColor(48,48,48));
    gui->addSpacer();
    gui->addLabel("Speed", OFX_UI_FONT_SMALL);
    gui->addSlider("bSpeed",0.3,0.6,&brightnessSpeed)->setColorBack(ofColor(48,48,48));
    gui->addSpacer();
    gui->addLabel("Spread", OFX_UI_FONT_SMALL);
    gui->addSlider("bSpread",0,0.33,&brightnessSpread)->setColorBack(ofColor(48,48,48));
    gui->addSpacer();
    gui->addLabel("");
    gui->addLabel("Observational Instrument",OFX_UI_FONT_MEDIUM);
    gui->addLabel("- an installation from: LED Lighting; Interdisciplinary LED Lighting Research.\n\nThe research project has been a three-year collaboration between\nThe Royal Danish Academy of Fine Arts; Schools of Architecture,\nDesign and Conservation and The IT University of Copenhagen.\n\nAssociate Professor: Karin Soendergaard\nResearcher: Karina Munkholm Madsen\nAssociate Professor: Kjell Yngve Petersen\nResearcher: Ole Kristensen\nLighting designer: Imke Wies van Mil\nLighting designer: Jesper Kongshaug\nLighting designer: Christina Augustesen\nResearch Assistant: Thyge Waehrens", OFX_UI_FONT_SMALL);

/*  gui->addLabel("");
    gui->addLabel("Presets", OFX_UI_FONT_LARGE);
    gui->addSpacer();
    gui->addLabel("Keep settings as xml files", OFX_UI_FONT_SMALL);
    gui->addSpacer();
    gui->addLabelButton("Load", bLoadSettings, gui->getRect()->getWidth()-8, 45)->setColorBack(ofColor(48,48,48));
    gui->addLabelButton("Save", bSaveSettings, gui->getRect()->getWidth()-8, 45)->setColorBack(ofColor(48,48,48));
    gui->addLabelButton("Save PDFs", bSavePDF, gui->getRect()->getWidth()-8, 45)->setColorBack(ofColor(48,48,48));
    gui->addSpacer();
    gui->addFPS();
*/
    ofxUIImage * i = gui->addImage("ITU_LOGO", &logoITU, logoITU.getWidth(), logoITU.getHeight(), false);
    i->initRect(14,(1080-20)-logoITU.getHeight(),logoITU.getWidth(), logoITU.getHeight());
    i = gui->addImage("KADK_LOGO", &logoKADK, logoKADK.getWidth(), logoKADK.getHeight(), false);
    i->initRect(330,(1080-20)-logoKADK.getHeight(), logoKADK.getWidth(), logoKADK.getHeight());
    //gui->getRect()->setHeight(ofGetHeight());
    gui->autoSizeToFitWidgets();
    gui->setHeight(ofGetHeight());
    gui->disableAppDrawCallback();
//    gui->setAutoDraw(false);

    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.getName();
    int kind = e.getKind();

    if(name[0] == 't'){
        lastTemperatureManipulationSeconds = ofGetElapsedTimef();
    }

    if(name[0] == 'b'){
        lastBrightnessManipulationSeconds = ofGetElapsedTimef();
    }

    //cout << "got event from: " << name << endl;
    if(name == "Save")
    {
        ofxUIButton *button = (ofxUIButton *) e.getButton();
        if(button->getValue())
        {
            ofFileDialogResult dr = ofSystemSaveDialog( ofToDataPath("settings")+"/settings.xml", "Save settings");
            if(dr.bSuccess)
            {
                ofFile file(dr.filePath);
                if(file.getExtension() != "xml")
                {
                    file = ofFile(dr.filePath + ".xml");
                }
                gui->saveSettings(file.getAbsolutePath());
            }
        }
    }
    else if(name == "Load")
    {
        ofxUIButton *button = (ofxUIButton *) e.getButton();
        if(button->getValue())
        {

            ofFileDialogResult dr = ofSystemLoadDialog("Load settings", false, ofToDataPath("settings")+"/*.xml");

            if(dr.bSuccess)
            {
                ofFile file(dr.filePath);
                if(file.getExtension() == "xml")
                {
                    gui->loadSettings(file.getAbsolutePath());
                }
                loadedFileName = file.getBaseName();
            }

        }
    }
    else if(name == "Save PDFs")
    {
        ofxUIButton *button = (ofxUIButton *) e.getButton();
        if(button->getValue())
        {


            string path = ofToDataPath("settings");
            ofDirectory dir(path);
            //only show png files
            dir.allowExt("xml");
            //populate the directory object
            dir.listDir();
            
            //go through and print out all the paths
            for(int i = 0; i < dir.size(); i++)
            {
                ofLogNotice(dir.getPath(i));
                ofFile file(dir.getPath(i));
                gui->loadSettings(file.getAbsolutePath());
                loadedFileName = file.getBaseName();
                bSavePDF = true;
                update();
                draw();
            }
        }
    }
}


//--------------------------------------------------------------
void ofApp::update()
{

    if(ofGetFrameNum() > 1)
    {
        cam.disableMouseInput();
    }


    if(lastTemperatureManipulationSeconds + manipulationTimeoutSeconds < ofGetElapsedTimef()){

    kelvinColdRange = (kelvinColdRange*0.99) + (kelvinColdRangeDefault*0.01);
    kelvinWarmRange = (kelvinWarmRange*0.99) + (kelvinWarmRangeDefault*0.01);
    temperatureSpeed = (temperatureSpeed*0.99) + (temperatureSpeedDefault*0.01);
    temperatureSpread = (temperatureSpread*0.99) + (temperatureSpreadDefault*0.01);

    }

    if(lastBrightnessManipulationSeconds + manipulationTimeoutSeconds < ofGetElapsedTimef()){

    brightnessRangeFrom = (brightnessRangeFrom*0.99) + (brightnessRangeFromDefault*0.01);
    brightnessRangeTo = (brightnessRangeTo*0.99) + (brightnessRangeToDefault*0.01);
    brightnessSpeed = (brightnessSpeed*0.99) + (brightnessSpeedDefault*0.01);
    brightnessSpread = (brightnessSpread*0.99) + (brightnessSpreadDefault*0.01);

    }

//    dmx.clear();
    
    double temperatureSpreadCubic = powf(temperatureSpread, 3);
    double brightnessSpreadCubic = powf(brightnessSpread, 3);

    int imageWidth = perlinNoiseImage.getWidth();
    int imageHeight = perlinNoiseImage.getHeight();

    for(int x = 0; x < imageWidth; x++)
    {
        for(int y = 0; y < imageHeight; y++)
        {
            double xMapped = ofMap(x, 0, imageWidth, tesselationRect.getMinX(), tesselationRect.getMaxX());
            double yMapped = ofMap(y, imageHeight, 0, tesselationRect.getMinY(), tesselationRect.getMaxY());

            float brightness = ofNoise(xMapped*brightnessSpreadCubic, yMapped*brightnessSpreadCubic, brightnessTime);
            brightness = ofMap(brightness, 0, 1, brightnessRangeFrom, brightnessRangeTo);

            float tempNoise = ofNoise(xMapped*temperatureSpreadCubic, yMapped*temperatureSpreadCubic, temperatureTime);
            unsigned int temp = round(ofMap(tempNoise, 0, 1, kelvinWarmRange, kelvinColdRange));

            ofColor c = LedFixture::temperatureToColor(temp);
            c *= brightness;

            perlinNoiseImage.setColor(x,y,c);
        }
    }
    perlinNoiseImage.update();

    #pragma omp parallel for
    for(int i = 0; i < tesselation.size(); i++)
    {
        TesselationSquare* t = tesselation[i];
        if(t->DMXchannels.size() > 0)
        {
            float tempNoise = ofNoise(t->getX()*temperatureSpreadCubic, t->getY()*temperatureSpreadCubic, temperatureTime);
            unsigned int temp = round(ofMap(tempNoise, 0, 1, fmaxf(t->kelvinWarm, kelvinWarmRange), fminf(t->kelvinCold, kelvinColdRange)));
            t->setTemperature(temp);

            float brightness = ofNoise(t->getX()*brightnessSpreadCubic, t->getY()*brightnessSpreadCubic, brightnessTime);
            brightness = ofMap(brightness, 0, 1, brightnessRangeFrom, brightnessRangeTo);
            if(t->DMXstartAddress > 0)
            {
                t->setBrightness(brightness);
            }

            for(std::vector<DMXchannel*>::iterator chIt = t->DMXchannels.begin(); chIt != t->DMXchannels.end(); ++chIt)
            {
                DMXchannel* c = *(chIt);
                float value = 0;
                if(c->type == DMX_CHANNEL_CW)
                {
                    value = ofMap(t->temperature, t->kelvinCold, t->kelvinWarm, 0, 1.);
                    value = fminf(1.,ofMap(value, 0 , 0.5, 0., 1.));
                }
                if(c->type == DMX_CHANNEL_WW)
                {
                    value = ofMap(t->temperature, t->kelvinWarm, t->kelvinCold, 0, 1.);
                    value = fminf(1.,ofMap(value, 0 , 0.5, 0., 1.));
                }
                value *= t->brightness;

                if(c->width16bit)
                {
                    unsigned int valueInt = ofMap(value, 0.,1., 0, pow(255,2));
                    dmx.setLevel(c->address, valueInt/255);
                    dmx.setLevel(c->address+1, valueInt%255);
                }
                else
                {
                    ofLogError() << "8 bit not implemented" << std::endl;
                }
            }
        }
        t->update();
    }

    dmx.update();
    
    //  cameraController.update();

    float now = ofGetElapsedTimef() + timeOffset;
    temperatureTime += powf(temperatureSpeed,8) * ( ( now - lastFrameSeconds ) / (1./60));
    brightnessTime += powf(brightnessSpeed,8) * ( ( now - lastFrameSeconds ) / (1./60));
    lastFrameSeconds = now;
}

//--------------------------------------------------------------
void ofApp::draw()
{
    if( bSavePDF )
    {
        ofBeginSaveScreenAsPDF(loadedFileName+" - "+ofGetTimestampString()+".pdf", false);
        ofBackground(255);
        ofSetColor(255);
        ofPushMatrix();{

        ofTranslate(-ofGetWidth()/12.5,-ofGetWidth()/60);

        // perlin noise

        ofPushMatrix();
        {
            ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);
            ofTranslate(0, -ofGetHeight()/4.0);

            float scaleFactor = tesselationRect.getWidth() / perlinNoiseImage.getWidth();
            ofScale(scaleFactor, scaleFactor);
            ofTranslate(-perlinNoiseImage.getWidth()/2.0, -perlinNoiseImage.getHeight()/2.0);

            ofTranslate(-4.6,0); // manual corrections
            float manualScaleFactor = 1.024;
            ofScale(manualScaleFactor,manualScaleFactor);

            perlinNoiseImage.draw(0,0);

            // data

            ofTranslate(0, perlinNoiseImage.getHeight() * 3.0);

            ofScale(1.0/manualScaleFactor,1.0/manualScaleFactor); // negate the manual scale
            ofScale(1.0/scaleFactor, 1.0/scaleFactor); // negate the automatic scale

            ofSetColor(0,255);

            printFontHeader.drawStringAsShapes(loadedFileName,0,0);

            float textBoxWidth = (perlinNoiseImage.getWidth()*scaleFactor*manualScaleFactor);
            float columnWidth = textBoxWidth/3.0;
            float graphWidth = columnWidth * 0.7;
            float valueTextWidth = columnWidth * 0.3;
            float columnOffsetWidth = columnWidth + (valueTextWidth/2.0);

            ofTranslate(columnOffsetWidth, 0);

            ofSetColor(64,255);

            ofPushMatrix();
            {

                printFontHeader.drawStringAsShapes("Temperature",0,0);

                ofTranslate(0, 24);

                /* void ofApp::drawSliderForPdf(string name,
                                                  float x, float y, float width, float height,
                                                  float valueMin, float valueMax,
                                                  float value,
                                                  float valueLow, float valueHigh){
                */

                drawSliderForPdf("range",
                                 0, 0, graphWidth, 12,
                                 kelvinWarm, kelvinCold,
                                 0,
                                 kelvinWarmRange, kelvinColdRange);

                ofTranslate(0, 24);

                drawSliderForPdf("speed",
                                 0, 0, graphWidth, 12,
                                 0, 1,
                                 temperatureSpeed);

                ofTranslate(0, 24);

                drawSliderForPdf("spread",
                                 0, 0, graphWidth, 12,
                                 0, 1,
                                 temperatureSpread);

            }
            ofPopMatrix();

            ofTranslate(columnOffsetWidth, 0);

            ofPushMatrix();
            {

                printFontHeader.drawStringAsShapes("Brightness",0,0);

                ofTranslate(0, 24);

                /* void ofApp::drawSliderForPdf(string name,
                                                  float x, float y, float width, float height,
                                                  float valueMin, float valueMax,
                                                  float value,
                                                  float valueLow, float valueHigh){
                */

                drawSliderForPdf("range",
                                 0, 0, graphWidth, 12,
                                 0, 1,
                                 0,
                                 brightnessRangeFrom, brightnessRangeTo);

                ofTranslate(0, 24);

                drawSliderForPdf("speed",
                                 0, 0, graphWidth, 12,
                                 0, 1,
                                 brightnessSpeed);

                ofTranslate(0, 24);

                drawSliderForPdf("spread",
                                 0, 0, graphWidth, 12,
                                 0, 1,
                                 brightnessSpread);

            }
            ofPopMatrix();


        }
        ofPopMatrix();

        // tesselation

        ofPushMatrix();
        {
            ofTranslate(ofGetWidth()/2, ofGetHeight()/2.15);

            for(std::vector<TesselationSquare*>::iterator it = tesselation.begin(); it != tesselation.end(); ++it)
            {
                TesselationSquare* t = *(it);
                t->drawForPdf();
            }

        }
        ofPopMatrix();

            }ofPopMatrix();

        ofEndSaveScreenAsPDF();
        bSavePDF = false;
    }

    ofBackgroundGradient(ofColor(40), ofColor(10), OF_GRADIENT_CIRCULAR);
    glEnable(GL_DEPTH_TEST);
    //ofEnableSmoothing();
    float viewportWidth = (ofGetWidth()-gui->getRect()->getWidth());
    float scale = viewportWidth * 1.35 / ofGetWidth();
    ofPushView();
    ofViewport(gui->getRect()->getWidth(),0,viewportWidth,ofGetHeight());
    cam.begin();
    ofPushMatrix();
    ofTranslate(viewportWidth/2, ofGetHeight()/3.0);
    ofScale(scale, scale, scale);
    for(std::vector<TesselationSquare*>::iterator it = tesselation.begin(); it != tesselation.end(); ++it)
    {
        TesselationSquare* t = *(it);
        t->draw();
    }
    
    
    ofTranslate(0, -ofGetHeight()/3.0);

    ofSetColor(255,255);
    glDisable(GL_DEPTH_TEST);
    perlinNoiseImage.draw(tesselationRect.getMinX(),tesselationRect.getMinY(),tesselationRect.getWidth(), tesselationRect.getHeight());
    ofPopMatrix();
    cam.end();
    ofPopView();
    
    ofPath tArcPath;
    tArcPath.setArcResolution(360);
    
    tArcPath.arc(gui->getPaddingRect()->getMaxX()-20,temperatureLabel->getRect()->getMaxY()-temperatureLabel->getRect()->getHalfHeight(),10,10,
                 0, 359.9*(fmaxf(-1.0,(lastTemperatureManipulationSeconds-ofGetElapsedTimef())/manipulationTimeoutSeconds))
                 );
    tArcPath.draw();

    ofPath bArcPath;
    bArcPath.setArcResolution(360);
    bArcPath.arc(gui->getPaddingRect()->getMaxX()-20,brightnessLabel->getRect()->getMaxY()-brightnessLabel->getRect()->getHalfHeight(),10,10,
                 0, 359.9*(fmaxf(-1.0,(lastBrightnessManipulationSeconds-ofGetElapsedTimef())/manipulationTimeoutSeconds))
                 );
    bArcPath.draw();
    
    gui->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    if(key == 'f')
    {
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    gui->getRect()->setHeight(ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}

void ofApp::drawSliderForPdf(string name, float x, float y, float width, float height,
                               float valueMin, float valueMax, float value, float valueLow, float valueHigh)
{
    ofFill();

    ofPushStyle();
    ofSetColor((ofGetStyle().color * 0.5)+127);
    ofDrawRectangle(x,y,width,height);
    ofPopStyle();

    string valueString = "";

    if(valueHigh == -1 && valueLow == -1)
    {
        ofDrawRectangle(x, y, ofMap(value, valueMin, valueMax, 0, width), height);
        valueString = ofToString(value);
    }
    else
    {
        ofDrawRectangle(x + ofMap(valueLow, valueMin, valueMax, 0, width), y, ofMap(valueHigh, valueMin, valueMax, 0, width-ofMap(valueLow, valueMin, valueMax, 0, width)), height);
        int rounding = ((valueMax - valueMin) / 1000.0 > 1.0 )? 0 : 3;
        valueString = ofToString(valueLow, rounding) + " ... " + ofToString(valueHigh, rounding);
    }

    ofPushMatrix();

    ofTranslate(-(printFontText.stringWidth(name)+height), height);
    printFontText.drawStringAsShapes(name, 0, 0);

    ofPopMatrix();

    ofPushMatrix();

    ofTranslate(width+height, height);
    printFontText.drawStringAsShapes(valueString, 0, 0);

    ofPopMatrix();

    /*  outline

        ofNoFill();
        ofPushStyle();
        ofSetColor((ofGetStyle().color * 0.5)+127);
        ofRect(x,y,width,height);
        ofPopStyle();
    */

}
