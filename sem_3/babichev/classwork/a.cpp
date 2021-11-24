#include <vector>
#include <algorithm>

#include <cstdlib>
#include <cstdio>
#include <cassert>

#include <cmath>

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
    int i = 0;

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

    // inline void set(uint8_t ind, const double &value) {
    //     content[ind] = value;
    // }

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

// inline Vec3d operator-(const Vec3d &first) {
//     return {first.content * -1};
// }

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

const double EPS = 1e-6;

double vec2cross(const Vec3d &a, const Vec3d &b) {
    return a.x() * b.y() - a.y() * b.x();
}

struct AngleComparator {
    AngleComparator(Vec3d p) : p(p) {}
    bool operator () (Vec3d p1, Vec3d p2) {
        double cross = vec2cross((p1 - p),(p2 - p));
        if (cross > EPS) { return true; }
        else if (cross < -EPS) { return false; }
        else return (p1 - p).len_squared() < (p2 - p).len_squared();
    }

    Vec3d p;
};

std::vector<Vec3d> convex(std::vector<Vec3d> &points) {
    std::vector<Vec3d> ret;
    
    Vec3d ld = points[0];
    for (size_t i = 0; i < points.size(); ++i) {
        if (ld.x() > points[i].x()) {
            ld = points[i];
        } else if (fabs(ld.x() - points[i].x()) < EPS && ld.y() > points[i].y()) {
            ld = points[i];
        }
    }

    std::sort(points.begin(), points.end(), AngleComparator(ld));

    ret.push_back(points[0]);
    ret.push_back(points[1]);
    for (int i = 2; i < points.size(); ++i) {
        const Vec3d &p = points[i];

        while (ret.size() > 2) {
            const Vec3d &p1 = ret[ret.size() - 1];
            const Vec3d &p2 = ret[ret.size() - 2];

            double c1 = vec2cross(p1 - p2, p - p2);
            if (c1 > 0) break;
            else ret.pop_back();
        }
        ret.push_back(p);
    }
    ret.push_back(points[0]);

    return ret;
}

std::pair<int, int> radius(std::vector<Vec3d> &points) {
    auto conv = convex(points);
    conv.pop_back();

    Vec3d lp = conv[0];
    Vec3d rp = conv[0];
    int l = 0;
    int r = 0;
    for (size_t i = 0; i < conv.size(); ++i) {
        if (lp.x() > conv[i].x()) {
            lp = conv[i];
            l = (int) i;
        }
        if (rp.x() < conv[i].x()) {
            rp = conv[i];
            r = (int) i;
        }
    }

    int il = conv[l].i;
    int ir = conv[r].i;
    double dmx = (conv[l] - conv[r]).len();

    for (int i = 0; i < 10 * conv.size(); ++i) {
        int nxl = (l + 1) % conv.size();
        int nxr = (r + 1) % conv.size();

        Vec3d vl = conv[nxl] - conv[l];
        Vec3d vr = conv[nxr] - conv[r];

        if (vec2cross(vl, vr) < 0) {
            l = nxl;
        } else {
            r = nxr;
        }

        double d = (conv[l] - conv[r]).len();
        if (d > dmx) {
            dmx = d;
            il = conv[l].i;
            ir = conv[r].i;
        }
    }

    return {il, ir};
}

int main() {
    int n = 0;
    scanf("%d", &n);

    std::vector<Vec3d> points(n);
    for (int i = 0; i < n; ++i) {
        double x, y;
        scanf("%lg %lg", &x, &y);
        points[i] = {x, y, 0};
        points[i].i = i;
    }

    auto dm = radius(points);
    if (dm.first > dm.second) {
        std::swap(dm.first, dm.second);
    }

    printf("%d %d\n", dm.first, dm.second);

    return 0;
}