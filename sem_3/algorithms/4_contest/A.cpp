#include <cmath>
#include <cstdio>
#include <vector>
#include <algorithm>


constexpr double GLOB_EPS = 1e-7;
#ifndef M_PI
#define M_PI 3.14159265359
#endif


template <typename T, int neutral>
struct Vec2T {
    static constexpr T EPS = GLOB_EPS;

    union {
        struct {
            T x;
            T y;
        };

        T data[2];
    };

    Vec2T() : x(neutral), y(neutral) {}
    Vec2T(const T &x, const T &y) : x(x), y(y) {}
    Vec2T(const T &fill) : x(fill), y(fill) {}

    inline double len() const {
        return hypot(x, y);
    }

    inline T len_squared() const {
        return x * x + y * y;
    }

    inline Vec2T normal() const {
        T l = len();
        if(l < EPS) {
            return {neutral, neutral};
        } else {
            return {x / l, y / l};
        }
    }

    inline void normalize() {
        T l = len();
        if (l < EPS) { x = neutral; y = neutral; }
        else { x /= l; y /= l; }
    }

    inline bool is_zero() const {
        return len() < EPS;
    }

    inline T dot(const Vec2T &other) const {
        return x * other.x + y * other.y;
    }

    inline T cross(const Vec2T &other) const {
        return x * other.y - y * other.x;
    }

    double operator[](const int i) const {
        return data[i];
    }

    inline Vec2T rot90(int dir = 1) const {
        return {(-1 * dir) * x, (1 * dir) * y};
    }

    inline Vec2T rot(double ang) {
        const double cs = cos(ang);
        const double sn = sin(ang);

        T px = x * cs - y * sn; 
        T py = x * sn + y * cs;
        return {px, py};
    }
};


template <typename T, int neutral>
inline Vec2T<T, neutral> operator+(const Vec2T<T, neutral> &first) {
    return first;
}

template <typename T, int neutral>
inline Vec2T<T, neutral> operator-(const Vec2T<T, neutral> &first) {
    return {-1 * first.x, -1 * first.y};
}

template <typename T, int neutral>
inline Vec2T<T, neutral> operator+(const Vec2T<T, neutral> &first, const Vec2T<T, neutral> &second) {
    return {first.x + second.x, first.y + second.y};
}

template <typename T, int neutral>
inline Vec2T<T, neutral> operator-(const Vec2T<T, neutral> &first, const Vec2T<T, neutral> &second) {
    return {first.x - second.x, first.y - second.y};
}

template <typename T, int neutral>
inline Vec2T<T, neutral> operator*(const Vec2T<T, neutral> &first, const Vec2T<T, neutral> &second) {
    return {first.x * second.x, first.y * second.y};
}

template <typename T, int neutral>
inline Vec2T<T, neutral> operator/(const Vec2T<T, neutral> &first, const Vec2T<T, neutral> &second) {
    return {first.x / second.x, first.y / second.y};
}

template <typename T, int neutral>
inline Vec2T<T, neutral> operator*(const Vec2T<T, neutral> &first, const double k) {
    return {static_cast<T>(k * first.x), static_cast<T>(k * first.y)};
}

template <typename T, int neutral>
inline Vec2T<T, neutral> operator*(const double k, const Vec2T<T, neutral> &first) {
    return {static_cast<T>(k * first.x), static_cast<T>(k * first.y)};
}

template <typename T, int neutral>
inline Vec2T<T, neutral> operator/(const Vec2T<T, neutral> &first, const double k) {
    return {static_cast<T>(first.x / k), static_cast<T>(first.y / k)};
}

template <typename T, int neutral>
inline Vec2T<T, neutral> &operator+=(Vec2T<T, neutral> &first, const Vec2T<T, neutral> &second) {
    first.x += second.x;
    first.y += second.y;
    return first;
}

template <typename T, int neutral>
inline Vec2T<T, neutral> &operator-=(Vec2T<T, neutral> &first, const Vec2T<T, neutral> &second) {
    first.x -= second.x;
    first.y -= second.y;
    return first;
}

