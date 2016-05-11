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
    
    CGDirectDisplayID mainDisplay;
    CGDirectDisplayID secondaryDisplay;
    
    // Grab the active displays
    CGGetActiveDisplayList(32, displays, &displayCount);
    
    int numDisplays= displayCount;
    
    // print display info.
    cout<<numDisplays<<" display(s) detected."<<endl;

    for (int i = 0; i < displayCount; i++){
        if(CGDisplayIsMain(displays[i])){
            mainDisplay = displays[i];
        } else {
            secondaryDisplay = displays[i];
        }
    }
    
    if(numDisplays == 1 ){

        CGRect mainDisplayBounds= CGDisplayBounds ( mainDisplay );
        
        windowSettings.width = mainDisplayBounds.size.width;
        windowSettings.height = mainDisplayBounds.size.height;
        windowSettings.setPosition(ofVec2f(mainDisplayBounds.origin.x, mainDisplayBounds.origin.y));
        windowSettings.decorated = false;
        windowSettings.resizable = false;
        
        cout << "configured default one display setup" << endl;
        
    } else if (numDisplays > 1){

        CGRect secondDisplayBounds= CGDisplayBounds ( secondaryDisplay );
        
        windowSettings.width = secondDisplayBounds.size.width;
        windowSettings.height = secondDisplayBounds.size.height;
        windowSettings.setPosition(ofVec2f(secondDisplayBounds.origin.x,secondDisplayBounds.origin.y));
        windowSettings.decorated = false;
        windowSettings.resizable = false;

        cout << "configured default two display setup" << endl;
    }

    shared_ptr<ofApp> mainApp(new ofApp);

    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(windowSettings);
    
    ofRunApp(mainWindow, mainApp);
    
    ofRunMainLoop();

}
