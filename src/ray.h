#pragma once

#include <glm/glm.hpp>

class Ray
{
public:
  Ray() {}
  Ray(const glm::vec3& origin, const glm::vec3& direction): origin_{origin}, direction_{direction} {}

  const glm::vec3& GetOrigin() const
  {
    return origin_;
  }

  const glm::vec3& GetDirection() const
  {
    return direction_;
  }

  glm::vec3 At(float t) const
  {
    return origin_ + direction_ * t;
  }

private:
  glm::vec3 origin_;
  glm::vec3 direction_;
};
