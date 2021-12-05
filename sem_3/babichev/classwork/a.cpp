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

#include <cmath>

struct Vector {
    double x;
    double y;
    double z;
    int i;

    Vector();
    Vector(double x_, double y_, double z_);

    double len() const;
    Vector normal() const;

    double dot(const Vector &other) const;
    Vector cross(const Vector &other) const;

    double len_squared() const {
        return x * x + y * y + z * z;
    }
};

Vector operator+(const Vector &first, const Vector &second);
Vector operator+=(Vector &first, const Vector &second);
Vector operator-(const Vector &first, const Vector &second);
Vector operator*(const Vector &first, const Vector &second);
Vector operator*=(Vector &first, const Vector &second);
Vector operator*(const Vector &first, const double k);
Vector operator*=(Vector &first, const double k);
Vector operator/(const Vector &first, const Vector &second);
Vector operator/(const Vector &first, const double k);
bool operator==(const Vector &first, const Vector &second);
bool operator<(const Vector &first, const Vector &second);

Vector oriented(Vector axis, Vector other);

Vector rotx(const Vector vec, double ang);
Vector roty(const Vector vec, double ang);
Vector rotz(const Vector vec, double ang);
Vector rotate(const Vector vec, double dx, double dy, double dz);
Vector rotate(const Vector vec, const Vector rotation);

using namespace std;

const double eps = 1e-5;

Vector::Vector() {
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

Vector::Vector(double x_, double y_, double z_) {
    x = x_;
    y = y_;
    z = z_;
}

double Vector::len() const{
    return sqrt(x * x + y * y + z * z);
}

Vector Vector::normal() const{
    double l = len();
    if (l < eps) {
        return {0, 0, 0};
    } else {
        return {x / l, y / l, z / l};
    }
}

double Vector::dot(const Vector &other) const{
    return x * other.x + y * other.y + z * other.z;
}

Vector Vector::cross(const Vector &other) const{
    double res_x = y * other.z - z * other.y;
    double res_y = z * other.x - x * other.z;
    double res_z = x * other.y - y * other.x;
    return {res_x, res_y, res_z};
}

Vector operator+(const Vector &first, const Vector &second) {
    return {first.x + second.x, first.y + second.y, first.z + second.z};
}

Vector operator+=(Vector &first, const Vector &second) {
    first.x += second.x;
    first.y += second.y;
    first.z += second.z;
    return first;
}

Vector operator-(const Vector &first, const Vector &second) {
    return {first.x - second.x, first.y - second.y, first.z - second.z};
}

Vector operator*(const Vector &first, const Vector &second) {
    return {first.x * second.x, first.y * second.y, first.z * second.z};
}

Vector operator*=(Vector &first, const Vector &second) {
    first.x *= second.x;
    first.y *= second.y;
    first.z *= second.z;
    return first;
}

Vector operator*(const Vector &first, const double k) {
    return {first.x * k, first.y * k, first.z * k};
}

Vector operator*=(Vector &first, const double k) {
    first.x *= k;
    first.y *= k;
    first.z *= k;
    return first;
}

Vector operator/(const Vector &first, const Vector &second) {
    return {first.x / second.x, first.y / second.y, first.z / second.z};
}

Vector operator/(const Vector &first, const double k) {
    return {first.x / k, first.y / k, first.z / k};
}

bool operator==(const Vector &first, const Vector &second) {
    return (first.x - second.x) < eps  && (first.y - second.y) < eps && (first.z -second.z) < eps;
}

bool operator<(const Vector &first, const Vector &second) {
    return first.x < second.x  && first.y < second.y && first.z < second.z;
}

Vector oriented(Vector axis, Vector other) {
    if (axis.dot(other) < 0) {
        other = other * (-1.0);
    }
    return other;
}

Vector rotx(const Vector vec, double ang) {
    double x = vec.x;
    double y = vec.y * cos(ang) - vec.z * sin(ang);
    double z = vec.z * cos(ang) + vec.y * sin(ang);
    return {x, y, z};
}

Vector roty(const Vector vec, double ang) {
    double x = vec.x * cos(ang) + vec.z * sin(ang);
    double y = vec.y;
    double z = vec.z * cos(ang) - vec.x * sin(ang);
    return {x, y, z};
}

Vector rotz(const Vector vec, double ang) {
    double x = vec.x * cos(ang) - vec.y * sin(ang);
    double y = vec.y * cos(ang) - vec.x * sin(ang);
    double z = vec.z;
    return {x, y, z};
}

Vector rotate(const Vector vec, double dx, double dy, double dz) {
    return rotz(roty(rotx(vec, dx), dy), dz);
}

Vector rotate(const Vector vec, const Vector rotation) {
    return rotz(roty(rotx(vec, rotation.x), rotation.y), rotation.z);
}

const double EPS = 1e-6;

double vec2cross(const Vector &a, const Vector &b) {
    return a.x * b.y - a.y * b.x;
}

struct AngleComparator {
    AngleComparator(Vector p) : p(p) {}
    bool operator () (Vector p1, Vector p2) {
        double cross = vec2cross((p1 - p),(p2 - p));
        if (cross > EPS) { return true; }
        else if (cross < -EPS) { return false; }
        else return (p1 - p).len_squared() < (p2 - p).len_squared();
    }

    Vector p;
};

std::vector<Vector> convex(std::vector<Vector> &points) {
    std::vector<Vector> ret;
    
    Vector ld = points[0];
    for (size_t i = 0; i < points.size(); ++i) {
        if (ld.x > points[i].x) {
            ld = points[i];
        } else if (fabs(ld.x - points[i].x) < EPS && ld.y > points[i].y) {
            ld = points[i];
        }
    }

    std::sort(points.begin(), points.end(), AngleComparator(ld));

    ret.push_back(points[0]);
    ret.push_back(points[1]);
    for (int i = 2; i < points.size(); ++i) {
        const Vector &p = points[i];

        while (ret.size() > 2) {
            const Vector &p1 = ret[ret.size() - 1];
            const Vector &p2 = ret[ret.size() - 2];

            double c1 = vec2cross(p1 - p2, p - p2);
            if (c1 > 0) break;
            else ret.pop_back();
        }
        ret.push_back(p);
    }
    ret.push_back(points[0]);

    return ret;
}

std::pair<int, int> radius(std::vector<Vector> &points) {
    auto conv = convex(points);
    conv.pop_back();

    Vector lp = conv[0];
    Vector rp = conv[0];
    int l = 0;
    int r = 0;
    for (size_t i = 0; i < conv.size(); ++i) {
        if (lp.x > conv[i].x) {
            lp = conv[i];
            l = (int) i;
        }
        if (rp.x < conv[i].x) {
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

        Vector vl = conv[nxl] - conv[l];
        Vector vr = conv[nxr] - conv[r];

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

    std::vector<Vector> points(n);
    for (int i = 0; i < n; ++i) {
        double x, y;
        scanf("%lg %lg", &x, &y);
        points[i] = {x, y, 0};
        points[i].i = i;
    }

    auto dm = convex(points);
    dm.pop_back();

    int sz = (int) dm.size();
    if (fabs(vec2cross(dm[sz - 1] - dm[0], dm[sz - 2] - dm[0])) < EPS) {
        dm.pop_back();
    }

    printf("%lu\n", dm.size());
    for (auto x : dm) {
        printf("%g %g\n", x.x, x.y);
    }

    return 0;
}