#pragma once

class Vec2
{
public:
    float x = 0, y = 0;

    Vec2();
    Vec2(float xin, float yin);

    bool operator==(const Vec2 &rhs) const;
    bool operator!=(const Vec2 &rhs) const;

    Vec2 operator+(const Vec2 &rhs) const;
    Vec2 operator-(const Vec2 &rhs) const;
    Vec2 operator/(const float val) const;
    Vec2 operator*(const float val) const;

    void operator+=(const Vec2 &rhs);
    void operator-=(const Vec2 &rhs);
    void operator/=(const float val);
    void operator*=(const float val);

    Vec2 unitVector(const Vec2 &rhs) const;
    float magnitude(const Vec2 &rhs) const;

    void print();
};