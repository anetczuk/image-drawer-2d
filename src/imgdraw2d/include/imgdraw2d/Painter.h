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

#ifndef IMGDRAW2D_INCLUDE_PAINTER_H_
#define IMGDRAW2D_INCLUDE_PAINTER_H_

#include "imgdraw2d/Image.h"

#include "imgdraw2d/Geometry.h"


namespace imgdraw2d {
    namespace painter {

        class AbstractPainter {
        public:

            virtual ~AbstractPainter() {
            }

            virtual void drawImage(const PointI& point, const Image& source) = 0;

            void drawLine(const PointI& fromPoint, const PointI& toPoint, const uint32_t width, const std::string& color) {
                const Image::Pixel pixColor = Image::convertColor(color);
                drawLine( fromPoint, toPoint, width, pixColor );
            }

            virtual void drawLine(const PointI& fromPoint, const PointI& toPoint, const uint32_t width, const Image::Pixel& pixColor) = 0;

            void drawArc(const PointI& center, const uint32_t radius, const uint32_t width, const double startAngle, const double range, const std::string& color) {
                const Image::Pixel pixColor = Image::convertColor(color);
                drawArc( center, radius, width, startAngle, range, pixColor );
            }

            virtual void drawArc(const PointI& center, const uint32_t radius, const uint32_t width, const double startAngle, const double range, const Image::Pixel& pixColor) = 0;

            void drawRing(const PointI& center, const uint32_t radius, const uint32_t width, const std::string& color) {
                const Image::Pixel pixColor = Image::convertColor(color);
                drawRing( center, radius, width, pixColor );
            }

            virtual void drawRing(const PointI& center, const uint32_t radius, const uint32_t width, const Image::Pixel& pixColor) = 0;

            void fillRect(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height, const std::string& color) {
                fillRect( PointI{x, y}, width, height, color );
            }

            void fillRect(const PointI& point, const uint32_t width, const uint32_t height, const std::string& color) {
                const Image::Pixel pixColor = Image::convertColor(color);
                fillRect( point, width, height, pixColor );
            }

            virtual void fillRect(const PointI& point, const uint32_t width, const uint32_t height, const Image::Pixel& pixColor) = 0;

            virtual void fillRect(const PointI& topLeft, const PointI& topRight, const PointI& bottomRight, const PointI& bottomLeft, const Image::Pixel& pixColor) = 0;

            void fillCircle(const PointI& center, const uint32_t radius, const std::string& color) {
                const Image::Pixel pixColor = Image::convertColor(color);
                fillCircle( center, radius, pixColor );
            }

            virtual void fillCircle(const PointI& center, const uint32_t radius, const Image::Pixel& pixColor) = 0;

        };


        class ModeWorker: public AbstractPainter {
        public:

            Image* img;

            ModeWorker(Image* image): img(image) {
            }

            virtual void setImage(Image* image) {
                img = image;
            }

            void setImage(Image& image) {
                setImage( &image );
            }

            using AbstractPainter::drawImage;

            using AbstractPainter::drawLine;

            using AbstractPainter::fillRect;

            using AbstractPainter::fillCircle;


            void drawImage(const uint32_t x, const uint32_t y, const Image& source) {
                drawImage( PointI{x, y}, source );
            }

            void drawLine(const uint32_t fromX, const uint32_t fromY, const uint32_t toX, const uint32_t toY, const uint32_t width, const std::string& color) {
                drawLine( PointI{fromX, fromY}, PointI{toX, toY}, width, color );
            }

            void fillCircle(const uint32_t x, const uint32_t y, const uint32_t radius, const Image::Pixel& pixColor) {
                fillCircle( PointI{x, y}, radius, pixColor );
            }

            void fillCircle(const uint32_t x, const uint32_t y, const uint32_t radius, const std::string& color) {
                fillCircle( PointI{x, y}, radius, color );
            }

        };

    }


    class Painter: public painter::ModeWorker {
    public:

        enum CompositionMode {
            CM_DESTINATION,             /// similar to QPainter::CompositionMode_Destination
            CM_DIFFERENCE               /// similar to QPainter::CompositionMode_Difference
        };


    private:

        CompositionMode mode;
        std::unique_ptr<ModeWorker> worker;


    public:

        Painter(Image& image);

        Painter(Image* image);

        void setImage(Image* image) override {
            ModeWorker::setImage( image );
            worker->setImage( image );
        }

        using painter::ModeWorker::setImage;

        void setCompositionMode(const CompositionMode mode);

        // ====================================================================

        using painter::ModeWorker::drawImage;

        using painter::ModeWorker::drawLine;

        using painter::ModeWorker::fillRect;

        using painter::ModeWorker::fillCircle;

        using painter::ModeWorker::drawRing;

        using painter::ModeWorker::drawArc;


        void drawImage(const PointI& point, const Image& source) override {
            worker->drawImage(point, source);
        }

        void drawLine(const PointI& fromPoint, const PointI& toPoint, const uint32_t width, const Image::Pixel& pixColor) override {
            worker->drawLine(fromPoint, toPoint, width, pixColor);
        }

        void drawArc(const PointI& center, const uint32_t radius, const uint32_t width, const double startAngle, const double range, const Image::Pixel& pixColor) override {
            worker->drawArc(center, radius, width, startAngle, range, pixColor);
        }

        void drawRing(const PointI& center, const uint32_t radius, const uint32_t width, const Image::Pixel& pixColor) override {
            worker->drawRing(center, radius, width, pixColor);
        }

        void fillRect(const PointI& point, const uint32_t width, const uint32_t height, const Image::Pixel& pixColor) override {
            worker->fillRect(point, width, height, pixColor);
        }

        void fillRect(const PointI& topLeft, const PointI& topRight, const PointI& bottomRight, const PointI& bottomLeft, const Image::Pixel& pixColor) override {
            worker->fillRect(topLeft, topRight, bottomRight, bottomLeft, pixColor);
        }

        void fillCircle(const PointI& center, const uint32_t radius, const Image::Pixel& pixColor) override {
            worker->fillCircle(center, radius, pixColor);
        }

    };

} /* namespace imgdraw2d */

#endif /* IMGDRAW2D_INCLUDE_PAINTER_H_ */
