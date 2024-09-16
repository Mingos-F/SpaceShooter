#include "Vec2.h"

#include <iostream>
#include <math.h>

Vec2::Vec2()
{
}

Vec2::Vec2(float xin, float yin)
    : x(xin),
      y(yin)
{
}

bool Vec2::operator==(const Vec2 &rhs) const
{
  return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator!=(const Vec2 &rhs) const
{
  return (x != rhs.x || y != rhs.y);
}

Vec2 Vec2::operator+(const Vec2 &rhs) const
{
  return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2 &rhs) const
{
  return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator/(const float val) const
{
  if (val == 0.0f)
  {
    std::cerr << "Divide by zero, (Vec2 Operator)" << std::endl;
    exit(-1);
  }

  return Vec2(x / val, y / val);
}

Vec2 Vec2::operator*(const float val) const
{
  return Vec2(x * val, y * val);
}

void Vec2::operator+=(const Vec2 &rhs)
{
  this->x + rhs.x;
  this->y + rhs.y;
}

void Vec2::operator-=(const Vec2 &rhs)
{
  this->x - rhs.x;
  this->y - rhs.y;
}

void Vec2::operator/=(const float val)
{
  this->x / val;
  this->y / val;
}

void Vec2::operator*=(const float val)
{
  this->x *val;
  this->y *val;
}

Vec2 Vec2::unitVector(const Vec2 &rhs) const
{
  float dx = rhs.x - x;
  float dy = rhs.y - y;
  float magnitude = sqrtf((dx * dx) + (dy * dy));

  return Vec2(dx / magnitude, dy / magnitude);
}

float Vec2::magnitude(const Vec2 &rhs) const
{
  float dx = rhs.x - x;
  float dy = rhs.y - y;
  float magnitude = sqrtf((dx * dx) + (dy * dy));

  return magnitude;
}

void Vec2::print()
{
  std::cout << "Coordinates: (" << x << "," << y << ")" << std::endl;
}