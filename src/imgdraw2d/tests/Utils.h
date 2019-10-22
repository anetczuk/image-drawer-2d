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


#define IMAGE_CHECK( image, path )                                                                          \
    {                                                                                                       \
        image.save( std::string("tests/") + path + ".png");                                                 \
        const bool compare = ImageComparator::compare(  image,                                              \
                                                        std::string("data/")  + path + ".png",              \
                                                        std::string("tests/") + path + ".diff.png");        \
        BOOST_CHECK( compare );                                                                             \
    }


#endif /* IMGDRAW2D_TESTS_UTILS_H_ */
