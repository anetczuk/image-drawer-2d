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

#include "imgdraw2d/Painter.h"

#include <cmath>


namespace imgdraw2d {

    inline uint32_t brightness(const Image::Pixel& pix) {
        return pix.red + pix.green + pix.blue;
    }

    inline Image::PixByte difference(const Image::PixByte& value, const Image::PixByte& subtractor) {
        const int valA = value;
        const int valB = subtractor;
        const int diff = valA - valB;
        if (diff < 0) {
            return 256 - diff;
        } else {
            return diff;
        }
    }

    inline Image::Pixel difference(const Image::Pixel& value, const Image::Pixel& subtractor) {
        return Image::Pixel(    difference(value.red   , subtractor.red),
                                difference(value.green , subtractor.green),
                                difference(value.blue  , subtractor.blue),
                                value.alpha
                            );
    }

    inline int64_t udiff(const int64_t value, const int64_t subtractor) {
        if (subtractor > value)
            return 0;
        return value - subtractor;
    }


    /// ======================================================================================


    class DestinationModeWorker: public painter::ModeWorker {
    public:

        DestinationModeWorker(Image* image): ModeWorker(image) {
        }

        void drawImage(const PointI& point, const Image& source) override {
            img->pasteImage(point.x, point.y, source);
        }

        void drawLine(const PointI& fromPoint, const PointI& toPoint, const uint32_t width, const std::string& color) override {
            const Image::Pixel pixColor = Image::convertColor( color );

            const PointI lineVector = toPoint - fromPoint;
            const PointI orthoVector = lineVector.ortho();
            const RayI orthoRay( orthoVector );

            const uint32_t radius = width / 2;
            RectI box = RectI::minmax(fromPoint, toPoint);
            box.expand( radius );

            const Linear parallelLine = Linear::createFromParallel(lineVector);

            for( int64_t j=box.a.y; j<=box.b.y; ++j ) {
                Image::RawImage::row_access tgtRow = img->row( j );
                for( int64_t i=box.a.x; i<=box.b.x; ++i ) {
                    const PointI currVector = PointI{i, j} - fromPoint;
                    const int64_t side1 = orthoRay.side( currVector );
                    if (side1 < 0) {
                        continue;
                    }
                    const PointI toVector = currVector - lineVector;
                    const int64_t side2 = orthoRay.side( toVector );
                    if (side2 > 0) {
                        continue;
                    }
                    const double dist = parallelLine.distance( currVector );
                    if (dist > radius) {
                        continue;
                    }
                    tgtRow[ i ] = pixColor;
                }
            }
        }

        void fillRect(const PointI& point, const uint32_t width, const uint32_t height, const Image::Pixel& pixColor) override {
            assert( point.x >= 0 );
            assert( point.y >= 0 );
            img->fillRect( point.x, point.y, point.x + width, point.y + height, pixColor );
        }

        void fillRect(const PointI& topLeft, const PointI& topRight, const PointI& bottomRight, const PointI& bottomLeft, const Image::Pixel& pixColor) override {
            const Linear line1 = Linear::createFromPoints( topLeft, topRight );
            const Linear line2 = Linear::createFromPoints( topRight, bottomRight );
            const Linear line3 = Linear::createFromPoints( bottomRight, bottomLeft );
            const Linear line4 = Linear::createFromPoints( bottomLeft, topLeft );

            RectI bbox = RectI::minmax(topLeft, topRight);
            bbox.expand(bottomRight);
            bbox.expand(bottomLeft);

            for( int64_t j = bbox.a.y; j<=bbox.b.y; ++j ) {
                Image::RawImage::row_access tgtRow = img->row( j );
                for( int64_t i = bbox.a.x; i<=bbox.b.x; ++i ) {
                    if (line1.pointSide( i, j ) < 0) continue;
                    if (line2.pointSide( i, j ) < 0) continue;
                    if (line3.pointSide( i, j ) < 0) continue;
                    if (line4.pointSide( i, j ) < 0) continue;
                    tgtRow[ i ] = pixColor;
                }
            }
        }

        RectI getBBoxOnCircle(const PointI& center, const uint32_t radius) {
            const int64_t w = img->width();
            const int64_t h = img->height();
            const int64_t startW = udiff( center.x, radius );
            const int64_t startH = udiff( center.y, radius );
            const int64_t endW = std::min( w-1, center.x + radius );
            const int64_t endH = std::min( h-1, center.y + radius );
            return RectI( PointI(startW, startH), PointI(endW, endH) );
        }

