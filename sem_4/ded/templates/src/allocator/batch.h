#pragma once


namespace kctf::allocator {


template<typename T, int BatchBytes=1024*1024>
class Batch {
public:
    using value_type = T;

    Batch() {}
    ~Batch() {}

    
};


}
