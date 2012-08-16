/*==============================================================================
 
 Copyright (c) 2010, 2011, 2012 Christopher Baker <http://christopherbaker.net>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 ==============================================================================*/


// TODO: add const correctness to match clipper library.
// current ofPolyline has some strange const inconsistencies

#pragma once

#include "ofMain.h"
#include "clipper.hpp"

typedef vector<ofPolyline> ofxPolylines;

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

class ofxClipper : public ClipperLib::Clipper {
public:
    ofxClipper();
    virtual ~ofxClipper();
    
    // Member Function
    ////////////////////////////////////////////////
    bool addPath(ofPath& path, ofxClipperPolyType clipperType);
    bool addPolylines(ofxPolylines& polylines,ofxClipperPolyType clipperType);
    bool addPolyline(ofPolyline& polyline,ofxClipperPolyType clipperType);
    bool addRectangle(ofRectangle& rectangle,ofxClipperPolyType clipperType);

    bool clip(ofxClipperClipType clipType, 
              ofxPolylines &solution,
              ofPolyWindingMode subFillType = OF_POLY_WINDING_ODD,
              ofPolyWindingMode clipFillType = OF_POLY_WINDING_ODD);

    void clear();
    
    // Static Utility Functions
    ////////////////////////////////////////////////
    static bool Orientation(ofPolyline &poly);
    static double Area(ofPolyline &poly);
    static void OffsetPolylines(ofxPolylines &in_polys, 
                        ofxPolylines &out_polys,
                        double offset, 
                        ofxClipperJoinType jointype = OFX_CLIPPER_JOINTYPE_SQUARE, 
                        double MiterLimit = 2);
    static void SimplifyPolyline(ofPolyline &in_poly,
                                 ofxPolylines &out_polys,
                                 ofPolyWindingMode windingMode = OF_POLY_WINDING_ODD);
    static void SimplifyPolylines(ofxPolylines &in_polys,
                                  ofxPolylines &out_polys,
                                  ofPolyWindingMode windingMode = OF_POLY_WINDING_ODD);
    static void SimplifyPolylines(ofxPolylines &polys,
                                  ofPolyWindingMode windingMode = OF_POLY_WINDING_ODD);
    static void SimplifyPath(ofPath &path,
                             ofxPolylines &out_polys,
                             ofPolyWindingMode windingMode = OF_POLY_WINDING_ODD);
    static void ReversePolyline(ofPolyline& p);
    static void ReversePolylines(ofxPolylines& p);
    static void ReversePath(ofPath& p,ofxPolylines &out_polys);

    static void setGlobalScale(ClipperLib::long64 newScale);

//protected:
    // conversion functions
    static void ofPath_to_Polygons(ofPath& path, ClipperLib::Polygons& polygons);
    static ClipperLib::Polygon ofPolyline_to_Polygon(ofPolyline& polyline);
    static void ofxPolylines_to_Polygons(ofxPolylines& polylines, ClipperLib::Polygons& polygons);
                
                                      
    static ofPolyline polygon_to_ofPolyline(ClipperLib::Polygon& polygon);
    static void polygons_to_ofxPolylines(ClipperLib::Polygons& polygons, ofxPolylines& polylines);

    static ClipperLib::PolyFillType convertWindingMode(ofPolyWindingMode windingMode);

    
};