        RectI getBBoxInCircle(const PointI& center, const uint32_t radius) {
            const uint32_t squareWidth = std::sqrt( 2 ) * radius;
            const uint32_t side = squareWidth / 2;
            const int64_t w = img->width();
            const int64_t h = img->height();
            const int64_t startW = udiff( center.x, side );
            const int64_t startH = udiff( center.y, side );
            const int64_t endW = std::min( w-1, center.x + side );
            const int64_t endH = std::min( h-1, center.y + side );
            return RectI( PointI(startW, startH), PointI(endW, endH) );
        }

        void fillCircle(const PointI& center, const uint32_t radius, const std::string& color) override {
            assert( center.x >= 0 );
            assert( center.y >= 0 );

            const Image::Pixel pixColor = Image::convertColor( color );

            const RectI outerBox = getBBoxOnCircle( center, radius );
            const RectI innerBox = getBBoxInCircle( center, radius );

            /// fill edges
            CircleCondition circle( radius );
            drawCircleEdges(center, outerBox, innerBox, pixColor, circle );

            /// fill inner square
            img->fillRect( innerBox.a.x, innerBox.a.y, innerBox.b.x, innerBox.b.y, pixColor );
        }

        void drawRing(const PointI& center, const uint32_t radius, const uint32_t width, const std::string& color) override {
            const uint32_t maxRadius = radius + std::max( width / 2, (uint32_t) 1 );      /// draw at least 1px width
            const uint32_t minRadius = udiff( radius, width / 2 );
            if (minRadius == 0) {
                fillCircle( center, maxRadius, color );
                return ;
            }

            assert( center.x >= 0 );
            assert( center.y >= 0 );

            const Image::Pixel pixColor = Image::convertColor( color );

            const RectI outerBox  = getBBoxOnCircle( center, maxRadius );
            const RectI middleBox = getBBoxInCircle( center, maxRadius );
            const RectI innerBox  = getBBoxInCircle( center, minRadius );

            /// fill edges
            RingCondition circle( minRadius, maxRadius );
            drawCircleEdges(center, outerBox, middleBox, pixColor, circle );
            drawRectEdges(center, middleBox, innerBox, pixColor, circle );
        }

        void drawArc(const PointI& center, const uint32_t radius, const uint32_t width, const double startAngle, const double range, const std::string& color) override {
            if ( std::abs(range) >= 2 * M_PI ) {
                drawRing( center, radius, width, color );
                return ;
            }

            double minAngle = 0.0;
            double maxAngle = 0.0;
            normalizeAngleRange(startAngle, range, minAngle, maxAngle);

            const bool sum = ( std::abs(range) > M_PI );

            const Image::Pixel pixColor = Image::convertColor( color );

            //const uint32_t maxRadius = radius + width / 2;
            const uint32_t maxRadius = radius + std::max( width / 2, (uint32_t) 1 );      /// draw at least 1px width
            const uint32_t minRadius = udiff( radius, width / 2 );

            const PointI fromVector = rotateVector( PointI(1000, 0), minAngle );
            const PointI toVector   = rotateVector( PointI(1000, 0), maxAngle );

            const RayI fromRay( fromVector );
            const RayI toRay( toVector );

            const RectI outerBox  = getBBoxOnCircle( center, maxRadius );
            const RectI middleBox = getBBoxInCircle( center, maxRadius );
            const RectI innerBox  = getBBoxInCircle( center, minRadius );

            /// fill edges
            ArcCondition circle( minRadius, maxRadius, fromRay, toRay, sum );
            drawCircleEdges(center, outerBox, middleBox, pixColor, circle );
            drawRectEdges(center, middleBox, innerBox, pixColor, circle );
        }


        /// ===================================================================================


        struct CircleCondition {
            uint32_t rSquare;

            CircleCondition(const uint32_t radius): rSquare(radius * radius) {
            }

            bool operator()(const int64_t x, const int64_t y) {
                const int64_t distSquare = x * x + y * y;
                return !( distSquare > rSquare );
            }
        };

        struct RingCondition {
            uint32_t minSquare;
            uint32_t maxSquare;

            RingCondition(const uint32_t minRadius, const uint32_t maxRadius):
                minSquare(minRadius * minRadius), maxSquare(maxRadius * maxRadius)
            {
            }

            bool operator()(const int64_t x, const int64_t y) {
                const int64_t distSquare = x * x + y * y;
                if ( distSquare > maxSquare )
                    return false;
                if ( distSquare < minSquare )
                    return false;
                return true;
            }
        };

