#pragma once

// A simple Vector 2. For demo purposes only!
struct Vec2
{
    static const Vec2 Zero;
    static const Vec2 Right;

    float x, y;

    Vec2 operator *(float val) const {
        return{ x*val, y*val };
    }

    Vec2 operator +(const Vec2& val) const {
        return{ x + val.x, y + val.y };
    }

    Vec2& operator +=(const Vec2& val) {
        Vec2 newVal = *this + val;
        *this = newVal;
        return *this;
    }
};