template <typename T, int neutral>
inline Vec2T<T, neutral> &operator*=(Vec2T<T, neutral> &first, const Vec2T<T, neutral> &second) {
    first.x *= second.x;
    first.y *= second.y;
    return first;
}

template <typename T, int neutral>
inline Vec2T<T, neutral> &operator/=(Vec2T<T, neutral> &first, const Vec2T<T, neutral> &second) {
    first.x /= second.x;
    first.y /= second.y;
    return first;
}

template <typename T, int neutral>
inline Vec2T<T, neutral> &operator*=(Vec2T<T, neutral> &first, const double k) {
    first.x *= k;
    first.y *= k;
    return first;
}

template <typename T, int neutral>
inline Vec2T<T, neutral> &operator/=(Vec2T<T, neutral> &first, const double k) {
    first.x /= k;
    first.y /= k;
    return first;
}

template <typename T, int neutral>
inline bool operator==(const Vec2T<T, neutral> &first, const Vec2T<T, neutral> &second) {
    return abs(first.x - second.x) < first.EPS && abs(first.y - second.y) < first.EPS;
}

template <typename T, int neutral>
inline bool operator<(const Vec2T<T, neutral> &first, const Vec2T<T, neutral> &second) {
    return first.x - second.x < first.EPS && first.y - second.y < first.EPS;
}

template <typename T, int neutral>
inline Vec2T<T, neutral> sqrt(const Vec2T<T, neutral> &first) {
    return {sqrt(first.x),
            sqrt(first.y)};
}

template <typename T, int neutral>
inline Vec2T<T, neutral> pow(const Vec2T<T, neutral> &first, const double power) {
    return {pow(first.x, power),
            pow(first.y, power)};
}

template<typename T>
struct LineT {
    static constexpr T EPS = GLOB_EPS;

    union {
        struct {
            T a;
            T b;
            T c;
        };
        T data[3];
    };

    LineT(T a, T b, T c) : a(a), b(b), c(c) {}
    LineT(const Vec2T<T, 0> &p1, const Vec2T<T, 0> &p2) : a(p1.y - p2.y), b(p2.x - p1.x), c(p1.cross(p2)) {}

    bool intersect(const LineT<T> &line, Vec2T<T, 0> &point) {
        T d = a * line.b - b * line.a;
        if (fabs(d) < EPS) {
            return false;
        } else {
            T d1 = b * line.c - c * line.b;
            T d2 = c * line.a - a * line.c;
            point = {d1 / d, d2 / d};
            return true;
        }
    }

    inline Vec2T<T, 0> norm() const {
        return {a, b};
    }

    inline Vec2T<T, 0> dir() const {
        return Vec2T<T, 0>(-b, a).normal();
    }

    inline T norm_coef() {
        return hypot(a, b);
    }

    T dist(const Vec2T<T, 0> &point) {
        return fabs(a * point.x + b * point.y + c) / norm_coef();
    }

    T dist(const LineT<T> &line) {
        return fabs((double) c / norm().cross(dir()) - (double) line.c / line.norm().cross(dir()));
    }

};

template<typename T>
struct SegmentT {
    static constexpr T EPS = GLOB_EPS;

    Vec2T<T, 0> p1;
    Vec2T<T, 0> p2;

    SegmentT(const Vec2T<T, 0> &p1, const Vec2T<T, 0> &p2) : p1(p1), p2(p2) {}

    bool contains(const Vec2T<T, 0> &p) const {
        return fabs(((p - p1).len() + (p - p2).len() - (p1 - p2).len())) < 0.000000000001;
    }

    bool diff_side(const SegmentT &seg) const {
        return (seg.p1 - p1).cross(p2 - p1) * (seg.p2 - p1).cross(p2 - p1) < 0;
    }

    bool intersect(const SegmentT &seg) const {
        if (contains(seg.p1) || contains(seg.p2) || seg.contains(p1) || seg.contains(p2)) {
            return true;
        }

        if (!(diff_side(seg) && seg.diff_side(*this))) {
            return false;
        }

        return true;
    }
};

