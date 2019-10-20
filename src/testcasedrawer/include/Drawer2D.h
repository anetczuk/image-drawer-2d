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

#ifndef TESTCASEDRAWER_INCLUDE_DRAWER2D_H_
#define TESTCASEDRAWER_INCLUDE_DRAWER2D_H_

#include "Painter.h"


namespace tcd {

    class Drawer2D {

        ImagePtr img;
        Painter painter;


    public:

        Drawer2D();

        const Image& image() const {
            return *(img.get());
        }

        void save(const std::string& path) {
            img->save(path);
        }

        void drawImage(const PointI& point, const Image& source) {
            painter.drawImage( point, source );
        }

        void drawLine(const PointI& fromPoint, const PointI& toPoint, const uint32_t radius, const std::string& color) {
            painter.drawLine( fromPoint, toPoint, radius, color );
        }

        void fillRect(const PointI& point, const uint32_t width, const uint32_t height, const std::string& color) {
            painter.fillRect( point, width, height, color );
        }

        void fillCircle(const PointI& point, const uint32_t radius, const std::string& color) {
            painter.fillCircle( point, radius, color );
        }

    };

} /* namespace tcd */

#endif /* TESTCASEDRAWER_INCLUDE_DRAWER2D_H_ */
