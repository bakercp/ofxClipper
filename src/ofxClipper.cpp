//
//  ofxClipper.cpp
//  clipperExample
//
//  Created by Christopher Baker on 5/22/12.
//  Copyright (c) 2012 School of the Art Institute of Chicago. All rights reserved.
//

#include "ofxClipper.h"

//--------------------------------------------------------------
ofxClipper::ofxClipper() {}

//--------------------------------------------------------------
ofxClipper::~ofxClipper() {
    // is super called here:
}

//--------------------------------------------------------------
bool ofxClipper::addPath(ofPath& path, 
                         ofxClipperPolyType clipperType,
                         ClipperLib::long64 xScale,
                         ClipperLib::long64 yScale) {
    
    ClipperLib::Polygons out;
    ofPathToPolygons(path,out,xScale,yScale);
    AddPolygons(out,(ClipperLib::PolyType)clipperType);
}

//--------------------------------------------------------------
bool ofxClipper::addPolylines(ofPolylines& polylines, 
                             ofxClipperPolyType clipperType,
                             ClipperLib::long64 xScale,
                             ClipperLib::long64 yScale) {
    
    
    ClipperLib::Polygons out;
    ofPolylinesToPolygons(polylines, out, xScale, yScale);
    AddPolygons(out,(ClipperLib::PolyType)clipperType);
}

//--------------------------------------------------------------
bool ofxClipper::addPolyline(ofPolyline& polyline, 
                             ofxClipperPolyType clipperType,
                             ClipperLib::long64 xScale,
                             ClipperLib::long64 yScale) {
    ClipperLib::Polygon out = ofPolylineToPolygon(polyline,xScale,yScale);
    AddPolygon(out,(ClipperLib::PolyType)clipperType);
}
//--------------------------------------------------------------
bool ofxClipper::addRectangle(ofRectangle& rectangle, 
                              ofxClipperPolyType clipperType,
                              ClipperLib::long64 xScale,
                              ClipperLib::long64 yScale) {
    
    ofPolyline r;
    r.addVertex(ofPoint(rectangle.x,rectangle.y));
    r.addVertex(ofPoint(rectangle.x+rectangle.width,rectangle.y));
    r.addVertex(ofPoint(rectangle.x+rectangle.width,rectangle.y+rectangle.height));
    r.addVertex(ofPoint(rectangle.x,rectangle.y+rectangle.height));
    r.close();

    ClipperLib::Polygon out = ofPolylineToPolygon(r,xScale,yScale);
    AddPolygon(out,(ClipperLib::PolyType)clipperType);
}

//--------------------------------------------------------------
bool ofxClipper::clip(ofxClipperClipType clipType, 
                      ofPolylines &solution,
                      ofPolyWindingMode subFillType,
                      ofPolyWindingMode clipFillType,
                      ClipperLib::long64 xScale,
                      ClipperLib::long64 yScale) {
    
    m_UseFullRange = true; // need this for our conversions
    
    ClipperLib::Polygons out;
    
    bool success = Execute((ClipperLib::ClipType)clipType,
                            out,
                            convertWindingType(subFillType),
                            convertWindingType(clipFillType));
        
    polygonsToofPolylines(out,solution,xScale,yScale);
    
    return success;
}

//--------------------------------------------------------------
void ofxClipper::clear() {
    ClipperLib::Clipper::Clear();
}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void ofPathToPolygons(ofPath& path, 
                                  ClipperLib::Polygons& polygons,
                                  ClipperLib::long64 xScale,
                                  ClipperLib::long64 yScale) {
    
    return ofPolylinesToPolygons(path.getOutline(),polygons,xScale,yScale);

}
//--------------------------------------------------------------
ClipperLib::Polygon ofPolylineToPolygon(ofPolyline& polyline,
                                        ClipperLib::long64 xScale,
                                        ClipperLib::long64 yScale) {

	vector<ofPoint> verts = polyline.getVertices();
    vector<ofPoint>::iterator iter;
    ClipperLib::Polygon polygon;
    
    for(iter = verts.begin(); iter != verts.end(); iter++) {
        ClipperLib::IntPoint ip((*iter).x * xScale, 
                                (*iter).y * yScale);
        polygon.push_back(ip);
    }
    
    return polygon;
    
}
//--------------------------------------------------------------
void ofPolylinesToPolygons(ofPolylines& polylines, 
                           ClipperLib::Polygons& polygons,
                           ClipperLib::long64 xScale,
                           ClipperLib::long64 yScale) {
    
    vector<ofPolyline>::iterator iter;
    for(iter = polylines.begin(); iter != polylines.end(); iter++) {
        polygons.push_back(ofPolylineToPolygon((*iter),xScale,yScale));
    }
    
}

