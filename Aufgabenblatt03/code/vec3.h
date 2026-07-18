#pragma once

#include <algorithm>    // min, max
#include <cmath>        // abs, sqrt, acos...
#include <iostream>     // ostream
#include <limits>       // numeric_limits
#include <stdexcept>    // invalid_argument

/**
 * Class representing a 3D vector in euclidian space.
 */
template<typename T>
class Vec3
{
public:
    // compare two floating point values on equality
    static bool approxEq(T a, T b)
    {
        return std::abs(a - b) < std::numeric_limits<T>::epsilon();
    }

    // constructors
    Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
    Vec3(T s) : x(s), y(s), z(s) {}
    Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}

    // destructor
    ~Vec3() {}

    // binary operators and scaling operators
    const Vec3& operator+=(const Vec3 &r)
    {
        this->x += r.x;
        this->y += r.y;
        this->z += r.z;
        return *this;
    }

    const Vec3& operator-=(const Vec3 &r)
    {
        this->x -= r.x;
        this->y -= r.y;
        this->z -= r.z;
        return *this;
    }

    // scaling
    const Vec3& operator/=(const T &r)
    {
        this->x /= r;
        this->y /= r;
        this->z /= r;
        return *this;
    }

    const Vec3& operator*=(const T &r)
    {
        this->x *= r;
        this->y *= r;
        this->z *= r;
        return *this;
    }

    // two parameter binary operators
    // vector and vector
    inline friend Vec3 operator+(Vec3 lhs, const Vec3 &rhs)
    {
        return (lhs += rhs);
    }

    inline friend Vec3 operator-(Vec3 lhs, const Vec3 &rhs)
    {
        return (lhs -= rhs);
    }

    // scaling
    inline friend Vec3 operator*(const T &s, Vec3 v)
    {
        return (v *= s);
    }

    inline friend Vec3 operator*(Vec3 v, const T &s)
    {
        return (v *= s);
    }

    inline friend Vec3 operator/(Vec3 v, const T &s)
    {
        return (v /= s);
    }

    // unary oprators
    // negate the vec3
    Vec3 operator- () const
    {
        return Vec3<T>(-x, -y, -z);
    }

    // return true if this vec3 is the Null vector
    bool operator! () const
    {
        return (approxEq(x, 0) && approxEq(y, 0) && approxEq(z, 0));
    }

    // comparison operators (component wise comparison)
    friend bool operator==(const Vec3 &lhs, const Vec3 &rhs)
    {
        return (approxEq(lhs.x, rhs.x)) && (approxEq(lhs.y, rhs.y)) && (approxEq(lhs.z, rhs.z));
    }

    friend bool operator!=(const Vec3 &lhs, const Vec3 &rhs)
    {
        return !(lhs == rhs);
    }

    // subscript operator
    // non-const version may be used for assignemnt
    T& operator[] (const int index)
    {
        switch (index)
        {
            case 0: return x; break;
            case 1: return y; break;
            case 2: return z; break;
            default:
                throw std::invalid_argument("Invalid index used for vec3 single component access. "
                                            "Index must be in range [0..2].");
        }
    }

    // for const objects (read access only)
    const T& operator[] (const int index) const
    {
        switch (index)
        {
            case 0: return x; break;
            case 1: return y; break;
            case 2: return z; break;
            default:
                throw std::invalid_argument("Invalid index used for vec3 single component access. "
                                            "Index must be in range [0..2].");
        }
    }

    // stream output, non const return allows for chaining
    friend std::ostream& operator<<(std::ostream &s, const Vec3<T> &v)
    {
        return s << '[' << v.x << ' ' << v.y << ' ' << v.z << ']';
    }

    // geometric functions
    // length, normalize, cross, dot, distance, angle
    //
    // scalar product between this vec3 and another one
    inline T dot(const Vec3<T> &v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }

    // norm of this vec3
    inline T norm() const
    {
        return x * x + y * y + z * z;
    }

    // length of this vec3
    inline T length() const
    {
        return sqrt(norm());
    }

    // calculates the distance between this vec3 and another one
    inline T distance(const Vec3<T> &v) const
    {
        return (*this - v).length();
    }

    // calculates the angle between this vec3 and another one
    inline T angle(const Vec3<T> &v) const
    {
        return acos(this->dot(v) / sqrt(this->norm() * v.norm()));
    }

    // calculate the cross product (right handed coord system)
    inline Vec3 cross(const Vec3<T> &v) const
    {
        return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    // normalize this vec3
    inline const Vec3& normalize()
    {
        if (!*this == false)
            *this *= 1 / length();
        return *this;
    }

    // linearly interpolates between two vectors a and b based on mixValue
    static inline Vec3 mix(const Vec3 &a, const Vec3 &b, const T &mixValue)
    {
        return a * (1 - mixValue) + b * mixValue;
    }

    // clamp vector v to range given by lo and hi
    static inline Vec3 clamp(const T &lo, const T &hi, const Vec3 &v)
    {
        return Vec3(std::max(lo, std::min(hi, v.x)),
                    std::max(lo, std::min(hi, v.y)),
                    std::max(lo, std::min(hi, v.z)));
    }

private:
    // 3D vector components as member variables
    T x, y, z;
};

typedef Vec3<double> Vec3d;
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
