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

#include <ostream>
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

        bool operator==(const Point<T>& other) const {
            if (x != other.x)
                return false;
            if (y != other.y)
                return false;
            return true;
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
        double norm() const {
            return std::sqrt(x*x + y*y);
        }

        void setInRange(const Point<T>& pA, const Point<T>& pB) {
            if (x < pA.x) {
                x = pA.x;
            }
            if (y < pA.y) {
                y = pA.y;
            }
            if (x > pB.x) {
                x = pB.x;
            }
            if (y > pB.y) {
                y = pB.y;
            }
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

    inline double radToDeg(const double angle) {
        return 180 * angle / M_PI;
    }

    /// normailze to range [0, 2*PI)
    inline double normalizeAngle(const double angle) {
        int parts = angle / (2 * M_PI);
        if (angle < 0.0)
            parts -= 1;
        return angle - 2 * M_PI * parts;
    }

    /// returns: minAngle < maxAngle
    /// minAngle in range [0, 4*PI)
    /// maxAngle in range [0, 6*PI)
    /// upper limit is 6*PI to allow indicating one full circle (less than two full circles in reality)
    inline void normalizeAngleRange(const double startAngle, const double range, double& minAngle, double& maxAngle) {
        double normRange = range;
        const int rParts = range / ( 2 * M_PI );
        if (rParts > 1) {
            normRange -= (rParts-1) * 2 * M_PI;
        } else if (rParts < -1) {
            normRange -= (rParts+1) * 2 * M_PI;
        }
        /// normRange is in range (-4*PI, 4*PI)

        if (normRange >= 0.0) {
            minAngle = normalizeAngle( startAngle );
            maxAngle = normalizeAngle( startAngle ) + normRange;
            return ;
        }
        /// negative case
        minAngle = normalizeAngle( startAngle ) + normRange;
        maxAngle = normalizeAngle( startAngle );
        if (minAngle < 0.0) {
            minAngle += 4 * M_PI;
            maxAngle += 4 * M_PI;
        }
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

    template <typename PointT>
    inline PointT rotateVector(const PointT& vector, const double angle) {
        const double cosA = cos(angle);
        const double sinA = sin(angle);
        const double x = vector[0];
        const double y = vector[1];
        return PointT( x*cosA - y*sinA, x*sinA + y*cosA );
    }

    template <typename PointT>
    inline PointT rotateSenseVector(const double angle) {
        return rotateVector( PointT(1.0, 0.0), angle );
    }


    /// ========================================================


    struct RayI {

        PointI vector;
        double xFactor;
        double yFactor;


        RayI(const PointI& vector): vector(vector), xFactor(0.0), yFactor(0.0) {
            if (vector.x != 0) {
                yFactor = (double) vector.y / vector.x;
            }
            if (vector.y != 0) {
                xFactor = (double) vector.x / vector.y;
            }
        }

        /// value:
        ///     positive -- left side,
        ///     negative -- right side,
        ///     zero     -- on vector
        double side( const PointI& point ) const {
            if (vector.x > 0) {
                const double xBase = yFactor * point.x;
                return point.y - xBase;
            } else if (vector.x < 0) {
                const double xBase = yFactor * point.x;
                return -(point.y - xBase);
            }

            if (vector.y > 0) {
                const double yBase = xFactor * point.y;
                return -(point.x - yBase);
            } else if (vector.y < 0) {
                const double yBase = xFactor * point.y;
                return point.x - yBase;
            }

            return 0.0;
        }

        /// value:
        ///     positive -- left side,
        ///     negative -- right side,
        ///     zero     -- on vector
        double side( const int64_t px, const int64_t py ) const {
            if (vector.x > 0) {
                const double xBase = yFactor * px;
                return py - xBase;
            } else if (vector.x < 0) {
                const double xBase = yFactor * px;
                return -(py - xBase);
            }

            if (vector.y > 0) {
                const double yBase = xFactor * py;
                return -(px - yBase);
            } else if (vector.y < 0) {
                const double yBase = xFactor * py;
                return px - yBase;
            }

            return 0.0;
        }

    };


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

        Rect(const T x1, const T y1): a(x1, y1), b(x1, y1) {
        }

        Rect(const T x1, const T y1, const T x2, const T y2 ): a(x1, y1), b(x2, y2) {
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

        void expand(const Point<T>& point) {
            expand( point.x, point.y );
        }

        void expand(const T pointX, const T pointY) {
            if (a.x > pointX)
                a.x = pointX;
            if (a.y > pointY)
                a.y = pointY;
            if (b.x < pointX)
                b.x = pointX;
            if (b.y < pointY)
                b.y = pointY;
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

        void expand(const Point<T>& center, const double innerRadius, const double outerRadius, const double angle) {
            const Point<T> sense = rotateSenseVector< Point<T> >( angle );
            const Point<T> outer = center + sense * outerRadius;
            const Point<T> inner = center + sense * innerRadius;
            expand( outer );
            expand( inner );
        }

        void trim( const Point<T>& pB ) {
            const Point<T> pA(0.0, 0.0);
            a.setInRange( pA, pB );
            b.setInRange( pA, pB );
        }

        void trim( const Point<T>& pA, const Point<T>& pB ) {
            a.setInRange( pA, pB );
            b.setInRange( pA, pB );
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
        double caFactor;        /// -C/A
        double cbFactor;        /// -C/B
        double abFactor;        /// -A/B
        double baFactor;        /// -B/A
        double distFactorInv;


        Linear(const value_type a, const value_type b, const value_type c):
                A(a), B(b), C(c),
                caFactor(1.0), cbFactor(1.0), abFactor(1.0), baFactor(1.0),
                distFactorInv( 1.0 / std::sqrt( A*A + B*B ) )
        {
            if (A != 0) {
                caFactor = (double) - C / A;
                baFactor = (double) - B / A;
            }
            if (B != 0) {
                cbFactor = (double) - C / B;
                abFactor = (double) - A / B;
            }
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
            /// return (double) (-C - A*x) / B;
            return (double) cbFactor + abFactor * x;
        }
        value_type valueX(const value_type y) const {
            // Ax + By + C = 0
            // Ax =  -C - By
            //  x = (-C - By) / A
            /// return (double) (-C - B*y) / A;
            return (double) caFactor + baFactor * y;
        }

        value_type pointSide(const PointI& point) const {
            return pointSide( point.x, point.y );
        }

        /// value:
        ///     positive -- left side,
        ///     negative -- right side,
        ///     zero     -- on line
        value_type pointSide(const value_type x, const value_type y) const {
            if (B > 0) {
                const PointI::value_type base = valueY( x );
                return y - base;
            } else if (B < 0) {
                const PointI::value_type base = valueY( x );
                return -(y - base);
            }

            if (A > 0) {
                const PointI::value_type base = valueX( y );
                return x - base;
            } else if (A < 0) {
                const PointI::value_type base = valueX( y );
                return -(x - base);
            }

            return 0;
        }

        double distance(const PointI& point) const {
            const double factorA = std::abs( A * point.x + B * point.y + C );
            return factorA * distFactorInv;
        }
    };

} /* namespace imgdraw2d */


namespace std {

    template <typename T>
    inline ostream& operator<< (ostream& os, const imgdraw2d::Point<T>& point) {
        os << "[" << point[0] << ", " << point[1] << "]";
        return os;
    }

}


#endif /* IMGDRAW2D_INCLUDE_GEOMETRY_H_ */
