#pragma once

#include "helpers.h"
#include "material.h"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

class Lambertian: public Material
{
public:
  Lambertian(const glm::vec3& color): albedo_{color} {}
  bool Scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override
  {
    static const float kEpsilon = 1.e-8f;
    auto scatter_direction = rec.normal + helpers::RandomInUnitSphere();
    // If the random unit vector we generate is exactly opposite the normal vector,
    // the two will sum to zero, which will result in a zero scatter direction vector
    auto result = glm::epsilonEqual(scatter_direction, glm::vec3{0.f}, kEpsilon);
    if (result.x && result.y && result.z)
      scatter_direction = rec.normal;
    scattered = Ray(rec.point, scatter_direction);
    attenuation = albedo_;
    return true;
  }

private:
  glm::vec3 albedo_;
};