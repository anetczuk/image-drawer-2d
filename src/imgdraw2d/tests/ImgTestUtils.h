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

#ifndef IMGDRAW2D_TESTS_IMGTESTUTILS_H_
#define IMGDRAW2D_TESTS_IMGTESTUTILS_H_

#include "imgdraw2d/ImageComparator.h"

#include <boost/test/unit_test.hpp>
#include <sstream>


#define CONCAT_STRINGS( strings )            ((std::stringstream&)(std::stringstream() << strings)).str()


#define IMAGE_CHECK( image, path )                                                                              \
    {                                                                                                           \
        const std::string sourcePath = CONCAT_STRINGS( "refimg/" << path << ".png" );                           \
        const std::string testPath   = CONCAT_STRINGS( "outimg/" << path << ".png" );                           \
        image.save( testPath );                                                                                 \
        const bool compare = imgdraw2d::ImageComparator::compare(  image, sourcePath,                           \
                                                        CONCAT_STRINGS( "outimg/" << path << ".xdiff.png" ) );  \
        BOOST_CHECK_MESSAGE( compare, CONCAT_STRINGS( testPath <<  " differs from " << sourcePath )  );         \
    }


#define IMAGE_CHECK_CASE( image, checkCase )                                                                  \
    {                                                                                                         \
        const std::string testCaseName = boost::unit_test::framework::current_test_case().p_name;             \
        const std::string imgDir = checkCase + std::string("/") + testCaseName;                               \
        IMAGE_CHECK( image, imgDir );                                                                         \
    }


#endif /* IMGDRAW2D_TESTS_IMGTESTUTILS_H_ */