template <typename T>
T sign(T x) {
    return x > 0 ? 1 : x < 0 ? -1 : 0;
}

template <typename T>
struct PolygonT : public std::vector<Vec2T<T, 0>> {
    static constexpr T EPS = GLOB_EPS;

    std::vector<Vec2T<T, 0>> &data = *this;

    PolygonT() : std::vector<Vec2T<T, 0>>() {}

    bool is_convex() {
        Vec2T<T, 0> l1 = data[data.size() - 1];
        Vec2T<T, 0> l2 = data[data.size() - 2];

        long long sgn = 0;
        for (int i = 0; i < data.size(); ++i) {
            if (!sgn) sgn = sign((data[i] - l1).cross(l1 - l2));

            l2 = l1;
            l1 = data[i];
        }

        for (int i = 0; i < data.size(); ++i) {
            long long cur_sgn = sign((data[i] - l1).cross(l1 - l2));
            l2 = l1;
            l1 = data[i];

            if (cur_sgn * sgn < 0) {
                return false;
            }
        }

        return true;
    }

    bool is_inside(const Vec2T<T, 0> &p) const {
        int cnt = 0;
        const Vec2T<T, 0> dir(1, 0);

        Vec2T<T, 0> prev = data.back();
        for (int i = 0; i < data.size(); ++i) {
            SegmentT<T> seg(prev, data[i]);
            prev = data[i];

            if (seg.contains(p)) {
                return true;
            }

            if (seg.p1.y == seg.p2.y) {
                continue;
            }

            if (seg.p1.y > seg.p2.y) {
                std::swap(seg.p1, seg.p2);
            }

            SegmentT<T> ray(p, p + dir * 100000);
            if (ray.intersect(seg) && seg.p2.y > p.y && seg.p1.y <= p.y && ((p - seg.p1).cross(seg.p2 - seg.p1)) < 0) {
                cnt++;
            }
        }

        return cnt % 2 == 1;
    }

    struct AngleComparator {
        AngleComparator(Vec2T<T, 0> p) : p(p) {}
        bool operator () (Vec2T<T, 0> p1, Vec2T<T, 0> p2) {
            T cross = (p1 - p).cross(p2 - p);
            if (cross > EPS) { return true; }
            else if (cross < -EPS) { return false; }
            else return (p1 - p).len_squared() < (p2 - p).len_squared();
        }
    
        Vec2T<T, 0> p;
    };

    Vec2T<T, 0> find_left_down(size_t *idx = nullptr) {
        Vec2T<T, 0> ld = data[0];
        int id = 0;

        for (size_t i = 0; i < data.size(); ++i) {
            if (ld.x > data[i].x) {
                ld = data[i];
                id = i;
            } else if (ld.x == data[i].x && ld.y > data[i].y) {
                ld = data[i];
                id = i;
            }
        }

        if (idx) {
            *idx = id;
        }

        return ld;
    }
    
    PolygonT<T> get_convex() {
        PolygonT<T> ret;
        
        Vec2T<T, 0> ld = find_left_down();
    
        std::sort(data.begin(), data.end(), AngleComparator(ld));
    
        ret.push_back(data[0]);
        ret.push_back(data[1]);
        for (int i = 2; i < data.size(); ++i) {
            const Vec2T<T, 0> &p = data[i];
    
            while (ret.size() > 2) {
                const Vec2T<T, 0> &p1 = ret[ret.size() - 1];
                const Vec2T<T, 0> &p2 = ret[ret.size() - 2];
    
                const T c1 = (p1 - p2).cross(p - p2);
                if (c1 > 0) break;
                else ret.pop_back();
            }
            ret.push_back(p);
        }
    
        return ret;
    }

