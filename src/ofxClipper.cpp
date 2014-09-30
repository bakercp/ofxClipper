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


#include "ofxClipper.h"


static int clipperGlobalScale = 1000000000;


ofxClipper::ofxClipper() {}


ofxClipper::~ofxClipper() {
    // is super called here:
}


bool ofxClipper::addPath(ofPath& path, 
                         ofxClipperPolyType clipperType) {
    // clipper wants to know if the path is closed, so i iterate through subpaths (ofPolylines) and tell clipper it is not closed, if a single one of them is open
    bool isClosed = true;
    for (ofPolyline poly : path.getOutline()) {
        isClosed = poly.isClosed() == true ? isClosed : false;
        if(!isClosed){
            poly.close();
            isClosed = true;
            ofLog(OF_LOG_NOTICE, "path not closed, so we closed it for you in ofxClipper.cpp::addPath(...)");
        }
    }
    
    ClipperLib::Paths out;
    ofPath_to_Polygons(path,out);
    return AddPaths(out,(ClipperLib::PolyType)clipperType, isClosed);
}


bool ofxClipper::addPolylines(ofxPolylines& polylines, 
                              ofxClipperPolyType clipperType) {
    // clipper wants to know if the path is closed, so i iterate through the ofPolyline's and tell clipper it is not closed, if a single one of them is open
    bool isClosed = true;
    for (ofPolyline poly : polylines) {
        isClosed = poly.isClosed() == true ? isClosed : false;
        if(!isClosed){
            poly.close();
            isClosed = true;
            ofLog(OF_LOG_NOTICE, "polyline not closed, so we closed it for you in ofxClipper.cpp::addPolylines(...)");
        }
    }
    
    ClipperLib::Paths out;
    ofxPolylines_to_Polygons(polylines, out);
    return AddPaths(out,(ClipperLib::PolyType)clipperType, isClosed);
}


bool ofxClipper::addPolyline(ofPolyline& polyline, 
                             ofxClipperPolyType clipperType) {
    // clipper wants to know if the path is closed, so i iterate through the ofPolyline's and tell clipper it is not closed, if a single one of them is open
    if(!polyline.isClosed()){
        polyline.close();
        ofLog(OF_LOG_NOTICE, "polyline not closed, so we closed it for you in ofxClipper.cpp::addPolyline(...)");
    }
    
    ClipperLib::Path out = ofPolyline_to_Polygon(polyline);
    return AddPath(out,(ClipperLib::PolyType)clipperType, polyline.isClosed());
}


bool ofxClipper::addRectangle(ofRectangle& rectangle, 
                              ofxClipperPolyType clipperType) {
    
    ofPolyline r;
    r.addVertex(ofPoint(rectangle.x,rectangle.y));
    r.addVertex(ofPoint(rectangle.x+rectangle.width,rectangle.y));
    r.addVertex(ofPoint(rectangle.x+rectangle.width,rectangle.y+rectangle.height));
    r.addVertex(ofPoint(rectangle.x,rectangle.y+rectangle.height));
    r.close(); // polyline is closed, so we can send a "true" to clipper

    ClipperLib::Path out = ofPolyline_to_Polygon(r);
    return AddPath(out,(ClipperLib::PolyType)clipperType, true); // we just closed the polyline above, so we send "true"
}


bool ofxClipper::clip(ofxClipperClipType clipType, 
                      ofxPolylines &solution,
                      ofPolyWindingMode subFillType,
                      ofPolyWindingMode clipFillType) {
    m_UseFullRange = true; // need this for our conversions
    ClipperLib::Paths out;
    bool success = Execute((ClipperLib::ClipType)clipType,
                            out,
                            convertWindingMode(subFillType),
                            convertWindingMode(clipFillType));
    polygons_to_ofxPolylines(out,solution);
    return success;
}


void ofxClipper::clear() {
    ClipperLib::Clipper::Clear();
}




void ofxClipper::ofPath_to_Polygons(ofPath& path,ClipperLib::Paths& polygons) {
    return ofxPolylines_to_Polygons(path.getOutline(),polygons);
}

ClipperLib::Path ofxClipper::ofPolyline_to_Polygon(ofPolyline& polyline) {
	vector<ofPoint> verts = polyline.getVertices();
    vector<ofPoint>::iterator iter;
    ClipperLib::Path polygon;
    for(iter = verts.begin(); iter != verts.end(); iter++) {
        ClipperLib::IntPoint ip((*iter).x * clipperGlobalScale, 
                                (*iter).y * clipperGlobalScale);
        polygon.push_back(ip);
    }
    return polygon;
}


void ofxClipper::ofxPolylines_to_Polygons(ofxPolylines& polylines,ClipperLib::Paths& polygons) {
    vector<ofPolyline>::iterator iter;
    for(iter = polylines.begin(); iter != polylines.end(); iter++) {
        polygons.push_back(ofPolyline_to_Polygon((*iter)));
    }
}


