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

#ifndef TESTCASEDRAWER_INCLUDE_IMAGE_H_
#define TESTCASEDRAWER_INCLUDE_IMAGE_H_

#include <string>
#include <cstdint>
#include <memory>

#include <png++/png.hpp>


namespace tcd {

    class Image;

    typedef std::unique_ptr<Image> ImagePtr;


    class Image {
    public:

        typedef png::byte PixByte;
        typedef png::basic_rgba_pixel< PixByte > Pixel;
        typedef png::image< Pixel > RawImage;


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

        Pixel pixel(const std::size_t x, const std::size_t y) const;

        PixByte red(const std::size_t x, const std::size_t y) const;

        PixByte green(const std::size_t x, const std::size_t y) const;

        PixByte blue(const std::size_t x, const std::size_t y) const;

        void fill(const std::string& color);

        void fillTransparent();

        void setPixel(const std::size_t x, const std::size_t y, const Pixel& color);

        void setPixelColor(const std::size_t x, const std::size_t y, const std::string& color);

        bool load(const std::string& path);

        void save(const std::string& path);


        static ImagePtr make() {
            return ImagePtr( new Image() );
        }

        static Pixel convertColor(const std::string& color);


    private:

        bool compare(const RawImage& image) const;

    };

} /* namespace tcd */

#endif /* TESTCASEDRAWER_INCLUDE_IMAGE_H_ */
