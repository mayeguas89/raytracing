#pragma once

#include "helpers.h"
#include "material.h"

#include <glm/glm.hpp>

class Dielectric: public Material
{
public:
  Dielectric(float index_of_refraction): index_of_refraction_{index_of_refraction} {}
  bool Scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override
  {
    auto refr_ratio = rec.front_face ? (1.0f / index_of_refraction_) : index_of_refraction_;
    auto unit_ray = glm::normalize(r_in.GetDirection());
    auto cos_theta = glm::min(glm::dot(-unit_ray, rec.normal), 1.f);
    auto sin_theta = glm::sqrt(1.f - cos_theta * cos_theta);
    auto cannot_refract = refr_ratio * sin_theta > 1.f;
    glm::vec3 direction;
    if (cannot_refract || helpers::Reflectance(cos_theta, refr_ratio) > helpers::RandomSample())
      direction = helpers::Reflect(unit_ray, rec.normal);
    else
      direction = helpers::Refract(unit_ray, rec.normal, refr_ratio);
    scattered = Ray(rec.point, direction);
    attenuation = glm::vec3{1.f};
    return true;
  }

private:
  float index_of_refraction_;
};