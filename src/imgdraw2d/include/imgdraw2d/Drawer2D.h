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

    class Drawer2DBase {

        static const double MARGIN;


    protected:

        ImagePtr img;
        Painter painter;
        RectD sizeBox;
        double scale;

        Image::Pixel backgroundColor;


    public:

        Drawer2DBase(const double scale = 10.0);

        Drawer2DBase(const RectD& size, const double scale = 10.0, const Image::Pixel& backgroundColor = Image::TRANSPARENT);

        const Image& image() const {
            return *(img.get());
        }

        Image& image() {
            return *(img.get());
        }

        void save(const std::string& path) {
            img->save(path);
        }

        void setBackground(const std::string& color) {
            backgroundColor = Image::convertColor( color );
        }

        void setBackground(const Image::Pixel& color) {
            backgroundColor = color;
        }

        void drawImage(const double top, const double left, const Image& source) {
            const PointI from = transformCoords(top, left);
            painter.drawImage( from, source );
        }


        PointI transformCoords(const double x, const double y) const;


    protected:

        void resize(const RectD& box);

        void resetImage();

        void resizeImage();

    };


    /// ==========================================================


    template <typename PointT>
    class Drawer2D: public Drawer2DBase {
    public:

        Drawer2D(const double scale = 10.0): Drawer2DBase(scale) {
        }

        Drawer2D(const RectD& size, const double scale = 10.0, const Image::Pixel& backgroundColor = Image::TRANSPARENT):
            Drawer2DBase(size, scale, backgroundColor)
        {
        }

        using Drawer2DBase::drawImage;

        void drawImage(const PointT& topLeftPoint, const Image& source) {
            drawImage( topLeftPoint[0], topLeftPoint[1], source );
        }

        void drawLine(const PointT& fromPoint, const PointT& toPoint, const double width, const std::string& color) {
            expand( fromPoint, toPoint, width / 2.0 );

            const PointI from = transformPoint(fromPoint);
            const PointI to = transformPoint(toPoint);
            const uint32_t w = width * scale;
            painter.drawLine( from, to, w, color );
        }

        void drawArc(const PointT& center, const double radius, const double width, const double startAngle, const double range, const std::string& color) {
            expand( center, radius + width / 2.0 );

            const PointI point = transformPoint(center);
            const uint32_t rad = radius * scale;
            const uint32_t w = width * scale;
            const double angle = -normalizeAngle( startAngle );
            painter.drawArc( point, rad, w, angle, -range, color );
        }

        void fillRect(const PointT& bottomLeftPoint, const double width, const double height, const std::string& color) {
            const Image::Pixel pixColor = Image::convertColor( color );
            fillRect(bottomLeftPoint, width, height, pixColor);
        }

        void fillRect(const PointT& bottomLeft, const double width, const double height, const Image::Pixel& color) {
            const PointT topRight = bottomLeft + PointT(width, height);
            expand( bottomLeft, topRight );

            const PointD topLeft = bottomLeft + PointT(0.0, height);
            const PointI point = transformPoint(topLeft);
            const uint32_t w = width * scale;
            const uint32_t h = height * scale;
            painter.fillRect( point, w, h, color );
        }

        void fillCircle(const PointT& center, const double radius, const std::string& color) {
            expand(center, radius);

            const PointI point = transformPoint(center);
            const uint32_t rad = radius * scale;
            painter.fillCircle( point, rad, color );
        }


        PointI transformPoint(const PointT& point) const {
            return transformCoords( point[0], point[1] );
        }


    protected:

        void expand(const PointT& center, const double radius) {
            const PointD centerPoint{ center[0], center[1] };
            RectD box( centerPoint );
            box.expand(radius);
            resize(box);
        }

        void expand(const PointT& bottomLeft, const PointT& topRight) {
            const RectD box = RectD::minmax( bottomLeft, topRight );
            resize(box);
        }

        void expand(const PointT& fromPoint, const PointT& toPoint, const double radius) {
            RectD box = RectD::minmax( fromPoint, toPoint );
            box.expand( radius );
            resize(box);
        }

    };


    typedef Drawer2D<PointD> Drawer2DD;


} /* namespace imgdraw2d */


#endif /* IMGDRAW2D_INCLUDE_DRAWER2D_H_ */
