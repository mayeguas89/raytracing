#pragma once

#include "fmt/format.h"
#include "glm/glm.hpp"
#include "ray.h"

#include <iostream>
#include <optional>
#include <random>

inline std::ostream& operator<<(std::ostream& os, const glm::vec3& color)
{
  int ir = static_cast<int>(255 * color.r);
  int ib = static_cast<int>(255 * color.b);
  int ig = static_cast<int>(255 * color.g);
  os << fmt::format("{} {} {}\n", ir, ib, ig);
  return os;
}

inline glm::vec3 RayColor(const Ray& ray)
{
  auto norm_dir = glm::normalize(ray.GetDirection());
  auto t = 0.5f * (norm_dir.y + 1.0f);
  return (1.f - t) * glm::vec3{1.f} + t * glm::vec3{0.5f, 0.7f, 1.f};
}

inline float RamdomFloat()
{
  static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
  static std::mt19937 generator;
  return distribution(generator);
}