//--------------------------------------------------------------
ofPolyline polygonToofPolyline(ClipperLib::Polygon& polygon,
                               ClipperLib::long64 xScale,
                               ClipperLib::long64 yScale) {

    vector<ClipperLib::IntPoint>::iterator iter;
    ofPolyline polyline;
    
    for(iter = polygon.begin(); iter != polygon.end(); iter++) {
        ofPoint pnt((*iter).X / double(xScale),  // bring back to our coords
                    (*iter).Y / double(yScale)); // bring back to our coords
        polyline.addVertex(pnt);
    }
    
    polyline.close(); // close it

    return polyline;
}

//--------------------------------------------------------------
void polygonsToofPolylines(ClipperLib::Polygons& polygons, 
                           ofPolylines& polylines,
                           ClipperLib::long64 xScale,
                           ClipperLib::long64 yScale) {
    
    vector<ClipperLib::Polygon>::iterator iter;
    for(iter = polygons.begin(); iter != polygons.end(); iter++) {
        polylines.push_back(polygonToofPolyline((*iter),xScale,yScale));
    }
    
}


// utility functions
//--------------------------------------------------------------
bool Orientation(ofPolyline &poly,
                 ClipperLib::long64 xScale,
                 ClipperLib::long64 yScale) {
    
    return ClipperLib::Orientation(ofPolylineToPolygon(poly,xScale,yScale));
}

//--------------------------------------------------------------
double Area(ofPolyline &poly,
            ClipperLib::long64 xScale,
            ClipperLib::long64 yScale) {

    return ClipperLib::Area(ofPolylineToPolygon(poly,xScale,yScale));

}

//--------------------------------------------------------------
void OffsetPolygons(ofPolylines &in_polys, 
                    ofPolylines &out_polys,
                    double delta, 
                    ofxClipperJoinType jointype, 
                    double MiterLimit,
                    ClipperLib::long64 xScale,
                    ClipperLib::long64 yScale) {
    
    delta = delta * xScale;
    MiterLimit = MiterLimit * xScale;
    
    ClipperLib::Polygons in, out;
    ofPolylinesToPolygons(in_polys,in,xScale,yScale);
    OffsetPolygons(in,out,delta,(ClipperLib::JoinType)jointype,MiterLimit);
    polygonsToofPolylines(out,out_polys,xScale,yScale);
}

//--------------------------------------------------------------
void SimplifyPolygon(ofPolyline &in_poly, 
                     ofPolylines  &out_polys,
                     ClipperLib::long64 xScale,
                     ClipperLib::long64 yScale) {
    ClipperLib::Polygon in;
    ClipperLib::Polygons out;
    in = ofPolylineToPolygon(in_poly,xScale,yScale);
    ClipperLib::SimplifyPolygon(in,out);
    polygonsToofPolylines(out,out_polys,xScale,yScale);
}

//--------------------------------------------------------------
void SimplifyPolygons(ofPolylines &in_polys, 
                      ofPolylines &out_polys,
                      ClipperLib::long64 xScale,
                      ClipperLib::long64 yScale) {
    ClipperLib::Polygons in;
    ClipperLib::Polygons out;
    ofPolylinesToPolygons(in_polys,in,xScale,yScale);
    ClipperLib::SimplifyPolygons(in,out);
    polygonsToofPolylines(out,out_polys,xScale,yScale);
}

//--------------------------------------------------------------
void SimplifyPolygons(ofPolylines &polys,
                      ClipperLib::long64 xScale,
                      ClipperLib::long64 yScale) {
    ClipperLib::Polygons in;
    ofPolylinesToPolygons(polys,in,xScale,yScale);
    ClipperLib::SimplifyPolygons(in);
    polys.clear();
    polygonsToofPolylines(in,polys,xScale,yScale);
}

//--------------------------------------------------------------
void ReversePoints(ofPolyline& poly,
                   ClipperLib::long64 xScale,
                   ClipperLib::long64 yScale) {
    ClipperLib::Polygon in;
    in = ofPolylineToPolygon(poly,xScale,yScale);
    ClipperLib::ReversePoints(in);
    poly = polygonToofPolyline(in,xScale,yScale);
}

//--------------------------------------------------------------
void ReversePoints(ofPolylines& polys,
                   ClipperLib::long64 xScale,
                   ClipperLib::long64 yScale) {
    ClipperLib::Polygons in;
    ofPolylinesToPolygons(polys,in,xScale,yScale);
    ClipperLib::ReversePoints(in);
    polys.clear();
    polygonsToofPolylines(in,polys,xScale,yScale);
}

//--------------------------------------------------------------
ClipperLib::PolyFillType convertWindingType(ofPolyWindingMode windingMode) {
    
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