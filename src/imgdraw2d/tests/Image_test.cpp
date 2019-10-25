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

#include "imgdraw2d/Image.h"

#include <boost/test/unit_test.hpp>


using namespace imgdraw2d;


BOOST_AUTO_TEST_SUITE( ImageSuite )

    BOOST_AUTO_TEST_CASE( compare_same_instance ) {
        const Image object;
        BOOST_CHECK_EQUAL( (object == object), true );
    }

    BOOST_AUTO_TEST_CASE( compare_empty ) {
        const Image object1;
        const Image object2;
        BOOST_CHECK_EQUAL( (object1 == object2), true );
    }

    BOOST_AUTO_TEST_CASE( load_valid ) {
        Image object;
        const bool loaded = object.load("refimg/blue.png");

        BOOST_REQUIRE_EQUAL( loaded, true );
        BOOST_CHECK_EQUAL( object.empty(), false );
        BOOST_CHECK_EQUAL(object.width(), 300);
        BOOST_CHECK_EQUAL(object.height(), 300);
        BOOST_CHECK_EQUAL(object.blue(0, 0), 255);
    }

    BOOST_AUTO_TEST_CASE( load_fail ) {
        Image object;
        const bool loaded = object.load("not_existing_file.png");
        BOOST_REQUIRE_EQUAL( loaded, false );
        BOOST_CHECK_EQUAL( object.empty(), true );
        BOOST_CHECK_EQUAL(object.width(), 0);
        BOOST_CHECK_EQUAL(object.height(), 0);
    }

    BOOST_AUTO_TEST_CASE( save_empty ) {
        Image object;
        object.save( "empty.png" );
    }

    BOOST_AUTO_TEST_CASE( save_load ) {
        Image object(10, 10);
        BOOST_CHECK_EQUAL(object.width(), 10);
        BOOST_CHECK_EQUAL(object.height(), 10);
        object.fill("red");
        {
            const Image::Pixel pix = object.pixel(1, 1);
            BOOST_CHECK_EQUAL(pix.red, 255);
            BOOST_CHECK_EQUAL(pix.green, 0);
            BOOST_CHECK_EQUAL(pix.blue, 0);
            BOOST_CHECK_EQUAL(pix.alpha, 255);
        }

        const std::string path( "save_test.png" );
        object.save(path);

        const Image loaded(path);
        BOOST_CHECK_EQUAL(loaded.width(), 10);
        BOOST_CHECK_EQUAL(loaded.height(), 10);

        const Image::Pixel pix = loaded.pixel(1, 1);
        BOOST_CHECK_EQUAL(pix.red, 255);
        BOOST_CHECK_EQUAL(pix.green, 0);
        BOOST_CHECK_EQUAL(pix.blue, 0);
        BOOST_CHECK_EQUAL(pix.alpha, 255);
    }

    BOOST_AUTO_TEST_CASE( compare_differ ) {
        Image object1;
        object1.load("refimg/red.png");
        BOOST_CHECK_EQUAL( object1.empty(), false );

        Image object2;
        object2.load("refimg/blue.png");
        BOOST_CHECK_EQUAL( object2.empty(), false );

        BOOST_CHECK_EQUAL( (object1 == object2), false );
    }

BOOST_AUTO_TEST_SUITE_END()
