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

#include "imgdraw2d/Drawer2D.h"


namespace imgdraw2d {

    const double Drawer2DBase::MARGIN = 0.5;


    Drawer2DBase::Drawer2DBase(const double scale):
            img( new Image() ), painter( *img ),
            sizeBox{ {0.0, 0.0}, {0.0, 0.0} }, scale(scale),
            backgroundColor(0, 0, 0, 0)                             /// transparent color
    {
    }

    Drawer2DBase::Drawer2DBase(const RectD& size, const double scale, const Image::Pixel& backgroundColor): Drawer2DBase(scale) {
        setBackground( backgroundColor );
        resize(size);
    }

    PointI Drawer2DBase::transformCoords(const double x, const double y) const {
        ///flip y coord
        PointD relative{ x - sizeBox.a.x, sizeBox.b.y - y };
        relative.x += MARGIN;
        relative.y += MARGIN;
        PointD scaled = relative * scale;
        const PointI pixelPoint{ (PointI::value_type)scaled.x, (PointI::value_type)scaled.y };
        return pixelPoint;
    }

    void Drawer2DBase::resize(const RectD& box) {
        if (img->empty()) {
            sizeBox = box;
            resizeImage();
            img->fill( backgroundColor );
            return ;
        }

        const RectD oldBox = sizeBox;
        const bool changed = sizeBox.expand( box );
        if (changed) {
            ImagePtr oldImg;
            oldImg.swap( img );
            resetImage();
            resizeImage();
            img->fill( backgroundColor );

            const double top = oldBox.a.x - MARGIN;
            const double left = oldBox.b.y + MARGIN;
            const Image* source = oldImg.get();
            drawImage( top, left, *source );
        }
    }

    void Drawer2DBase::resetImage() {
        img.reset( new Image() );
        painter.setImage( img.get() );
    }

    void Drawer2DBase::resizeImage() {
        const double boxW = sizeBox.width();
        const double boxH = sizeBox.height();
        const std::size_t w = scale * ( 2 * MARGIN + boxW );
        const std::size_t h = scale * ( 2 * MARGIN + boxH );
        img->resize(w, h);
    }

} /* namespace imgdraw2d */
