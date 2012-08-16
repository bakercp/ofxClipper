#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

#include "ofxGui.h"
#include "ofxClipper.h"

enum TestPolyType {
    CIRCLES,
    SQUARES,
    RANDOM_POLY,
};

class testApp : public ofxiPhoneApp{
	
    public:
        void setup();
        void update();
        void draw();
        void exit();
	
        void touchDown(ofTouchEventArgs & touch);
        void touchMoved(ofTouchEventArgs & touch);
        void touchUp(ofTouchEventArgs & touch);
        void touchDoubleTap(ofTouchEventArgs & touch);
        void touchCancelled(ofTouchEventArgs & touch);

        void lostFocus();
        void gotFocus();
        void gotMemoryWarning();
        void deviceOrientationChanged(int newOrientation);

    
    ofxClipper clipper;
    bool bNeedsUpdate;
    
    ofxClipperClipType currentClipperType;
    ofxClipperJoinType currentClipperJoinType;
    
    TestPolyType clipSubjectType;
    ofxPolylines clipSubjects;
    
    TestPolyType clipMaskType;
    ofxPolylines clipMasks;
    
    ofxPolylines clips;
    ofxPolylines offsets;
    
    // minimal gui
    
    ofPath complexPath;
    ofxPolylines simplifiedPath;
    
    ofxPanel clipTypePanel;
    ofxIntSlider clipTypeSlider;
    ofxIntSlider nClipMasks;
    ofxIntSlider maskTypeSlider;
    ofxIntSlider nClipSubjects;
    ofxIntSlider subjectTypeSlider;
    ofxToggle      useMouseClipper;
    
    
    ofxPanel offsetPanel;
    ofxIntSlider joinTypeSlider;
    ofxFloatSlider miterLimitSlider;
    ofxFloatSlider offsetDeltaSlider;
    ofxToggle      enableOffsetsToggle;
    
    ofxPanel simplifyPanel;
    ofxIntSlider simplifyPath;
    bool bSimplifyPathNeedsUpdate;
    
    void genSimplifyPath(int &ct);
    
    void genSubjects(int & ct);
    void genMasks(int & ct);
    
    
    void clipType(int & ct);
    void joinType(int & ct);
    void miterLimit(float & f);
    void offsetDelta(float & f);
    void subjectPolyType(int & ct);
    void maskPolyType(int & ct);
    
    void mouseClipper(bool & b);
    
    void makeMouseClip();
    bool mclip;

};


