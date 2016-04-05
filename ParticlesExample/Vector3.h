#pragma once

// A simple Vector 3. For demo purposes only!
struct Vec3
{
    static const Vec3 Zero;
    static const Vec3 Right;

    float x, y, z;

    Vec3 operator *(float val) const {
        return{ x*val, y*val, z*val };
    }

    Vec3 operator +(const Vec3& val) const {
        return{ x + val.x, y + val.y, z + val.z };
    }

    Vec3& operator +=(const Vec3& val) {
        Vec3 newVal = *this + val;
        *this = newVal;
        return *this;
    }
};
