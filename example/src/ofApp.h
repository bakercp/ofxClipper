//
// Copyright (c) 2010 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include "ofMain.h"
#include "ofxGui.h"
#include "ofxClipper.h"


class ofApp: public ofBaseApp
{
public:
    enum TestPolyType
    {
        CIRCLES,
        SQUARES,
        RANDOM_POLY,
    };

    void setup();
    void update();
    void draw();

    void mouseMoved(int x, int y);

    ofx::Clipper clipper;
    bool bNeedsUpdate;

    ClipperLib::ClipType currentClipperType;
    ClipperLib::JoinType currentClipperJoinType;
    ClipperLib::EndType currentClipperEndType;

    TestPolyType clipSubjectType;
    std::vector<ofPolyline> clipSubjects;

    TestPolyType clipMaskType;
    std::vector<ofPolyline> clipMasks;
    
    std::vector<ofPolyline> clips;
    std::vector<ofPolyline> offsets;

    ofRectangle boundingBox;

    // minimal gui

    ofPath complexPath;
    std::vector<ofPolyline> simplifiedPath;
    
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
    ofxToggle      enableOffsetsToggle;
    
    ofxPanel simplifyPanel;
    ofxIntSlider simplifyPath;
    bool bSimplifyPathNeedsUpdate;
    
    void genSimplifyPath(ofxSlider<int>& ct);
    
    void genSubjects(ofxSlider<int>& ct);
    void genMasks(ofxSlider<int>& ct);
    
    
    void clipType(int& ct);
    void joinType(int& ct);
    void endType(int& ct);
    void miterLimit(float& f);
    void offsetDelta(float& f);
    void subjectPolyType(int& ct);
    void maskPolyType(int& ct);
    
    void mouseClipper(bool& b);
    
    void makeMouseClip();
    bool mclip;

};
