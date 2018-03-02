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

Issues
------

#### Animation

Clipper works internally with integers. If you do slow animated clipping operations, motion may not be smooth. One solution is to do supersampling. To get 0.1 pixel resolution (10x the default) multiply all ofPolyline vertex coordinates by 10, use the enlarged polyline with Clipper, and finally scale the result down with `ofScale(0.1)` when drawing the result.

#### Filled shapes

Clipper outputs a `std::vector<ofPolyline>`. ofPolyline can not be rendered as a filled shape (OF 0.10). One way to draw polylines as filled shapes is to use `ofVertex()`:

```cpp
ofBeginShape();
for(auto & vert : aPolyline) {
  ofVertex(vert);
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


#License

The Clipper license file can be found in the `libs/clipper` folder above.

See [license.txt](https://github.com/bakercp/ofxClipper/raw/master/license.txt) for wrapper details.
