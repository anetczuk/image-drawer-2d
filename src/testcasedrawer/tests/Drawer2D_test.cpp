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

#include <boost/test/unit_test.hpp>

#include "Drawer2D.h"

#include "ImageComparator.h"


using namespace tcd;


BOOST_AUTO_TEST_SUITE( Drawer2DSuite )

    BOOST_AUTO_TEST_CASE( drawLine_1 ) {
        Drawer2D drawer;
        drawer.drawLine( Point{20, 20}, Point{200, 120}, 1, "blue" );
        drawer.save("tests/drawer_line_1.png");

        const bool compare = ImageComparator::compare(drawer.image(), "data/drawer_line_1.png", "tests/drawer_line_1.png");
        BOOST_CHECK( compare );
    }

BOOST_AUTO_TEST_SUITE_END()
