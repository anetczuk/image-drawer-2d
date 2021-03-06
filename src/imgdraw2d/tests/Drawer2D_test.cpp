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


#define CHECK_IMAGE( image )    IMAGE_CHECK_CASE( image, "drawer2d" )
#define SAVE_IMAGE( image )     IMAGE_SAVE( image, "drawer2d" )


BOOST_AUTO_TEST_SUITE( Drawer2DSuite )

    BOOST_AUTO_TEST_CASE( drawLine_1 ) {
        Drawer2DD drawer;
        drawer.setDrawColor( "blue" );
        drawer.drawLine( PointD{1.0, 1.0}, PointD{10.0, 6.0}, 0.2 );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawLine_negative_short ) {
        Drawer2DD drawer(200.0);
        drawer.setDrawColor( "blue" );
        drawer.drawLine( PointD{ -9.70711, -9.70711}, PointD{ -9.0, -9.0}, 0.2 );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawLine_thin ) {
        Drawer2DD drawer(100.0);
        drawer.setDrawColor( "black" );
        drawer.drawLine( PointD{ 0.0, 0.0}, PointD{ 1.0, 1.0}, 0.001 );
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
        drawer.setDrawColor( Image::GREEN );
        drawer.fillRect( PointD{10.0, 10.0}, 2.0, 5.0, 0.0 );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( fillRect02 ) {
        Drawer2DD drawer;
        drawer.setDrawColor( Image::GREEN );
        drawer.fillRect( PointD{10.0, 10.0}, 2.0, 5.0, M_PI_2 + M_PI_4 / 2.0 );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( fillRect ) {
        Drawer2DD drawer(100.0);
        drawer.setDrawColor( "red" );
        drawer.fillRect( PointD{1.0, 1.0}, 5.0, 5.0 );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( fillCircle_positive ) {
        Drawer2DD drawer(100.0);
        drawer.setDrawColor( "red" );
        drawer.fillCircle( PointD{1.0, 1.0}, 1.0 );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( fillCircle_negative ) {
        Drawer2DD drawer(100.0);
        drawer.setDrawColor( "red" );
        drawer.fillCircle( PointD{-5.0, -5.0}, 1.0 );
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
        drawer.setDrawColor( "blue" );
        drawer.drawRing( PointD{10.0, 10.0}, 4.0, 10.0 );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawRing_thin ) {
        Drawer2DD drawer(20.0);
        drawer.setDrawColor( "blue" );
        drawer.drawRing( PointD{0.0, 0.0}, 1.0, 0.001 );
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
        drawer.setDrawColor( "blue" );
        drawer.drawArc( PointD{10.0, 10.0}, 5.0, 1.0, 0.0, 2 * M_PI );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_90 ) {
        Drawer2DD drawer( 10.0, 1.0 );
        drawer.setDrawColor( "blue" );
        drawer.drawArc( PointD{10.0, 10.0}, 5.0, 1.0, 0.0, M_PI_2 );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_1q ) {
        Drawer2DD drawer;
        drawer.setDrawColor( "blue" );
        drawer.drawArc( PointD{10.0, 10.0}, 5.0, 1.0, M_PI_4 / 2, M_PI_4 );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_2q ) {
        Drawer2DD drawer;
        drawer.setDrawColor( "blue" );
        drawer.drawArc( PointD{10.0, 10.0}, 5.0, 1.0, M_PI_2 + M_PI_4 / 2, M_PI_4 );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_3q ) {
        Drawer2DD drawer;
        drawer.setDrawColor( "blue" );
        drawer.drawArc( PointD{10.0, 10.0}, 5.0, 1.0, M_PI + M_PI_4 / 2, M_PI_4 );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_4q ) {
        Drawer2DD drawer;
        drawer.setDrawColor( "blue" );
        drawer.drawArc( PointD{10.0, 10.0}, 5.0, 1.0, 3 * M_PI_2 + M_PI_4 / 2, M_PI_4 );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( drawArc_thin ) {
        Drawer2DD drawer(100.0);
        drawer.setDrawColor( "black" );
        drawer.drawArc( PointD{0.0, 0.0}, 1.0, 0.001, M_PI_4 / 2, M_PI_4 );
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

    BOOST_AUTO_TEST_CASE( drawClothoid_distance ) {
        Drawer2DD drawer( 200.0 );
        drawer.setBackground( Image::WHITE );
        drawer.autoResize = false;
        drawer.resizeImage( 2.0 );

        drawer.setDrawColor( "black" );
        drawer.drawClothoid( PointD(0.0, 0.0), 0.0, 0.01,  5.0, 1.0 );
        drawer.setDrawColor( "red" );
        drawer.drawClothoid( PointD(0.0, 0.0), 0.0, 0.01, -5.0, 1.0 );

        drawer.setDrawColor( "green" );
        drawer.drawClothoid( PointD(0.0, 0.0), 0.0, 0.01,  5.0, 2.0 );
        drawer.setDrawColor( "blue" );
        drawer.drawClothoid( PointD(0.0, 0.0), 0.0, 0.01, -5.0, 2.0 );

        CHECK_IMAGE( drawer.image() );
    }

    BOOST_AUTO_TEST_CASE( drawClothoid_flatness ) {
        Drawer2DD drawer( 200.0 );
        drawer.setBackground( Image::WHITE );
        drawer.autoResize = false;
        drawer.resizeImage( 1.0 );

        drawer.setDrawColor( "black" );
        drawer.drawClothoid( PointD(0.0, 0.0), 0.0, 0.01,  4.0,  1.0 );
        drawer.setDrawColor( "red" );
        drawer.drawClothoid( PointD(0.0, 0.0), 0.0, 0.01,  4.0, -1.0 );

        drawer.setDrawColor( "green" );
        drawer.drawClothoid( PointD(0.0, 0.0), 0.0, 0.01, -4.0,  1.0 );
        drawer.setDrawColor( "blue" );
        drawer.drawClothoid( PointD(0.0, 0.0), 0.0, 0.01, -4.0, -1.0 );

        CHECK_IMAGE( drawer.image() );
    }

    BOOST_AUTO_TEST_CASE( drawClothoid_offset ) {
        Drawer2DD drawer( 200.0 );
        drawer.setBackground( Image::WHITE );
        drawer.autoResize = false;
        drawer.resizeImage( -0.5, 2.5 );

        drawer.setDrawColor( "black" );
        drawer.fillCircle( PointD(0.0, 0.0), 0.05 );

        drawer.setDrawColor( "red" );
        drawer.drawClothoid( PointD(1.0, 1.0), -M_PI_4, 0.03,  5.0, 1.0 );
        drawer.setDrawColor( "green" );
        drawer.drawClothoid( PointD(1.0, 1.0), -M_PI_4, 0.03, -5.0, 1.0 );

        CHECK_IMAGE( drawer.image() );
    }

    BOOST_AUTO_TEST_CASE( drawClothoid_point ) {
        Drawer2DD drawer( 200.0 );
        drawer.setBackground( Image::WHITE );
        drawer.autoResize = false;
        drawer.resizeImage( -0.5, 3.0 );

        drawer.setDrawColor( "black" );
        drawer.fillCircle( PointD(0.0, 0.0), 0.03 );

        drawer.setDrawColor( "orange" );
        const PointD startPoint(1.0, 2.5);
        drawer.fillCircle( startPoint, 0.03 );

        drawer.setDrawColor( Image::RED );
        drawer.drawClothoid( startPoint, M_PI_2, 0.01, -3.0, 4.0, 1.5 );

        CHECK_IMAGE( drawer.image() );
    }

    BOOST_AUTO_TEST_CASE( drawClothoidLR ) {
        Drawer2DD drawer( 100.0 );
        drawer.setBackground( Image::WHITE );
        drawer.autoResize = false;
        drawer.resizeImage( 2.0 );

        drawer.setDrawColor( Image::BLACK );
        drawer.drawClothoidLR( PointD(0.0, 0.0), 0.0, 0.01,  2.0,  0.25 );
        drawer.setDrawColor( Image::RED );
        drawer.drawClothoidLR( PointD(0.0, 0.0), 0.0, 0.01,  2.0, -0.25 );

        drawer.setDrawColor( Image::GREEN );
        drawer.drawClothoidLR( PointD(0.0, 0.0), 0.0, 0.01, -2.0,  0.25 );
        drawer.setDrawColor( Image::BLUE );
        drawer.drawClothoidLR( PointD(0.0, 0.0), 0.0, 0.01, -2.0, -0.25 );

        CHECK_IMAGE( drawer.image() );
    }

    BOOST_AUTO_TEST_CASE( drawClothoidLA ) {
        Drawer2DD drawer( 300.0 );
        drawer.setBackground( Image::WHITE );
        drawer.autoResize = false;
        drawer.resizeImage( 2.0 );

        drawer.setDrawColor( Image::BLACK );
        drawer.drawClothoidLA( PointD(0.0, 0.0), 0.0, 0.01,  2.0, M_PI_2 );
        drawer.setDrawColor( Image::RED );
        drawer.drawClothoidLA( PointD(0.0, 0.0), 0.0, 0.01, -2.0, M_PI_2 );

        drawer.setDrawColor( Image::GREEN );
        drawer.drawClothoidLA( PointD(0.0, 0.0), 0.0, 0.01,  2.0, -M_PI_2 );
        drawer.setDrawColor( Image::BLUE );
        drawer.drawClothoidLA( PointD(0.0, 0.0), 0.0, 0.01, -2.0, -M_PI_2 );

        CHECK_IMAGE( drawer.image() );
    }

    BOOST_AUTO_TEST_CASE( drawClothoidLA_angle ) {
        Drawer2DD drawer( 300.0 );
        drawer.setBackground( Image::WHITE );
        drawer.autoResize = false;
        drawer.resizeImage( -0.5, 1.5 );

        drawer.setDrawColor( Image::BLACK );
        drawer.drawClothoidLA( PointD(0.0, 0.0), 0.0, 0.01,  2.0,     M_PI_2 );
        drawer.setDrawColor( Image::RED );
        drawer.drawClothoidLA( PointD(0.0, 0.0), 0.0, 0.01,  2.0, 2 * M_PI_2 );
        drawer.setDrawColor( Image::GREEN );
        drawer.drawClothoidLA( PointD(0.0, 0.0), 0.0, 0.01,  2.0, 3 * M_PI_2 );
        drawer.setDrawColor( Image::BLUE );
        drawer.drawClothoidLA( PointD(0.0, 0.0), 0.0, 0.01,  2.0, 4 * M_PI_2 );
        drawer.setDrawColor( Image::ORANGE );
        drawer.drawClothoidLA( PointD(0.0, 0.0), 0.0, 0.01,  2.0, 5 * M_PI_2 );

        CHECK_IMAGE( drawer.image() );
    }

    BOOST_AUTO_TEST_CASE( drawClothoidRA ) {
        Drawer2DD drawer( 300.0 );
        drawer.setBackground( Image::WHITE );
        drawer.autoResize = false;
        drawer.resizeImage( -1.5, 1.5 );

        drawer.setDrawColor( Image::BLACK );
        drawer.drawClothoidRA( PointD(0.0, 0.0), 0.0, 0.01,  0.5,   2 * M_PI_2 );
        drawer.setDrawColor( Image::RED );
        drawer.drawClothoidRA( PointD(0.0, 0.0), 0.0, 0.01, -0.5,   2 * M_PI_2 );
        drawer.setDrawColor( Image::GREEN );
        drawer.drawClothoidRA( PointD(0.0, 0.0), 0.0, 0.01,  0.5,  -2 * M_PI_2 );
        drawer.setDrawColor( Image::BLUE );
        drawer.drawClothoidRA( PointD(0.0, 0.0), 0.0, 0.01, -0.5,  -2 * M_PI_2 );

        SAVE_IMAGE( drawer.image() );
    }

    BOOST_AUTO_TEST_CASE( resizeImage ) {
        Drawer2DD drawer;
        drawer.setBackground( Image::RED );
        drawer.resizeImage( PointD{ 2.0, 2.0}, PointD{ 12.0, 12.0} );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( expand ) {
        Drawer2DD drawer;
        drawer.setDrawColor( "blue" );
        drawer.drawLine( PointD{ 2.0, 2.0}, PointD{10.0,  2.0}, 0.2 );
        drawer.setDrawColor( "red" );
        drawer.drawLine( PointD{10.0, 2.0}, PointD{10.0, 10.0}, 0.2 );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( expand_positive ) {
        Drawer2DD drawer;
        drawer.setDrawColor( "red" );
        drawer.fillCircle( PointD{ 5.0,  5.0}, 1.0 );
        drawer.setDrawColor( "green" );
        drawer.fillCircle( PointD{-5.0, -5.0}, 1.0 );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( expand_negative ) {
        Drawer2DD drawer;
        drawer.setDrawColor( "green" );
        drawer.fillCircle( PointD{-5.0, -5.0}, 1.0 );
        drawer.setDrawColor( "red" );
        drawer.fillCircle( PointD{ 5.0,  5.0}, 1.0 );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( example ) {
        Drawer2DD drawer(20.0);
        drawer.setDrawColor( "red" );
        drawer.drawLine( PointD{0.0, 0.0}, PointD{10.0, 0.0}, 1.0 );
        drawer.setDrawColor( "blue" );
        drawer.drawArc( PointD{5.0, 5.0}, 5.0, 1.0, 0.0, M_PI );
        drawer.setDrawColor( "green" );
        drawer.fillRect( PointD{3.0, 3.0}, 4.0, 4.0 );
        drawer.setDrawColor( "orange" );
        drawer.fillCircle( PointD{5.0, 14.0}, 2.0 );

        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( setBackground ) {
        Drawer2DD drawer(50.0);
        drawer.setBackground("white");
        drawer.setDrawColor( "red" );
        drawer.fillCircle( PointD{1.0, 1.0}, 1.0 );
        drawer.setDrawColor( "green" );
        drawer.fillCircle( PointD{5.0, 5.0}, 1.0 );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

    BOOST_AUTO_TEST_CASE( autoResize_disabled ) {
        Drawer2DD drawer(50.0);
        drawer.resizeImage( 0.0, 0.0, 10.0, 10.0 );
        drawer.setBackground("white");
        drawer.autoResize = false;

        drawer.setDrawColor( "red" );
        drawer.fillCircle( PointD{10.0, 10.0}, 10.0 );
        Image& image = drawer.image();

        CHECK_IMAGE( image );
    }

BOOST_AUTO_TEST_SUITE_END()
