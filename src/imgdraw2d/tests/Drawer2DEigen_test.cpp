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
#include <Eigen/Core>
#include <chrono>


using namespace imgdraw2d;


#define CHECK_IMAGE( image )                 IMAGE_CHECK_CASE( image, "drawer2dEigen" )

#define COMPARE_IMAGES( imageA, imageB )     IMAGES_COMPARE( imageA, imageB, "drawer2dEigen" )


typedef Eigen::Vector2d Vec2;
typedef Drawer2D< Vec2 > Drawer2DE;


BOOST_AUTO_TEST_SUITE( Drawer2DEigenSuite )

    BOOST_AUTO_TEST_CASE( fillRect ) {
        Drawer2DD drawer1( 10.0, 1.0 );
        drawer1.setDrawColor( Image::RED );
        drawer1.fillRect( PointD{10.0, 10.0}, 1.0, 1.0, 0.0 );
        Image& image1 = drawer1.image();

        Drawer2DE drawer2( 10.0, 1.0 );
        drawer2.setDrawColor( Image::RED );
        drawer2.fillRect( Vec2{10.0, 10.0}, 1.0, 1.0, 0.0 );
        Image& image2 = drawer2.image();

        COMPARE_IMAGES( image1, image2 );
    }

    BOOST_AUTO_TEST_CASE( drawArc_90 ) {
        Drawer2DD drawer1( 10.0, 1.0 );
        drawer1.setDrawColor( "blue" );
        drawer1.drawArc( PointD{10.0, 10.0}, 5.0, 1.0, 0.0, M_PI_2 );
        Image& image1 = drawer1.image();

        Drawer2DE drawer2( 10.0, 1.0 );
        drawer2.setDrawColor( "blue" );
        drawer2.drawArc( Vec2{10.0, 10.0}, 5.0, 1.0, 0.0, M_PI_2 );
        Image& image2 = drawer2.image();

        COMPARE_IMAGES( image1, image2 );
    }

    BOOST_AUTO_TEST_CASE( drawClothoid_distance ) {
        Drawer2DE drawer( 200.0 );
        drawer.setBackground( Image::WHITE );
        drawer.autoResize = false;
        drawer.resizeImage( 2.0 );

        drawer.setDrawColor( "black" );
        drawer.drawClothoid( Vec2(0.0, 0.0), 0.0, 0.01,  5.0, 1.0 );
        drawer.setDrawColor( "red" );
        drawer.drawClothoid( Vec2(0.0, 0.0), 0.0, 0.01, -5.0, 1.0 );

        drawer.setDrawColor( "green" );
        drawer.drawClothoid( Vec2(0.0, 0.0), 0.0, 0.01,  5.0, 2.0 );
        drawer.setDrawColor( "blue" );
        drawer.drawClothoid( Vec2(0.0, 0.0), 0.0, 0.01, -5.0, 2.0 );

        CHECK_IMAGE( drawer.image() );
    }

BOOST_AUTO_TEST_SUITE_END()
