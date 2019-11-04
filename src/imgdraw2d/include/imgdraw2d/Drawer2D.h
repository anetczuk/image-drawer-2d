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

        void resizeImage(const double radius) {
            const RectD bbox( -radius, -radius, radius, radius );
            imgBox.resize( bbox );
        }

        void resizeImage(const double minRadius, const double maxRadius) {
            const RectD bbox = RectD::minmax( minRadius, minRadius, maxRadius, maxRadius );
            imgBox.resize( bbox );
        }

        void resizeImage(const double left, const double bottom, const double right, const double top) {
            const RectD bbox = RectD::minmax( left, bottom, right, top );
            imgBox.resize( bbox );
        }

        void resizeImage(const RectD& bbox) {
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

        Image::Pixel drawColor;
        bool autoResize;


        Drawer2D(const double scale = 10.0, const double margin = 0.5):
            Drawer2DBase(scale, margin),
            drawColor( Image::BLACK ), autoResize(true)
        {
        }

        void setDrawColor(const Image::Pixel& color) {
            drawColor = color;
        }

        void setDrawColor(const std::string& color) {
            drawColor = Image::convertColor(color);
        }

        using Drawer2DBase::resizeImage;

        void resizeImage(const PointT& bottomLeft, const PointT& topRight) {
            resizeImage( bottomLeft[0], bottomLeft[1], topRight[0], topRight[1] );
        }

        void drawImage(const PointT& topLeftPoint, const Image& source) {
            const PointI from = imgBox.transformCoords(topLeftPoint[0], topLeftPoint[1]);
            painter.drawImage( from, source );
        }

        void drawLine(const PointT& fromPoint, const PointT& toPoint, const double width) {
            if (autoResize) {
                expand( fromPoint, toPoint, width / 2.0 );
            }

            const PointI from = imgBox.transformCoords( fromPoint[0], fromPoint[1] );
            const PointI to   = imgBox.transformCoords( toPoint[0], toPoint[1] );
            const uint32_t w  = width * imgBox.scale;
            painter.drawLine( from, to, w, drawColor );
        }

        void fillRect(const PointT& center, const double width, const double height, const double angle) {
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
            painter.fillRect( a, b, c, d, drawColor );
        }

        void fillRect(const PointT& bottomLeft, const double width, const double height) {
            if (autoResize) {
                const PointT topRight = bottomLeft + PointT(width, height);
                expand( bottomLeft, topRight );
            }

            const PointT topLeft = bottomLeft + PointT(0.0, height);
            const PointI point = imgBox.transformCoords( topLeft[0], topLeft[1] );
            const uint32_t w = width * imgBox.scale;
            const uint32_t h = height * imgBox.scale;
            painter.fillRect( point, w, h, drawColor );
        }

        void fillCircle(const PointT& center, const double radius) {
            if (autoResize) {
                expand(center, radius);
            }

            const PointI point = imgBox.transformCoords( center[0], center[1] );
            const uint32_t rad = radius * imgBox.scale;
            painter.fillCircle( point, rad, drawColor );
        }

        void drawRing(const PointT& center, const double radius, const double width) {
            if (autoResize) {
                expand( center, radius + width / 2.0 );
            }

            const PointI point = imgBox.transformCoords( center[0], center[1] );
            const uint32_t rad = radius * imgBox.scale;
            const uint32_t w   = width * imgBox.scale;
            painter.drawRing( point, rad, w, drawColor );
        }

        void drawArc(const PointT& center, const double radius, const double width, const double startAngle, const double range) {
            if (autoResize) {
                if (std::abs(range) < 2*M_PI) {
                    expand( center, radius, width, startAngle, range );
                } else
                {
                    expand( center, radius + width / 2.0 );
                }
            }

            const PointI point = imgBox.transformCoords( center[0], center[1] );
            const uint32_t rad = radius * imgBox.scale;
            const uint32_t w   = width * imgBox.scale;
            const double angle = -normalizeAngle( startAngle );
            painter.drawArc( point, rad, w, angle, -range, drawColor );
        }


        /// =========================================================================


        /// L -- distance
        /// R -- radius
        /// A -- flatness or homothetic parameter
        /// r*l = Ri * Li = A^2
        /// A = sqrt(R*L)
        ///
        /// alpha -- direction angle
        /// da = (Li)^2/(2*A^2) = Li / (2*Ri)
        ///
        /// flatness = 1 / a
        void drawClothoid(const PointT& start, const double startHeading, const double width, const double curveLength, const double flatness) {
            drawClothoid( start, startHeading, width, 0.0, curveLength, flatness );
        }

        void drawClothoid(const PointT& start, const double startHeading, const double width, const double curveLengthStart, const double curveLengthEnd, const double flatness) {
            const double lengthDiff = curveLengthEnd - curveLengthStart;
            double ds = 0.01;
            if (lengthDiff < 0.0) {
                ds *= -1.0;
            }

            const double flatnessStart = curveLengthStart / flatness;
            const double flatnessLength = lengthDiff / flatness;
            const std::size_t steps = std::abs(flatnessLength / ds);
            double dsXFactor = ds * flatness;
            if (flatness < 0.0) dsXFactor *= -1.0;
            const double dsYFactor = ds * flatness;

            PointT prev = start;
            double s = flatnessStart;                         /// position on curvature

            double headingOffset = startHeading - s*s;

            PointT current;
            for(std::size_t i=0; i<steps; ++i) {
                /// Fresnel integral
                const double dx = cos(s*s + headingOffset) * dsXFactor;
                const double dy = sin(s*s + headingOffset) * dsYFactor;
                s += ds;

                current = PointT( prev[0] + dx, prev[1] + dy );
                drawLine( prev, current, width );
                prev = current;
            }
        }

        void drawClothoidLR(const PointT& start, const double startHeading, const double width, const double curveLength, const double radius) {
            /// 2*R*L = 1 / a^2
            /// a^2 = 1 / ( 2*R*L )
            /// a = 1 / sqrt( 2*R*L )
            /// 1 / a = sqrt( 2*R*L )

            double flatness = 0.0;                  /// 1 / a
            if (curveLength < 0.0) {
                if (radius < 0.0)
                    flatness = -std::sqrt( 2.0 * -curveLength * -radius );
                else
                    flatness =  std::sqrt( 2.0 * -curveLength *  radius );
            } else {
                if (radius < 0.0)
                    flatness = -std::sqrt( 2.0 *  curveLength * -radius );
                else
                    flatness =  std::sqrt( 2.0 *  curveLength *  radius );
            }

            drawClothoid( start, startHeading, width, curveLength, flatness );
        }

        void drawClothoidLA(const PointT& start, const double startHeading, const double width, const double curveLength, const double angle) {
            const double radius = 0.5 * curveLength / angle;
            drawClothoidLR( start, startHeading, width, curveLength, radius );
        }

        void drawClothoidRA(const PointT& start, const double startHeading, const double width, const double radius, const double angle) {
            const double distance = radius * angle * 2;
            drawClothoidLR( start, startHeading, width, distance, radius );
        }


        /// =========================================================================


    protected:

        void expand(const PointT& center, const double radius) {
            const PointD centerPoint{ center[0], center[1] };
            RectD box( centerPoint );
            box.expand(radius);
            extendImage(box);
        }

        void expand(const PointT& center, const double radius, const double width, const double startAngle, const double range) {
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
