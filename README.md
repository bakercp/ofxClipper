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

Notes
-----

#### Animation

Clipper works internally with integers. If you do slow animated clipping operations, motion may not be smooth. One solution is to do supersampling. To get 0.1 pixel resolution (10x the default) multiply all ofPolyline vertex coordinates by 10, use the enlarged polyline with Clipper, and finally scale the result down with `ofScale(0.1)` when drawing the result.

#### Filled shapes

Clipper outputs a `std::vector<ofPolyline>`. ofPolyline can not be rendered as a filled shape (OF 0.10). One way to draw polylines as filled shapes is to use `ofVertex()`:

```cpp
ofBeginShape();
for(auto& v: aPolyline)
{
    ofVertex(v);
}
ofEndShape();
```

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
void ofApp::draw()
{
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

    // Create two partially overlapping rotating pentagons

    ofPolyline pa, pb;

    for(int i = 0; i < 5; i++)
    {
        float a = i * glm::two_pi<float>() / 5 + ofGetElapsedTimef();
        pa.addVertex(300 * std::cos(a) - 80, 300 * std::sin(a));
        pb.addVertex(300 * std::cos(-a) + 80, 300 * std::sin(-a));
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
    for (auto& line: intersection)
    {
        line.draw();
    }
}
```

Note: this minimal example tries to be short, not fast. Avoid creating instances of `ofPolyline` and `ofx::Clipper` inside `draw()`.

Build Status
------------

No CI.

Compatibility
-------------

The `stable` branch of this repository is meant to be compatible with the openFrameworks [stable branch](https://github.com/openframeworks/openFrameworks/tree/stable), which corresponds to the latest official openFrameworks release.

The `master` branch of this repository is meant to be compatible with the openFrameworks [master branch](https://github.com/openframeworks/openFrameworks/tree/master).

Some past openFrameworks releases are supported via tagged versions, but only `stable` and `master` branches are actively supported.

Versioning
----------

This project uses Semantic Versioning, although strict adherence will only come into effect at version 1.0.0.

Licensing
---------

See [LICENSE.md](LICENSE.md)

Contributing
------------

Check out the [Help Wanted](https://github.com/bakercp/ofxClipper/issues?q=is%3Aissue+is%3Aopen+label%3A%22help+wanted%22) tag in the issues section for specific ideas or propose your own new ideas.

Pull Requests are always welcome, so if you make any improvements please feel free to float them back upstream :)

1.  Fork this repository.
2.  Create your feature branch (`git checkout -b my-new-feature`).
3.  Commit your changes (`git commit -am 'Add some feature'`).
4.  Push to the branch (`git push origin my-new-feature`).
5.  Create new Pull Request.
