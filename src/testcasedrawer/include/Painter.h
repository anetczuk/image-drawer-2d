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

#ifndef TESTCASEDRAWER_INCLUDE_PAINTER_H_
#define TESTCASEDRAWER_INCLUDE_PAINTER_H_

#include "Geometry.h"
#include "Image.h"


namespace tcd {
    namespace painter {

        class AbstractPainter {
        public:

            virtual ~AbstractPainter() {
            }

            virtual void drawImage(const Point& point, const Image& source) = 0;

            virtual void drawLine(const Point& fromPoint, const Point& toPoint, const uint32_t radius, const std::string& color) = 0;

            //TODO: draw arc

            //TODO: draw clothoid

            virtual void fillRect(const Point& point, const uint32_t width, const uint32_t height, const std::string& color) = 0;

            virtual void fillCircle(const Point& point, const uint32_t radius, const std::string& color) = 0;

        };


        class ModeWorker: public AbstractPainter {
        public:

            Image* img;

            ModeWorker(Image* image): img(image) {
            }

            using AbstractPainter::drawImage;

            using AbstractPainter::drawLine;

            using AbstractPainter::fillRect;

            using AbstractPainter::fillCircle;


            void drawImage(const uint32_t x, const uint32_t y, const Image& source) {
                drawImage( Point{x, y}, source );
            }

            void drawLine(const uint32_t fromX, const uint32_t fromY, const uint32_t toX, const uint32_t toY, const uint32_t radius, const std::string& color) {
                drawLine( Point{fromX, fromY}, Point{toX, toY}, radius, color );
            }

            void fillRect(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height, const std::string& color) {
                fillRect( Point{x, y}, width, height, color );
            }

            void fillCircle(const uint32_t x, const uint32_t y, const uint32_t radius, const std::string& color) {
                fillCircle( Point{x, y}, radius, color );
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

        void setCompositionMode(const CompositionMode mode);

        // ====================================================================

        using painter::ModeWorker::drawImage;

        using painter::ModeWorker::drawLine;

        using painter::ModeWorker::fillRect;

        using painter::ModeWorker::fillCircle;


        void drawImage(const Point& point, const Image& source) override {
            worker->drawImage(point, source);
        }

        void drawLine(const Point& fromPoint, const Point& toPoint, const uint32_t radius, const std::string& color) override {
            worker->drawLine(fromPoint, toPoint, radius, color);
        }

        void fillRect(const Point& point, const uint32_t width, const uint32_t height, const std::string& color) override {
            worker->fillRect(point, width, height, color);
        }

        void fillCircle(const Point& point, const uint32_t radius, const std::string& color) override {
            worker->fillCircle(point, radius, color);
        }

    };

} /* namespace tcd */

#endif /* TESTCASEDRAWER_INCLUDE_PAINTER_H_ */
