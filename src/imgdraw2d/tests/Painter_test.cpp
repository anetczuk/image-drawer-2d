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

#include "ImgTestUtils.h"


using namespace imgdraw2d;


#define CHECK_IMAGE( image )     IMAGE_CHECK_CASE( image, "painter" )


BOOST_AUTO_TEST_SUITE( PainterSuite )

    BOOST_AUTO_TEST_CASE( fillCircle ) {
        Image image(400, 400);
        Painter painter( image );
        painter.fillCircle( 200, 200, 100, "red" );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( fillRect_01 ) {
        Image image(400, 400);
        Painter painter( image );
        painter.fillRect( 100, 100, 200, 200, "blue" );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( fillRect_02 ) {
        Image image(260, 260);
        Painter painter( image );
        const PointI topLeft{100, 100};
        const PointI topRight{200, 120};
        const PointI bottomRight{140, 140};
        const PointI bottomLeft{120, 190};
        painter.fillRect( topLeft, topRight, bottomRight, bottomLeft, Image::BLUE );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawLine_horizontal_1 ) {
        Image image(220, 140);
        Painter painter( image );
        painter.drawLine( 20, 20, 200, 120, 2, "blue" );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawLine_horizontal_10 ) {
        Image image(140, 40);
        Painter painter( image );
        painter.drawLine( 20, 20, 120, 20, 20, "blue" );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawLine_vertical_1 ) {
        Image image(140, 220);
        Painter painter( image );
        painter.drawLine( 20, 20, 120, 200, 2, "blue" );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawLine_vertical_10 ) {
        Image image(40, 140);
        Painter painter( image );
        painter.drawLine( 20, 20, 20, 120, 20, "blue" );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawLine_diagonal_1 ) {
        Image image(140, 140);
        Painter painter( image );
        painter.drawLine( 20, 20, 120, 120, 2, "blue" );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawLine_diagonal_10 ) {
        Image image(140, 140);
        Painter painter( image );
        painter.drawLine( 20, 20, 120, 120, 20, "blue" );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawLine_diagonal_10_negative ) {
        Image image(140, 140);
        Painter painter( image );
        painter.drawLine( 120, 120, 20, 20, 20, "blue" );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawRing_thin ) {
        Image image(140, 140);
        Painter painter( image );
        painter.drawRing( PointI{70, 70}, 40, 1, "blue" );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_full_positive ) {
        {
            Image image(140, 140);
            Painter painter( image );
            painter.drawArc( PointI{70, 70}, 40, 6,  0.5, 2 * M_PI, "blue" );

            IMAGE_CHECK( image, "painter/drawArc_full_positive_a" );
        }
        {
            Image image(140, 140);
            Painter painter( image );
            painter.drawArc( PointI{70, 70}, 40, 6, -0.5, 2 * M_PI, "blue" );

            IMAGE_CHECK( image, "painter/drawArc_full_positive_b" );
        }
    }

    BOOST_AUTO_TEST_CASE( drawArc_full_negative ) {
        {
            Image image(140, 140);
            Painter painter( image );
            painter.drawArc( PointI{70, 70}, 40, 6,  0.5, - 2 * M_PI, "blue" );

            IMAGE_CHECK( image, "painter/drawArc_full_negative_a" );
        }
        {
            Image image(140, 140);
            Painter painter( image );
            painter.drawArc( PointI{70, 70}, 40, 6, -0.5, - 2 * M_PI, "blue" );

            IMAGE_CHECK( image, "painter/drawArc_full_negative_b" );
        }
    }

    BOOST_AUTO_TEST_CASE( drawArc_1q ) {
        Image image(140, 140);
        Painter painter( image );
        painter.drawArc( PointI{70, 70}, 40, 6, M_PI_4 / 2, M_PI_4, "blue" );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_2q ) {
        Image image(140, 140);
        Painter painter( image );
        painter.drawArc( PointI{70, 70}, 40, 6, M_PI_2 + M_PI_4 / 2, M_PI_4, "blue" );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_3q ) {
        Image image(140, 140);
        Painter painter( image );
        painter.drawArc( PointI{70, 70}, 40, 6, M_PI + M_PI_4 / 2, M_PI_4, "blue" );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_4q ) {
        Image image(140, 140);
        Painter painter( image );
        painter.drawArc( PointI{70, 70}, 40, 6, 3 * M_PI_2 + M_PI_4 / 2, M_PI_4, "blue" );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_negative_1 ) {
        Image image(140, 140);
        Painter painter( image );
        painter.drawArc( PointI{70, 70}, 40, 6, M_PI + M_PI_4, -M_PI_2, "blue" );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_negative_2 ) {
        Image image(140, 140);
        Painter painter( image );
        painter.drawArc( PointI{70, 70}, 40, 6, M_PI_4, -M_PI_2, "blue" );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_positive_1 ) {
        Image image(140, 140);
        Painter painter( image );
        painter.drawArc( PointI{70, 70}, 40, 6, M_PI - M_PI_4, M_PI_2, "blue" );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_positive_2 ) {
        Image image(140, 140);
        Painter painter( image );
        painter.drawArc( PointI{70, 70}, 40, 6, -M_PI_4, M_PI_2, "blue" );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_positive_range ) {
        Image image(140, 140);
        Painter painter( image );
        painter.drawArc( PointI{70, 70}, 40, 6, 0.0,  3 * M_PI_2, "blue" );

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_negative_range ) {
        Image image(140, 140);
        Painter painter( image );
        painter.drawArc( PointI{70, 70}, 40, 6, 0.0, -3 * M_PI_2, "blue" );

        CHECK_IMAGE( image );
    }

BOOST_AUTO_TEST_SUITE_END()
