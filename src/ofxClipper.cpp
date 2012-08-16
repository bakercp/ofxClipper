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

#include "ofxClipper.h"

static int clipperGlobalScale = 1000000000;

//--------------------------------------------------------------
ofxClipper::ofxClipper() {}

//--------------------------------------------------------------
ofxClipper::~ofxClipper() {
    // is super called here:
}

//--------------------------------------------------------------
bool ofxClipper::addPath(ofPath& path, 
                         ofxClipperPolyType clipperType) {
    ClipperLib::Polygons out;
    ofPath_to_Polygons(path,out);
    return AddPolygons(out,(ClipperLib::PolyType)clipperType);
}

//--------------------------------------------------------------
bool ofxClipper::addPolylines(ofxPolylines& polylines, 
                             ofxClipperPolyType clipperType) {
    ClipperLib::Polygons out;
    ofxPolylines_to_Polygons(polylines, out);
    return AddPolygons(out,(ClipperLib::PolyType)clipperType);
}

//--------------------------------------------------------------
bool ofxClipper::addPolyline(ofPolyline& polyline, 
                             ofxClipperPolyType clipperType) {
    ClipperLib::Polygon out = ofPolyline_to_Polygon(polyline);
    return AddPolygon(out,(ClipperLib::PolyType)clipperType);
}

//--------------------------------------------------------------
bool ofxClipper::addRectangle(ofRectangle& rectangle, 
                              ofxClipperPolyType clipperType) {
    
    ofPolyline r;
    r.addVertex(ofPoint(rectangle.x,rectangle.y));
    r.addVertex(ofPoint(rectangle.x+rectangle.width,rectangle.y));
    r.addVertex(ofPoint(rectangle.x+rectangle.width,rectangle.y+rectangle.height));
    r.addVertex(ofPoint(rectangle.x,rectangle.y+rectangle.height));
    r.close();

    ClipperLib::Polygon out = ofPolyline_to_Polygon(r);
    return AddPolygon(out,(ClipperLib::PolyType)clipperType);
}

//--------------------------------------------------------------
bool ofxClipper::clip(ofxClipperClipType clipType, 
                      ofxPolylines &solution,
                      ofPolyWindingMode subFillType,
                      ofPolyWindingMode clipFillType) {
    m_UseFullRange = true; // need this for our conversions
    ClipperLib::Polygons out;
    bool success = Execute((ClipperLib::ClipType)clipType,
                            out,
                            convertWindingType(subFillType),
                            convertWindingType(clipFillType));
    polygons_to_ofxPolylines(out,solution);
    return success;
}

//--------------------------------------------------------------
void ofxClipper::clear() {
    ClipperLib::Clipper::Clear();
}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void ofxClipper::ofPath_to_Polygons(ofPath& path,ClipperLib::Polygons& polygons) {
    return ofxPolylines_to_Polygons(path.getOutline(),polygons);
}
//--------------------------------------------------------------
ClipperLib::Polygon ofxClipper::ofPolyline_to_Polygon(ofPolyline& polyline) {
	vector<ofPoint> verts = polyline.getVertices();
    vector<ofPoint>::iterator iter;
    ClipperLib::Polygon polygon;
    for(iter = verts.begin(); iter != verts.end(); iter++) {
        ClipperLib::IntPoint ip((*iter).x * clipperGlobalScale, 
                                (*iter).y * clipperGlobalScale);
        polygon.push_back(ip);
    }
    return polygon;
    
}
//--------------------------------------------------------------
void ofxClipper::ofxPolylines_to_Polygons(ofxPolylines& polylines,ClipperLib::Polygons& polygons) {
    vector<ofPolyline>::iterator iter;
    for(iter = polylines.begin(); iter != polylines.end(); iter++) {
        polygons.push_back(ofPolyline_to_Polygon((*iter)));
    }
}

//--------------------------------------------------------------
ofPolyline ofxClipper::polygon_to_ofPolyline(ClipperLib::Polygon& polygon) {
    vector<ClipperLib::IntPoint>::iterator iter;
    ofPolyline polyline;
    for(iter = polygon.begin(); iter != polygon.end(); iter++) {
        ofPoint pnt((*iter).X / double(clipperGlobalScale),  // bring back to our coords
                    (*iter).Y / double(clipperGlobalScale)); // bring back to our coords
        polyline.addVertex(pnt);
    }
    polyline.close(); // close it
    return polyline;
}

//--------------------------------------------------------------
void ofxClipper::polygons_to_ofxPolylines(ClipperLib::Polygons& polygons,ofxPolylines& polylines) {
    vector<ClipperLib::Polygon>::iterator iter;
    for(iter = polygons.begin(); iter != polygons.end(); iter++) {
        polylines.push_back(polygon_to_ofPolyline((*iter)));
    }
}


