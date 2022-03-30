#pragma once


namespace kctf::storage
{


template <typename T>
class IndexedDynamic {
    constexpr static size_t MIN_CAPACITY = 4;

    T *data_;

    size_t capacity_;
    size_t size_;

    void set_capacity(size_t new_capacity) {
        T *new_data = (T*) new char[new_capacity * sizeof(T)];

        if (!new_data) {
            throw std::overflow_error("Can't allocate growed buffer for IndexedDynamic storage");
        }

        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
            
            data_[i].~T();
        }

        delete[] data_;

        data_ = new_data;
        capacity_ = new_capacity;
    }

    void grow_capacity(float coef=2) {
        if (!capacity_) {
            capacity_ = MIN_CAPACITY;
        }

        size_t new_capacity = capacity_ * coef;
        set_capacity(new_capacity);
    }

    int increment_size() {
        if (full()) {
            grow_capacity();
        }

        return ++size_;
    }

    int decrement_size() {
        return --size_;
    }

public:
    constexpr static bool is_dynamic = true;
    constexpr static bool can_give_data_ptr = true;
    constexpr static bool can_modify_size = true;

    IndexedDynamic() :
        data_(nullptr),
        capacity_(0),
        size_(0)
    {}

    IndexedDynamic(size_t length, const T &elem={}) :
        data_((T*) new char[length * sizeof(T)]),
        capacity_(length),
        size_(length)
    {
        if (!data_) throw std::bad_alloc();

        for (size_t i = 0; i < size_; ++i) {
            new(&data_[i]) T(elem);
        }
    }

    IndexedDynamic(std::initializer_list<T> list) :
        data_((T*) new char[list.size() * sizeof(T)]),
        capacity_(list.size()),
        size_(list.size())
    {
        size_t i = 0;
        for (auto it = list.begin(); it != list.end(); ++it, ++i) {
            new(&data_[i]) T(std::move(*it));
        }
    }

    IndexedDynamic(const IndexedDynamic &other) :
        data_((T*) new char[other.capacity_ * sizeof(T)]),
        capacity_(other.capacity_),
        size_(other.size_)
    {
        for (size_t i = 0; i < size_; ++i) {
            new(data_ + i) T(other.data(i));
        }
    }

    IndexedDynamic &operator=(const IndexedDynamic &other) {
        clear();

        data_ = (T*) calloc(other.capacity_, sizeof(T));
        if (!data_) throw std::bad_alloc();

        capacity_ = other.capacity_;
        size_ = other.size_;

        for (size_t i = 0; i < size_; ++i) {
            new(data_ + i) T(other.data(i));
        }

        return *this;
    }

    IndexedDynamic(IndexedDynamic &&other) :
        data_(std::move(other.data_)),
        capacity_(std::move(other.capacity_)),
        size_(std::move(other.size_))
    {
        other.clear();
    }

    IndexedDynamic &operator=(IndexedDynamic &&other) {
        clear();

        data_     = std::move(other.data_);
        capacity_ = std::move(other.capacity_);
        size_     = std::move(other.size_);

        other.data_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;

        return *this;
    }

    ~IndexedDynamic() {
        clear();
    }

    T *data_ptr() {
        return data_;
    }

    const T *data_ptr() const {
        return data_;
    }

    T &data(size_t i) {
        return data_[i];
    }

    const T &data(size_t i) const {
        return data_[i];
    }

// ============================================================================ capacity

    inline size_t size() const {
        return size_;
    }
    
    inline size_t capacity() const {
        return capacity_;
    }

    inline bool full() const {
        return size_ == capacity_;
    }

    void reserve(size_t capacity) {
        if (capacity_ >= capacity) return;
        set_capacity(capacity);
    }

    void shrink_to_fit() {
        set_capacity(size());
    }
// ============================================================================ modifirers

    T *expand_one() {
        int placement_i = increment_size() - 1;
        return &data(placement_i);
    }

    void extract_one() {
        data(size() - 1).~T();
        decrement_size();
    }

    void clear() {
        for (size_t i = 0; i < size(); ++i) {
            data_[i].~T();
        }

        delete[] data_;
        data_ = nullptr;

        capacity_ = 0;
        size_ = 0;
    }

    void resize(size_t new_size, const T &fill_elem={}) {
        while (size() > new_size) {
            extract_one();
        }

        while (size() < new_size) {
            *expand_one() = fill_elem;
        }
    }
};


} // namespace kctf::storage