ofPolyline ofxClipper::polygon_to_ofPolyline(ClipperLib::Path& polygon) {
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


void ofxClipper::polygons_to_ofxPolylines(ClipperLib::Paths& polygons,ofxPolylines& polylines) {
    vector<ClipperLib::Path>::iterator iter;
    for(iter = polygons.begin(); iter != polygons.end(); iter++) {
        polylines.push_back(polygon_to_ofPolyline((*iter)));
    }
}


void ofxClipper::polygons_to_ofPath(ClipperLib::Paths& polygons,ofPath& path) {
    vector<ClipperLib::Path>::iterator iter;
    for(iter = polygons.begin(); iter != polygons.end(); iter++) {
        path.getOutline().push_back(polygon_to_ofPolyline((*iter)));
    }
}


// utility functions

bool ofxClipper::Orientation(ofPolyline &poly) {
    return ClipperLib::Orientation(ofPolyline_to_Polygon(poly));
}


double ofxClipper::Area(ofPolyline &poly) {
    return ClipperLib::Area(ofPolyline_to_Polygon(poly));
}


void ofxClipper::OffsetPolylines(ofxPolylines &in_polys, 
                    ofxPolylines &out_polys,
                    double offset, 
                    ofxClipperJoinType jointype,
                    ofxClipperEndType endtype,
                    double MiterLimit) {
    
    offset = offset * clipperGlobalScale;
    MiterLimit = MiterLimit * clipperGlobalScale;
    
    ClipperLib::Paths in, out;
    ofxPolylines_to_Polygons(in_polys,in);
    ClipperLib::OffsetPaths(in,out,offset,(ClipperLib::JoinType)jointype,(ClipperLib::EndType_)endtype,MiterLimit);
    polygons_to_ofxPolylines(out,out_polys);
}


void ofxClipper::OffsetPath(ofPath &in_path,
                                 ofPath &out_path,
                                 double offset,
                                 ofxClipperJoinType jointype,
                                 ofxClipperEndType endtype,
                                 double MiterLimit) {
    
    offset = offset * clipperGlobalScale;
    MiterLimit = MiterLimit * clipperGlobalScale;
    
    ClipperLib::Paths in, out;
    ofPath_to_Polygons(in_path,in);
    ClipperLib::OffsetPaths(in,out,offset,(ClipperLib::JoinType)jointype,(ClipperLib::EndType_)endtype,MiterLimit);
    polygons_to_ofPath(out,out_path);
}



void ofxClipper::SimplifyPolyline(ofPolyline &in_poly, 
                     ofxPolylines  &out_polys,
                     ofPolyWindingMode windingMode) {
    ClipperLib::Path in;
    ClipperLib::Paths out;
    in = ofPolyline_to_Polygon(in_poly);
    ClipperLib::SimplifyPolygon(in,out, convertWindingMode(windingMode));
    polygons_to_ofxPolylines(out,out_polys);
}


void ofxClipper::SimplifyPolylines(ofxPolylines &in_polys, 
                      ofxPolylines &out_polys,
                      ofPolyWindingMode windingMode) {
    ClipperLib::Paths in;
    ClipperLib::Paths out;
    ofxPolylines_to_Polygons(in_polys,in);
    ClipperLib::SimplifyPolygons(in,out,convertWindingMode(windingMode));
    polygons_to_ofxPolylines(out,out_polys);
}


void ofxClipper::SimplifyPolylines(ofxPolylines &polys,
                                   ofPolyWindingMode windingMode) {
    ClipperLib::Paths in;
    ofxPolylines_to_Polygons(polys,in);
    ClipperLib::SimplifyPolygons(in,convertWindingMode(windingMode));
    polys.clear();
    polygons_to_ofxPolylines(in,polys);
}


void ofxClipper::SimplifyPath(ofPath &path,
                              ofxPolylines &out_polys,
                              ofPolyWindingMode windingMode) {
    ClipperLib::Paths in,out;
    ofPath_to_Polygons(path, in);
    ClipperLib::SimplifyPolygons(in,out,convertWindingMode(windingMode));
    out_polys.clear();
    polygons_to_ofxPolylines(out,out_polys);
}


void ofxClipper::ReversePolyline(ofPolyline& poly) {
    ClipperLib::Path in;
    in = ofPolyline_to_Polygon(poly);
    ClipperLib::ReversePath(in);
    poly = polygon_to_ofPolyline(in);
}


void ofxClipper::ReversePolylines(ofxPolylines& polys) {
    ClipperLib::Paths in;
    ofxPolylines_to_Polygons(polys,in);
    ClipperLib::ReversePaths(in);
    polys.clear();
    polygons_to_ofxPolylines(in,polys);
}


void ofxClipper::ReversePath(ofPath& path, ofxPolylines &out_polys) {
    ClipperLib::Paths in;
    ofPath_to_Polygons(path, in);
    ClipperLib::ReversePaths(in);
    out_polys.clear();
    polygons_to_ofxPolylines(in,out_polys);
}


ClipperLib::PolyFillType ofxClipper::convertWindingMode(ofPolyWindingMode windingMode) {
    
    //enum PolyFillType { pftEvenOdd, pftNonZero, pftPositive, pftNegative };
    
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
            ofLogWarning("ofxClipper") << " OF_POLY_WINDING_ABS_GEQ_TWO not implemented by clipper, using OF_POLY_WINDING_ODD";
            return ClipperLib::pftEvenOdd;
            break;
    }
}


void ofxClipper::setGlobalScale(long long newScale) {
    clipperGlobalScale = newScale;
}
