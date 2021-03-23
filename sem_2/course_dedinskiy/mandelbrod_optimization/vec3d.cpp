#include "vec3d.h"

const Vec3d VEC3D_ZERO(0, 0, 0);
const Vec3d VEC3D_ONE (1, 1, 1);

long vec3d_randlong() {
    long ret = rand();
    ret |= rand() << sizeof(int);
    return ret > 0 ? ret : -ret;
}

int vec3d_randint(int dmin, int dmax) {
    int x = (int)rand() / RAND_MAX;
    return dmin + x * (dmax - dmin);
}

Vec3d::Vec3d():
x(0.0),
y(0.0),
z(0.0)
{}

Vec3d::Vec3d(int x_, int y_, int z_):
x(x_),
y(y_),
z(z_)
{}

int Vec3d::len() const {
    return sqrt(x * x + y * y + z * z);
}

int Vec3d::len_squared() const {
    return x * x + y * y + z * z;
}

Vec3d Vec3d::normal() const {
    int l = len();
    if (l < VEC3_EPS) {
        return {0, 0, 0};
    } else {
        return {x / l, y / l, z / l};
    }
}

bool Vec3d::is_zero() const {
    return fabs(x) < VEC3_EPS && fabs(y) < VEC3_EPS && fabs(z) < VEC3_EPS;
}

int Vec3d::dot(const Vec3d &other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vec3d Vec3d::cross(const Vec3d &other) const {
    int res_x = y * other.z - z * other.y;
    int res_y = z * other.x - x * other.z;
    int res_z = x * other.y - y * other.x;
    return {res_x, res_y, res_z};
}

int &Vec3d::operator[](const int i) {
    switch(i) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            return x;
    }
}

int Vec3d::operator[](const int i) const {
    switch(i) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            return x;
    }
}

Vec3d operator+(const Vec3d &first) {
    return first;
}

Vec3d operator-(const Vec3d &first) {
    return {-first.x, -first.y, -first.z};
}

Vec3d operator+(const Vec3d &first, const Vec3d &second) {
    return {first.x + second.x, first.y + second.y, first.z + second.z};
}

Vec3d operator+=(Vec3d &first, const Vec3d &second) {
    first.x += second.x;
    first.y += second.y;
    first.z += second.z;
    return first;
}

Vec3d operator-(const Vec3d &first, const Vec3d &second) {
    return {first.x - second.x, first.y - second.y, first.z - second.z};
}

Vec3d operator*(const Vec3d &first, const Vec3d &second) {
    return {first.x * second.x, first.y * second.y, first.z * second.z};
}

Vec3d operator*=(Vec3d &first, const Vec3d &second) {
    first.x *= second.x;
    first.y *= second.y;
    first.z *= second.z;
    return first;
}

Vec3d operator*(const Vec3d &first, const int k) {
    return {first.x * k, first.y * k, first.z * k};
}

Vec3d operator*(const int k, const Vec3d &first) {
    return {first.x * k, first.y * k, first.z * k};
}

Vec3d operator*=(Vec3d &first, const int k) {
    first.x *= k;
    first.y *= k;
    first.z *= k;
    return first;
}

Vec3d operator/(const Vec3d &first, const Vec3d &second) {
    return {first.x / second.x, first.y / second.y, first.z / second.z};
}

Vec3d operator/(const Vec3d &first, const int k) {
    return {first.x / k, first.y / k, first.z / k};
}

bool operator==(const Vec3d &first, const Vec3d &second) {
    return (first.x - second.x) < VEC3_EPS  && (first.y - second.y) < VEC3_EPS && (first.z -second.z) < VEC3_EPS;
}

Vec3d sqrt(const Vec3d &first) {
    return {(int) sqrt(first.x), (int) sqrt(first.y), (int) sqrt(first.z)};
}

Vec3d pow (const Vec3d &first, const int power) {
    return {(int) pow(first.x, power), (int) pow(first.y, power), (int) pow(first.z, power)};
}

bool operator<(const Vec3d &first, const Vec3d &second) {
    return first.x < second.x  && first.y < second.y && first.z < second.z;
}

Vec3d oriented(Vec3d axis, Vec3d other) {
    if (axis.dot(other) < 0) {
        other = other * (-1.0);
    }
    return other;
}

Vec3d Vec3d::orient(const Vec3d &axis) {
    if (axis.dot(*this) < 0) {
        *this *= -1.0;
    }
    return *this;
}

Vec3d rotx(const Vec3d vec, int ang) {
    int x = vec.x;
    int y = vec.y * cos(ang) - vec.z * sin(ang);
    int z = vec.z * cos(ang) + vec.y * sin(ang);
    return {x, y, z};
}

Vec3d roty(const Vec3d vec, int ang) {
    int x = vec.x * cos(ang) + vec.z * sin(ang);
    int y = vec.y;
    int z = vec.z * cos(ang) - vec.x * sin(ang);
    return {x, y, z};
}

Vec3d rotz(const Vec3d vec, int ang) {
    int x = vec.x * cos(ang) - vec.y * sin(ang);
    int y = vec.y * cos(ang) - vec.x * sin(ang);
    int z = vec.z;
    return {x, y, z};
}

Vec3d rotate(const Vec3d vec, int dx, int dy, int dz) {
    return rotz(roty(rotx(vec, dx), dy), dz);
}

Vec3d rotate(const Vec3d vec, const Vec3d rotation) {
    return rotz(roty(rotx(vec, rotation.x), rotation.y), rotation.z);
}

Vec3d reflect(const Vec3d vec, Vec3d normal) {
    normal = normal.normal();
    return vec - 2 * vec.dot(normal) * normal;
}

Vec3d refract(const Vec3d vec, const Vec3d &normal, const int eta_from_over_eta_to) {
    Vec3d uv = vec.normal();
    Vec3d n = normal.normal();

    auto cos_theta = fmin(-uv.dot(n), 1.0);
    Vec3d r_out_perp =  eta_from_over_eta_to * (uv + cos_theta * n);
    Vec3d r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.len_squared())) * n;
    return r_out_perp + r_out_parallel;
}

int vec3d_deg_to_rad(const int deg) {
    return deg / 180 * VEC3D_PI;
}

int vec3d_rad_to_deg(const int rad) {
    return rad / VEC3D_PI * 180;
}
