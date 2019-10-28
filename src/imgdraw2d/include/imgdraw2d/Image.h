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

#ifndef IMGDRAW2D_INCLUDE_IMAGE_H_
#define IMGDRAW2D_INCLUDE_IMAGE_H_

#include <png++/png.hpp>

#include <string>
#include <cstdint>
#include <memory>


namespace imgdraw2d {

    class Image;

    typedef std::unique_ptr<Image> ImagePtr;


    class Image {
    public:

        typedef png::byte PixByte;
        typedef png::basic_rgba_pixel< PixByte > Pixel;
        typedef png::image< Pixel > RawImage;

        static const Pixel TRANSPARENT;
        static const Pixel BLACK;
        static const Pixel WHITE;
        static const Pixel RED;
        static const Pixel GREEN;
        static const Pixel BLUE;


    protected:

        RawImage img;


    public:

        Image(const std::string& path = "");

        Image(const uint32_t width, const uint32_t height);

        bool operator==(const Image& image) const {
            return equals(image);
        }

        bool equals(const Image& image) const;

        bool empty() const;

        uint32_t width() const;

        uint32_t height() const;

        // RawImage::row_const_access is reference to "row" type
        RawImage::row_const_access row(const std::size_t y) const;

        // RawImage::row_access is reference to "row" type
        RawImage::row_access row(const std::size_t y);

        const Pixel& pixel(const std::size_t x, const std::size_t y) const;

        void setPixel(const std::size_t x, const std::size_t y, const Pixel& color);

        void setPixelColor(const std::size_t x, const std::size_t y, const std::string& color);

        void fillTransparent();

        void fill(const std::string& color);

        void fill(const Pixel& color);

        void fillRect(const std::size_t sx, const std::size_t sy, const std::size_t ex, const std::size_t ey, const Pixel& color );

        void pasteImage(const std::size_t x, const std::size_t y, const Image& source );

        bool load(const std::string& path);

        void save(const std::string& path);

        void resize(const std::size_t width, const std::size_t height);


        static ImagePtr make() {
            return ImagePtr( new Image() );
        }

        static Pixel convertColor(const std::string& color);


    private:

        bool compare(const RawImage& image) const;

    };


    inline bool operator==(const imgdraw2d::Image::Pixel& colorA, const imgdraw2d::Image::Pixel& colorB) {
        if ( colorA.red != colorB.red )
            return false;
        if ( colorA.green != colorB.green )
            return false;
        if ( colorA.blue != colorB.blue )
            return false;
        if ( colorA.alpha != colorB.alpha )
            return false;
        return true;
    }

    inline bool operator!=(const imgdraw2d::Image::Pixel& colorA, const imgdraw2d::Image::Pixel& colorB) {
        if ( colorA.red != colorB.red )
            return true;
        if ( colorA.green != colorB.green )
            return true;
        if ( colorA.blue != colorB.blue )
            return true;
        if ( colorA.alpha != colorB.alpha )
            return true;
        return false;
    }

} /* namespace imgdraw2d */

#endif /* IMGDRAW2D_INCLUDE_IMAGE_H_ */
