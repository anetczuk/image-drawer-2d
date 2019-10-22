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

#include "Drawer2D.h"


static const double MARGIN = 0.5;


namespace tcd {

    Drawer2D::Drawer2D(const double scale): img( new Image() ), painter( *img ), sizeBox{ {0.0, 0.0}, {0.0, 0.0} }, scale(scale) {
    }

    Drawer2D::Drawer2D(const RectD& size, const double scale): Drawer2D(scale) {
        resize(size);
    }

    void Drawer2D::drawImage(const PointD& topLeftPoint, const Image& source) {
        const PointI from = transformPoint(topLeftPoint);
        painter.drawImage( from, source );
    }

    void Drawer2D::drawLine(const PointD& fromPoint, const PointD& toPoint, const double radius, const std::string& color) {
        RectD box = RectD::minmax( fromPoint, toPoint );
        box.expand(radius);
        resize(box);
        const PointI from = transformPoint(fromPoint);
        const PointI to = transformPoint(toPoint);
        const uint32_t rad = radius * scale;
        painter.drawLine( from, to, rad, color );
    }

    void Drawer2D::fillRect(const PointD& bottomLeftPoint, const double width, const double height, const std::string& color) {
        const PointD topRight = bottomLeftPoint + PointD{width, height};
        const RectD box = RectD::minmax( bottomLeftPoint, topRight );
        resize(box);
        const PointD topLeft = bottomLeftPoint + PointD{0.0, height};
        const PointI point = transformPoint(topLeft);
        const uint32_t w = width * scale;
        const uint32_t h = height * scale;
        painter.fillRect( point, w, h, color );
    }

    void Drawer2D::fillCircle(const PointD& point, const double radius, const std::string& color) {
        RectD box = RectD::minmax( point, point );
        box.expand(radius);
        resize(box);
        const PointI center = transformPoint(point);
        const uint32_t rad = radius * scale;
        painter.fillCircle( center, rad, color );
    }

    PointI Drawer2D::transformPoint(const PointD& point) const {
        ///flip y coord
        PointD relative{ point.x - sizeBox.a.x, sizeBox.b.y - point.y };
        relative.x += MARGIN;
        relative.y += MARGIN;
        PointD scaled = relative * scale;
        const PointI pixelPoint{ (PointI::value_type)scaled.x, (PointI::value_type)scaled.y };
        return pixelPoint;
    }

    void Drawer2D::resize(const RectD& box) {
        if (img->empty()) {
            sizeBox = box;
            resizeImage();
            return ;
        }

        const RectD oldBox = sizeBox;
        const bool changed = sizeBox.expand( box );
        if (changed) {
            ImagePtr oldImg;
            oldImg.swap( img );
            resetImage();
            resizeImage();

            const PointD topLeft = PointD{ oldBox.a.x - MARGIN, oldBox.b.y + MARGIN };
            const Image* source = oldImg.get();
            drawImage( topLeft, *source );
        }
    }

    void Drawer2D::resetImage() {
        img.reset( new Image() );
        painter.setImage( img.get() );
    }

    void Drawer2D::resizeImage() {
        const double boxW = sizeBox.width();
        const double boxH = sizeBox.height();
        const size_t w = scale * ( 2 * MARGIN + boxW );
        const size_t h = scale * ( 2 * MARGIN + boxH );
        img->resize(w, h);
    }

} /* namespace tcd */
