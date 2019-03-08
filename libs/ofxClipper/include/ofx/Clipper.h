//
// Copyright (c) 2010 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include <vector>
#include "clipper.hpp"
#include "ofPolyline.h"
#include "ofPath.h"


namespace ofx {


class Clipper: public ClipperLib::Clipper
{
public:
    Clipper();
    ~Clipper();

    std::vector<ofPolyline> getClipped(ClipperLib::ClipType clipType,
                                       ofPolyWindingMode subFillType = OF_POLY_WINDING_ODD,
                                       ofPolyWindingMode clipFillType = OF_POLY_WINDING_ODD,
                                       ClipperLib::cInt scale = DEFAULT_CLIPPER_SCALE);

    ofRectangle getBounds(ClipperLib::cInt scale = DEFAULT_CLIPPER_SCALE) const;

    bool addRectangle(const ofRectangle& rectangle,
                      ClipperLib::PolyType PolyTyp,
                      bool autoClose = false,
                      ClipperLib::cInt scale = DEFAULT_CLIPPER_SCALE);

    bool addPolyline(const ofPolyline& polyline,
                     ClipperLib::PolyType PolyTyp,
                     bool autoClose = false,
                     ClipperLib::cInt scale = DEFAULT_CLIPPER_SCALE);

    bool addPolylines(const std::vector<ofPolyline>& polylines,
                      ClipperLib::PolyType PolyTyp,
                      bool autoClose = false,
                      ClipperLib::cInt scale = DEFAULT_CLIPPER_SCALE);

    bool addPath(const ofPath& path,
                 ClipperLib::PolyType PolyTyp,
                 bool autoClose = false,
                 ClipperLib::cInt scale = DEFAULT_CLIPPER_SCALE);

    static void close(ofPolyline& polyline);
    static void close(std::vector<ofPolyline>& polylines);

    static bool areAllClosed(const std::vector<ofPolyline>& polylines);

    static ClipperLib::IntPoint toClipper(const ofDefaultVertexType& vertex,
                                          ClipperLib::cInt scale);

    static ClipperLib::IntRect toClipper(const ofRectangle& rectangle,
                                         ClipperLib::cInt scale);

    static ClipperLib::Path toClipper(const ofPolyline& polyline,
                                      ClipperLib::cInt scale);

    static ClipperLib::Paths toClipper(const std::vector<ofPolyline>& polylines,
                                       ClipperLib::cInt scale);

    static ClipperLib::Paths toClipper(const ofPath& path,
                                       ClipperLib::cInt scale);

    static ofRectangle toOf(const ClipperLib::IntRect& rectangle,
                            ClipperLib::cInt scale);

    static ofDefaultVertexType toOf(const ClipperLib::IntPoint& point,
                                    ClipperLib::cInt scale);

    static ofPolyline toOf(const ClipperLib::Path& path,
                           bool isClosed,
                           ClipperLib::cInt scale);

    static std::vector<ofPolyline> toOf(const ClipperLib::Paths& paths,
                                        bool isClosed,
                                        ClipperLib::cInt scale);

    static std::string toString(ClipperLib::ClipType clipType);
    static std::string toString(ClipperLib::PolyType polyType);
    static std::string toString(ClipperLib::PolyFillType polyfillType);
    static std::string toString(ClipperLib::InitOptions initOption);
    static std::string toString(ClipperLib::JoinType joinType);
    static std::string toString(ClipperLib::EndType endType);

    static ofPolyWindingMode next(ofPolyWindingMode s);
    static ClipperLib::PolyFillType next(ClipperLib::PolyFillType s);
    static ClipperLib::ClipType next(ClipperLib::ClipType s);
    
    static ofPolyWindingMode toOf(ClipperLib::PolyFillType polyfillType);
    static ClipperLib::PolyFillType toClipper(ofPolyWindingMode windingMode);

    static std::vector<ofPolyline> simplifyPolyline(const ofPolyline& polyline,
                                                    ofPolyWindingMode windingMode = OF_POLY_WINDING_ODD,
                                                    ClipperLib::cInt scale = DEFAULT_CLIPPER_SCALE);

    static std::vector<ofPolyline> simplifyPolylines(const std::vector<ofPolyline>& polylines,
                                                     ofPolyWindingMode windingMode = OF_POLY_WINDING_ODD,
                                                     ClipperLib::cInt scale = DEFAULT_CLIPPER_SCALE);

    static std::vector<ofPolyline> simplifyPath(const ofPath& path,
                                                ofPolyWindingMode windingMode = OF_POLY_WINDING_ODD,
                                                ClipperLib::cInt scale = DEFAULT_CLIPPER_SCALE);

    static std::vector<ofPolyline> getOffsets(const std::vector<ofPolyline>& polylines,
                                              double offset,
                                              ClipperLib::JoinType jointype = ClipperLib::jtSquare,
                                              ClipperLib::EndType endtype = ClipperLib::etOpenSquare,
                                              double miterLimit = DEFAULT_MITER_LIMIT,
                                              double arcTolerance = DEFAULT_ARC_TOLERANCE,
                                              ClipperLib::cInt scale = DEFAULT_CLIPPER_SCALE);

    static ofPolyline reversPolyline(const ofPolyline& polyline,
                                     ClipperLib::cInt scale = DEFAULT_CLIPPER_SCALE);
    
    static std::vector<ofPolyline> reversePolylines(const std::vector<ofPolyline>& polylines,
                                                    ClipperLib::cInt scale = DEFAULT_CLIPPER_SCALE);
    
    static const ClipperLib::cInt DEFAULT_CLIPPER_SCALE;
    static const double DEFAULT_MITER_LIMIT;
    static const double DEFAULT_ARC_TOLERANCE;

};


} // namespace ofx
