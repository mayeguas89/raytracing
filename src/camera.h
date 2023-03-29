#pragma once

#include "helpers.h"
#include "ray.h"

#include <glm/glm.hpp>

//------------- IMAGE -----------------------------//
namespace image
{
const auto kAspectRatio = 3.0 / 2.0;
const int kImageWidth = 400;
const int kImageHeight = static_cast<int>(kImageWidth / kAspectRatio);
}

//------------- CAMERA -----------------------------//
class Camera
{
public:
  Camera(const glm::vec3& look_from,
         const glm::vec3& look_at,
         const glm::vec3& up,
         float fov_degrees, // vertical fov
         float aspect_ratio,
         float aperture,
         float focus_distance)
  {
    auto theta = glm::radians(fov_degrees);
    auto h = glm::tan(theta / 2);
    auto viewport_height = 2.f * h;
    auto viewport_width = aspect_ratio * viewport_height;

    auto w = glm::normalize(look_from - look_at);
    u_ = glm::normalize(glm::cross(glm::normalize(up), w));
    v_ = glm::normalize(glm::cross(w, u_));

    origin_ = look_from;
    horizontal_ = focus_distance * viewport_width * u_;
    vertical_ = focus_distance * viewport_height * v_;
    lower_left_corner_ = origin_ - horizontal_ / 2.f - vertical_ / 2.f - focus_distance * w;
    // The larger the radius the greater the defocus blur
    lens_radius_ = aperture / 2;
  }

  Ray GetRay(float s, float t) const
  {
    glm::vec3 rd(lens_radius_ * helpers::RandomInUnitDisk());
    glm::vec3 offset = u_ * rd.x + v_ * rd.y;
    return Ray(origin_ + offset, (lower_left_corner_ + s * horizontal_ + t * vertical_) - (origin_ + offset));
  }

private:
  glm::vec3 horizontal_;
  glm::vec3 vertical_;
  glm::vec3 lower_left_corner_;
  glm::vec3 origin_;
  float lens_radius_;
  glm::vec3 u_;
  glm::vec3 v_;
};
