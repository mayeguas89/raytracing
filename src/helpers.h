#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include <random>

namespace helpers
{
float RandomSample()
{
  static std::uniform_real_distribution<float> distribution{0.f, 1.f};
  static std::mt19937 generator;
  return distribution(generator);
}

float RandomSample(float min, float max)
{
  return min + (max - min) * RandomSample();
}

glm::vec3 RandomColor()
{
  return {RandomSample() * RandomSample(), RandomSample() * RandomSample(), RandomSample() * RandomSample()};
}

glm::vec3 RandomInSphere(float radius)
{
  return glm::sphericalRand(radius);
}

// Returns a unit vector in a sphere of radious 1
glm::vec3 RandomInUnitSphere()
{
  return glm::normalize(glm::sphericalRand(1.f));
}

// Returns a unit vector in the same hemisphere as the vec_in parameter
// which means if the random vector has a dot product less than 1 we change its direction
// to face the hemisphere vec_in is pointing
glm::vec3 RandomInHempisphere(const glm::vec3& vec_in)
{
  auto random_unit = RandomInUnitSphere();
  if (glm::dot(random_unit, vec_in) < 0.f)
    return -random_unit;
  return random_unit;
}

glm::vec3 RandomInUnitDisk()
{
  return glm::vec3(glm::diskRand(1.f), 0.f);
}

// Using reflect definition in https://registry.khronos.org/OpenGL-Refpages/gl4/html/reflect.xhtml
glm::vec3 Reflect(const glm::vec3& incident, const glm::vec3& normal)
{
  return incident - 2 * glm::dot(incident, normal) * normal;
}

// Using refract definition in https://registry.khronos.org/OpenGL-Refpages/gl4/html/refract.xhtml
glm::vec3 Refract(const glm::vec3& incident, const glm::vec3& normal, float eta)
{
  auto k = 1.0f - eta * eta * (1.0f - glm::dot(normal, incident) * glm::dot(normal, incident));
  if (k < 0.0)
    return {0.f, 0.f, 0.f};
  return eta * incident - (eta * glm::dot(normal, incident) + glm::sqrt(k)) * normal;
}

float Reflectance(float cosine, float ref_idx)
{
  // Use Schlick's approximation for reflectance.
  auto r0 = (1.f - ref_idx) / (1.f + ref_idx);
  r0 = r0 * r0;
  return r0 + (1.f - r0) * static_cast<float>(std::pow((1.f - cosine), 5));
}

}