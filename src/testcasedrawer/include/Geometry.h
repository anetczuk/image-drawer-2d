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

#ifndef TESTCASEDRAWER_INCLUDE_GEOMETRY_H_
#define TESTCASEDRAWER_INCLUDE_GEOMETRY_H_

#include <cstdint>
#include <algorithm>


namespace tcd {

    template <typename T>
    struct Point {
        typedef T value_type;

        T x;
        T y;

        Point<T> operator-() const {
            return Point<T>{ -x, -y };
        }
        Point<T> operator-(const Point<T>& point) const {
            return Point<T>{ x - point.x, y - point.y };
        }
        Point<T> operator+(const Point<T>& point) const {
            return Point<T>{ x + point.x, y + point.y };
        }

        Point<T> ortho() const {
            return Point<T>{y, -x};
        }
    };

    typedef Point<int64_t> PointI;

    typedef Point<double> PointD;


    struct Rect {
        PointI a;
        PointI b;


        static Rect minmax(const PointI& p1, const PointI& p2) {
            return minmax(p1.x, p1.y, p2.x, p2.y);
        }

        static Rect minmax(const int64_t x1, const int64_t y1, const int64_t x2, const int64_t y2) {
            Rect ret;
            ret.a.x = std::min(x1, x2);
            ret.a.y = std::min(y1, y2);
            ret.b.x = std::max(x1, x2);
            ret.b.y = std::max(y1, y2);
            return ret;
        }
    };

} /* namespace tcd */

#endif /* TESTCASEDRAWER_INCLUDE_GEOMETRY_H_ */
