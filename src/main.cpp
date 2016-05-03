#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    
    ofGLFWWindowSettings windowSettings;
    
    windowSettings.setGLVersion(4, 1);
    windowSettings.numSamples = 4;


    // Get screen widths and heights from Quartz Services
    // See https://developer.apple.com/library/mac/documentation/GraphicsImaging/Reference/Quartz_Services_Ref/index.html
    
    CGDisplayCount displayCount;
    CGDirectDisplayID displays[32];
    
    // Grab the active displays
    CGGetActiveDisplayList(32, displays, &displayCount);
    int numDisplays= displayCount;
    
    // print display info.
    cout<<numDisplays<<" display(s) detected."<<endl;
    
    for (int i = 0; i < displayCount; i++){
        cout << displays[i] << "\t(" << CGDisplayPixelsWide(displays[i]) << "x" << CGDisplayPixelsHigh(displays[i]) << ")"<< endl;
    }
    
    CGRect mainDisplayBounds= CGDisplayBounds ( displays[0] );
    
    if(numDisplays == 1 ){
        
        windowSettings.width = mainDisplayBounds.size.width;
        windowSettings.height = mainDisplayBounds.size.height;
        windowSettings.decorated = true;
        windowSettings.resizable = true;
        
        cout << "configured default one display setup" << endl;
        
    } else if (numDisplays > 1){
        
        // two displays: palce resizeable gui on first and fill second with undecorated mainWindow - will also work with dual head configuraions for projectors
        
        CGRect secondDisplayBounds= CGDisplayBounds ( displays[1] );
        
        windowSettings.width = secondDisplayBounds.size.width;
        windowSettings.height = secondDisplayBounds.size.height;
        windowSettings.setPosition(ofVec2f(secondDisplayBounds.origin.x,secondDisplayBounds.origin.y));
        windowSettings.decorated = false;
        windowSettings.resizable = false;

        cout << "configured default two display setup" << endl;
    }

    shared_ptr<ofApp> mainApp(new ofApp);

    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(windowSettings);
    
    ofPoint screenSize = mainWindow->getScreenSize();
    
    mainWindow->setWindowPosition(screenSize.x, 0);
    mainWindow->setFullscreen(true);

    ofRunApp(mainWindow, mainApp);
    
    ofRunMainLoop();

}
