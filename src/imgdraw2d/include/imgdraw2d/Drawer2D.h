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

#ifndef IMGDRAW2D_INCLUDE_DRAWER2D_H_
#define IMGDRAW2D_INCLUDE_DRAWER2D_H_

#include "Painter.h"


namespace imgdraw2d {

    class Drawer2D {

        ImagePtr img;
        Painter painter;
        RectD sizeBox;
        double scale;

        Image::Pixel backgroundColor;


    public:

        Drawer2D(const double scale = 10.0);

        Drawer2D(const RectD& size, const double scale = 10.0);

        const Image& image() const {
            return *(img.get());
        }

        Image& image() {
            return *(img.get());
        }

        void save(const std::string& path) {
            img->save(path);
        }

        void setBackground(const std::string& color);

        void drawImage(const PointD& topLeftPoint, const Image& source);

        void drawLine(const PointD& fromPoint, const PointD& toPoint, const double radius, const std::string& color);

        void drawArc(const PointD& center, const double radius, const double width, const double startAngle, const double range, const std::string& color);

        void fillRect(const PointD& bottomLeftPoint, const double width, const double height, const std::string& color);

        void fillRect(const PointD& bottomLeftPoint, const double width, const double height, const Image::Pixel& color);

        void fillCircle(const PointD& point, const double radius, const std::string& color);


        PointI transformPoint(const PointD& point) const;


    protected:

        void resize(const RectD& box);

        void resetImage();

        void resizeImage();

    };

} /* namespace imgdraw2d */

#endif /* IMGDRAW2D_INCLUDE_DRAWER2D_H_ */
