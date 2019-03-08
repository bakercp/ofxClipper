//
// Copyright (c) 2010 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    ofSetBackgroundColor(0);
    
    bNeedsUpdate = true; // a housekeeping variable to respond to gui changes

    currentClipperType = ClipperLib::ctIntersection; // the clip type
    currentClipperJoinType = ClipperLib::jtSquare; // for offsets, the joint type
    currentClipperEndType = ClipperLib::etClosedPolygon;  // for offsets, the joint type
    
    clipSubjectType = CIRCLES;  // what kind of test subjects to create
    clipMaskType = CIRCLES;  // what kind of test clip masks to create

    // gui setup
    clipTypePanel.setup("clipper","settings.xml", 10, 10);
    clipTypePanel.add(clipTypeSlider.setup("TYPE: INTERSECTION",0,0,3));
    clipTypePanel.add(subjectTypeSlider.setup("SUBJ. POLYS: CIRCLES",0,0,2));
    clipTypePanel.add(nClipSubjects.setup("# clip subjects",2,1,100));
    clipTypePanel.add(maskTypeSlider.setup("CLIP POLYS: CIRCLES",0,0,2));
    clipTypePanel.add(nClipMasks.setup("# clip masks   ",1,1,100));
    clipTypePanel.add(useMouseClipper.setup("Use Mouse Clip Mask", true));
               
    useMouseClipper.addListener(this, &ofApp::mouseClipper);
    
    clipTypeSlider.addListener(this, &ofApp::clipType);
    
    maskTypeSlider.addListener(this, &ofApp::maskPolyType);
    subjectTypeSlider.addListener(this, &ofApp::subjectPolyType);
    
    offsetPanel.setup("offsets","settings.xml", 10, 200);
    offsetPanel.add(enableOffsetsToggle.setup("show offset", true));
    offsetPanel.add(offsetDeltaSlider.setup("offset delta",20,-200,200));
    offsetPanel.add(joinTypeSlider.setup("JOIN TYPE: SQUARE",0,0,2));
    offsetPanel.add(endTypeSlider.setup("END TYPE: SQUARE",0,0,3));
    offsetPanel.add(miterLimitSlider.setup("miter limit",2,0,30));
    
    joinTypeSlider.addListener(this, &ofApp::joinType);
    endTypeSlider.addListener(this, &ofApp::endType);
    miterLimitSlider.addListener(this, &ofApp::miterLimit);
    offsetDeltaSlider.addListener(this, &ofApp::offsetDelta);

    simplifyPanel.setup("Simplify","settings.xml", ofGetWidth()-100,10);
    simplifyPanel.add(simplifyPath.setup("num vertices",10,3,40));

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
        // Clear out our temporary clipped data.
        clips.clear();
        
        // Clear the clipper's internal polys.
        clipper.Clear();

        // Add the clipper subjects (i.e. the things that will be clipped).
        clipper.addPolylines(clipSubjects, ClipperLib::ptSubject);
        
        // add the clipper masks (i.e. the things that will do the clipping).
        clipper.addPolylines(clipMasks, ClipperLib::ptClip);

        // Get the bounding box.
        boundingBox = clipper.getBounds();

        // Gxecute the clipping operation based on the current clipping type.
        clips = clipper.getClipped(currentClipperType);

        std::cout << clips.size() << std::endl;

        if (bSimplifyPathNeedsUpdate)
        {
            simplifiedPath = ofx::Clipper::simplifyPath(complexPath);
        }
        
        // If we have offsets enabled, generate the offsets.
        if (enableOffsetsToggle)
        {
            offsets = ofx::Clipper::getOffsets(clips,
                                               offsetDeltaSlider,
                                               currentClipperJoinType,
                                               currentClipperEndType,
                                               miterLimitSlider);
        }
        else
        {
            offsets.clear();
        }
        
        bNeedsUpdate = false;
    }
    

}


void ofApp::draw()
{
    // Make a little legend for the color.
    int y = ofGetHeight() - 60;
    int x = 10;
    int h = 12;
    int w = 30;
    
    ofColor cSubject(255, 0, 0, 200);
    ofColor cMask(255, 255, 0, 200);
    ofColor cResult(0, 255, 0, 200);
    ofColor cOffset(0, 0, 255, 200);
    
    ofFill();
    ofSetColor(cSubject);
    ofDrawRectangle(x, y - h + 2, w, h);
    ofSetColor(255);
    ofDrawBitmapString("Polygon Clip Subjects", glm::vec3(x + w + 2, y, 0));
    
    y+= 16;
    ofFill();
    ofSetColor(cMask);
    ofDrawRectangle(x, y - h + 2, w, h);
    ofSetColor(255);
    ofDrawBitmapString("Polygon Clip Masks", glm::vec3(x + w + 2, y, 0));

    y+= 16;
    ofFill();
    ofSetColor(cResult);
    ofDrawRectangle(x, y - h + 2, w, h);
    ofSetColor(255);
    ofDrawBitmapString("Clip Results", glm::vec3(x + w + 2, y, 0));

    y+= 16;
    ofFill();
    ofSetColor(cOffset);
    ofDrawRectangle(x, y - h + 2, w, h);
    ofSetColor(255);
    ofDrawBitmapString("Offset Results", glm::vec3(x + w + 2, y, 0));

    
    // draw all of the pieces
    ////////////////////////////////////////////////
    ofPushMatrix();
    ofTranslate(-ofGetWidth() / 4, 0);

    for (auto& clipSubject: clipSubjects)
    {
        ofSetColor(cSubject);
        ofNoFill();
        clipSubject.draw();
    }
    
    for (auto& clipMask: clipMasks)
    {
        ofSetColor(cMask);
        ofNoFill();
        clipMask.draw();
    }

    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 4, 0);

    ofSetColor(0, 255, 255,  180);
    ofNoFill();
    ofDrawRectangle(boundingBox);


    for (auto& clip: clips)
    {
        ofSetColor(cResult);
        ofNoFill();
        clip.draw();
    }

    if (enableOffsetsToggle)
    {
        for (auto& offset: offsets)
        {
            ofSetColor(cOffset);
            ofNoFill();
            offset.draw();
        }
    }
    
    ofPopMatrix();
    
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() * 2 / 4, 100);
    complexPath.draw();

    for (auto& polyline: simplifiedPath)
    {
        ofSetColor(255, 255, 0);
        polyline.draw();
    }
    
    ofPopMatrix();
    
    // draw the gui
    ////////////////////////////////////////////////
    clipTypePanel.draw();
    offsetPanel.draw();

}


