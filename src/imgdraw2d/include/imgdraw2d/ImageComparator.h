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

#ifndef IMAGE_COMPARATOR_H_
#define IMAGE_COMPARATOR_H_

#include "imgdraw2d/Image.h"

#include <string>


namespace imgdraw2d {

    class ImageComparator {
    public:

        static ImagePtr compare(const Image& imgA, const Image& imgB);

        static ImagePtr compare(const Image* imgA, const Image* imgB);

        static bool compare(const Image& imgA, const std::string& imgB, const std::string& diffImage);

        static bool compare(const Image* imgA, const std::string& imgB, const std::string& diffImage) {
            if (imgA == nullptr)
                return false;
            return compare( *imgA, imgB, diffImage );
        }

        static bool compare(const ImagePtr& imgA, const std::string& imgB, const std::string& diffImage) {
            return compare( imgA.get(), imgB, diffImage );
        }

    };

}

#endif /* IMAGE_COMPARATOR_H_ */
