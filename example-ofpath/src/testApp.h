#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxClipper.h"

enum TestPolyType {
    CIRCLES,
    SQUARES,
    RANDOM_POLY,
};

class testApp : public ofBaseApp {

public:
    void setup();
    void update();
    void draw();

    void mouseMoved( int x, int y );
    void mousePressed( int x, int y, int button);
    void mouseReleased( int x, int y, int button);

    ofxClipper clipper;
    bool bNeedsUpdate;

    ofxClipperClipType currentClipperType;
    ofxClipperJoinType currentClipperJoinType;
    ofxClipperEndType currentClipperEndType;
    
    TestPolyType clipSubjectType;
    ofPath clipSubjects;

    TestPolyType clipMaskType;
    ofPath clipMasks;
    
    ofPath clips;
    ofPath offsets;

    // minimal gui

    ofPath complexPath;
    ofPath simplifiedPath;
    
    ofxPanel clipTypePanel;
    ofxIntSlider clipTypeSlider;
    ofxIntSlider nClipMasks;
    ofxIntSlider maskTypeSlider;
    ofxIntSlider nClipSubjects;
    ofxIntSlider subjectTypeSlider;
    ofxToggle useMouseClipper;

    
    ofxPanel offsetPanel;
    ofxIntSlider joinTypeSlider;
    ofxIntSlider endTypeSlider;
    ofxFloatSlider miterLimitSlider;
    ofxFloatSlider offsetDeltaSlider;
    ofxToggle enableOffsetsToggle;
    
    ofxPanel simplifyPanel;
    ofxIntSlider simplifyPath;
    bool bSimplifyPathNeedsUpdate;
    
    void genSimplifyPath(ofxSlider<int> &ct);
    
    void genSubjects(ofxSlider<int> & ct);
    void genMasks(ofxSlider<int> & ct);
    
    
    void clipType(int & ct);
    void joinType(int & ct);
    void endType(int & ct);
    void miterLimit(float & f);
    void offsetDelta(float & f);
    void subjectPolyType(int & ct);
    void maskPolyType(int & ct);
    
    void mouseClipper(bool & b);
    
    void makeMouseClip();
    bool mclip;
    
    
};
