#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofSetBackgroundColor(0);
    
    bNeedsUpdate = true; // a housekeeping variable to respond to gui changes
    
    currentClipperType     = OFX_CLIPPER_INTERSECTION;      // the clip type
    currentClipperJoinType = OFX_CLIPPER_JOINTYPE_SQUARE;   // for offsets, the joint type
    
    clipSubjectType = CIRCLES;  // what kind of test subjects to create
    clipMaskType    = CIRCLES;  // what kind of test clip masks to create

    // gui setup
    clipTypePanel.setup("clipper","settings.xml", 10, 10);
    clipTypePanel.add(clipTypeSlider.setup("TYPE: INTERSECTION",0,0,3));
    clipTypePanel.add(subjectTypeSlider.setup("SUBJ. POLYS: CIRCLES",0,0,2));
    clipTypePanel.add(     nClipSubjects.setup("# clip subjects",2,1,100));
    clipTypePanel.add(   maskTypeSlider.setup("CLIP POLYS: CIRCLES",0,0,2));
    clipTypePanel.add(nClipMasks.setup("# clip masks   ",1,1,100));
    clipTypePanel.add(useMouseClipper.setup("Use Mouse Clip Mask", true));
               
    useMouseClipper.addListener(this,&testApp::mouseClipper);
    
    clipTypeSlider.addListener(this,&testApp::clipType);
    
    maskTypeSlider.addListener(this,&testApp::maskPolyType);
    subjectTypeSlider.addListener(this,&testApp::subjectPolyType);
    
    nClipMasks.addListener(this,&testApp::genMasks);
    nClipSubjects.addListener(this,&testApp::genSubjects);
    
    offsetPanel.setup("offsets","settings.xml", 10, 200);
    offsetPanel.add(enableOffsetsToggle.setup("show offset", true));
    offsetPanel.add(offsetDeltaSlider.setup("offset delta",20,-200,200));
    offsetPanel.add(joinTypeSlider.setup("TYPE: SQUARE",0,0,2));
    offsetPanel.add(miterLimitSlider.setup("miter limit",2,0,30));
    
    joinTypeSlider.addListener(this,&testApp::joinType);
    miterLimitSlider.addListener(this,&testApp::miterLimit);
    offsetDeltaSlider.addListener(this,&testApp::offsetDelta);

    // generate some initial data
    genSubjects(nClipSubjects);
    genMasks(nClipMasks);

    mclip = useMouseClipper;
    
}

//--------------------------------------------------------------
void testApp::update(){

    if(bNeedsUpdate) {
        
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
        
        
        // if we have offsets enabled, generate the offsets
        if(enableOffsetsToggle) {
            offsets.clear();
            OffsetPolygons(clips, 
                           offsets, 
                           offsetDeltaSlider,
                           currentClipperJoinType,
                           miterLimitSlider);
        }
        
        
        bNeedsUpdate = false;
    }
    

}

//--------------------------------------------------------------
void testApp::draw(){

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
    ofDrawBitmapString("Polygon Clip Subjects", ofPoint(x + w + 2,y));
    
    y+= 16;
    ofFill();
    ofSetColor(cMask);
    ofRect(x,y-h+2,w,h);
    ofSetColor(255);
    ofDrawBitmapString("Polygon Clip Masks", ofPoint(x + w + 2,y));

    y+= 16;
    ofFill();
    ofSetColor(cResult);
    ofRect(x,y-h+2,w,h);
    ofSetColor(255);
    ofDrawBitmapString("Clip Results", ofPoint(x + w + 2,y));

    y+= 16;
    ofFill();
    ofSetColor(cOffset);
    ofRect(x,y-h+2,w,h);
    ofSetColor(255);
    ofDrawBitmapString("Offset Results", ofPoint(x + w + 2,y));

    
    // draw all of the pieces
    ////////////////////////////////////////////////
    ofPushMatrix();
    ofTranslate(-ofGetWidth() / 4, 0);

    for(int i = 0; i < clipSubjects.size(); i++) {
        ofSetColor(cSubject);
        ofNoFill();
        clipSubjects[i].draw();
    }
    
    for(int i = 0; i < clipMasks.size(); i++) {
        ofSetColor(cMask);
        ofNoFill();
        clipMasks[i].draw();
    }

    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 4, 0);
    
    for(int i = 0; i < clips.size(); i++) {
        ofSetColor(cResult);
        ofNoFill();
        clips[i].draw();
    }

    if(enableOffsetsToggle) {
        for(int i = 0; i < offsets.size(); i++) {
            ofSetColor(cOffset);
            ofNoFill();
            offsets[i].draw();
        }
    }
    
    ofPopMatrix();
    
    
    // draw the gui
    ////////////////////////////////////////////////
    clipTypePanel.draw();
    offsetPanel.draw();

    
    ofDisableAlphaBlending();

    
    
}

//--------------------------------------------------------------
void testApp::makeMouseClip() {
    if(mclip) {
        clipMasks.clear();
        ofPolyline p;
        ofPoint ctr = ofPoint(ofGetMouseX() + ofGetWidth() / 4, ofGetMouseY());
        p.arc(ctr,100,100,0,360);
        p.close();
        clipMasks.push_back(p);
        bNeedsUpdate = true;
    }
}