        struct ArcCondition {
            uint32_t minSquare;
            uint32_t maxSquare;
            const RayI& fromRay;
            const RayI& toRay;
            bool sum;

            ArcCondition(const uint32_t minRadius, const uint32_t maxRadius, const RayI& fromRay, const RayI& toRay, const bool sum):
                minSquare(minRadius * minRadius), maxSquare(maxRadius * maxRadius),
                fromRay(fromRay), toRay(toRay), sum(sum)
            {
            }

            bool operator()(const int64_t x, const int64_t y) {
                const int64_t distSquare = x * x + y * y;
                if ( distSquare > maxSquare )
                    return false;
                if ( distSquare < minSquare )
                    return false;

                if (sum) {
                    const double fromSide = fromRay.side( x, y );
                    if ( fromSide < 0.0 ) {
                        const double toSide = toRay.side( x, y );
                        if ( toSide > 0.0 ) {
                            return false;
                        }
                    }
                } else {
                    const double fromSide = fromRay.side( x, y );
                    if (fromSide <= 0.0) {
                        return false;
                    }
                    const double toSide = toRay.side( x, y );
                    if (toSide >= 0.0) {
                        return false;
                    }
                }

                return true;
            }
        };


        template <typename Operator>
        void drawRectEdges(const PointI& center, const RectI& outerBox, const RectI& innerBox, const Image::Pixel& pixColor, Operator& op) {
            /// top
            for( int64_t j = outerBox.a.y; j<=innerBox.a.y; ++j ) {
                const int64_t diffY = j - center.y;
                Image::RawImage::row_access tgtRow = img->row( j );
                for( int64_t i = outerBox.a.x; i<=outerBox.b.x; ++i ) {
                    const int64_t diffX = i - center.x;
                    if ( op(diffX, diffY) ) {
                        tgtRow[ i ] = pixColor;
                    }
                }
            }

            /// left and right
            for( int64_t j = innerBox.a.y; j<=innerBox.b.y; ++j ) {
                const int64_t diffY = j - center.y;
                Image::RawImage::row_access tgtRow = img->row( j );

                /// left
                for( int64_t i = outerBox.a.x; i<=innerBox.a.x; ++i ) {
                    const int64_t diffX = i - center.x;
                    if ( op(diffX, diffY) ) {
                        tgtRow[ i ] = pixColor;
                    }
                }

                /// right
                for( int64_t i = innerBox.b.x; i<=outerBox.b.x; ++i ) {
                    const int64_t diffX = i - center.x;
                    if ( op(diffX, diffY) ) {
                        tgtRow[ i ] = pixColor;
                    }
                }
            }

            /// bottom
            for( int64_t j = innerBox.b.y; j<=outerBox.b.y; ++j ) {
                const int64_t diffY = j - center.y;
                Image::RawImage::row_access tgtRow = img->row( j );
                for( int64_t i = outerBox.a.x; i<=outerBox.b.x; ++i ) {
                    const int64_t diffX = i - center.x;
                    if ( op(diffX, diffY) ) {
                        tgtRow[ i ] = pixColor;
                    }
                }
            }
        }

        template <typename Operator>
        void drawCircleEdges(const PointI& center, const RectI& outerBox, const RectI& innerBox, const Image::Pixel& pixColor, Operator& op) {
            /// top
            for( int64_t j = outerBox.a.y; j<=innerBox.a.y; ++j ) {
                const int64_t diffY = j - center.y;
                Image::RawImage::row_access tgtRow = img->row( j );
                for( int64_t i = innerBox.a.x; i<=innerBox.b.x; ++i ) {
                    const int64_t diffX = i - center.x;
                    if ( op(diffX, diffY) ) {
                        tgtRow[ i ] = pixColor;
                    }
                }
            }

            /// left and right
            for( int64_t j = innerBox.a.y; j<=innerBox.b.y; ++j ) {
                const int64_t diffY = j - center.y;
                Image::RawImage::row_access tgtRow = img->row( j );

                /// left
                for( int64_t i = outerBox.a.x; i<=innerBox.a.x; ++i ) {
                    const int64_t diffX = i - center.x;
                    if ( op(diffX, diffY) ) {
                        tgtRow[ i ] = pixColor;
                    }
                }

                /// right
                for( int64_t i = innerBox.b.x; i<=outerBox.b.x; ++i ) {
                    const int64_t diffX = i - center.x;
                    if ( op(diffX, diffY) ) {
                        tgtRow[ i ] = pixColor;
                    }
                }
            }

            /// bottom
            for( int64_t j = innerBox.b.y; j<=outerBox.b.y; ++j ) {
                const int64_t diffY = j - center.y;
                Image::RawImage::row_access tgtRow = img->row( j );
                for( int64_t i = innerBox.a.x; i<=innerBox.b.x; ++i ) {
                    const int64_t diffX = i - center.x;
                    if ( op(diffX, diffY) ) {
                        tgtRow[ i ] = pixColor;
                    }
                }
            }
        }

    };


