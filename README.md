ofxClipper
==========

![Screenshot](https://github.com/bakercp/ofxClipper/raw/master/screen.png)

Description
-----------

An [openFrameworks](http://www.openframeworks.cc/) wrapper for [Clipper](http://www.angusj.com/delphi/clipper.php) - "an open source freeware polygon clipping library".

From the author's website:

_"The Clipper library primarily performs the boolean clipping operations - intersection, union, difference & xor - on 2D polygons. It also performs polygon offsetting._

_The library handles complex (self-intersecting) polygons, polygons with holes and polygons with overlapping co-linear edges._
_Input polygons for clipping can use EvenOdd, NonZero, Positive and Negative filling modes._
_The clipping code is based on the Vatti clipping algorithm, and out performs other clipping libraries."_

[http://www.openframeworks.cc/](http://www.openframeworks.cc/)

Installation
------------

To use ofxClipper, first you need to download and install [Open Frameworks](https://github.com/openframeworks/openFrameworks).

To get a copy of the repository you can download the source from [http://github.com/bakercp/ofxClipper](http://github.com/bakercp/ofxClipper) or, alternatively, you can use git clone:

```
git clone git://github.com/bakercp/ofxClipper.git
```

The addon should sit in `openFrameworks/addons/ofxClipper/`.

#### Which version to use?

ofxClipper has been tested with the latest development version of openFrameworks.

Example
-------
```cpp
void ofApp::draw() {
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

    // Create two partially overlapping rotating pentagons
    ofPolyline pa, pb;
    for(int i = 0; i < 5; i++) {
        float a = i * static_cast<float>(TWO_PI) / 5 + ofGetElapsedTimef();
        pa.addVertex(300 * cos(a) - 80, 300 * sin(a));
        pb.addVertex(300 * cos(-a) + 80, 300 * sin(-a));
    }
    pa.close();
    pb.close();

    // Create and populate clipper
    ofx::Clipper clipper;
    clipper.addPolyline(pa, ClipperLib::ptSubject);
    clipper.addPolyline(pb, ClipperLib::ptClip);

    // Calculate intersection between the two pentagons
    auto intersection = clipper.getClipped(ClipperLib::ClipType::ctIntersection);
    
    // Draw result
    for(auto & line : intersection) {
        line.draw();
    }
}
```
Note: this minimal example tries to be short, not fast. Avoid creating instances of `ofPolyline` and `ofx::Clipper` inside `draw()`.

License
-------

The Clipper license file can be found in the `libs/clipper` folder above.

See [license.txt](https://github.com/bakercp/ofxClipper/raw/master/license.txt) for wrapper details.
