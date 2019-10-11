/*
 * PngDrawer.h
 *
 *  Created on: Oct 11, 2019
 *      Author: bob
 */

#ifndef TESTCASEDRAWER_SRC_PNGDRAWER_H_
#define TESTCASEDRAWER_SRC_PNGDRAWER_H_

#include <string>


namespace tcd {

    class PngDrawer {
    public:

        PngDrawer();

        virtual ~PngDrawer();

        bool loadImage(const std::string& path);

    };

} /* namespace tcd */

#endif /* TESTCASEDRAWER_SRC_PNGDRAWER_H_ */
