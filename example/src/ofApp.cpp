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


#include "ofApp.h"


void ofApp::setup()
{

    ofSetBackgroundColor(0);
    
    bNeedsUpdate = true; // a housekeeping variable to respond to gui changes
    
    currentClipperType     = OFX_CLIPPER_INTERSECTION;      // the clip type
    currentClipperJoinType = OFX_CLIPPER_JOINTYPE_SQUARE;   // for offsets, the joint type
    currentClipperEndType = OFX_CLIPPER_ENDTYPE_SQUARE;   // for offsets, the joint type
    
    clipSubjectType = CIRCLES;  // what kind of test subjects to create
    clipMaskType    = CIRCLES;  // what kind of test clip masks to create

    // gui setup
    clipTypePanel.setup("clipper","settings.xml", 10, 10);
    clipTypePanel.add(clipTypeSlider.setup("TYPE: INTERSECTION",0,0,3));
    clipTypePanel.add(subjectTypeSlider.setup("SUBJ. POLYS: CIRCLES",0,0,2));
    clipTypePanel.add(nClipSubjects.setup("# clip subjects",2,1,100));
    clipTypePanel.add(maskTypeSlider.setup("CLIP POLYS: CIRCLES",0,0,2));
    clipTypePanel.add(nClipMasks.setup("# clip masks   ",1,1,100));
    clipTypePanel.add(useMouseClipper.setup("Use Mouse Clip Mask", true));
               
    useMouseClipper.addListener(this,&ofApp::mouseClipper);
    
    clipTypeSlider.addListener(this,&ofApp::clipType);
    
    maskTypeSlider.addListener(this,&ofApp::maskPolyType);
    subjectTypeSlider.addListener(this,&ofApp::subjectPolyType);
    
//    nClipMasks.addListener(this,&ofApp::genMasks);              // ! results in poco non viable overload, how to fix?
//    nClipSubjects.addListener(this,&ofApp::genSubjects);        // ! results in poco non viable overload, how to fix?
    
    offsetPanel.setup("offsets","settings.xml", 10, 200);
    offsetPanel.add(enableOffsetsToggle.setup("show offset", true));
    offsetPanel.add(offsetDeltaSlider.setup("offset delta",20,-200,200));
    offsetPanel.add(joinTypeSlider.setup("JOIN TYPE: SQUARE",0,0,2));
    offsetPanel.add(endTypeSlider.setup("END TYPE: SQUARE",0,0,3));
    offsetPanel.add(miterLimitSlider.setup("miter limit",2,0,30));
    
    joinTypeSlider.addListener(this,&ofApp::joinType);
    endTypeSlider.addListener(this,&ofApp::endType);
    miterLimitSlider.addListener(this,&ofApp::miterLimit);
    offsetDeltaSlider.addListener(this,&ofApp::offsetDelta);

    simplifyPanel.setup("Simplify","settings.xml", ofGetWidth()-100,10);
    simplifyPanel.add(simplifyPath.setup("num vertices",10,3,40));
//    simplifyPath.addListener(this,&ofApp::genSimplifyPath);     // ! results in poco non viable overload, how to fix?
    
    genSimplifyPath(simplifyPath);
    bSimplifyPathNeedsUpdate = true;

    
    // generate some initial data
    genSubjects(nClipSubjects);
    genMasks(nClipMasks);
    mclip = useMouseClipper;
    
}


void ofApp::update()
{

    if (bNeedsUpdate)
    {
        // clear out our temporary clipped data
        clips.clear();
        
        // clear the clipper's internal polys
        clipper.clear();
        // add the clipper subjects (i.e. the things that will be clipped)
        clipper.addPolylines(clipSubjects,OFX_CLIPPER_SUBJECT);
        
        // add the clipper masks (i.e. the things that will do the clipping)
        clipper.addPolylines(clipMasks,OFX_CLIPPER_CLIP);

        // execute the clipping operation based on the current clipping type
        clipper.clip(currentClipperType,clips);
        
        if (bSimplifyPathNeedsUpdate)
        {
            ofxClipper::SimplifyPath(complexPath, simplifiedPath);
        }
        
        // if we have offsets enabled, generate the offsets
        if (enableOffsetsToggle)
        {
            offsets.clear();
            ofxClipper::OffsetPolylines(clips, 
                                       offsets, 
                                       offsetDeltaSlider,
                                       currentClipperJoinType,
                                       currentClipperEndType,
                                       miterLimitSlider);
        }
        
        
        bNeedsUpdate = false;
    }
    

}


