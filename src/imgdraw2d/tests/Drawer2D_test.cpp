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

#include "imgdraw2d/Drawer2D.h"

#include "ImgTestUtils.h"
#include <chrono>


using namespace imgdraw2d;


#define CHECK_IMAGE( image )     IMAGE_CHECK_CASE( image, "drawer2d" )


BOOST_AUTO_TEST_SUITE( Drawer2DSuite )

    BOOST_AUTO_TEST_CASE( drawLine_1 ) {
        Drawer2DD drawer;
        drawer.drawLine( PointD{1.0, 1.0}, PointD{10.0, 6.0}, 0.2, "blue" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawLine_negative_short ) {
        Drawer2DD drawer(200.0);
        drawer.drawLine( PointD{ -9.70711, -9.70711}, PointD{ -9.0, -9.0}, 0.2, "blue" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawLine_thin ) {
        Drawer2DD drawer(100.0);
        drawer.drawLine( PointD{ 0.0, 0.0}, PointD{ 1.0, 1.0}, 0.001, "black" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

//    BOOST_AUTO_TEST_CASE( drawLine_fill ) {
//        const std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//
//        Drawer2DD drawer(200.0);
//        for(std::size_t i=0; i< 10; ++i) {
//            const PointD start{ 0.0 + i, 10.0 + i};
//            const PointD end{  10.0 + i, 10.0 + i};
//            drawer.drawLine( start, end, 2.0, "blue" );
//        }
//
//        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//        std::cerr << "time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;
//    }


    BOOST_AUTO_TEST_CASE( fillRect01 ) {
        Drawer2DD drawer;
        drawer.fillRect( PointD{10.0, 10.0}, 2.0, 5.0, 0.0, Image::GREEN );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( fillRect02 ) {
        Drawer2DD drawer;
        drawer.fillRect( PointD{10.0, 10.0}, 2.0, 5.0, M_PI_2 + M_PI_4 / 2.0, Image::GREEN );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( fillRect ) {
        Drawer2DD drawer(100.0);
        drawer.fillRect( PointD{1.0, 1.0}, 5.0, 5.0, "red" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( fillCircle_positive ) {
        Drawer2DD drawer(100.0);
        drawer.fillCircle( PointD{1.0, 1.0}, 1.0, "red" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( fillCircle_negative ) {
        Drawer2DD drawer(100.0);
        drawer.fillCircle( PointD{-5.0, -5.0}, 1.0, "red" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

//    BOOST_AUTO_TEST_CASE( fillCircle_big ) {
//        const std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//
//        Drawer2DD drawer(200.0);
//        drawer.fillCircle( PointD{-5.0, -5.0}, 20.0, "red" );
//
//        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//        std::cerr << "time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;
//    }

    BOOST_AUTO_TEST_CASE( drawRing_circle ) {
        Drawer2DD drawer(20.0);
        drawer.drawRing( PointD{10.0, 10.0}, 4.0, 10.0, "blue" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawRing_thin ) {
        Drawer2DD drawer(20.0);
        drawer.drawRing( PointD{0.0, 0.0}, 1.0, 0.001, "blue" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

//    BOOST_AUTO_TEST_CASE( drawRing_big ) {
//        const std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//
//        Drawer2DD drawer(200.0);
//        drawer.drawRing( PointD{10.0, 10.0}, 20.0, 10.0, "blue" );
//
//        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//        std::cerr << "drawRing time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;
//    }

    BOOST_AUTO_TEST_CASE( drawArc_full ) {
        Drawer2DD drawer;
        drawer.drawArc( PointD{10.0, 10.0}, 5.0, 1.0, 0.0, 2 * M_PI, "blue" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_90 ) {
        Drawer2DD drawer( 10.0, 1.0 );
        drawer.drawArc( PointD{10.0, 10.0}, 5.0, 1.0, 0.0, M_PI_2, "blue" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_1q ) {
        Drawer2DD drawer;
        drawer.drawArc( PointD{10.0, 10.0}, 5.0, 1.0, M_PI_4 / 2, M_PI_4, "blue" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_2q ) {
        Drawer2DD drawer;
        drawer.drawArc( PointD{10.0, 10.0}, 5.0, 1.0, M_PI_2 + M_PI_4 / 2, M_PI_4, "blue" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_3q ) {
        Drawer2DD drawer;
        drawer.drawArc( PointD{10.0, 10.0}, 5.0, 1.0, M_PI + M_PI_4 / 2, M_PI_4, "blue" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_4q ) {
        Drawer2DD drawer;
        drawer.drawArc( PointD{10.0, 10.0}, 5.0, 1.0, 3 * M_PI_2 + M_PI_4 / 2, M_PI_4, "blue" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_thin ) {
        Drawer2DD drawer(100.0);
        drawer.drawArc( PointD{0.0, 0.0}, 1.0, 0.001, M_PI_4 / 2, M_PI_4, "black" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

//    BOOST_AUTO_TEST_CASE( drawArc_big ) {
//        const std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//
//        Drawer2DD drawer(200.0);
//        drawer.drawArc( PointD{10.0, 10.0}, 20.0, 10.0, 0.0, 3 * M_PI_2, "blue" );
//
//        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//        std::cerr << "drawArc time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;
//    }

    BOOST_AUTO_TEST_CASE( resize ) {
        Drawer2DD drawer;
        drawer.drawLine( PointD{ 2.0, 2.0}, PointD{10.0,  2.0}, 0.2, "blue" );
        drawer.drawLine( PointD{10.0, 2.0}, PointD{10.0, 10.0}, 0.2, "red" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( resize_positive ) {
        Drawer2DD drawer;
        drawer.fillCircle( PointD{ 5.0,  5.0}, 1.0, "red" );
        drawer.fillCircle( PointD{-5.0, -5.0}, 1.0, "green" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( resize_negative ) {
        Drawer2DD drawer;
        drawer.fillCircle( PointD{-5.0, -5.0}, 1.0, "green" );
        drawer.fillCircle( PointD{ 5.0,  5.0}, 1.0, "red" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( example ) {
        Drawer2DD drawer(20.0);
        drawer.drawLine( PointD{0.0, 0.0}, PointD{10.0, 0.0}, 1.0, "red" );
        drawer.drawArc( PointD{5.0, 5.0}, 5.0, 1.0, 0.0, M_PI, "blue" );
        drawer.fillRect( PointD{3.0, 3.0}, 4.0, 4.0, "green" );
        drawer.fillCircle( PointD{5.0, 14.0}, 2.0, "orange" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( setBackground ) {
        Drawer2DD drawer(50.0);
        drawer.setBackground("white");
        drawer.fillCircle( PointD{1.0, 1.0}, 1.0, "red" );
        drawer.fillCircle( PointD{5.0, 5.0}, 1.0, "green" );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

BOOST_AUTO_TEST_SUITE_END()
