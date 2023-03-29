#include "camera.h"
#include "dielectric.h"
#include "hittable.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "material.h"
#include "metal.h"
#include "ray.h"
#include "sphere.h"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <random>
#include <vector>

//------------- CONSTANTS -----------------------------//
const float infinity = std::numeric_limits<float>::infinity();

const int kNumSamples = 500;

// Contante para determinar el numero maximo de rebotes del rayo
// Usado para determinar el color del pixel
const int kMaxBounces = 50;

//------------- HELPERS -----------------------------//
std::ostream& operator<<(std::ostream& out, glm::vec3 c)
{
  out << c.r << " " << c.g << " " << c.b << std::endl;
  return out;
}

std::ostream& PrepareFileHeader(std::ostream& outfile)
{
  outfile << "P3\n";
  outfile << image::kImageWidth << " " << image::kImageHeight << std::endl;
  outfile << "255\n";
  return outfile;
}

// Funcion que determina el color del pixel en funcion de los hit de un rayo
// El rayo original sale de la camara y si encuentra una superficie se genera un
// rayo que rebota en una direcci�n aleatoria y asi sucesivamente,
// Hasta que se alcanza el numero m�ximo de rebotes o no colisiona con nada,
// En ese caso se devuelve un lerp entre colores para un degradado (cielo/background)
glm::vec3 RayColor(const Ray& r, const Hittable& world, int depth)
{
  if (depth <= 0)
    return {0.f, 0.f, 0.f};

  if (auto record = world.Hit(r, 0.0001f, infinity); record.has_value())
  {
    // Elegimos un punto desde el impacto del rayo a la superficie de una esfera
    // de radio 1 y de centro el punto de impacto apuntando hacia el mismo hemisferio que la la normal
    // auto target = record->point + RandomInHempisphere(record->normal);
    // return 0.5f * RayColor({record->point, target - record->point}, world, depth - 1);

    // return 0.5f * (record->normal + glm::vec3{1.f});

    Ray scattered;
    glm::vec3 color_attenuation;
    if (record->mat_ptr->Scatter(r, *record, color_attenuation, scattered))
      return color_attenuation * RayColor(scattered, world, depth - 1);
    return {0.f, 0.f, 0.f};
  }
  auto unit_direction = glm::normalize(r.GetDirection());
  auto t = 0.5f * (unit_direction.y + 1.f);
  return (1.f - t) * glm::vec3{1.f, 1.f, 1.f} + t * glm::vec3{1.f, 0.7f, 0.5f};
}

std::optional<float> HitSphere(const Ray& r, const Sphere& sphere)
{
  auto& ray_direction = r.GetDirection();
  auto vector_sphere_center_to_ray = r.GetOrigin() - sphere.center;
  // auto param_a = glm::dot(ray_direction, ray_direction);
  auto param_a = glm::length2(ray_direction);
  auto param_h = glm::dot(ray_direction, vector_sphere_center_to_ray);
  // auto param_c =
  //   glm::dot(vector_sphere_center_to_ray, vector_sphere_center_to_ray) - sphere.radius * sphere.radius;
  auto param_c = glm::length2(vector_sphere_center_to_ray) - sphere.radius * sphere.radius;
  auto discriminant = param_h * param_h - param_a * param_c;
  if (discriminant < 0)
    return std::nullopt;
  return (-param_h - std::sqrt(discriminant)) / param_a;
}

glm::vec3 GetColor(const glm::vec3& pixel_color, int numberOfSamples)
{
  auto r = pixel_color.r;
  auto g = pixel_color.g;
  auto b = pixel_color.b;

  auto scale = 1.f / (float)numberOfSamples;

  r = glm::sqrt(r * scale);
  g = glm::sqrt(g * scale);
  b = glm::sqrt(b * scale);

  r = 256.f * glm::clamp(r, 0.f, 1.f);
  g = 256.f * glm::clamp(g, 0.f, 1.f);
  b = 256.f * glm::clamp(b, 0.f, 1.f);
  return {r, g, b};
}