void ofApp::draw()
{

    ofBackground(0);

    ofEnableAlphaBlending();

    // make a little legend for the color
    ////////////////////////////////////////////////
    int y = ofGetHeight() - 60;
    int x = 10;
    int h = 12;
    int w = 30;
    
    ofColor cSubject(255,0,0,200);
    ofColor cMask(255,255,0,200);
    ofColor cResult(0,255,0,200);
    ofColor cOffset(0,0,255,200);
    
    ofFill();
    ofSetColor(cSubject);
    ofRect(x,y-h+2,w,h);
    ofSetColor(255);
    ofDrawBitmapString("Polygon Clip Subjects", ofVec3f(x + w + 2,y));
    
    y+= 16;
    ofFill();
    ofSetColor(cMask);
    ofRect(x,y-h+2,w,h);
    ofSetColor(255);
    ofDrawBitmapString("Polygon Clip Masks", ofVec3f(x + w + 2,y));

    y+= 16;
    ofFill();
    ofSetColor(cResult);
    ofRect(x,y-h+2,w,h);
    ofSetColor(255);
    ofDrawBitmapString("Clip Results", ofVec3f(x + w + 2,y));

    y+= 16;
    ofFill();
    ofSetColor(cOffset);
    ofRect(x,y-h+2,w,h);
    ofSetColor(255);
    ofDrawBitmapString("Offset Results", ofVec3f(x + w + 2,y));

    
    // draw all of the pieces
    ////////////////////////////////////////////////
    ofPushMatrix();
    ofTranslate(-ofGetWidth() / 4, 0);

    for(int i = 0; i < clipSubjects.size(); ++i)
    {
        ofSetColor(cSubject);
        ofNoFill();
        clipSubjects[i].draw();
    }
    
    for(int i = 0; i < clipMasks.size(); ++i)
    {
        ofSetColor(cMask);
        ofNoFill();
        clipMasks[i].draw();
    }

    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 4, 0);
    
    for (int i = 0; i < clips.size(); ++i)
    {
        ofSetColor(cResult);
        ofNoFill();
        clips[i].draw();
    }

    if (enableOffsetsToggle)
    {
        for (int i = 0; i < offsets.size(); ++i)
        {
            ofSetColor(cOffset);
            ofNoFill();
            offsets[i].draw();
        }
    }
    
    ofPopMatrix();
    
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() * 2 / 4, 100);
    complexPath.draw();
    for (int i = 0; i < simplifiedPath.size(); ++i)
    {
        ofSetColor(255,255,0);
        simplifiedPath[i].draw();
    }
    ofPopMatrix();
    
    // draw the gui
    ////////////////////////////////////////////////
    clipTypePanel.draw();
    offsetPanel.draw();

    
    ofDisableAlphaBlending();

}



void ofApp::makeMouseClip()
{
    if (mclip)
    {
        clipMasks.clear();
        ofPolyline p;
        ofVec3f ctr = ofVec3f(ofGetMouseX() + ofGetWidth() / 4, ofGetMouseY());
        p.arc(ctr,100,100,0,360);
        p.close();
        clipMasks.push_back(p);
        bNeedsUpdate = true;
    }
}


void ofApp::mousePressed(int x, int y, int button)
{
    makeMouseClip();
}



void ofApp::mouseReleased(int x, int y, int button)
{
    makeMouseClip();
}


void ofApp::mouseMoved(int x, int y)
{
    makeMouseClip();
}