//--------------------------------------------------------------
void testApp::mousePressed( int x, int y, int button ) {
    makeMouseClip();
}
//--------------------------------------------------------------
void testApp::mouseReleased( int x, int y, int button ) {
    makeMouseClip();
}

//--------------------------------------------------------------
void testApp::mouseMoved( int x, int y ) {
    makeMouseClip();
}

//--------------------------------------------------------------
void testApp::genSubjects(int & ct) {
    clipSubjects.clear();
    
    ofPoint screenCtr = ofPoint(ofGetWidth()/2, ofGetHeight()/2);
    
    for(int i = 0; i < ct; i++) {
        if(clipSubjectType == CIRCLES) {
            ofPolyline p;
            float rad = ofRandom(50,120);
            ofPoint ctr = screenCtr + ofPoint(ofRandom(-100,100),ofRandom(-100,100)); 
            p.arc(ctr,rad,rad,0,360);
            p.close();
            clipSubjects.push_back(p);
        } else if(clipSubjectType == SQUARES) {
            ofRectangle p;
            float rad = ofRandom(50,120);
            ofPoint ctr = screenCtr + ofPoint(ofRandom(-100,100),ofRandom(-100,100)); 
            p.setFromCenter(ctr.x,ctr.y,rad,rad);
            ofPolyline r;
            r.addVertex(ofPoint(p.x,p.y));
            r.addVertex(ofPoint(p.x+p.width,p.y));
            r.addVertex(ofPoint(p.x+p.width,p.y+p.height));
            r.addVertex(ofPoint(p.x,p.y+p.height));
            r.close();
            clipSubjects.push_back(r);
        } else if(clipSubjectType == RANDOM_POLY) {
        }
    }
    
    if(clipSubjectType == RANDOM_POLY) {
        ofPolyline r;
        
        for(int i = 0; i < ct; i++) {
            ofPoint ctr = screenCtr + ofPoint(ofRandom(-150,150),ofRandom(-150,150)); 
            r.addVertex(ctr);
        }
        r.close();
        clipSubjects.push_back(r);
    }
 
    
    bNeedsUpdate = true;
}

//--------------------------------------------------------------
void testApp::genMasks(int & ct) {
    
    clipMasks.clear();
    ofPoint screenCtr = ofPoint(ofGetWidth()/2, ofGetHeight()/2);

    for(int i = 0; i < ct; i++) {
        if(clipMaskType == CIRCLES) {
            ofPolyline p;
            float rad = ofRandom(50,120);
            ofPoint ctr = screenCtr + ofPoint(ofRandom(-100,100),ofRandom(-100,100)); 
            p.arc(ctr,rad,rad,0,360);
            p.close();
            clipMasks.push_back(p);
        } else if(clipMaskType == SQUARES) {
            ofRectangle p;
            float rad = ofRandom(50,120);
            ofPoint ctr = screenCtr + ofPoint(ofRandom(-100,100),ofRandom(-100,100)); 
            p.setFromCenter(ctr.x,ctr.y,rad,rad);
            ofPolyline r;
            r.addVertex(ofPoint(p.x,p.y));
            r.addVertex(ofPoint(p.x+p.width,p.y));
            r.addVertex(ofPoint(p.x+p.width,p.y+p.height));
            r.addVertex(ofPoint(p.x,p.y+p.height));
            r.close();
            clipMasks.push_back(r);
        } else if(clipMaskType == RANDOM_POLY) {
        }
    }

    if(clipMaskType == RANDOM_POLY) {
        ofPolyline r;
        for(int i = 0; i < ct; i++) {
            ofPoint ctr = screenCtr + ofPoint(ofRandom(-150,150),ofRandom(-150,150)); 
            r.addVertex(ctr);
        }
        r.close();
        clipMasks.push_back(r);
    }
    

    bNeedsUpdate = true;
}

//--------------------------------------------------------------
void testApp::clipType(int & ct) {
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

//--------------------------------------------------------------
void testApp::joinType(int & ct) {
    switch (ct) {
        case 0:
            currentClipperJoinType = OFX_CLIPPER_JOINTYPE_SQUARE;
            joinTypeSlider.setName("TYPE: SQUARE");
            break;
        case 1:
            currentClipperJoinType = OFX_CLIPPER_JOINTYPE_ROUND;
            joinTypeSlider.setName("TYPE: ROUND");
            break;
        case 2:
            currentClipperJoinType = OFX_CLIPPER_JOINTYPE_MITER;
            joinTypeSlider.setName("TYPE: MITER");
            break;
        default:
            break;
    }
    bNeedsUpdate = true;
}

//--------------------------------------------------------------
void testApp::miterLimit(float & f) {
    bNeedsUpdate = true;
}


//--------------------------------------------------------------
void testApp::subjectPolyType(int & ct) {
    switch (ct) {
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

//--------------------------------------------------------------
void testApp::maskPolyType(int & ct) {
    switch (ct) {
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

//--------------------------------------------------------------
void testApp::offsetDelta(float & f) {
    bNeedsUpdate = true;
}

//--------------------------------------------------------------
void testApp::mouseClipper(bool & b) {

    mclip = b;
    
    if(!mclip) {
        genMasks(nClipMasks);
    }
    
    bNeedsUpdate = true;
}


