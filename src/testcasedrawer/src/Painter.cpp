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


    /// ======================================================================================


    class DestinationModeWorker: public Painter::ModeWorker {
    public:

        DestinationModeWorker(Image* image): ModeWorker(image) {
        }

        void drawImage(const uint32_t x, const uint32_t y, const Image& source) override {
            const uint32_t w = img->width();
            const uint32_t h = img->height();
            const uint32_t endW = std::min(w, x + source.width() );
            const uint32_t endH = std::min(h, y + source.height() );
            for( uint32_t i = x; i<endW; ++i ) {
                for( uint32_t j = y; j<endH; ++j ) {
                    const Image::Pixel src = source.pixel( i-x, j-y );
                    img->setPixel( i, j, src );
                }
            }
        }

        void fillRect(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height, const std::string& color) override {
            //
        }

    };

    class DifferenceModeWorker: public Painter::ModeWorker {
    public:

        DifferenceModeWorker(Image* image): ModeWorker(image) {
        }

        void drawImage(const uint32_t x, const uint32_t y, const Image& source) override {
            const uint32_t w = img->width();
            const uint32_t h = img->height();
            const uint32_t endW = std::min(w, x + source.width() );
            const uint32_t endH = std::min(h, y + source.height() );
            for( uint32_t i = x; i<endW; ++i ) {
                for( uint32_t j = y; j<endH; ++j ) {
                    const Image::Pixel orig = img->pixel( i, j );
                    const Image::Pixel src = source.pixel( i-x, j-y );
                    const uint32_t origLight = brightness(orig);
                    const uint32_t srcLight = brightness(src);
                    if (origLight > srcLight) {
                        const Image::Pixel diff = difference(orig, src);
                        img->setPixel( i, j, diff );
                    } else {
                        const Image::Pixel diff = difference(src, orig);
                        img->setPixel( i, j, diff );
                    }
                }
            }
        }

        void fillRect(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height, const std::string& color) override {
            //
        }

    };


    /// ===============================================================


    Painter::Painter(Image& image): img(&image), mode(CM_DESTINATION), worker(nullptr) {
        setCompositionMode(mode);
    }

    Painter::Painter(Image* image): img(image), mode(CM_DESTINATION), worker(nullptr) {
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

    void Painter::drawImage(const uint32_t x, const uint32_t y, const Image& source) {
        worker->drawImage(x, y, source);
    }

    void Painter::fillRect(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height, const std::string& color) {
        const Image::Pixel pixColor = Image::convertColor( color );
        const uint32_t w = img->width();
        const uint32_t h = img->height();
        const uint32_t endW = std::min(w, x + width );
        const uint32_t endH = std::min(h, y + height );
        for( uint32_t i = x; i<endW; ++i ) {
            for( uint32_t j = y; j<endH; ++j ) {
                img->setPixel( i, j, pixColor );
            }
        }
    }

} /* namespace tcd */