void ofApp::genSubjects(ofxSlider<int>& ct)
{
    clipSubjects.clear();
    
    ofVec3f screenCtr = ofVec3f(ofGetWidth()/2, ofGetHeight()/2);
    
    for (int i = 0; i < ct; ++i)
    {
        if (clipSubjectType == CIRCLES)
        {
            ofPolyline p;
            float rad = ofRandom(50,120);
            ofVec3f ctr = screenCtr + ofVec3f(ofRandom(-100,100),ofRandom(-100,100)); 
            p.arc(ctr,rad,rad,0,360);
            p.close();
            clipSubjects.push_back(p);
        }
        else if (clipSubjectType == SQUARES)
        {
            ofRectangle p;
            float rad = ofRandom(50,120);
            ofVec3f ctr = screenCtr + ofVec3f(ofRandom(-100,100),ofRandom(-100,100)); 
            p.setFromCenter(ctr.x,ctr.y,rad,rad);
            ofPolyline r;
            r.addVertex(ofVec3f(p.x,p.y));
            r.addVertex(ofVec3f(p.x+p.width,p.y));
            r.addVertex(ofVec3f(p.x+p.width,p.y+p.height));
            r.addVertex(ofVec3f(p.x,p.y+p.height));
            r.close();
            clipSubjects.push_back(r);
        }
        else if(clipSubjectType == RANDOM_POLY)
        {
        }
    }
    
    if (clipSubjectType == RANDOM_POLY)
    {
        ofPolyline r;
        
        for (int i = 0; i < ct; ++i)
        {
            ofVec3f ctr = screenCtr + ofVec3f(ofRandom(-150,150),ofRandom(-150,150)); 
            r.addVertex(ctr);
        }
        r.close();
        clipSubjects.push_back(r);
    }
 
    
    bNeedsUpdate = true;
}


void ofApp::genMasks(ofxSlider<int>& ct)
{
    clipMasks.clear();
    ofVec3f screenCtr = ofVec3f(ofGetWidth()/2, ofGetHeight()/2);

    for (int i = 0; i < ct; ++i)
    {
        if (clipMaskType == CIRCLES)
        {
            ofPolyline p;
            float rad = ofRandom(50,120);
            ofVec3f ctr = screenCtr + ofVec3f(ofRandom(-100,100),ofRandom(-100,100)); 
            p.arc(ctr,rad,rad,0,360);
            p.close();
            clipMasks.push_back(p);
        }
        else if(clipMaskType == SQUARES)
        {
            ofRectangle p;
            float rad = ofRandom(50,120);
            ofVec3f ctr = screenCtr + ofVec3f(ofRandom(-100,100),ofRandom(-100,100)); 
            p.setFromCenter(ctr.x,ctr.y,rad,rad);
            ofPolyline r;
            r.addVertex(ofVec3f(p.x,p.y));
            r.addVertex(ofVec3f(p.x+p.width,p.y));
            r.addVertex(ofVec3f(p.x+p.width,p.y+p.height));
            r.addVertex(ofVec3f(p.x,p.y+p.height));
            r.close();
            clipMasks.push_back(r);
        }
        else if (clipMaskType == RANDOM_POLY)
        {
        }
    }

    if (clipMaskType == RANDOM_POLY)
    {
        ofPolyline r;
        for (int i = 0; i < ct; ++i)
        {
            ofVec3f ctr = screenCtr + ofVec3f(ofRandom(-150,150),ofRandom(-150,150)); 
            r.addVertex(ctr);
        }

        r.close();
        clipMasks.push_back(r);
    }
    

    bNeedsUpdate = true;
}


