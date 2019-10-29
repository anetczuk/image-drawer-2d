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

#include "imgdraw2d/ImageComparator.h"

#include <boost/test/unit_test.hpp>


using namespace imgdraw2d;


BOOST_AUTO_TEST_SUITE( ImageComparatorSuite )

    BOOST_AUTO_TEST_CASE( makeDiff_null ) {
        const Image image("refimg/blue.png");
        BOOST_CHECK_EQUAL( image.empty(), false );

        {
            const ImagePtr result = ImageComparator::compare(nullptr, &image);
            BOOST_REQUIRE( result != nullptr );
            BOOST_CHECK_EQUAL( result->empty(), true );
        }

        {
            const ImagePtr result = ImageComparator::compare(&image, nullptr);
            BOOST_REQUIRE( result != nullptr );
            BOOST_CHECK_EQUAL( result->empty(), true );
        }
    }

    BOOST_AUTO_TEST_CASE( makeDiff_same ) {
        const Image image("refimg/blue.png");
        BOOST_CHECK_EQUAL( image.empty(), false );

        const ImagePtr result = ImageComparator::compare(image, image);
        BOOST_REQUIRE( result != nullptr );
        BOOST_REQUIRE_EQUAL( result->empty(), false );

        result->save("outimg/comparator/blue_diff.png");

        const Image data("refimg/comparator/blue_diff.png");
        BOOST_CHECK_EQUAL( data.empty(), false );

        BOOST_CHECK( result->equals( data ) );
    }

    BOOST_AUTO_TEST_CASE( makeDiff_diff ) {
        const Image imageA("refimg/red.png");
        BOOST_CHECK_EQUAL( imageA.empty(), false );

        const Image imageB("refimg/blue.png");
        BOOST_CHECK_EQUAL( imageB.empty(), false );

        const ImagePtr result = ImageComparator::compare(imageA, imageB);
        BOOST_REQUIRE( result != nullptr );
        BOOST_REQUIRE_EQUAL( result->empty(), false );

        result->save("outimg/comparator/red_blue_diff.png");

        const Image data("refimg/comparator/red_blue_diff.png");
        BOOST_REQUIRE_EQUAL( data.empty(), false );

        BOOST_CHECK_EQUAL( result->equals( data ), true );
    }

    BOOST_AUTO_TEST_CASE( makeDiff_black ) {
        const Image imageA("refimg/black.png");
        BOOST_CHECK_EQUAL( imageA.empty(), false );

        const Image image_empty;

        const ImagePtr result = ImageComparator::compare(imageA, image_empty);
        BOOST_REQUIRE( result != nullptr );
        BOOST_REQUIRE_EQUAL( result->empty(), false );

        result->save("outimg/comparator/black.xdiff.png");

        const Image data("refimg/comparator/black.diff.png");
        BOOST_REQUIRE_EQUAL( data.empty(), false );

        BOOST_CHECK_EQUAL( result->equals( data ), true );
    }

    BOOST_AUTO_TEST_CASE( makeDiff_white ) {
        const Image imageA("refimg/white.png");
        BOOST_CHECK_EQUAL( imageA.empty(), false );

        const Image image_empty;

        const ImagePtr result = ImageComparator::compare(imageA, image_empty);
        BOOST_REQUIRE( result != nullptr );
        BOOST_REQUIRE_EQUAL( result->empty(), false );

        result->save("outimg/comparator/white.xdiff.png");

        const Image data("refimg/comparator/white.diff.png");
        BOOST_REQUIRE_EQUAL( data.empty(), false );

        BOOST_CHECK_EQUAL( result->equals( data ), true );
    }

    BOOST_AUTO_TEST_CASE( makeDiff_emptyA ) {
        const Image imageA;                         // empty
        BOOST_CHECK_EQUAL( imageA.empty(), true );

        const Image imageB("refimg/blue.png");
        BOOST_CHECK_EQUAL( imageB.empty(), false );

        const ImagePtr result = ImageComparator::compare(imageA, imageB);
        BOOST_REQUIRE( result != nullptr );
        BOOST_REQUIRE_EQUAL( result->empty(), false );

        result->save("outimg/comparator/emptyA_diff.png");

        const Image data("refimg/comparator/emptyA_diff.png");
        BOOST_REQUIRE_EQUAL( data.empty(), false );

        BOOST_CHECK( result->equals( data ) );
    }

    BOOST_AUTO_TEST_CASE( makeDiff_emptyB ) {
        const Image imageA("refimg/blue.png");
        BOOST_CHECK_EQUAL( imageA.empty(), false );

        const Image imageB;                    // empty
        BOOST_CHECK_EQUAL( imageB.empty(), true );

        const ImagePtr result = ImageComparator::compare(imageA, imageB);
        BOOST_REQUIRE( result != nullptr );
        BOOST_REQUIRE_EQUAL( result->empty(), false );

        result->save("outimg/comparator/emptyB_diff.png");

        const Image data("refimg/comparator/emptyB_diff.png");
        BOOST_REQUIRE_EQUAL( data.empty(), false );

        BOOST_CHECK( result->equals( data ) );
    }

    BOOST_AUTO_TEST_CASE( makeDiff_empty_both ) {
        const Image imageA;                    // empty
        BOOST_CHECK( imageA.empty() );

        const Image imageB;                    // empty
        BOOST_CHECK( imageB.empty() );

        const ImagePtr result = ImageComparator::compare(imageA, imageB);
        BOOST_REQUIRE( result != nullptr );

        result->save("outimg/comparator/emptyC_diff.png");

        const Image data("refimg/comparator/emptyC_diff.png");
        BOOST_REQUIRE_EQUAL( data.empty(), false );

        BOOST_CHECK( result->equals( data ) );
    }

    BOOST_AUTO_TEST_CASE( makeDiff_customSize ) {
        Image imageA(304, 304);
        imageA.fill("red");

        const Image imageB;                    // empty
        BOOST_CHECK_EQUAL( imageB.empty(), true );

        const ImagePtr result = ImageComparator::compare(imageA, imageB);
        BOOST_REQUIRE( result != nullptr );

        result->save("outimg/comparator/chess_custom_diff.png");

        const Image data("refimg/comparator/chess_custom_diff.png");
        BOOST_REQUIRE_EQUAL( data.empty(), false );

        BOOST_CHECK( result->equals( data ) );
    }

    BOOST_AUTO_TEST_CASE( makeDiff_minSize ) {
        Image imageA(24, 24);
        imageA.fill("red");

        const Image imageB;                    // empty
        BOOST_CHECK( imageB.empty() );

        const ImagePtr result = ImageComparator::compare(imageA, imageB);
        BOOST_REQUIRE( result != nullptr );

        result->save("outimg/comparator/chess_min_diff.png");

        const Image data("refimg/comparator/chess_min_diff.png");
        BOOST_REQUIRE_EQUAL( data.empty(), false );

        BOOST_CHECK( result->equals( data ) );
    }

    BOOST_AUTO_TEST_CASE( makeDiff_diff_sizes ) {
        Image imageA(100, 100);
        imageA.fill("red");

        Image imageB(80, 80);
        imageB.fill("red");

        const ImagePtr result = ImageComparator::compare(imageA, imageB);
        BOOST_REQUIRE( result != nullptr );

        result->save("outimg/comparator/diff_sizes.xdiff.png");

        const Image data("refimg/comparator/diff_sizes.diff.png");
        BOOST_REQUIRE_EQUAL( data.empty(), false );

        BOOST_CHECK( result->equals( data ) );
    }

BOOST_AUTO_TEST_SUITE_END()
