/*
 * Utils.h
 *
 *  Created on: Oct 22, 2019
 *      Author: bob
 */

#ifndef IMGDRAW2D_TESTS_UTILS_H_
#define IMGDRAW2D_TESTS_UTILS_H_

#include "imgdraw2d/ImageComparator.h"

#include <boost/test/unit_test.hpp>
#include <sstream>


#define CONCAT_STRINGS( strings )            ((std::stringstream&)(std::stringstream() << strings)).str()


#define IMAGE_CHECK( image, path )                                                                            \
    {                                                                                                         \
        const std::string sourcePath = CONCAT_STRINGS( "data/" << path << ".png" );                           \
        const std::string testPath = CONCAT_STRINGS( "tests/" << path << ".png" );                            \
        image.save( testPath );                                                                               \
        const bool compare = ImageComparator::compare(  image, sourcePath,                                    \
                                                        CONCAT_STRINGS( "tests/" << path << ".diff.png" ) );  \
        BOOST_CHECK_MESSAGE( compare, CONCAT_STRINGS( testPath <<  " differs from " << sourcePath )  );       \
    }


#define IMAGE_CHECK_CASE( image, checkCase )                                                                  \
    {                                                                                                         \
        const std::string testCaseName = boost::unit_test::framework::current_test_case().p_name;             \
        const std::string imgDir = checkCase + std::string("/") + testCaseName;                               \
        IMAGE_CHECK( image, imgDir );                                                                         \
    }


#endif /* IMGDRAW2D_TESTS_UTILS_H_ */