void ofApp::clipType(int & ct)
{
    switch (ct) {
        case 0:
            currentClipperType = OFX_CLIPPER_INTERSECTION;
            clipTypeSlider.setName("TYPE: INTERSECTION");
            break;
        case 1:
            currentClipperType = OFX_CLIPPER_UNION;
            clipTypeSlider.setName("TYPE: UNION");
            break;
        case 2:
            currentClipperType = OFX_CLIPPER_DIFFERENCE;
            clipTypeSlider.setName("TYPE: DIFFERENCE");
            break;
        case 3:
            currentClipperType = OFX_CLIPPER_INTERSECTION;
            clipTypeSlider.setName("TYPE: XOR");
            break;
        default:
            break;
    }

    bNeedsUpdate = true;
}


void ofApp::joinType(int& ct)
{
    switch (ct)
    {
        case 0:
            currentClipperJoinType = OFX_CLIPPER_JOINTYPE_SQUARE;
            joinTypeSlider.setName("JOIN TYPE: SQUARE");
            break;
        case 1:
            currentClipperJoinType = OFX_CLIPPER_JOINTYPE_ROUND;
            joinTypeSlider.setName("JOIN TYPE: ROUND");
            break;
        case 2:
            currentClipperJoinType = OFX_CLIPPER_JOINTYPE_MITER;
            joinTypeSlider.setName("JOIN TYPE: MITER");
            break;
        default:
            break;
    }

    bNeedsUpdate = true;
}


void ofApp::endType(int& ct)
{
    switch (ct)
    {
        case 0:
            currentClipperEndType = OFX_CLIPPER_ENDTYPE_SQUARE;
            endTypeSlider.setName("END TYPE: SQUARE");
            break;
        case 1:
            currentClipperEndType = OFX_CLIPPER_ENDTYPE_ROUND;
            endTypeSlider.setName("END TYPE: ROUND");
            break;
        case 2:
            currentClipperEndType = OFX_CLIPPER_ENDTYPE_BUTT;
            endTypeSlider.setName("END TYPE: BUTT");
            break;
        case 3:
            currentClipperEndType = OFX_CLIPPER_ENDTYPE_CLOSED;
            endTypeSlider.setName("END TYPE: CLOSED");
            break;
        default:
            break;
    }

    bNeedsUpdate = true;
}


void ofApp::miterLimit(float& f)
{
    bNeedsUpdate = true;
}


void ofApp::subjectPolyType(int& ct)
{
    switch (ct)
    {
        case 0:
            clipSubjectType = CIRCLES;
            subjectTypeSlider.setName("SUBJ. POLYS: CIRCLE");
            break;
        case 1:
            clipSubjectType = SQUARES;
            subjectTypeSlider.setName("SUBJ. POLYS: SQUARE");
            break;
        case 2:
            clipSubjectType = RANDOM_POLY;
            subjectTypeSlider.setName("SUBJ. POLYS: RANDOM");
            break;
        default:
            break;
    }
    
    genSubjects(nClipSubjects);
    bNeedsUpdate = true;
}


void ofApp::maskPolyType(int& ct)
{
    switch (ct)
    {
        case 0:
            clipMaskType = CIRCLES;
            maskTypeSlider.setName("MASK POLYS: CIRCLE");
            break;
        case 1:
            clipMaskType = SQUARES;
            maskTypeSlider.setName("MASK POLYS: SQUARE");
            break;
        case 2:
            clipMaskType = RANDOM_POLY;
            maskTypeSlider.setName("MASK POLYS: RANDOM");
            break;
        default:
            break;
    }

    genMasks(nClipMasks);
    bNeedsUpdate = true;
}

void ofApp::offsetDelta(float& f)
{
    bNeedsUpdate = true;
}


void ofApp::mouseClipper(bool& b)
{
    mclip = b;
    
    if (!mclip)
    {
        genMasks(nClipMasks);
    }
    
    bNeedsUpdate = true;
}


void ofApp::genSimplifyPath(ofxSlider<int>& ct)
{
    complexPath.clear();
    for (int i = 0; i < ct; ++i)
    {
        complexPath.lineTo(ofVec3f(ofRandom(-80,80),ofRandom(-80,80)));
    }

    complexPath.close();
        
    bSimplifyPathNeedsUpdate = true;
}

