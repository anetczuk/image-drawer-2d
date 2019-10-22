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

#include "imgdraw2d/Image.h"

#include <png++/types.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>


namespace imgdraw2d {

    Image::Image(const std::string& path): img() {
        if (path.empty() == false)
            img.read(path);
    }

    Image::Image(const uint32_t width, const uint32_t height): img(width, height) {
    }

    bool Image::equals(const Image& image) const {
        if (this == &image)
            return true;
        return compare(image.img);
    }

    bool Image::empty() const {
        if ( img.get_width() != 0 )
            return false;
        if ( img.get_height() != 0 )
            return false;
        return true;
    }

    uint32_t Image::width() const {
        return img.get_width();
    }

    uint32_t Image::height() const {
        return img.get_height();
    }

    Image::Pixel Image::pixel(const std::size_t x, const std::size_t y) const {
        return img.get_pixel(x, y);
    }

    Image::PixByte Image::red(const std::size_t x, const std::size_t y) const {
        const Pixel& pix = img.get_pixel(x, y);
        return pix.red;
    }

    Image::PixByte Image::green(const std::size_t x, const std::size_t y) const {
        const Pixel& pix = img.get_pixel(x, y);
        return pix.green;
    }

    Image::PixByte Image::blue(const std::size_t x, const std::size_t y) const {
        const Pixel& pix = img.get_pixel(x, y);
        return pix.blue;
    }

    void Image::fill(const std::string& color) {
        const Pixel pixColor = convertColor(color);
        const uint32_t w = width();
        const uint32_t h = height();
        for( uint32_t x = 0; x<w; ++x ) {
            for( uint32_t y = 0; y<h; ++y ) {
                img.set_pixel( x, y, pixColor );
            }
        }
    }

    void Image::fillTransparent() {
        fill("transparent");
    }

    void Image::setPixel(const std::size_t x, const std::size_t y, const Pixel& color) {
        img.set_pixel( x, y, color );
    }

    void Image::setPixelColor(const std::size_t x, const std::size_t y, const std::string& color) {
        const Image::Pixel pixColor = convertColor(color);
        img.set_pixel(x, y, pixColor);
    }

    bool Image::load(const std::string& path) {
        try {
            img.read(path);
            return true;
        } catch (const png::std_error& e) {
            return false;
        }
        return false;
    }

    void Image::save(const std::string& path) {
        {
            boost::filesystem::path filePath( path );
            boost::filesystem::path fileDir = filePath.parent_path();
            if (fileDir.empty() == false)
                boost::filesystem::create_directories(fileDir);
            boost::filesystem::ofstream output( filePath );
        }
        img.write(path);
    }

    void Image::resize(const std::size_t width, const std::size_t height) {
        img.resize(width, height);
    }

    Image::Pixel Image::convertColor(const std::string& color) {
        if ( color[0] == '#' ) {
            std::string valueCode = color.substr(1);
            if (valueCode.length() != 6) {
                throw std::runtime_error( "unknown color format: " + color );
            }
            const unsigned long value = std::stoul(valueCode, nullptr, 16);
            const Image::PixByte red = (value >> 16) & 0xff;
            const Image::PixByte green = (value >> 16) & 0xff;
            const Image::PixByte blue = (value >> 16) & 0xff;
            return Image::Pixel(red, green, blue, 255);
        }

        if ( color.compare( "red" ) == 0 ) {
            return Image::Pixel(255, 0, 0, 255);
        }
        if ( color.compare( "green" ) == 0 ) {
            return Image::Pixel(0, 255, 0, 255);
        }
        if ( color.compare( "blue" ) == 0 ) {
            return Image::Pixel(0, 0, 255, 255);
        }
        if ( color.compare( "black" ) == 0 ) {
            return Image::Pixel(0, 0, 0, 255);
        }
        if ( color.compare( "white" ) == 0 ) {
            return Image::Pixel(255, 255, 255, 255);
        }
        if ( color.compare( "transparent" ) == 0 ) {
            return Image::Pixel(0, 0, 0, 0);
        }

        throw std::runtime_error( "unknown color: " + color );
        return Image::Pixel();
    }

    bool Image::compare(const RawImage& image) const {
        const png::uint_32 width = img.get_width();
        if ( width != image.get_width() )
            return false;
        const png::uint_32 height = img.get_height();
        if ( height != image.get_height() )
            return false;

        /// compare pixels
        for(png::uint_32 x=0; x<width; ++x) {
            for(png::uint_32 y=0; y<height; ++y) {
                const Pixel& pix = img.get_pixel(x, y);
                const Pixel& other = image.get_pixel(x, y);
                if (pix.red != other.red)
                    return false;
                if (pix.green != other.green)
                    return false;
                if (pix.blue != other.blue)
                    return false;
            }
        }

        return true;
    }

} /* namespace imgdraw2d */
