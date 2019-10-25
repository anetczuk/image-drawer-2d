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

#ifndef IMGDRAW2D_INCLUDE_GEOMETRY_H_
#define IMGDRAW2D_INCLUDE_GEOMETRY_H_

#include <cstdint>
#include <cmath>
#include <algorithm>
#include <cassert>


namespace imgdraw2d {

    template <typename T>
    struct Point {
        typedef T value_type;

        T x;
        T y;


        Point(): x(), y() {
        }

        Point(T x, T y): x(x), y(y) {
        }

        T operator[](const std::size_t n) const {
            switch(n) {
            case 0:  return x;
            case 1:  return y;
            default: {
            	assert( (n < 2) && "bad index" );
                return -1;
            }
            }
        }

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
    
    /// value:
    ///     positive -- left side,
    ///     negative -- right side,
    ///     zero     -- on vector
    template <typename T>
    inline double pointPosition(const Point<T>& baseVector, const Point<T>& point) {
        if (baseVector.x > 0) {
            const double base = linearY( baseVector, point.x );
            return point.y - base;
        } else if (baseVector.x < 0) {
            const double base = linearY( baseVector, point.x );
            return -(point.y - base);

        } else if (baseVector.y > 0) {
            const double base = linearX( baseVector, point.y );
            return -(point.x - base);
        } else if (baseVector.y < 0) {
            const double base = linearX( baseVector, point.y );
            return point.x - base;
        }

        return 0;
    }

    /// normailze in range [0, 2*PI)
    inline double normalizeAngle(const double angle) {
        int parts = angle / (2 * M_PI);
        if (angle < 0.0)
            parts -= 1;
        return angle - 2 * M_PI * parts;
    }

    inline double angleFromOXByTan(const double tanAngle, const bool positiveX) {
        double value = std::atan( tanAngle );
        if (positiveX == false) {
            value += M_PI;
        }
        return normalizeAngle(value);
    }

    template <typename T>
    double angleFromOX(const T x, const T y) {
        if (x == 0 && y == 0) {
            return M_PI_2;
        }
        const double tanAngle = (double) y / x;
        return angleFromOXByTan( tanAngle, x >= 0.0 );
    }

    inline bool isInRangeAngle( const double angle, const double angleFrom, const double angleTo ) {
        if (angleFrom < angleTo) {
            if (angle < angleFrom)
                return false;
            if (angle > angleTo)
                return false;
            return true;
        } else {
            if (angle > angleFrom)
                return true;
            if (angle < angleTo)
                return true;
            return false;
        }
    }


    /// ========================================================


    struct Tangens {
        double value;
        bool posX;


        bool operator>(const Tangens& other) const {
            const std::size_t currQ = quarter();
            const std::size_t otherQ = other.quarter();
            if (currQ == otherQ) {
                return (value > other.value);
            }
            return (currQ > otherQ);
        }
        bool operator>=(const Tangens& other) const {
            const std::size_t currQ = quarter();
            const std::size_t otherQ = other.quarter();
            if (currQ == otherQ) {
                return (value >= other.value);
            }
            return (currQ > otherQ);
        }

        bool operator<(const Tangens& other) const {
            const std::size_t currQ = quarter();
            const std::size_t otherQ = other.quarter();
            if (currQ == otherQ) {
                return (value < other.value);
            }
            return (currQ < otherQ);
        }
        bool operator<=(const Tangens& other) const {
            const std::size_t currQ = quarter();
            const std::size_t otherQ = other.quarter();
            if (currQ == otherQ) {
                return (value <= other.value);
            }
            return (currQ < otherQ);
        }

        std::size_t quarter() const {
            if (posX) {
                if (value < 0.0) {
                    return 4;
                } else {
                    return 1;
                }
            } else {
                if (value < 0.0) {
                    return 2;
                } else {
                    return 3;
                }
            }
        }

        bool isInRange(const Tangens& minTan, const Tangens& maxTan) const {
            if (maxTan >= minTan) {
                if (*this < minTan)
                    return false;
                if (*this > maxTan)
                    return false;
                return true;
            } else {
                if (*this > minTan)
                    return true;
                if (*this < maxTan)
                    return true;
                return false;
            }
        }


        static Tangens fromAngle(const double angleNormalized) {
            const double value = std::tan( angleNormalized );
            if (angleNormalized < M_PI_2) {
                return Tangens{value, true};
            }
            if (angleNormalized < 3 * M_PI_2) {
                return Tangens{value, false};
            }
            return Tangens{value, true};
        }

        static Tangens fromCoords(const double x, const double y) {
            const double value = y / x;
            if (std::isnan(value)) {
                return Tangens{ std::tan(M_PI_2), true};
            }
            if (x < 0.0) {
                return Tangens{value, false};
            } else {
                return Tangens{value, true};
            }
        }
    };


    /// ========================================================


    template <typename T>
    struct Rect {
        typedef T value_type;

        Point<T> a;
        Point<T> b;


        Rect(): a(), b() {
        }

        Rect(const Point<T>& point): a(point), b(point) {
        }

        Rect(const Point<T>& pointA, const Point<T>& pointB): a(pointA), b(pointB) {
        }

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

        bool expand(const Rect<T>& box) {
            bool changed = false;
            if (a.x > box.a.x) {
                a.x = box.a.x;
                changed = true;
            }
            if (a.y > box.a.y) {
                a.y = box.a.y;
                changed = true;
            }
            if (b.x < box.b.x) {
                b.x = box.b.x;
                changed = true;
            }
            if (b.y < box.b.y) {
                b.y = box.b.y;
                changed = true;
            }
            return changed;
        }

        static Rect<T> minmax(const Point<T>& p1, const Point<T>& p2) {
            return minmax(p1.x, p1.y, p2.x, p2.y);
        }

        template <typename PointT>
        static Rect<T> minmax(const PointT& p1, const PointT& p2) {
            return minmax(p1[0], p1[1], p2[0], p2[1]);
        }

        static Rect<T> minmax(const T x1, const T y1, const T x2, const T y2) {
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

} /* namespace imgdraw2d */

#endif /* IMGDRAW2D_INCLUDE_GEOMETRY_H_ */
