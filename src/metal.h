#pragma once

#include "helpers.h"
#include "material.h"

#include <glm/glm.hpp>

class Metal: public Material
{
public:
  Metal(const glm::vec3& color, float fuzziness): albedo_{color}, fuzziness_{fuzziness} {}
  bool Scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override
  {
    auto reflected = helpers::Reflect(glm::normalize(r_in.GetDirection()), rec.normal);
    reflected += fuzziness_ * helpers::RandomInUnitSphere();
    scattered = Ray(rec.point, reflected);
    attenuation = albedo_;
    return glm::dot(scattered.GetDirection(), rec.normal) > 0.f;
  }

private:
  glm::vec3 albedo_;
  float fuzziness_;
};