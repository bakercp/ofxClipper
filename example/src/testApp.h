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
