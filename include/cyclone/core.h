#pragma once
#include "cyclone/precision.h"
#include <math.h>
namespace cyclone
{
    class Vector3
    {
    public:
        real x;
        real y;
        real z;
        const static Vector3 GRAVITY;
        const static Vector3 HIGH_GRAVITY;
        const static Vector3 UP;
        const static Vector3 RIGHT;
        const static Vector3 OUT_OF_SCREEN;
        const static Vector3 X;
        const static Vector3 Y;
        const static Vector3 Z;
    private:
        real pad;

    public:
        Vector3(): x(0), y(0), z(0) {}
        Vector3(const real x, const real y, const real z): x(x), y(y), z(z) {}


        // const会引起重载
        real operator[](unsigned i) const
        {
            if(i == 0) return x;
            if(i == 1) return y;
        }

        // 为什么返回引用？
        real& operator[](unsigned i)
        {
            if(i == 0) return x;
            if(i == 1) return y;
            return z;
        }

        void operator+=(const Vector3& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;

        }

        Vector3 operator+(const Vector3& v) const
        {
            return Vector3(x + v.x, y + v.y, z + v.z);
        }

        void operator-=(const Vector3& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
        }
        Vector3 operator-(const Vector3& v) const
        {
            return Vector3(x - v.x, y - v.y, z - v.z);
        }

        void operator*=(const real value)
        {
            x *= value;
            y *= value;
            z *= value;
        }

        Vector3 operator*(const real value) const
        {
            return Vector3(x * value, y * value, z * value);
        }

        Vector3 componentProduct(const Vector3 &vector) const
        {
            return Vector3(x * vector.x, y * vector.y, z * vector.z);
        }

        void componentProductUpdate(const Vector3 &vector)
        {
            x *= vector.x;
            y *= vector.y;
            z *= vector.z;
        }

        Vector3 vectorProduct(const Vector3 &vector) const
        {
            return Vector3(y * vector.z - z * vector.y, 
                           z * vector.x - x * vector.z,
                           x * vector.y - y * vector.x
                        );
        }

        void operator %=(const Vector3 &vector)
        {
            *this = vectorProduct(vector);
        }

        Vector3 operator%(const Vector3 &vector) const
        {
            return Vector3(y * vector.z - z * vector.y, 
                           z * vector.x - x * vector.z,
                           x * vector.y - y * vector.x
                        );            
        }

        real scalarProduct(const Vector3 &vector) const
        {
            return x * vector.x + y * vector.y + z * vector.z;
        }

        real operator*(const Vector3& vector) const
        {
            return x * vector.x + y * vector.y + z * vector.z;
        }

        void addScaleVector(const Vector3& vector, real scale)
        {
            x += vector.x * scale;
            y += vector.y * scale;
            z += vector.z * scale;
        }

        void invert()
        {
            x = -x;
            y = -y;
            z = -z;
        }
        real magnitude() const
        {
            return real_sqrt(x * x + y * y + z * z);
        }

        real squareMagnitude() const
        {
            return x * x + y * y + z * z;
        }

        void normalize()
        {
            real l = magnitude();
            if(l > 0)
            {
                (*this) *= ((real)l) / l;
            }
        }

        void clear()
        {
            x = y = z = 0;
        }
    };
    const Vector3 Vector3::GRAVITY = Vector3(0, -9.81, 0);
    const Vector3 Vector3::HIGH_GRAVITY = Vector3(0, -19.62, 0);
    const Vector3 Vector3::UP = Vector3(0, 1, 0);
    const Vector3 Vector3::RIGHT = Vector3(1, 0, 0);
    const Vector3 Vector3::OUT_OF_SCREEN = Vector3(0, 0, 1);
    const Vector3 Vector3::X = Vector3(0, 1, 0);
    const Vector3 Vector3::Y = Vector3(1, 0, 0);
    const Vector3 Vector3::Z = Vector3(0, 0, 1);
}