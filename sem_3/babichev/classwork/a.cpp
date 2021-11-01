#include <cstdlib>
#include <cstdio>
#include <cassert>


#ifndef GENERAL_VEC3D
#define GENERAL_VEC3D

#include <cmath>
#include <iostream>

const double VEC3_EPS = 1e-6;
const double VEC3_MAX_RANDOM_TRIES_CNT = 50;
const double VEC3D_INF = 1e9;
const double VEC3D_PI = 3.1415926535897; // TODO M_PI

/**
 * Vectorization by AlexRoar
 * Debug and inlining help by JakMobius
 */

//=============================================================================
//<KCTF> Random for vector ====================================================

long vec3d_randlong();

double vec3d_randdouble(double dmin = 0.0, double dmax = 1.0);

//=============================================================================
//<KCTF> Vec2_header ==========================================================

struct Vec3d {
    typedef double content3 __attribute__((ext_vector_type(4)));

    content3 content;

    Vec3d();

    Vec3d(content3 newContent);

    Vec3d(double x_, double y_, double z_);

    inline double len() const {
        return sqrt(len_squared());
    }

    inline double len_squared() const {
        const auto squared = content * content;
        return squared[0] + squared[1] + squared[2];
    }

    inline Vec3d normal() const {
        double l = len();
        if(l < VEC3_EPS) {
            return {0, 0, 0};
        } else {
            return {content / l};
        }
    }

    inline void normalize() {
        double l = len();
        if(l < VEC3_EPS) content = {0, 0, 0};
        else content /= l;
    }

    inline bool is_zero() const {
        const auto squared = (content * content) < VEC3_EPS;
        return squared[0] * squared[1] * squared[2];
    }

    inline double dot(const Vec3d &other) const {
        const auto res = (content * other.content);
        return res[0] + res[1] + res[2];
    }

    inline Vec3d cross(const Vec3d &other) const {
        const content3 left_first = {content[1], content[2], content[0]};
        const content3 right_first = {other.content[2], other.content[0], other.content[1]};
        const content3 left_second = {content[2], content[0], content[1]};
        const content3 right_second = {other.content[1], other.content[2], other.content[0]};
        const auto res = left_first * right_first - right_second * left_second;
        return {res};
    }

    [[maybe_unused]] Vec3d orient(const Vec3d &axis);

    Vec3d &abs();

    double operator[](const int i) const {
        return content[i];
    }

    inline void set(uint8_t ind, const double &value) {
        content[ind] = value;
    }

    static Vec3d random_in_unit_sphere();

    inline static double sign(const double x) {
        return fabs(x) < VEC3_EPS ? 0 : x > 0 ? 1 : -1;
    }

    Vec3d &rotx(double ang);
    Vec3d &roty(double ang);
    Vec3d &rotz(double ang);

    inline double x() const { return content[0]; }
    inline double y() const { return content[1]; }
    inline double z() const { return content[2]; }
};

extern const Vec3d VEC3D_ZERO;
extern const Vec3d VEC3D_ONE;

inline Vec3d operator+(const Vec3d &first) {
    return first;
}

inline Vec3d operator-(const Vec3d &first) {
    return {first.content * -1};
}

inline Vec3d operator+(const Vec3d &first, const Vec3d &second) {
    return {first.content + second.content};
}

inline Vec3d operator-(const Vec3d &first, const Vec3d &second) {
    return {first.content - second.content};
}

inline Vec3d operator*(const Vec3d &first, const Vec3d &second) {
    return {first.content * second.content};
}

inline Vec3d operator/(const Vec3d &first, const Vec3d &second) {
    return {first.content / second.content};
}

inline Vec3d operator*(const Vec3d &first, const double k) {
    return {first.content * k};
}

inline Vec3d operator*(const double k, const Vec3d &first) {
    return {first.content * k};
}

inline Vec3d operator/(const Vec3d &first, const double k) {
    return {first.content / k};
}

inline Vec3d &operator+=(Vec3d &first, const Vec3d &second) {
    first.content += second.content;
    return first;
}

inline Vec3d &operator-=(Vec3d &first, const Vec3d &second) {
    first.content -= second.content;
    return first;
}

inline Vec3d &operator*=(Vec3d &first, const Vec3d &second) {
    first.content *= second.content;
    return first;
}

inline Vec3d &operator*=(Vec3d &first, const double k) {
    first.content *= k;
    return first;
}

inline Vec3d &operator/=(Vec3d &first, const Vec3d &second) {
    first.content /= second.content;
    return first;
}

inline Vec3d &operator/=(Vec3d &first, const double k) {
    first.content /= k;
    return first;
}

inline bool operator==(const Vec3d &first, const Vec3d &second) {
    const auto res = first.content - second.content;
    return (res[0]) < VEC3_EPS && (res[1]) < VEC3_EPS && (res[2]) < VEC3_EPS;
}

inline bool operator<(const Vec3d &first, const Vec3d &second) {
    const auto res = first.content < second.content;
    return res[0] * res[1] * res[2];
}

