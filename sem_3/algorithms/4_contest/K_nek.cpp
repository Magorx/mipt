#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <bitset>


const double eps = 1e-8;
const double PI = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825;

class Vector{
    public:

    long long x;
    long long y;

    Vector (long long x, long long y){
        this->x = x;
        this->y = y;
    }

    Vector (){
        this->x = 0;
        this->y = 0;
    }

    double length(){
        return sqrt(x*x + y*y);
    }

    

    void orthogonalize(){
        long long tmp = x;
        x = -y;
        y = tmp;
    }

    Vector operator+ (const Vector& a) const{
        return {this->x + a.x, this->y + a.y};
    }

    Vector operator- (const Vector& a) const{
        return {this->x - a.x, this->y - a.y};
    }

    long long operator* (const Vector& a) const{
        return this->x * a.x + this->y * a.y;
    }

    long long operator^ (const Vector& a) const{
        return x * a.y - a.x * y;
    }

    Vector operator* (const long long coef) const{
        return {this->x * coef, this->y * coef};
    }


    void operator*= (const long long coef){
        this->x *= coef;
        this->y *= coef;
        // return {this->x * coef, this->y * coef};
    }

    void operator+= (const Vector& a){
        this->x += a.x;
        this->y += a.y;
        
    }

    bool operator!= (const Vector& a) const{
        return (this->x != a.x) || (this->y != a.y);
    }

    bool operator== (const Vector& a) const{
        return (this->x == a.x) && (this->y == a.y);
    }
    
    bool operator< (const Vector& a) const{
        if (x == a.x){
            return y < a.y;
        }
        return x < a.x;
    }

    private:
    
};




std::ostream& operator<< (std::ostream& out, const Vector& vector){
    out << vector.x << " " << vector.y;
    return out;
}

std::istream& operator>> (std::istream& in, Vector& vector){
    in >> vector.x >> vector.y;
    return in;
}


class Pair_of_points{
    public:
    
    int id1;
    int id2;
    double distance;
};



int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int cnt = 0;
    std::cin >> cnt;

    std::vector<Vector> points(cnt);
    std::vector<Pair_of_points> pairs;
    std::vector<std::bitset<4096>> bitsets; //- для каждой вершины храним с какими  вершинами уже есть ребра на данной итерации. Далее будет понятно зачем  

    for (int i = 0; i < cnt; i++){
        std::cin >> points[i]; 
        bitsets.push_back(std::bitset<4096>(0));
    }


    for (int i = 0; i < cnt; i++){
        for (int j = i + 1; j < cnt; j++){
            Pair_of_points curr_pair = {i, j, (points[i] - points[j]).length()};
            pairs.push_back(curr_pair);
        }
    }
 
    std::sort(pairs.begin(), pairs.end(), [](const Pair_of_points& a, const Pair_of_points& b){
        return a.distance > b.distance;
    });

    double answer = 0;

    for (int i = 0; i < pairs.size(); i++){
        int first = pairs[i].id1;
        int second = pairs[i].id2;

        if ((bitsets[first] & bitsets[second]).any()){
            answer = pairs[i].distance;
            break;
        }

        bitsets[first][second] = true;
        bitsets[second][first] = true;


    }



    std::cout.precision(8);
    std::cout << answer/2 << std::endl;

    

    
    
    

};