void ofApp::makeMouseClip()
{
    if (mclip)
    {
        clipMasks.clear();
        ofPolyline p;
        glm::vec3 ctr(ofGetMouseX() + ofGetWidth() / 4, ofGetMouseY(), 0);
        p.arc(ctr, 100, 100, 0, 360);
        p.close();
        clipMasks.push_back(p);
        bNeedsUpdate = true;
    }
}


void ofApp::mouseMoved(int x, int y)
{
    makeMouseClip();
}


void ofApp::genSubjects(ofxSlider<int>& ct)
{
    clipSubjects.clear();
    
    glm::vec3 screenCtr = glm::vec3(ofGetWidth() / 2, ofGetHeight() / 2, 0);
    
    for (int i = 0; i < ct; ++i)
    {
        if (clipSubjectType == CIRCLES)
        {
            ofPolyline p;
            float rad = ofRandom(50, 120);
            glm::vec3 ctr = screenCtr + glm::vec3(ofRandom(-100,100), ofRandom(-100,100), 0);
            p.arc(ctr, rad, rad, 0, 360);
            p.close();
            clipSubjects.push_back(p);
        }
        else if (clipSubjectType == SQUARES)
        {
            ofRectangle p;
            float rad = ofRandom(50, 120);
            glm::vec3 ctr = screenCtr + glm::vec3(ofRandom(-100, 100), ofRandom(-100, 100), 0);
            p.setFromCenter(ctr.x, ctr.y, rad, rad);
            clipSubjects.push_back(ofPolyline::fromRectangle(p));
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
            glm::vec3 ctr = screenCtr + glm::vec3(ofRandom(-150, 150), ofRandom(-150, 150), 0);
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
    glm::vec3 screenCtr(ofGetWidth() / 2, ofGetHeight() / 2, 0);

    for (int i = 0; i < ct; ++i)
    {
        if (clipMaskType == CIRCLES)
        {
            ofPolyline p;
            float rad = ofRandom(50,120);
            glm::vec3 ctr = screenCtr + glm::vec3(ofRandom(-100, 100), ofRandom(-100, 100), 0);
            p.arc(ctr,rad,rad,0,360);
            p.close();
            clipMasks.push_back(p);
        }
        else if(clipMaskType == SQUARES)
        {
            ofRectangle p;
            float rad = ofRandom(50,120);
            glm::vec3 ctr = screenCtr + glm::vec3(ofRandom(-100, 100), ofRandom(-100, 100), 0);
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
            glm::vec3 ctr = screenCtr + glm::vec3(ofRandom(-150, 150), ofRandom(-150, 150), 0);
            r.addVertex(ctr);
        }

        r.close();
        clipMasks.push_back(r);
    }
    

    bNeedsUpdate = true;
}


void ofApp::clipType(int & ct)
{
    clipTypeSlider.setName("TYPE: " + ofx::Clipper::toString(ClipperLib::ClipType(ct)));
    bNeedsUpdate = true;
}


void ofApp::joinType(int& jt)
{
    joinTypeSlider.setName("TYPE: " + ofx::Clipper::toString(ClipperLib::JoinType(jt)));
    bNeedsUpdate = true;
}


void ofApp::endType(int& et)
{
    endTypeSlider.setName("TYPE: " + ofx::Clipper::toString(ClipperLib::EndType(et)));
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
    clipMaskType = TestPolyType(ct);

    switch (clipMaskType)
    {
        case CIRCLES:
            maskTypeSlider.setName("MASK POLYS: CIRCLE");
            break;
        case SQUARES:
            maskTypeSlider.setName("MASK POLYS: SQUARE");
            break;
        case RANDOM_POLY:
            maskTypeSlider.setName("MASK POLYS: RANDOM");
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
        complexPath.lineTo(glm::vec3(ofRandom(-80, 80), ofRandom(-80, 80), 0));
    }

    complexPath.close();
        
    bSimplifyPathNeedsUpdate = true;
}