inline Vec3d sqrt(const Vec3d &first) {
    return {sqrt(first.content[0]),
            sqrt(first.content[1]),
            sqrt(first.content[2])};
}

inline Vec3d pow(const Vec3d &first, const double power) {
    return {pow(first.content[0], power),
            pow(first.content[1], power),
            pow(first.content[2], power)};
}

inline Vec3d oriented(Vec3d axis, Vec3d other) {
    if(axis.dot(other) < 0) {
        other *= (-1.0);
    }
    return other;
}

inline Vec3d reflect(Vec3d vec, Vec3d normal) {
    normal /= normal.len();
    normal *= vec.dot(normal) * 2;
    vec -= normal;
    return vec;
}

Vec3d refract(const Vec3d vec, const Vec3d &normal, const double eta_from_over_eta_to);

double vec3d_deg_to_rad(const double deg);

double vec3d_rad_to_deg(const double rad);

#endif // GENERAL_VEC3D


double vec3d_randdouble(double dmin, double dmax) {
    double x = (double) rand() / RAND_MAX;
    return dmin + x * (dmax - dmin);
}

Vec3d::Vec3d() {
    content = {0, 0, 0};
}

Vec3d::Vec3d(double x_, double y_, double z_) {
    content = {x_, y_, z_};
}

Vec3d::Vec3d(content3 newContent) : content(newContent) {}

[[maybe_unused]] Vec3d Vec3d::orient(const Vec3d &axis) {
    if(axis.dot(*this) < 0) {
        *this *= -1.0;
    }
    return *this;
}

Vec3d &Vec3d::abs() {
    content[0] = fabs(content[0]);
    content[1] = fabs(content[1]);
    content[2] = fabs(content[2]);

    return *this;
}

Vec3d &Vec3d::rotx(double ang) {
    double x = content[0];
    double y = content[1] * cos(ang) - content[2] * sin(ang);
    double z = content[2] * cos(ang) + content[1] * sin(ang);
    
    content[0] = x;
    content[1] = y;
    content[2] = z;

    return *this;
}

Vec3d &Vec3d::roty(double ang) {
    double x = content[0] * cos(ang) + content[2] * sin(ang);
    double y = content[1];
    double z = content[2] * cos(ang) - content[0] * sin(ang);
    
    content[0] = x;
    content[1] = y;
    content[2] = z;

    return *this;
}

Vec3d &Vec3d::rotz(double ang) {
    double x = content[0] * cos(ang) - content[1] * sin(ang);
    double y = content[1] * cos(ang) + content[0] * sin(ang);
    double z = content[2];
    
    content[0] = x;
    content[1] = y;
    content[2] = z;

    return *this;
}


Vec3d refract(const Vec3d vec, const Vec3d &normal, const double eta_from_over_eta_to) {
    Vec3d uv = vec.normal();
    Vec3d n = normal.normal();

    auto cos_theta = fmin(-uv.dot(n), 1.0);
    Vec3d r_out_perp = eta_from_over_eta_to * (uv + cos_theta * n);
    Vec3d r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.len_squared())) * n;
    return r_out_perp + r_out_parallel;
}

double vec3d_deg_to_rad(const double deg) {
    return deg / 180 * VEC3D_PI;
}

double vec3d_rad_to_deg(const double rad) {
    return rad / VEC3D_PI * 180;
}

struct Segment {
    Vec3d p1;
    Vec3d p2;

    bool contains(const Vec3d &p) {
        // printf("l1 %.100lg\n", (p - p1).len());
        // printf("l2 %.100lg\n", (p - p2).len());
        // printf("l3 %.100lg\n", (p2 - p1).len());
        return fabs(((p - p1).len() + (p - p2).len() - (p1 - p2).len())) < 0.000000000001;
    }
};

double cross_sign(const Vec3d &vec1, const Vec3d &vec2) {
    double z = vec1.cross(vec2).z();
    return z;
}

bool different_side(const Segment &a, const Segment &b) {
    Vec3d p = a.p1;
    return cross_sign(b.p1 - p, a.p2 - p) * cross_sign(b.p2 - p, a.p2 - p) < 0;
}

int main() {
    double x1, y1, x2, y2;

    scanf("%lg %lg %lg %lg", &x1, &y1, &x2, &y2);
    Segment s1 {{x1, y1, 0}, {x2, y2, 0}};

    scanf("%lg %lg %lg %lg", &x1, &y1, &x2, &y2);
    Segment s2 {{x1, y1, 0}, {x2, y2, 0}};

    if (s1.contains(s2.p1) || s1.contains(s2.p2) || s2.contains(s1.p1) || s2.contains(s1.p2)) {
        printf("YES\n");
        return 0;
    }

    if (!(different_side(s1, s2) && different_side(s2, s1))) {
        printf("NO\n");
        return 0;
    }
    
    printf("YES\n");

    return 0;
}
