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

    /**
     * Class allowing scaling and image resize.
     */
    class ImageBox {

        ImagePtr img;
        RectD sizeBox;
        Image::Pixel backgroundColor;
        double margin;


    public:

        double scale;


        ImageBox(const double scale = 10.0, const double margin = 0.5);

        ImageBox(const double scale = 10.0, const Image::Pixel& backgroundColor = Image::TRANSPARENT, const double margin = 0.5);

        void reset();

        const Image& image() const {
            return *(img.get());
        }

        Image& image() {
            return *(img.get());
        }

        ImagePtr takeImage();

        void save(const std::string& path) {
            img->save(path);
        }

        void setBackground(const std::string& color) {
            backgroundColor = Image::convertColor( color );
        }

        void setBackground(const Image::Pixel& color) {
            backgroundColor = color;
        }


        PointI transformCoords(const double x, const double y) const;

        void resize(const RectD& box);

        /// returns true if image instance changed, otherwise false
        bool expand(const RectD& box);


    protected:

        void resizeImage();

        void fillBackground(const RectI& gap);

    };


    /// ==========================================================


    class Drawer2DBase {
    public:

        ImageBox imgBox;
        Painter painter;


        Drawer2DBase(const double scale = 10.0, const double margin = 0.5): imgBox(scale, margin), painter( imgBox.image() ) {
        }

        const Image& image() const {
            return imgBox.image();
        }

        Image& image() {
            return imgBox.image();
        }

        ImagePtr takeImage() {
            ImagePtr oldImage = imgBox.takeImage();
            Image& img = imgBox.image();
            painter.setImage( img );
            return oldImage;
        }

        void setBackground(const std::string& color) {
            imgBox.setBackground( color );
        }

        void setBackground(const Image::Pixel& color) {
            imgBox.setBackground( color );
        }

        void resizeImage(const double left, const double bottom, const double right, const double top) {
            const RectD bbox = RectD::minmax( left, bottom, right, top );
            imgBox.resize( bbox );
        }

        void extendImage(const RectD& box) {
            if (imgBox.expand(box)) {
                Image& img = imgBox.image();
                painter.setImage( img );
            }
        }

    };


    template <typename PointT>
    class Drawer2D: public Drawer2DBase {
    public:

        bool autoResize;


        Drawer2D(const double scale = 10.0, const double margin = 0.5): Drawer2DBase(scale, margin), autoResize(true) {
        }

        using Drawer2DBase::resizeImage;

        void resizeImage(const PointT& bottomLeft, const PointT& topRight) {
            resizeImage( bottomLeft[0], bottomLeft[1], topRight[0], topRight[1] );
        }

        void drawImage(const PointT& topLeftPoint, const Image& source) {
            const PointI from = imgBox.transformCoords(topLeftPoint[0], topLeftPoint[1]);
            painter.drawImage( from, source );
        }

        void drawLine(const PointT& fromPoint, const PointT& toPoint, const double width, const std::string& color) {
            if (autoResize) {
                expand( fromPoint, toPoint, width / 2.0 );
            }

            const PointI from = imgBox.transformCoords( fromPoint[0], fromPoint[1] );
            const PointI to   = imgBox.transformCoords( toPoint[0], toPoint[1] );
            const uint32_t w  = width * imgBox.scale;
            painter.drawLine( from, to, w, color );
        }

        void drawArc(const PointT& center, const double radius, const double width, const double startAngle, const double range, const std::string& color) {
            if (autoResize) {
                if (std::abs(range) < 2*M_PI) {
                    double minAngle = 0.0;
                    double maxAngle = 0.0;
                    normalizeAngleRange(startAngle, range, minAngle, maxAngle);

                    const double innerRadius = std::max( radius - width / 2.0, 0.0);
                    const double outerRadius = radius + width / 2.0;

                    const PointD centerPoint{ center[0], center[1] };

                    RectD bbox;
                    {
                        const PointD start = centerPoint + rotateSenseVector<PointD>( minAngle ) * radius;
                        bbox = RectD( start );
                    }
                    bbox.expand( centerPoint, innerRadius, outerRadius, minAngle );
                    bbox.expand( centerPoint, innerRadius, outerRadius, maxAngle );

                    std::size_t i    = minAngle / M_PI_2 + 1;
                    std::size_t iMax = maxAngle / M_PI_2 + 1;
                    for(; i<iMax; ++i) {
                        const double angle = M_PI_2 * i;
                        bbox.expand( centerPoint, innerRadius, outerRadius, angle );
                    }

                    extendImage( bbox );
                } else
                {
                    expand( center, radius + width / 2.0 );
                }
            }

            const PointI point = imgBox.transformCoords( center[0], center[1] );
            const uint32_t rad = radius * imgBox.scale;
            const uint32_t w   = width * imgBox.scale;
            const double angle = -normalizeAngle( startAngle );
            painter.drawArc( point, rad, w, angle, -range, color );
        }

        void drawRing(const PointT& center, const double radius, const double width, const std::string& color) {
            if (autoResize) {
                expand( center, radius + width / 2.0 );
            }

            const PointI point = imgBox.transformCoords( center[0], center[1] );
            const uint32_t rad = radius * imgBox.scale;
            const uint32_t w   = width * imgBox.scale;
            painter.drawRing( point, rad, w, color );
        }

        void fillRect(const PointT& bottomLeftPoint, const double width, const double height, const std::string& color) {
            const Image::Pixel pixColor = Image::convertColor( color );
            fillRect(bottomLeftPoint, width, height, pixColor);
        }

        void fillRect(const PointT& center, const double width, const double height, const double angle, const Image::Pixel& color) {
            const PointD centerPoint{ center[0], center[1] };

            const PointD topLeft     = centerPoint + rotateVector( PointD( -width/2.0,  height / 2.0 ), angle );
            const PointD topRight    = centerPoint + rotateVector( PointD(  width/2.0,  height / 2.0 ), angle );
            const PointD bottomRight = centerPoint + rotateVector( PointD(  width/2.0, -height / 2.0 ), angle );
            const PointD bottomLeft  = centerPoint + rotateVector( PointD( -width/2.0, -height / 2.0 ), angle );

            if (autoResize) {
                RectD bbox = RectD::minmax(topLeft, topRight);
                bbox.expand(bottomRight);
                bbox.expand(bottomLeft);
                extendImage( bbox );
            }

            const PointI a = imgBox.transformCoords( topLeft[0],     topLeft[1] );
            const PointI b = imgBox.transformCoords( topRight[0],    topRight[1] );
            const PointI c = imgBox.transformCoords( bottomRight[0], bottomRight[1] );
            const PointI d = imgBox.transformCoords( bottomLeft[0],  bottomLeft[1] );
            painter.fillRect( a, b, c, d, color );
        }

        void fillRect(const PointT& bottomLeft, const double width, const double height, const Image::Pixel& color) {
            if (autoResize) {
                const PointT topRight = bottomLeft + PointT(width, height);
                expand( bottomLeft, topRight );
            }

            const PointT topLeft = bottomLeft + PointT(0.0, height);
            const PointI point = imgBox.transformCoords( topLeft[0], topLeft[1] );
            const uint32_t w = width * imgBox.scale;
            const uint32_t h = height * imgBox.scale;
            painter.fillRect( point, w, h, color );
        }

        void fillCircle(const PointT& center, const double radius, const std::string& color) {
            if (autoResize) {
                expand(center, radius);
            }

            const PointI point = imgBox.transformCoords( center[0], center[1] );
            const uint32_t rad = radius * imgBox.scale;
            painter.fillCircle( point, rad, color );
        }


    protected:

        void expand(const PointT& center, const double radius) {
            const PointD centerPoint{ center[0], center[1] };
            RectD box( centerPoint );
            box.expand(radius);
            extendImage(box);
        }

        void expand(const PointT& bottomLeft, const PointT& topRight) {
            const RectD box = RectD::minmax( bottomLeft, topRight );
            extendImage(box);
        }

        void expand(const PointT& fromPoint, const PointT& toPoint, const double radius) {
            RectD box = RectD::minmax( fromPoint, toPoint );
            box.expand( radius );
            extendImage(box);
        }

    };


    typedef Drawer2D<PointD> Drawer2DD;


} /* namespace imgdraw2d */


#endif /* IMGDRAW2D_INCLUDE_DRAWER2D_H_ */
