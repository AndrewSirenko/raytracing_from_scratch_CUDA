#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        point3 center;
        double radius;
    
    public:
        sphere() : {}
        sphere(point3 cen, double r): center(cen), radius(r) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
}

bool sphere::hit (const ray& r, double t_min, double t_max, hit_record& rec) {
    // Equation of the sphere in vector form is: (𝐏−𝐂)⋅(𝐏−𝐂)=𝑟^2; P(t) = A + tb
    // Eqn to check if ray hits sphere: 𝑡^2𝐛⋅𝐛+2𝑡𝐛⋅(𝐀−𝐂)+(𝐀−𝐂)⋅(𝐀−𝐂)−𝑟^2=0 
    // Can use quadratic eqn, check for location of intersections
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared(); // v • v = |v|^2
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    
    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    // Store useful info in hit_record
    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;

    return true;
}

#endif