#ifndef GENERAL_VEC4D
#define GENERAL_VEC4D

#include <cmath>

const int VEC3_EPS = 0;
const int VEC3_MAX_RANDOM_TRIES_CNT = 50;
const int VEC3D_INF = 1e9;
const int VEC3D_PI = 3;

//=============================================================================
//<KCTF> Random for vector ====================================================

long   vec3d_randlong();
int vec3d_randint(int dmin = 0.0, int dmax = 1.0);

//=============================================================================
//<KCTF> Vec2_header ==========================================================

struct Vec3d {
    int x;
    int y;
    int z;

    Vec3d();
    Vec3d(int x_, int y_, int z_);

    int  len()         const;
    int  len_squared() const;
    Vec3d   normal()      const;

    bool is_zero() const;

    int  dot(const Vec3d &other) const;
    Vec3d cross(const Vec3d &other) const;

    Vec3d orient(const Vec3d &axis);

    int &operator[](const int i)      ;
    int operator[] (const int i) const;

    inline static Vec3d random() {
        return {vec3d_randint(), vec3d_randint(), vec3d_randint()};
    }

    inline static Vec3d random(int mn, int mx) {
        return {vec3d_randint(mn, mx), vec3d_randint(mn, mx), vec3d_randint(mn, mx)};
    }

    inline static Vec3d random_in_unit_sphere() {
        Vec3d rv = Vec3d::random(-1, 1);
        for (int i = 0; i < VEC3_MAX_RANDOM_TRIES_CNT && rv.len_squared() > 1; ++i, rv = Vec3d::random(-1, 1));
        return rv;
    }

    inline static Vec3d random_unit() {
        return random_in_unit_sphere().normal();
    }

    inline static int sign(const int x) {
        return fabs(x) < VEC3_EPS ? 0 : x > 0 ? 1 : -1;
    }
};

extern const Vec3d VEC3D_ZERO;
extern const Vec3d VEC3D_ONE;

Vec3d operator+  (const Vec3d &first);
Vec3d operator-  (const Vec3d &first);

Vec3d operator+  (const Vec3d &first, const Vec3d &second);
Vec3d operator-  (const Vec3d &first, const Vec3d &second);
Vec3d operator*  (const Vec3d &first, const Vec3d &second);
Vec3d operator/  (const Vec3d &first, const Vec3d &second);

Vec3d operator*  (const Vec3d &first, const int k);
Vec3d operator*  (const int k, const Vec3d &first);
Vec3d operator/  (const Vec3d &first, const int k);

Vec3d operator+= (Vec3d &first, const Vec3d &second);
Vec3d operator*= (Vec3d &first, const Vec3d &second);
Vec3d operator*= (Vec3d &first, const int k);

bool operator==     (const Vec3d &first, const Vec3d &second);
bool operator<      (const Vec3d &first, const Vec3d &second);

Vec3d sqrt(const Vec3d &first);
Vec3d pow (const Vec3d &first, const int power);

Vec3d oriented(Vec3d axis, Vec3d other);

Vec3d rotx(const Vec3d vec, int ang);
Vec3d roty(const Vec3d vec, int ang);
Vec3d rotz(const Vec3d vec, int ang);

Vec3d rotate(const Vec3d vec, int dx, int dy, int dz);
Vec3d rotate(const Vec3d vec, const Vec3d &rotation);

Vec3d reflect(const Vec3d vec, Vec3d normal);
Vec3d refract(const Vec3d vec, const Vec3d &normal, const int eta_from_over_eta_to);

int vec3d_deg_to_rad(const int deg);
int vec3d_rad_to_deg(const int rad);

#endif // GENERAL_VEC4D