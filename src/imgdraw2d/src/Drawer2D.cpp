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

    static const double MARGIN = 0.5;


    ImageBox::ImageBox(const double scale):
            img(nullptr),
            sizeBox(),
            backgroundColor(0, 0, 0, 0),                             /// transparent color
            scale(scale)
    {
        reset();
    }

    ImageBox::ImageBox(const RectD& size, const double scale, const Image::Pixel& backgroundColor): ImageBox(scale) {
        setBackground( backgroundColor );
        resize(size);
    }

    void ImageBox::reset() {
        img.reset( new Image() );
        sizeBox = RectD{ {0.0, 0.0}, {0.0, 0.0} };
    }

    ImagePtr ImageBox::takeImage() {
        ImagePtr image;
        img.swap( image );
        reset();
        return image;
    }

    PointI ImageBox::transformCoords(const double x, const double y) const {
        ///flip y coord
        PointD relative{ x - sizeBox.a.x, sizeBox.b.y - y };
        relative.x += MARGIN;
        relative.y += MARGIN;
        PointD scaled = relative * scale;
        const PointI pixelPoint{ (PointI::value_type)scaled.x, (PointI::value_type)scaled.y };
        return pixelPoint;
    }

    bool ImageBox::resize(const RectD& box) {
        if (img->empty()) {
            sizeBox = box;
            resizeImage();
            img->fill( backgroundColor );
            return false;
        }

        const RectD oldBox = sizeBox;
        const bool changed = sizeBox.expand( box );
        if (changed == false) {
            return false;
        }
        ImagePtr oldImg;
        oldImg.swap( img );

        img.reset( new Image() );

        resizeImage();

        const double top = oldBox.a.x - MARGIN;
        const double left = oldBox.b.y + MARGIN;
        const Image* source = oldImg.get();

        const PointI from = transformCoords(top, left);

        if ( backgroundColor != Image::TRANSPARENT ) {
//            img->fill( backgroundColor );
            const PointI oldSize( oldImg->width(), oldImg->height() );
            PointI to = from + oldSize;
            const uint32_t w = img->width();
            const uint32_t h = img->height();
            if (to.x > w) to.x = w;
            if (to.y > h) to.y = h;
            const RectI gap( from, to );
            fillBackground( gap );
        }

        img->pasteImage(from.x, from.y, *source);
        return true;
    }

    void ImageBox::resizeImage() {
        const double boxW = sizeBox.width();
        const double boxH = sizeBox.height();
        const std::size_t w = scale * ( 2 * MARGIN + boxW );
        const std::size_t h = scale * ( 2 * MARGIN + boxH );
        img->resize(w, h);
    }

    void ImageBox::fillBackground( const RectI& gap ) {
        const uint32_t w = img->width();
        const uint32_t h = img->height();
        if (gap.a.y > 0) {
            /// top side
            img->fillRect(0, 0, w, gap.a.y, backgroundColor);
        }
        if (gap.b.y < h) {
            /// bottom side
            img->fillRect(0, gap.b.y, w, h, backgroundColor);
        }

        if (gap.a.x > 0) {
            /// left side
            img->fillRect(0, gap.a.y, gap.a.x, gap.b.y, backgroundColor);
        }
        if (gap.b.x < w) {
            /// right side
            img->fillRect(gap.b.x, gap.a.y, w, gap.b.y, backgroundColor);
        }
    }

} /* namespace imgdraw2d */
