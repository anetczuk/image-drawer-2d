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
        const Image image("data/blue.png");
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
        const Image image("data/blue.png");
        BOOST_CHECK_EQUAL( image.empty(), false );

        const ImagePtr result = ImageComparator::compare(image, image);
        BOOST_REQUIRE( result != nullptr );
        BOOST_REQUIRE_EQUAL( result->empty(), false );

        result->save("tests/diff/blue_diff.png");

        const Image data("data/diff/blue_diff.png");
        BOOST_CHECK_EQUAL( data.empty(), false );

        BOOST_CHECK( result->equals( data ) );
    }

    BOOST_AUTO_TEST_CASE( makeDiff_diff ) {
        const Image imageA("data/red.png");
        BOOST_CHECK_EQUAL( imageA.empty(), false );

        const Image imageB("data/blue.png");
        BOOST_CHECK_EQUAL( imageB.empty(), false );

        const ImagePtr result = ImageComparator::compare(imageA, imageB);
        BOOST_REQUIRE( result != nullptr );
        BOOST_REQUIRE_EQUAL( result->empty(), false );

        result->save("tests/diff/red_blue_diff.png");

        const Image data("data/diff/red_blue_diff.png");
        BOOST_REQUIRE_EQUAL( data.empty(), false );

        BOOST_CHECK_EQUAL( result->equals( data ), true );
    }

    BOOST_AUTO_TEST_CASE( makeDiff_emptyA ) {
        const Image imageA;                         // empty
        BOOST_CHECK_EQUAL( imageA.empty(), true );

        const Image imageB("data/blue.png");
        BOOST_CHECK_EQUAL( imageB.empty(), false );

        const ImagePtr result = ImageComparator::compare(imageA, imageB);
        BOOST_REQUIRE( result != nullptr );
        BOOST_REQUIRE_EQUAL( result->empty(), false );

        result->save("tests/diff/emptyA_diff.png");

        const Image data("data/diff/emptyA_diff.png");
        BOOST_REQUIRE_EQUAL( data.empty(), false );

        BOOST_CHECK( result->equals( data ) );
    }

    BOOST_AUTO_TEST_CASE( makeDiff_emptyB ) {
        const Image imageA("data/blue.png");
        BOOST_CHECK_EQUAL( imageA.empty(), false );

        const Image imageB;                    // empty
        BOOST_CHECK_EQUAL( imageB.empty(), true );

        const ImagePtr result = ImageComparator::compare(imageA, imageB);
        BOOST_REQUIRE( result != nullptr );
        BOOST_REQUIRE_EQUAL( result->empty(), false );

        result->save("tests/diff/emptyB_diff.png");

        const Image data("data/diff/emptyB_diff.png");
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

        result->save("tests/diff/emptyC_diff.png");

        const Image data("data/diff/emptyC_diff.png");
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

        result->save("tests/diff/chess_custom_diff.png");

        const Image data("data/diff/chess_custom_diff.png");
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

        result->save("tests/diff/chess_min_diff.png");

        const Image data("data/diff/chess_min_diff.png");
        BOOST_REQUIRE_EQUAL( data.empty(), false );

        BOOST_CHECK( result->equals( data ) );
    }

BOOST_AUTO_TEST_SUITE_END()
