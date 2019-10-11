/*
 * PngDrawer.cpp
 *
 *  Created on: Oct 11, 2019
 *      Author: bob
 */

#include "PngDrawer.h"

#include <png++/png.hpp>


namespace tcd {

    PngDrawer::PngDrawer() {
        // TODO Auto-generated constructor stub

    }

    PngDrawer::~PngDrawer() {
        // TODO Auto-generated destructor stub
    }

    bool PngDrawer::loadImage(const std::string& path) {
        ///

        png::image< png::rgb_pixel > image("input.png");
        image.write("output.png");


        return false;
    }

} /* namespace tcd */
