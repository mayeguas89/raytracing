#pragma once

#include "ray.h"

#include <glm/glm.hpp>

#include <memory>
class Material;
struct HitRecord
{
  glm::vec3 point;
  glm::vec3 normal;
  std::shared_ptr<Material> mat_ptr;
  float t;
  bool front_face;

  void SetNormal(const Ray& ray, const glm::vec3& outward_normal)
  {
    // La normal apunta al exterior o al interior
    // Si el rayo viene del interior, la normal calculada arriba y el rayo tienen la misma direccion
    // Por tanto hay que cambiar el sentido de la normal para reflejar que apunta al centro de la esfera y no hacia afuera
    // Para la iluminaci�n solo iluminamos front face not back face, salvo que sea transparente
    normal = outward_normal;
    front_face = true;
    if (glm::dot(ray.GetDirection(), outward_normal) > 0.f)
    {
      normal = -outward_normal;
      front_face = false;
    }
  }
};
