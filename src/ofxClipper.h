//
//  ofxClipper.h
//  clipperExample
//
//  Created by Christopher Baker on 5/22/12.
//  Copyright (c) 2012 School of the Art Institute of Chicago. All rights reserved.
//

// TODO: add const correctness to match clipper library.
// current ofPolyline has some strange const inconsistencies

#pragma once

#include "ofMain.h"

#include "clipper.hpp"

#define DEFAULT_SCALE 1000000000

typedef vector<ofPolyline> ofPolylines;

enum ofxClipperClipType {
    OFX_CLIPPER_INTERSECTION = ClipperLib::ctIntersection,
    OFX_CLIPPER_UNION        = ClipperLib::ctUnion,
    OFX_CLIPPER_DIFFERENCE   = ClipperLib::ctDifference,
    OFX_CLIPPER_XOR          = ClipperLib::ctXor,
};

enum ofxClipperPolyType { 
    OFX_CLIPPER_SUBJECT = ClipperLib::ptSubject,
    OFX_CLIPPER_CLIP    = ClipperLib::ptClip,
};

enum ofxClipperJoinType { 
    OFX_CLIPPER_JOINTYPE_SQUARE = ClipperLib::jtSquare, 
    OFX_CLIPPER_JOINTYPE_ROUND  = ClipperLib::jtRound, 
    OFX_CLIPPER_JOINTYPE_MITER  = ClipperLib::jtMiter, 
};

// conversion functions
void ofPathToPolygons(ofPath& path, ClipperLib::Polygons& polygons,
                      ClipperLib::long64 xScale,
                      ClipperLib::long64 yScale);

ClipperLib::Polygon ofPolylineToPolygon(ofPolyline& polyline,
                                        ClipperLib::long64 xScale,
                                        ClipperLib::long64 yScale);

void ofPolylinesToPolygons(ofPolylines& polylines, 
                           ClipperLib::Polygons& polygons,
                           ClipperLib::long64 xScale,
                           ClipperLib::long64 yScale);


ofPolyline polygonToofPolyline(ClipperLib::Polygon& polygon,
                               ClipperLib::long64 xScale,
                               ClipperLib::long64 yScale);


void polygonsToofPolylines(ClipperLib::Polygons& polygons, 
                           ofPolylines& polylines,
                           ClipperLib::long64 xScale,
                           ClipperLib::long64 yScale);

ClipperLib::PolyFillType convertWindingType(ofPolyWindingMode windingMode);

// utility functions
bool Orientation(const ofPolyline &poly,
                 ClipperLib::long64 xScale,
                 ClipperLib::long64 yScale);
double Area(const ofPolyline &poly,
            ClipperLib::long64 xScale,
            ClipperLib::long64 yScale);
void OffsetPolygons(ofPolylines &in_polys, 
                    ofPolylines &out_polys,
                    double delta, ofxClipperJoinType 
                    jointype = OFX_CLIPPER_JOINTYPE_SQUARE, 
                    double MiterLimit = 2,
                    ClipperLib::long64 xScale = DEFAULT_SCALE,
                    ClipperLib::long64 yScale = DEFAULT_SCALE);
void SimplifyPolygon(ofPolyline &in_poly, 
                     ofPolylines  &out_polys,
                     ClipperLib::long64 xScale = DEFAULT_SCALE,
                     ClipperLib::long64 yScale = DEFAULT_SCALE);
void SimplifyPolygons(ofPolylines &in_polys, 
                      ofPolylines &out_polys,
                      ClipperLib::long64 xScale = DEFAULT_SCALE,
                      ClipperLib::long64 yScale = DEFAULT_SCALE);
void SimplifyPolygons(ofPolylines &polys,
                      ClipperLib::long64 xScale = DEFAULT_SCALE,
                      ClipperLib::long64 yScale = DEFAULT_SCALE);
void ReversePoints(ofPolyline& p,
                   ClipperLib::long64 xScale = DEFAULT_SCALE,
                   ClipperLib::long64 yScale = DEFAULT_SCALE);
void ReversePoints(ofPolylines& p,
                   ClipperLib::long64 xScale = DEFAULT_SCALE,
                   ClipperLib::long64 yScale = DEFAULT_SCALE);

class ofxClipper : public ClipperLib::Clipper {
public:
    ofxClipper();
    virtual ~ofxClipper();
    
    bool addPath(ofPath& path, 
                 ofxClipperPolyType clipperType,
                 ClipperLib::long64 xScale = DEFAULT_SCALE,
                 ClipperLib::long64 yScale = DEFAULT_SCALE);
    bool addPolylines(ofPolylines& polylines, 
                     ofxClipperPolyType clipperType,
                     ClipperLib::long64 xScale = DEFAULT_SCALE,
                     ClipperLib::long64 yScale = DEFAULT_SCALE);
    bool addPolyline(ofPolyline& polyline, 
                     ofxClipperPolyType clipperType,
                     ClipperLib::long64 xScale = DEFAULT_SCALE,
                     ClipperLib::long64 yScale = DEFAULT_SCALE);
    bool addRectangle(ofRectangle& rectangle, 
                      ofxClipperPolyType clipperType,
                      ClipperLib::long64 xScale = DEFAULT_SCALE,
                      ClipperLib::long64 yScale = DEFAULT_SCALE);

    bool clip(ofxClipperClipType clipType, 
                 ofPolylines &solution,
                 ofPolyWindingMode subFillType = OF_POLY_WINDING_ODD,
                 ofPolyWindingMode clipFillType = OF_POLY_WINDING_ODD,
                 ClipperLib::long64 xScale = DEFAULT_SCALE, 
                 ClipperLib::long64 yScale = DEFAULT_SCALE);

    void clear();
    
private:
    
};
