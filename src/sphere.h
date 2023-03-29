#pragma once

#include "hittable.h"
#include "material.h"
#include "hit_record.h"
#include <glm/glm.hpp>

#include <memory>
struct Sphere: public Hittable
{
  Sphere(const glm::vec3& p_center, float p_radius, std::shared_ptr<Material> material):
    center{p_center},
    radius{p_radius},
    mat_ptr{material}
  {}
  glm::vec3 center{0.f, 0.f, 0.f};
  glm::vec3 color{0.1f, 0.1f, 1.f};
  float radius{1.0f};
  std::shared_ptr<Material> mat_ptr;

  std::optional<HitRecord> Hit(const Ray& ray, float t_min, float t_max) const override
  {
    auto& ray_direction = ray.GetDirection();
    auto vector_sphere_center_to_ray = ray.GetOrigin() - center;
    // auto param_a = glm::dot(ray_direction, ray_direction);
    auto param_a = glm::length2(ray_direction);
    auto param_h = glm::dot(ray_direction, vector_sphere_center_to_ray);
    // auto param_c =
    //   glm::dot(vector_sphere_center_to_ray, vector_sphere_center_to_ray) - sphere.radius * sphere.radius;
    auto param_c = glm::length2(vector_sphere_center_to_ray) - radius * radius;
    auto discriminant = param_h * param_h - param_a * param_c;
    if (discriminant < 0)
      return std::nullopt;
    auto sqrt_d = std::sqrt(discriminant);
    auto solution = (-param_h - sqrt_d) / param_a;
    if (solution < t_min || solution > t_max)
    {
      solution = (-param_h + sqrt_d) / param_a;
      if (solution < t_min || solution > t_max)
      {
        return std::nullopt;
      }
    }
    HitRecord hit_record;
    hit_record.point = ray.At(solution);
    hit_record.t = solution;
    hit_record.mat_ptr = mat_ptr;
    // Calulo de la normal
    auto normal = glm::normalize(hit_record.point - center);
    hit_record.SetNormal(ray, normal);

    return hit_record;
  }
};