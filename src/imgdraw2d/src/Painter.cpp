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

    /// functor -- functor or function pointer
    template <typename Functor>
    static void linear(const PointI& vector, Functor& functor) {
        if (std::abs(vector.x) > std::abs(vector.y)) {
            int64_t from = 0;
            int64_t to = 0;
            if (vector.x > 0) {
                from = 0;
                to = vector.x;
            } else {
                from = vector.x;
                to = 0;
            }
            for( int64_t i = from; i<=to; ++i ) {
                const int64_t j = linearY(vector, i);
                functor(i, j);
            }
        } else {
            int64_t from = 0;
            int64_t to = 0;
            if (vector.y > 0) {
                from = 0;
                to = vector.y;
            } else {
                from = vector.y;
                to = 0;
            }
            for( int64_t j = from; j<=to; ++j ) {
                const int64_t i = linearX(vector, j);
                functor(i, j);
            }
        }
    }

    struct PixelDrawer {
        Image* img;
        const PointI& startPoint;
        const Image::Pixel& pixColor;

        void operator()(const int64_t x, const int64_t y) {
            img->setPixel( startPoint.x + x, startPoint.y + y, pixColor );
        }
    };


    /// ======================================================================================


    class DestinationModeWorker: public painter::ModeWorker {
    public:

        struct VectorDrawer {
            DestinationModeWorker* worker;
            const PointI& startPoint;
            const PointI& vector;
            const Image::Pixel& pixColor;

            void operator()(const int64_t x, const int64_t y) {
                const PointI currStartPoint{ startPoint.x + x, startPoint.y + y };
                worker->drawVector(currStartPoint, vector, pixColor);
            }
        };


        DestinationModeWorker(Image* image): ModeWorker(image) {
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
            for( int64_t i = x; i<endW; ++i ) {
                for( int64_t j = y; j<endH; ++j ) {
                    const Image::Pixel src = source.pixel( i-x, j-y );
                    img->setPixel( i, j, src );
                }
            }
        }

        void drawLine(const PointI& fromPoint, const PointI& toPoint, const uint32_t radius, const std::string& color) override {
            const Image::Pixel pixColor = Image::convertColor( color );

            const PointI lineVector = toPoint - fromPoint;
            const PointI orthoVector = lineVector.ortho();

            RectI box = RectI::minmax(fromPoint, toPoint);
            box.expand( radius );

            const Linear parallelLine = Linear::createFromParallel(lineVector);

            for( int64_t i=box.a.x; i<=box.b.x; ++i ) {
                for( int64_t j=box.a.y; j<=box.b.y; ++j ) {
                    const PointI currVector = PointI{i, j} - fromPoint;
                    const int64_t side1 = pointPosition(orthoVector, currVector);
                    if (side1 < 0) {
                        continue;
                    }
                    const PointI toVector = currVector - lineVector;
                    const int64_t side2 = pointPosition(orthoVector, toVector);
                    if (side2 > 0) {
                        continue;
                    }
                    const double dist = parallelLine.distance( currVector );
                    if (dist >= radius) {
                        continue;
                    }
                    img->setPixel( i, j, pixColor );
                }
            }
        }

        void drawArc(const PointI& center, const uint32_t radius, const uint32_t width, const double startAngle, const double range, const std::string& color) override {
            assert( center.x >= 0 );
            assert( center.y >= 0 );

            double minAngle = 0.0;
            double maxAngle = 0.0;
            if ( std::abs(range) >= 2 * M_PI ) {
                minAngle = 0.0;
                maxAngle = 2 * M_PI;
            } else if ( range > 0.0 ) {
                minAngle = normalizeAngle(startAngle);
                maxAngle = normalizeAngle(startAngle + range);
            } else {
                minAngle = normalizeAngle(startAngle + range);
                maxAngle = normalizeAngle(startAngle);
            }

            const uint32_t maxRadius = radius + width / 2;
            const uint32_t minRadius = udiff( radius, width / 2 );
            const uint32_t maxRSquare = maxRadius * maxRadius;
            const uint32_t minRSquare = minRadius * minRadius;
            const Image::Pixel pixColor = Image::convertColor( color );
            const int64_t w = img->width();
            const int64_t h = img->height();
            const int64_t startW = udiff( center.x, maxRadius );
            const int64_t startH = udiff( center.y, maxRadius );
            const int64_t endW = std::min( w, center.x + maxRadius );
            const int64_t endH = std::min( h, center.y + maxRadius );
            for( int64_t i = startW; i<endW; ++i ) {
                for( int64_t j = startH; j<endH; ++j ) {
                    const int64_t diffX = i - center.x;
                    const int64_t diffY = j - center.y;

                    const double currAngle = angleFromOX(diffX, diffY);
                    if (isInRange(currAngle, minAngle, maxAngle) == false) {
                        continue ;
                    }

                    const int64_t distSquare = diffX * diffX + diffY * diffY;
                    if ( distSquare < minRSquare ) {
                        continue;
                    }
                    if ( distSquare > maxRSquare ) {
                        continue;
                    }

                    img->setPixel( i, j, pixColor );
                }
            }
        }

        void drawVector(const PointI& startPoint, const PointI& vector, const Image::Pixel& pixColor) {
            PixelDrawer drawer{img, startPoint, pixColor};
            linear( vector, drawer );
        }

        void fillRect(const PointI& point, const uint32_t width, const uint32_t height, const std::string& color) override {
            assert( point.x >= 0 );
            assert( point.y >= 0 );
            const Image::Pixel pixColor = Image::convertColor( color );
            const int64_t x = point.x;
            const int64_t y = point.y;
            const int64_t w = img->width();
            const int64_t h = img->height();
            const int64_t endW = std::min(w, x + width );
            const int64_t endH = std::min(h, y + height );
            for( int64_t i = x; i<endW; ++i ) {
                for( int64_t j = y; j<endH; ++j ) {
                    img->setPixel( i, j, pixColor );
                }
            }
        }

        void fillCircle(const PointI& center, const uint32_t radius, const std::string& color) override {
            assert( center.x >= 0 );
            assert( center.y >= 0 );
            const uint32_t rSquare = radius * radius;
            const Image::Pixel pixColor = Image::convertColor( color );
            const int64_t w = img->width();
            const int64_t h = img->height();
            const int64_t startW = udiff( center.x, radius );
            const int64_t startH = udiff( center.y, radius );
            const int64_t endW = std::min( w, center.x + radius );
            const int64_t endH = std::min( h, center.y + radius );
            for( int64_t i = startW; i<endW; ++i ) {
                for( int64_t j = startH; j<endH; ++j ) {
                    const int64_t diffX = i - center.x;
                    const int64_t diffY = j - center.y;
                    const int64_t distSquare = diffX * diffX + diffY * diffY;
                    if ( distSquare < rSquare ) {
                        img->setPixel( i, j, pixColor );
                    }
                }
            }
        }

    };

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
            for( int64_t i = x; i<endW; ++i ) {
                for( int64_t j = y; j<endH; ++j ) {
                    const Image::Pixel src = source.pixel( i-x, j-y );
                    setPixel(i, j, src);
                }
            }
        }

        void drawLine(const PointI& /*fromPoint*/, const PointI& /*toPoint*/, const uint32_t /*radius*/, const std::string& /*color*/) override {
            //TODO: implement
            throw std::runtime_error("drawLine not implemented");
        }

        void drawArc(const PointI& /*center*/, const uint32_t /*radius*/, const uint32_t /*width*/, const double /*startAngle*/, const double /*range*/, const std::string& /*color*/) override {
            //TODO: implement
            throw std::runtime_error("drawArc not implemented");
        }

        void fillRect(const PointI& point, const uint32_t width, const uint32_t height, const std::string& color) override {
            assert( point.x >= 0 );
            assert( point.y >= 0 );
            const Image::Pixel pixColor = Image::convertColor( color );
            const int64_t x = point.x;
            const int64_t y = point.y;
            const int64_t w = img->width();
            const int64_t h = img->height();
            const int64_t endW = std::min(w, x + width );
            const int64_t endH = std::min(h, y + height );
            for( int64_t i = x; i<endW; ++i ) {
                for( int64_t j = y; j<endH; ++j ) {
                    setPixel( i, j, pixColor );
                }
            }
        }

        void fillCircle(const PointI& /*center*/, const uint32_t /*radius*/, const std::string& /*color*/) override {
            //TODO: implement
            throw std::runtime_error("fillCircle not implemented");
        }


    private:

        void setPixel(const uint32_t x, const uint32_t y, const Image::Pixel& src) {
            const Image::Pixel orig = img->pixel( x, y );
            const uint32_t origLight = brightness(orig);
            const uint32_t srcLight = brightness(src);
            if (origLight > srcLight) {
                const Image::Pixel diff = difference(orig, src);
                img->setPixel( x, y, diff );
            } else {
                const Image::Pixel diff = difference(src, orig);
                img->setPixel( x, y, diff );
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