    /// =================================================================================================


    class DifferenceModeWorker: public painter::ModeWorker {
    public:

        DifferenceModeWorker(Image* image): ModeWorker(image) {
        }

        void drawImage(const PointI& point, const Image& source) override {
            assert( point.x >= 0 );
            assert( point.y >= 0 );
            const int64_t x = point.x;
            const int64_t y = point.y;
            const int64_t w = img->width();
            const int64_t h = img->height();
            const int64_t endW = std::min(w, x + source.width() );
            const int64_t endH = std::min(h, y + source.height() );
            for( int64_t j = y; j<endH; ++j ) {
                Image::RawImage::row_const_access srcRow = source.row( j - y );
                Image::RawImage::row_access tgtRow = img->row(j);
                for( int64_t i = x; i<endW; ++i ) {
                    const Image::Pixel& src = srcRow[ i - x ];
                    const Image::Pixel& orig = tgtRow[ i ];
                    tgtRow[ i ] = diffPixels(orig, src);
                }
            }
        }

        void drawLine(const PointI& /*fromPoint*/, const PointI& /*toPoint*/, const uint32_t /*width*/, const std::string& /*color*/) override {
            //TODO: implement
            throw std::runtime_error("drawLine not implemented");
        }

        void fillRect(const PointI& point, const uint32_t width, const uint32_t height, const Image::Pixel& pixColor) override {
            assert( point.x >= 0 );
            assert( point.y >= 0 );
            const int64_t x = point.x;
            const int64_t y = point.y;
            const int64_t w = img->width();
            const int64_t h = img->height();
            const int64_t endW = std::min(w, x + width );
            const int64_t endH = std::min(h, y + height );
            for( int64_t j = y; j<endH; ++j ) {
                Image::RawImage::row_access tgtRow = img->row(j);
                for( int64_t i = x; i<endW; ++i ) {
                    const Image::Pixel& orig = tgtRow[ i ];
                    tgtRow[ i ] = diffPixels(orig, pixColor);
                }
            }
        }

        void fillRect(const PointI& /*topLeft*/, const PointI& /*topRight*/, const PointI& /*bottomRight*/, const PointI& /*bottomLeft*/, const Image::Pixel& /*pixColor*/) override {
            //TODO: implement
            throw std::runtime_error("fillRect not implemented");
        }

        void drawArc(const PointI& /*center*/, const uint32_t /*radius*/, const uint32_t /*width*/, const double /*startAngle*/, const double /*range*/, const std::string& /*color*/) override {
            //TODO: implement
            throw std::runtime_error("drawArc not implemented");
        }

        void drawRing(const PointI& /*center*/, const uint32_t /*radius*/, const uint32_t /*width*/, const std::string& /*color*/) override {
            //TODO: implement
            throw std::runtime_error("drawRing not implemented");
        }

        void fillCircle(const PointI& /*center*/, const uint32_t /*radius*/, const std::string& /*color*/) override {
            //TODO: implement
            throw std::runtime_error("fillCircle not implemented");
        }


    private:

        static Image::Pixel diffPixels(const Image::Pixel& pixA, const Image::Pixel& pixB) {
            const uint32_t aLight = brightness(pixA);
            const uint32_t bLight = brightness(pixB);
            if (aLight > bLight) {
                return difference(pixA, pixB);
            } else {
                return difference(pixB, pixA);
            }
        }

    };


    /// ====================================================================================================


    Painter::Painter(Image& image): painter::ModeWorker(&image), mode(CM_DESTINATION), worker(nullptr) {
        setCompositionMode(mode);
    }

    Painter::Painter(Image* image): painter::ModeWorker(image), mode(CM_DESTINATION), worker(nullptr) {
        setCompositionMode(mode);
    }

    void Painter::setCompositionMode(const CompositionMode mode) {
        this->mode = mode;
        switch( mode ) {
        case CM_DESTINATION: {
            worker.reset( new DestinationModeWorker(img) );
            return ;
        }
        case CM_DIFFERENCE: {
            worker.reset( new DifferenceModeWorker(img) );
            return ;
        }
        }
    }

} /* namespace imgdraw2d */
