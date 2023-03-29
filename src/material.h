#pragma once

#include "hit_record.h"
#include "ray.h"

#include <glm/glm.hpp>

class Material
{
public:
  virtual bool Scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const = 0;
};