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
#include <cmath>
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
        Point<T>& operator-=(const Point<T>& point) {
            x -= point.x;
            y -= point.y;
            return *this;
        }
        Point<T> operator+(const Point<T>& point) const {
            return Point<T>{ x + point.x, y + point.y };
        }
        Point<T>& operator+=(const Point<T>& point) {
            x += point.x;
            y += point.y;
            return *this;
        }

        Point<T> operator*(const double factor) const {
            return Point<T>{ x * factor, y * factor };
        }

        Point<T> ortho() const {
            return Point<T>{y, -x};
        }
    };

    typedef Point<int64_t> PointI;
    typedef Point<double>  PointD;


    template <typename T>
    double linearX(const Point<T>& vector, const T value) {
        return (double) value * vector.x / vector.y;
    }
    template <typename T>
    double linearY(const Point<T>& vector, const T value) {
        // y/x = tg(alpha) = m
        // m -- direction coefficient
        return (double) value * vector.y / vector.x;
    }
    template <typename T>
    inline double pointPosition(const Point<T>& baseVector, const Point<T>& point) {
        if (baseVector.x != 0) {
            const double base = linearY( baseVector, point.x );
            return point.y - base;
        } else {
            const double base = linearX( baseVector, point.y );
            return point.x - base;
        }
    }
    template <typename T>
    inline double directionSign(const Point<T>& vector) {
        if (vector.x >=0)
            return 1.0;
        else
            return -1.0;
    }


    /// ========================================================


    template <typename T>
    struct Rect {
        typedef T value_type;

        Point<T> a;
        Point<T> b;


        T width() const {
            return b.x - a.x;
        }
        T height() const {
            return b.y - a.y;
        }

        void expand(const T radius) {
            Point<T> r{radius, radius};
            a -= r;
            b += r;
        }

        static Rect<T> minmax(const Point<T>& p1, const Point<T>& p2) {
            return minmax(p1.x, p1.y, p2.x, p2.y);
        }

        static Rect<T> minmax(const int64_t x1, const int64_t y1, const int64_t x2, const int64_t y2) {
            Rect<T> ret;
            ret.a.x = std::min(x1, x2);
            ret.a.y = std::min(y1, y2);
            ret.b.x = std::max(x1, x2);
            ret.b.y = std::max(y1, y2);
            return ret;
        }
    };

    typedef Rect<int64_t> RectI;
    typedef Rect<double>  RectD;


    /// ========================================================


    struct Linear {
        typedef PointI::value_type value_type;

        value_type A;
        value_type B;
        value_type C;
        double distFactor;


        Linear(const value_type a, const value_type b, const value_type c): A(a), B(b), C(c), distFactor( std::sqrt( A*A + B*B ) ) {
        }

        static Linear createFromOrthogonal(const PointI& start, const PointI& ortho) {
            const PointI sense = ortho.ortho();
            const PointI end = start + sense;
            return createFromPoints(start, end);
        }
        static Linear createFromOrthogonal(const PointI& ortho) {
            const PointI sense = ortho.ortho();
            return createFromPoints( PointI{0, 0}, sense );
        }

        static Linear createFromParallel(const PointI& start, const PointI& sense) {
            const PointI end = start + sense;
            return createFromPoints( start, end );
        }
        static Linear createFromParallel(const PointI& sense) {
            return createFromPoints( PointI{0, 0}, sense );
        }

        static Linear createFromPoints(const PointI& start, const PointI& end) {
            const PointI sense = end - start;
            const value_type cFactor = end.x * start.y - start.x * end.y;
            return Linear( -sense.y, sense.x, -cFactor );
        }

        value_type valueY(const value_type x) const {
            // Ax + By + C = 0
            // By =  -C - Ax
            //  y = (-C - Ax) / B
            return (double) (-C - A*x) / B;
        }
        value_type valueX(const value_type y) const {
            // Ax + By + C = 0
            // Ax =  -C - By
            //  x = (-C - By) / A
            return (double) (-C - B*y) / A;
        }

        value_type pointSide(const PointI& point) const {
            return pointSide( point.x, point.y );
        }

        value_type pointSide(const value_type x, const value_type y) const {
            if (B != 0) {
                const PointI::value_type base = valueY( x );
                return y - base;
            } else {
                const PointI::value_type base = valueX( y );
                return x - base;
            }
        }

        double distance(const PointI& point) const {
            const double factorA = std::abs( A * point.x + B * point.y + C );
            return factorA / distFactor;
        }
    };

} /* namespace tcd */

#endif /* TESTCASEDRAWER_INCLUDE_GEOMETRY_H_ */
