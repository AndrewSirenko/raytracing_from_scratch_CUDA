#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

double hit_sphere(const point3& center, double radius, const ray& r){
    // Equation of the sphere in vector form is: (𝐏−𝐂)⋅(𝐏−𝐂)=𝑟^2; P(t) = A + tb
    // Eqn to check if ray hits sphere: 𝑡^2𝐛⋅𝐛+2𝑡𝐛⋅(𝐀−𝐂)+(𝐀−𝐂)⋅(𝐀−𝐂)−𝑟^2=0
    // Find discriminant, check for 0,1,2 for amt of intersections
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    if (discriminant < 0){
        return -1.0;
    } 

    else {
        // Rest of quadratic eqn
        return (-b - sqrt(discriminant)) / (2.0*a);
    }
}

color ray_color(const ray& r) {
    // If we hit a sphere, paint sphere's color
    auto t = hit_sphere(point3(0,0,-1), 0.5, r);
    if (t > 0.0) {
        // Surface normal for shading
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1); // Cool normal visualization
    }
        
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5*(unit_direction.y() + 1.0);
    // Linear interpolation: blendedValue=(1−𝑡)⋅startValue+𝑡⋅endValue,
    return (1.0-t)*color(0.68, 0.44, 0.87) + t*color(0.95, 0.68, 0.84);
}

int main() {

    // Image

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);


    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}