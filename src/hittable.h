#pragma once

#include <optional>

class Ray;
struct HitRecord;

class Hittable
{
public:
  virtual std::optional<HitRecord> Hit(const Ray& ray, float t_min, float t_max) const = 0;
};