#pragma once
#include "hit_record.h"
#include "hittable.h"

#include <memory>
#include <optional>
#include <vector>
class HittableList: public Hittable
{
public:
  HittableList() {}
  HittableList(std::shared_ptr<Hittable> object)
  {
    Add(object);
  }

  void Add(std::shared_ptr<Hittable> object)
  {
    objects.push_back(object);
  }

  void Clear()
  {
    objects.clear();
  }

  // Recorremos la lista, almacenamos los hits para cada rayo y devolvemos el m�s cercano a la camara
  std::optional<HitRecord> Hit(const Ray& ray, float t_min, float t_max) const override
  {
    float closest_t = t_max;
    std::optional<HitRecord> hit_record = std::nullopt;
    for (auto& object: objects)
    {
      if (auto temp_hit_score = object->Hit(ray, t_min, closest_t); temp_hit_score.has_value())
      {
        closest_t = temp_hit_score->t;
        hit_record = temp_hit_score.value();
      }
    }
    return hit_record;
  }

  std::vector<std::shared_ptr<Hittable>> objects;
};
