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

#include "imgdraw2d/Painter.h"

#include "Utils.h"


using namespace imgdraw2d;


BOOST_AUTO_TEST_SUITE( PainterSuite )

    BOOST_AUTO_TEST_CASE( fillCircle ) {
        Image image(400, 400);
        Painter painter( image );
        painter.fillCircle( 200, 200, 100, "red" );

        IMAGE_CHECK( image, "painter/fillCircle" );
    }

    BOOST_AUTO_TEST_CASE( fillRect ) {
        Image image(400, 400);
        Painter painter( image );
        painter.fillRect( 100, 100, 200, 200, "blue" );

        IMAGE_CHECK( image, "painter/fillRect" );
    }

    BOOST_AUTO_TEST_CASE( drawLine_horizontal_1 ) {
        Image image(220, 140);
        Painter painter( image );
        painter.drawLine( 20, 20, 200, 120, 1, "blue" );

        IMAGE_CHECK( image, "painter/drawLine_horizontal_1" );
    }

    BOOST_AUTO_TEST_CASE( drawLine_horizontal_10 ) {
        Image image(140, 40);
        Painter painter( image );
        painter.drawLine( 20, 20, 120, 20, 10, "blue" );

        IMAGE_CHECK( image, "painter/drawLine_horizontal_10" );
    }

    BOOST_AUTO_TEST_CASE( drawLine_vertical_1 ) {
        Image image(140, 220);
        Painter painter( image );
        painter.drawLine( 20, 20, 120, 200, 1, "blue" );

        IMAGE_CHECK( image, "painter/drawLine_vertical_1" );
    }

    BOOST_AUTO_TEST_CASE( drawLine_vertical_10 ) {
        Image image(40, 140);
        Painter painter( image );
        painter.drawLine( 20, 20, 20, 120, 10, "blue" );

        IMAGE_CHECK( image, "painter/drawLine_vertical_10" );
    }

    BOOST_AUTO_TEST_CASE( drawLine_diagonal_1 ) {
        Image image(140, 140);
        Painter painter( image );
        painter.drawLine( 20, 20, 120, 120, 1, "blue" );

        IMAGE_CHECK( image, "painter/drawLine_diagonal_1" );
    }

    BOOST_AUTO_TEST_CASE( drawLine_diagonal_10 ) {
        Image image(140, 140);
        Painter painter( image );
        painter.drawLine( 20, 20, 120, 120, 10, "blue" );

        IMAGE_CHECK( image, "painter/drawLine_diagonal_10" );
    }

    BOOST_AUTO_TEST_CASE( drawLine_diagonal_10_negative ) {
        Image image(140, 140);
        Painter painter( image );
        painter.drawLine( 120, 120, 20, 20, 10, "blue" );

        IMAGE_CHECK( image, "painter/drawLine_diagonal_10_negative" );
    }

BOOST_AUTO_TEST_SUITE_END()