// utility functions
//--------------------------------------------------------------
bool ofxClipper::Orientation(ofPolyline &poly) {
    return ClipperLib::Orientation(ofPolyline_to_Polygon(poly));
}

//--------------------------------------------------------------
double ofxClipper::Area(ofPolyline &poly) {
    return ClipperLib::Area(ofPolyline_to_Polygon(poly));
}

//--------------------------------------------------------------
void ofxClipper::OffsetPolylines(ofxPolylines &in_polys, 
                    ofxPolylines &out_polys,
                    double offset, 
                    ofxClipperJoinType jointype, 
                    double MiterLimit) {
    
    offset = offset * clipperGlobalScale;
    MiterLimit = MiterLimit * clipperGlobalScale;
    
    ClipperLib::Polygons in, out;
    ofxPolylines_to_Polygons(in_polys,in);
    OffsetPolygons(in,out,offset,(ClipperLib::JoinType)jointype,MiterLimit);
    polygons_to_ofxPolylines(out,out_polys);
}

//--------------------------------------------------------------
void ofxClipper::SimplifyPolyline(ofPolyline &in_poly, 
                     ofxPolylines  &out_polys) {
    ClipperLib::Polygon in;
    ClipperLib::Polygons out;
    in = ofPolyline_to_Polygon(in_poly);
    ClipperLib::SimplifyPolygon(in,out);
    polygons_to_ofxPolylines(out,out_polys);
}

//--------------------------------------------------------------
void ofxClipper::SimplifyPolylines(ofxPolylines &in_polys, 
                      ofxPolylines &out_polys) {
    ClipperLib::Polygons in;
    ClipperLib::Polygons out;
    ofxPolylines_to_Polygons(in_polys,in);
    ClipperLib::SimplifyPolygons(in,out);
    polygons_to_ofxPolylines(out,out_polys);
}

//--------------------------------------------------------------
void ofxClipper::SimplifyPolylines(ofxPolylines &polys) {
    ClipperLib::Polygons in;
    ofxPolylines_to_Polygons(polys,in);
    ClipperLib::SimplifyPolygons(in);
    polys.clear();
    polygons_to_ofxPolylines(in,polys);
}

//--------------------------------------------------------------
void ofxClipper::SimplifyPath(ofPath &path, ofxPolylines &out_polys) {
    ClipperLib::Polygons in,out;
    ofPath_to_Polygons(path, in);
    ClipperLib::SimplifyPolygons(in,out);
    out_polys.clear();
    polygons_to_ofxPolylines(out,out_polys);
}

//--------------------------------------------------------------
void ofxClipper::ReversePolyline(ofPolyline& poly) {
    ClipperLib::Polygon in;
    in = ofPolyline_to_Polygon(poly);
    ClipperLib::ReversePoints(in);
    poly = polygon_to_ofPolyline(in);
}

//--------------------------------------------------------------
void ofxClipper::ReversePolylines(ofxPolylines& polys) {
    ClipperLib::Polygons in;
    ofxPolylines_to_Polygons(polys,in);
    ClipperLib::ReversePoints(in);
    polys.clear();
    polygons_to_ofxPolylines(in,polys);
}

//--------------------------------------------------------------
void ofxClipper::ReversePath(ofPath& path, ofxPolylines &out_polys) {
    ClipperLib::Polygons in;
    ofPath_to_Polygons(path, in);
    ClipperLib::ReversePoints(in);
    out_polys.clear();
    polygons_to_ofxPolylines(in,out_polys);
}

//--------------------------------------------------------------
ClipperLib::PolyFillType ofxClipper::convertWindingType(ofPolyWindingMode windingMode) {
    
    enum PolyFillType { pftEvenOdd, pftNonZero, pftPositive, pftNegative };
    
    switch(windingMode) {
        case OF_POLY_WINDING_ODD:
            return ClipperLib::pftEvenOdd;
            break;
        case OF_POLY_WINDING_NONZERO:
            return ClipperLib::pftNonZero;
            break;
        case OF_POLY_WINDING_POSITIVE:
            return ClipperLib::pftPositive;
            break;
        case OF_POLY_WINDING_NEGATIVE:
            return ClipperLib::pftNegative;
            break;
        case OF_POLY_WINDING_ABS_GEQ_TWO:
            ofLog(OF_LOG_WARNING,"ofxClipper::OF_POLY_WINDING_ABS_GEQ_TWO not implemented by clipper, using OF_POLY_WINDING_ODD");
            return ClipperLib::pftEvenOdd;
            break;
    }
}

//--------------------------------------------------------
void ofxClipper::setGlobalScale(long long newScale) {
    clipperGlobalScale = newScale;
}
