// =============================================================================
//
// Copyright (c) 2010-2014 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#pragma once


#include "ofMain.h"
#include "ofxGui.h"
#include "ofxClipper.h"


enum TestPolyType
{
    CIRCLES,
    SQUARES,
    RANDOM_POLY,
};


class ofApp: public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();

    void mouseMoved(int x, int y);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    ofxClipper clipper;
    bool bNeedsUpdate;

    ofxClipperClipType currentClipperType;
    ofxClipperJoinType currentClipperJoinType;
    ofxClipperEndType currentClipperEndType;
    
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
