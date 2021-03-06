#include "raytrace.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>

double hit_sphere(const point3& center, double radius, const ray& r){
    // Equation of the sphere in vector form is: (πβπ)β(πβπ)=π^2; P(t) = A + tb
    // Eqn to check if ray hits sphere: π‘^2πβπ+2π‘πβ(πβπ)+(πβπ)β(πβπ)βπ^2=0 
    // Can use quadratic eqn, check for location of intersections
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared(); // v β’ v = |v|^2
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0){
        return -1.0;
    } 

    else {
        // Rest of quadratic eqn
        return (-half_b - sqrt(discriminant)) / (a);
    }
}

color ray_color_rainbow(const ray& r, const hittable& world, int depth){
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1,1,1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.95, 0.68, 0.84);
}

color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0,0,0);
    }
        
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    // Linear interpolation: blendedValue=(1βπ‘)βstartValue+π‘βendValue,
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.95, 0.68, 0.84);
}

int main() {

    // Image

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // World
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto material_right = make_shared<lambertian>(color(0.75, 0.68, 0.74));

    world.add(make_shared<sphere>(point3(0,0,-1), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1,-0.2,-1.5), 0.5, material_right));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100, material_ground)); // spherical ground

    // Camera

    camera cam;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; j--) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            color pixel_color(0, 0, 0);
            // Randomly sample color of surrounding pixels for antialiasing
            for (int s = 0; s < samples_per_pixel; s++){
                auto u = double(i + random_double()) / (image_width-1);
                auto v = double(j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}