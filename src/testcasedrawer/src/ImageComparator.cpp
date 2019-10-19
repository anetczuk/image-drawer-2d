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

#include "ImageComparator.h"

#include "Painter.h"


//static QImage::Format DIFF_IMG_FORMAT = QImage::Format_RGB32;

static const int CHESS_GRID_SIZE = 10;
static const int DIFF_IMAGES_SPACING = CHESS_GRID_SIZE * 2;


namespace tcd {

    static ImagePtr generateChessboard(const uint32_t width, const uint32_t height) {
        ImagePtr chessPtr( new Image(width, height) );
        Image& chess = *chessPtr;
        chess.fill( "#666666" );
        Painter painter(&chess);
        const uint32_t imgMinGrid = std::min( width / 3, height / 3 );
        const uint32_t gridSize = (imgMinGrid < CHESS_GRID_SIZE) ? 2 : CHESS_GRID_SIZE;
        const uint32_t wSteps = width / gridSize;
        const uint32_t hSteps = height / gridSize;
        for (uint32_t wo=0; wo<=wSteps; wo++) {
            const uint32_t wrest = wo % 2;
            for (uint32_t ho=0; ho<=hSteps; ho++) {
                if (ho % 2 == wrest) {
                    const uint32_t currWidth = wo*gridSize;
                    const uint32_t currHeight = ho*gridSize;
                    painter.fillRect(currWidth, currHeight, gridSize, gridSize, "white");
                } else {
                    /// do nothing
                }
            }
        }
        return chessPtr;
    }

    ImagePtr ImageComparator::compare(const Image& imgA, const Image& imgB) {
        if(imgA.empty() && imgB.empty()) {
            ImagePtr emptyDiff( new Image(2, 1) );
            Image& empty = *emptyDiff;
            empty.setPixelColor(0, 0, "black");
            empty.setPixelColor(1, 0, "black");
            return emptyDiff;
        }
        const uint32_t widthA = imgA.width();
        const uint32_t heightA = imgA.height();

        const uint32_t widthB = imgB.width();
        const uint32_t heightB = imgB.height();

        const uint32_t widthMax = std::max(widthA, widthB);
        const uint32_t heightMax = std::max(heightA, heightB);

        ImagePtr diffPtr( new Image(widthMax, heightMax) );
        Image& diff = *diffPtr;
        diff.fillTransparent();
        {
            Painter painter(&diff);
            painter.setCompositionMode( Painter::CM_DIFFERENCE );
            painter.drawImage(0, 0, imgA);
            painter.drawImage(0, 0, imgB);
        }

        ImagePtr thresholdPtr( new Image(widthMax, heightMax) );
        Image& threshold = *thresholdPtr;
        threshold.fillTransparent();
        for (uint32_t wo=0; wo<widthMax; wo++) {
            for (uint32_t ho=0; ho<heightMax; ho++) {
                const Image::Pixel color = diff.pixel(wo, ho);
                if ( color.red > 0 || color.green > 0 || color.blue > 0 ) {
                    threshold.setPixelColor(wo, ho, "white");
                } else {
                    threshold.setPixelColor(wo, ho, "black");
                }
            }
        }

        ImagePtr joinPtr = generateChessboard(widthMax*2 + DIFF_IMAGES_SPACING, heightMax*2 + DIFF_IMAGES_SPACING);
        Image& join = *joinPtr;
        {
            Painter painter(&join);
            if (imgA.empty() == false) {
                painter.drawImage(0, 0, imgA);
            }
            if (imgB.empty() == false) {
                painter.drawImage(widthMax + DIFF_IMAGES_SPACING, 0, imgB);
            }
            painter.drawImage(0, heightMax + DIFF_IMAGES_SPACING, threshold);
            painter.drawImage(widthMax + DIFF_IMAGES_SPACING, heightMax + DIFF_IMAGES_SPACING, diff);
        }

        return joinPtr;
    }

    ImagePtr ImageComparator::compare(const Image* imgA, const Image* imgB) {
        if (imgA == nullptr || imgB == nullptr) {
            return ImagePtr( new Image() );
        }
        return compare( *imgA, *imgB );
    }

    bool ImageComparator::compare(const Image& imgA, const std::string& imgB, const std::string& diffImage) {
        Image imageB;
        imageB.load( imgB );
        ImagePtr diff = compare(imgA, imageB);
        diff->save( diffImage );
        return (imgA == imageB);
    }

}
