/// MIT License
///
/// Copyright (c) 2019 Arkadiusz Netczuk <dev.arnet@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include "Painter.h"

#include <boost/test/unit_test.hpp>

#include <vector>


using namespace imgdraw2d;


struct StripPoint {
    int x;
    int y;

    StripPoint(const int x = 0, const int y = 0): x(x), y(y) {
    }
};

typedef std::vector<StripPoint> LineStrip;


struct StripRect {
    int minX;
    int minY;
    int maxX;
    int maxY;
};


StripRect stripRect(const LineStrip& strip) {
    if (strip.size() < 1) {
        return StripRect();
    }
    StripRect ret;
    {
        const StripPoint& pt = strip[0];
        ret.minX = pt.x;
        ret.maxX = pt.x;
        ret.minY = pt.y;
        ret.maxY = pt.y;
    }
    for(std::size_t i=1; i<strip.size(); ++i) {
        const StripPoint& pt = strip[i];
        ret.minX = std::min(ret.minX, pt.x);
        ret.maxX = std::max(ret.maxX, pt.x);
        ret.minY = std::min(ret.minY, pt.y);
        ret.maxY = std::max(ret.maxY, pt.y);
    }
    return ret;
}

void draw(Image& image, const LineStrip& strip) {
    const uint32_t multiplier = 100;
    const uint32_t offset = 1;
    const StripRect rect = stripRect(strip);
    const uint32_t width  = (rect.maxX - rect.minX + 2 * offset) * multiplier;
    const uint32_t height = (rect.maxY - rect.minY + 2 * offset) * multiplier;
    image.resize(width, height);
    Painter drawer(image);

    const std::size_t sSize = strip.size();
    if (sSize > 1) {
        StripPoint prevPt = strip[0];
        for(std::size_t i=1; i<sSize; ++i) {
            const StripPoint& pt = strip[i];
            const uint32_t prevx = (prevPt.x - rect.minX + offset) * multiplier;
            const uint32_t prevy = (prevPt.y - rect.minY + offset) * multiplier;
            const uint32_t x = (pt.x - rect.minX + offset) * multiplier;
            const uint32_t y = (pt.y - rect.minY + offset) * multiplier;
            drawer.drawLine(prevx, prevy, x, y, 10, "blue");
            prevPt = pt;
        }
    }
    for(std::size_t i=0; i<sSize; ++i) {
        const StripPoint& pt = strip[i];
        const uint32_t x = (pt.x - rect.minX + offset) * multiplier;
        const uint32_t y = (pt.y - rect.minY + offset) * multiplier;
        drawer.fillCircle(x, y, 30, "red");
    }
}


BOOST_AUTO_TEST_SUITE( PointSuite )

    BOOST_AUTO_TEST_CASE( loadImage_not_found ) {
        LineStrip strip;
        strip.push_back( StripPoint{ 0, 10} );
        strip.push_back( StripPoint{ 5, 12} );
        strip.push_back( StripPoint{10,  2} );

        Image image;
        draw(image, strip);

        image.save("tests/point_strip.png");

//        const bool loaded = object.loadImage("not_found.png");
//        BOOST_CHECK_EQUAL( loaded, false );
    }

BOOST_AUTO_TEST_SUITE_END()
