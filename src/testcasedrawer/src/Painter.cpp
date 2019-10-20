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

#include "Painter.h"

#include <cmath>


namespace tcd {

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

    inline int64_t linearX(const int64_t x, const PointI& vector) {
        return (double)x * vector.y / vector.x;
    }
    inline int64_t linearY(const int64_t y, const PointI& vector) {
        return (double)y * vector.x / vector.y;
    }

    inline int64_t pointPosition(const PointI& baseVector, const PointI& vector) {
        if (baseVector.x != 0) {
            const int64_t base = linearX( vector.x, baseVector );
            return vector.y - base;
        } else {
            const int64_t base = linearY( vector.y, baseVector );
            return vector.x - base;
        }
    }

    inline int64_t pointPosition(const PointI& start, const PointI& vector, const PointI& point) {
        const PointI diff = point - start;
        return pointPosition( vector, diff );
    }

    inline int64_t distanceParallel(const PointI& sense, const PointI& vector) {
        // Ax + By + C = 0
        // A = y2 - y1
        // B = x1 - x2
        // C = x2 * y1 - x1 * y2

        const int64_t A = sense.y;
        const int64_t B = -sense.x;

        const double factorA = std::abs( A * vector.x + B * vector.y );
        const double factorB = std::sqrt( A*A + B*B );
        return factorA / factorB;
    }

    inline int64_t distanceParallel(const PointI& start, const PointI& sense, const PointI& point) {
        const PointI diff = point - start;
        return distanceParallel( sense, diff );
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
                const int64_t j = linearX(i, vector);
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
                const int64_t i = linearY(j, vector);
                functor(i, j);
            }
        }
    }
    template <typename Functor>
    static void linear(const PointI& vector, const uint32_t steps, Functor& functor) {
        if (std::abs(vector.x) > std::abs(vector.y)) {
            int64_t from = 0;
            int64_t to = 0;
            if (vector.x > 0) {
                from = 0;
                to = steps;
            } else {
                from = -(int64_t)steps + 1;
                to = 1;
            }
            for( int64_t i = from; i<to; ++i ) {
                const int64_t j = linearX(i, vector);
                functor(i, j);
            }
        } else {
            int64_t from = 0;
            int64_t to = 0;
            if (vector.y > 0) {
                from = 0;
                to = steps;
            } else {
                from = -(int64_t)steps + 1;
                to = 1;
            }
            for( int64_t j = from; j<to; ++j ) {
                const int64_t i = linearY(j, vector);
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
            const PointI ortho = lineVector.ortho();

            const int64_t startW = - (int64_t) radius;
            const int64_t startH = - (int64_t) radius;

            const int64_t endW = lineVector.x + radius;
            const int64_t endH = lineVector.y + radius;

            for( int64_t i = startW; i<=endW; ++i ) {
                for( int64_t j = startH; j<=endH; ++j ) {
                    const PointI currVector{i, j};
                    const int64_t side1 = pointPosition(ortho, currVector);
                    if (side1 < 0) {
                        continue;
                    }
                    const PointI toVector = currVector - lineVector;
                    const int64_t side2 = pointPosition(ortho, toVector);
                    if (side2 > 0) {
                        continue;
                    }
                    const int64_t dist = distanceParallel(lineVector, currVector);
                    if (dist >= radius) {
                        continue;
                    }
                    img->setPixel( i + fromPoint.x, j + fromPoint.y, pixColor );
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

        void fillCircle(const PointI& point, const uint32_t radius, const std::string& color) override {
            assert( point.x >= 0 );
            assert( point.y >= 0 );
            const uint32_t rSquare = radius * radius;
            const Image::Pixel pixColor = Image::convertColor( color );
            const int64_t x = point.x;
            const int64_t y = point.y;
            const int64_t w = img->width();
            const int64_t h = img->height();
            const int64_t startW = udiff( x, radius );
            const int64_t startH = udiff( y, radius );
            const int64_t endW = std::min( w, x + radius );
            const int64_t endH = std::min( h, y + radius );
            for( int64_t i = startW; i<endW; ++i ) {
                for( int64_t j = startH; j<endH; ++j ) {
                    const int64_t diffX = i - x;
                    const int64_t diffY = j - y;
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

        void fillCircle(const PointI& /*point*/, const uint32_t /*radius*/, const std::string& /*color*/) override {
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

} /* namespace tcd */
