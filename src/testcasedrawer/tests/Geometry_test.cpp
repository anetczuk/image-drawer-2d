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

#include "imgdraw2d/Geometry.h"

#include <boost/test/unit_test.hpp>
#include <iostream>


using namespace imgdraw2d;


BOOST_AUTO_TEST_SUITE( LinearSuite )

    BOOST_AUTO_TEST_CASE( createFromOrthogonal_OX_1 ) {
        const Linear linear = Linear::createFromOrthogonal( PointI{0, 5}, PointI{0, 1} );
        BOOST_CHECK_EQUAL( 5, linear.valueY(10) );
    }

    BOOST_AUTO_TEST_CASE( createFromOrthogonal_OX_2 ) {
        const Linear linear = Linear::createFromOrthogonal( PointI{0, -5}, PointI{0, 1} );
        BOOST_CHECK_EQUAL( -5, linear.valueY(10) );
    }

    BOOST_AUTO_TEST_CASE( createFromOrthogonal_OY ) {
        const Linear linear = Linear::createFromOrthogonal( PointI{5, 0}, PointI{1, 0} );
        BOOST_CHECK_EQUAL( 5, linear.valueX(10) );
    }

    BOOST_AUTO_TEST_CASE( createFromParallel_OX ) {
        const Linear linear = Linear::createFromParallel( PointI{0, 5}, PointI{1, 0} );
        BOOST_CHECK_EQUAL( linear.valueY(10), 5 );
    }

    BOOST_AUTO_TEST_CASE( createFromParallel_OY ) {
        const Linear linear = Linear::createFromParallel( PointI{5, 0}, PointI{0, 1} );
        BOOST_CHECK_EQUAL( linear.valueX(10), 5 );
    }

    BOOST_AUTO_TEST_CASE( pointSide_OX_above_01 ) {
        const Linear linear = Linear::createFromParallel( PointI{5, 0}, PointI{ 1, 0} );
        BOOST_CHECK_EQUAL( linear.pointSide( PointI{10, 10} ), 10 );
    }

    BOOST_AUTO_TEST_CASE( pointSide_OX_above_02 ) {
        const Linear linear = Linear::createFromParallel( PointI{5, 0}, PointI{-1, 0} );
        BOOST_CHECK_EQUAL( linear.pointSide( PointI{10, 10} ), 10 );
    }

    BOOST_AUTO_TEST_CASE( pointSide_OX_below_01 ) {
        const Linear linear = Linear::createFromParallel( PointI{5, 0}, PointI{ 1, 0} );
        BOOST_CHECK_EQUAL( linear.pointSide( PointI{10, -10} ), -10 );
    }

    BOOST_AUTO_TEST_CASE( pointSide_OX_below_02 ) {
        const Linear linear = Linear::createFromParallel( PointI{5, 0}, PointI{-1, 0} );
        BOOST_CHECK_EQUAL( linear.pointSide( PointI{10, -10} ), -10 );
    }

    BOOST_AUTO_TEST_CASE( pointSide_diagonal ) {
        const Linear linear = Linear::createFromParallel( PointI{5, 5}, PointI{ 1,  1} );
        BOOST_CHECK_EQUAL( linear.pointSide( PointI{5,  10} ),   5 );
        BOOST_CHECK_EQUAL( linear.pointSide( PointI{5, -10} ), -15 );
    }

    BOOST_AUTO_TEST_CASE( pointSide_diagonal_reverse ) {
        const Linear linear = Linear::createFromParallel( PointI{5, 5}, PointI{-1, -1} );
        BOOST_CHECK_EQUAL( linear.pointSide( PointI{5,  10} ),   5 );
        BOOST_CHECK_EQUAL( linear.pointSide( PointI{5, -10} ), -15 );
    }

    BOOST_AUTO_TEST_CASE( distance ) {
        const Linear linear = Linear::createFromParallel( PointI{5, 5}, PointI{1, 0} );
        BOOST_CHECK_CLOSE( linear.distance( PointI{5,  10} ),   5, 1.0 );
        BOOST_CHECK_CLOSE( linear.distance( PointI{5, -10} ),  15, 1.0 );
    }

BOOST_AUTO_TEST_SUITE_END()
