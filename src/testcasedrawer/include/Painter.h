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

#ifndef TESTCASEDRAWER_INCLUDE_PAINTER_H_
#define TESTCASEDRAWER_INCLUDE_PAINTER_H_

#include "Image.h"


namespace tcd {

    class Painter {
    public:

        class ModeWorker {
        public:

            Image* img;

            ModeWorker(Image* image): img(image) {
            }

            virtual ~ModeWorker() {
            }

            virtual void drawImage(const uint32_t x, const uint32_t y, const Image& source) = 0;

            virtual void fillRect(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height, const std::string& color) = 0;

        };


        enum CompositionMode {
            CM_DESTINATION,             /// similar to QPainter::CompositionMode_Destination
            CM_DIFFERENCE               /// similar to QPainter::CompositionMode_Difference
        };

    private:

        Image* img;
        CompositionMode mode;
        std::unique_ptr<ModeWorker> worker;


    public:

        Painter(Image& image);

        Painter(Image* image);

        void setCompositionMode(const CompositionMode mode);

        void drawImage(const uint32_t x, const uint32_t y, const Image& source);

        void fillRect(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height, const std::string& color);

    };

} /* namespace tcd */

#endif /* TESTCASEDRAWER_INCLUDE_PAINTER_H_ */