HittableList RandomScene()
{
  HittableList world;

  // Ground
  auto ground_material = std::make_shared<Lambertian>(glm::vec3{0.5f, 0.5f, 0.5f});
  world.Add(std::make_shared<Sphere>(glm::vec3{0.f, -1000.f, 0.f}, 1000.f, ground_material));

  for (int i = -11; i < 11; i++)
  {
    for (int j = -11; j < 11; j++)
    {
      auto choose_mat = helpers::RandomSample();

      std::shared_ptr<Material> material;
      if (choose_mat < 0.65)
      {
        // Diffuse
        material = std::make_shared<Lambertian>(helpers::RandomColor());
      }
      else if (choose_mat < 0.85)
      {
        // Metal
        material = std::make_shared<Metal>(helpers::RandomColor(), helpers::RandomSample());
      }
      else
      {
        // Dielectric - glass
        material = std::make_shared<Dielectric>(1.5f);
      }

      auto center = helpers::RandomSample(0.1f, 0.4f);
      world.Add(std::make_shared<Sphere>(
        glm::vec3{i + 0.9f * helpers::RandomSample(), center, j + 0.9f * helpers::RandomSample()},
        center,
        material));
    }
  }

  return world;
}

//------------- MAIN -----------------------------//
int main(int, char**)
{
  std::ofstream outfile("image.ppm");

  // auto material_ground = std::make_shared<Lambertian>(glm::vec3(0.8f, 0.8f, 0.0f));
  // auto material_center = std::make_shared<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f));
  // auto material_left = std::make_shared<Metal>(glm::vec3(0.8f, 0.8f, 0.8f), 0.f);
  // auto material_right = std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 0.f);
  // auto material_front = std::make_shared<Dielectric>(1.5);

  // //------------- WORLD -----------------------------//
  HittableList world = RandomScene();
  // world.Add(std::make_shared<Sphere>(glm::vec3(0.f, -100.5f, -1.f), 100.f, material_ground));
  // world.Add(std::make_shared<Sphere>(glm::vec3(0.f, 0.f, -1.f), 0.5f, material_center));
  // world.Add(std::make_shared<Sphere>(glm::vec3(-1.f, 0.f, -1.f), 0.5f, material_front));
  // world.Add(std::make_shared<Sphere>(glm::vec3(-1.f, 0.f, -1.f), -0.45f, material_front));
  // world.Add(std::make_shared<Sphere>(glm::vec3(1.f, 0.f, -1.f), 0.5f, material_right));
  // world.Add(std::make_shared<Sphere>(glm::vec3(-0.25f, 0.15f, -0.4f), 0.2f, material_front));
  // world.Add(std::make_shared<Sphere>(glm::vec3(-0.25f, 0.15f, -0.4f), -0.15f, material_front));
  // world.Add(std::make_shared<Sphere>(glm::vec3(0.25f, -0.15f, -0.4f), 0.1f, material_front));

  //------------- CAMERA -----------------------------//
  auto look_from = glm::vec3{13.f, 8.f, 3.f};
  auto look_at = glm::vec3{0.f, 0.f, 0.f};
  auto dist_to_focus = glm::length(look_from - look_at);
  auto aperture = .1f;
  Camera camera(look_from, look_at, glm::vec3{0.f, 1.f, 0.f}, 20.f, image::kAspectRatio, aperture, dist_to_focus);

  PrepareFileHeader(outfile);

  for (int j = image::kImageHeight - 1; j >= 0; j--)
  {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < image::kImageWidth; i++)
    {
      // Multisampling de cada pixel
      glm::vec3 pixel_color{0.f};

      for (int k = 0; k < kNumSamples; k++)
      {
        auto u = (float)(i + helpers::RandomSample()) / (image::kImageWidth - 1);
        auto v = (float)(j + helpers::RandomSample()) / (image::kImageHeight - 1);
        auto r = camera.GetRay(u, v);
        pixel_color += RayColor(r, world, kMaxBounces);
      }
      pixel_color = GetColor(pixel_color, kNumSamples);
      outfile << pixel_color;
    }
  }

  outfile.close();
  std::cerr << "\nDone.\n";
}