    PolygonT operator+(const PolygonT &other) {
        size_t i1 = 0;
        size_t i2 = 0;
        auto p1 = find_left_down(&i1);
        auto p2 = other.find_left_down(&i2);

        size_t cnt1 = 0;
        size_t cnt2 = 0;

        PolygonT ret;
        ret.push_back(p1 + p2);
        while (cnt1 < data.size() && cnt2 < other.size()) {
            Vec2T<T, 0> u = data[(i1 + 1) % data.size()] - data[i1];
            Vec2T<T, 0> v = other[(i2 + 1) % other.size()] - other[i2];

            auto sgn = sign(u.cross(v));
            if (sgn > 0) {
                ret.push_back(ret.back() + u);
                i1 = (i1 + 1) % data.size();
                cnt1++;
            } else if (sgn < 0) {
                ret.push_back(ret.back() + v);
                i2 = (i2 + 1) % other.size();
                cnt2++;
            } else {
                ret.push_back(ret.back() + u);
                i1 = (i1 + 1) % data.size();
                cnt1++;

                ret.push_back(ret.back() + v);
                i2 = (i2 + 1) % other.size();
                cnt2++;
            }
        }

        while (cnt1 < data.size()) {
            auto p = data[(i1 + 1) % data.size()] - data[i1];
            ret.push_back(ret.back() + p);
            i1 = (i1 + 1) % data.size();
            cnt1++;
        }

        while (cnt2 < other.size()) {
            auto p = other[(i2 + 1) % other.size()] - other[i2];
            ret.push_back(ret.back() + p);
            i2 = (i2 + 1) % other.size();
            cnt2++;
        }

        return ret;
    }
};


using NumT = double;

using Vec2    = Vec2T   <NumT, 0>;
using Line    = LineT   <NumT>;
using Segment = SegmentT<NumT>;
using Polygon = PolygonT<NumT>;


struct May : public Vec2 {
    double ang;
};

inline bool isbit(int n, int x) {
  return (n >> x) & 1;
}

inline int setbit(int n, int x) {
    return n | (1 << x);
}


double rad_to_deg(const double rad) {
    return rad / M_PI * 180;
}

double deg_to_rad(const double deg) {
    return deg / 180 * M_PI;
}

int main() {
    int n = 0;
    double l, r;
    scanf("%d %lg %lg", &n, &l, &r);

    std::vector<May> arr(n);
    for (int i = 0; i < n; ++i) {
        scanf("%lg %lg %lg", &arr[i].x, &arr[i].y, &arr[i].ang);
        arr[i].ang = deg_to_rad(arr[i].ang);
        if (arr[i].y < 0) {
            arr[i].y *= -1;
        }
    }

    int maxk = 1 << n;
    std::vector<Vec2> dp(maxk + 1, {l - 1, 0});
    dp[0] = {l, 0};

    for (int mask = 0; mask < maxk; ++mask) {
        for (int i = 0; i < n; ++i) {
            if (!isbit(mask, i)) {
                const double &ang = arr[i].ang;
                const Vec2 pos = {arr[i].x, arr[i].y};
                
                Vec2 v = dp[mask];
                v -= pos;

                if (pos.y == 0) {
                    if (v.x > 0) {
                        printf("%lg\n", r - l);
                        return 0;
                    } else {
                        dp[setbit(mask, i)].x = std::max(dp[setbit(mask, i)].x, pos.x);
                        if (pos.x >= r) {
                            printf("%lg\n", r - l);
                            return 0;
                        }
                        continue;
                    }
                }

                v = v.rot(ang);
                if (v.y >= 0) {
                    printf("%lg\n", r - l);
                    return 0;
                }

                double vl = v.len();
                if (!vl) {
                    continue;
                }

                Line l1({0, 0}, {1, 0});
                Line l2(pos, pos + v);
                Vec2 p;
                l1.intersect(l2, p);

                double lel = - pos.y / v.y;
                v = pos + v * lel; 
                
                int nm = setbit(mask, i);
                if (dp[nm].x < v.x) {
                    dp[nm].x = v.x;
                }
            }
        }
    }

    printf("%.7f\n", (float) std::min(dp[maxk - 1].x - l, r - l));

    return 0;
}
