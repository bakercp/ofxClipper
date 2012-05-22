#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxClipper.h"

enum TestPolyType {
    CIRCLES,
    SQUARES,
    RANDOM_POLY
};

class testApp : public ofBaseApp {

public:
    void setup();
    void update();
    void draw();

    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    ofxClipper clipper;
    bool bNeedsUpdate;

    ofxClipperClipType currentClipperType;
    ofxClipperJoinType currentClipperJoinType;
    
    TestPolyType clipSubjectType;
    ofPolylines clipSubjects;

    TestPolyType clipMaskType;
    ofPolylines clipMasks;
    
    ofPolylines clips;
    ofPolylines offsets;

    // minimal gui

    
    ofxPanel clipTypePanel;
    ofxIntSlider clipTypeSlider;
    ofxIntSlider nClipMasks;
    ofxIntSlider maskTypeSlider;
    ofxIntSlider nClipSubjects;
    ofxIntSlider subjectTypeSlider;
    
    
    ofxPanel offsetPanel;
    ofxIntSlider joinTypeSlider;
    ofxFloatSlider miterLimitSlider;
    ofxFloatSlider offsetDeltaSlider;
    ofxToggle      enableOffsetsToggle;
    
    void genSubjects(int & ct);
    void genMasks(int & ct);
    
    
    void clipType(int & ct);
    void joinType(int & ct);
    void miterLimit(float & f);
    void offsetDelta(float & f);
    void subjectPolyType(int & ct);
    void maskPolyType(int & ct);
    
    
    
    
};
