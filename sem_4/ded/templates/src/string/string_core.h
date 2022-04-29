#pragma once


#include "allocator/Allocator.h"


namespace kctf
{


template <typename DataT>
struct SharedData {
    DataT data_;
    size_t viewers_ = 0;

    SharedData(DataT &&data) : data_(std::move(data)) {}

    void view() {
        ++viewers_;
    }

    bool unview() {
        return (--viewers_) == 0;
    }
};

template <typename DataT>
class SharedDataPtr {
    SharedData<DataT> *ptr_;

public:

    SharedDataPtr(SharedData<DataT> *ptr) : ptr_(ptr) {
        ptr_->view();
    }

    SharedDataPtr(SharedDataPtr &other) : ptr_(other.ptr_) {
        ptr_->view();
    }

    SharedDataPtr &operator=(SharedDataPtr &other) {
        ptr_ = other.ptr_;
        ptr_->view();
        return *this;
    }

    SharedDataPtr(SharedDataPtr &&other) : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    SharedDataPtr &operator=(SharedDataPtr &&other) {
        std::swap(ptr_, other.ptr_);
        return *this;
    }

    ~SharedDataPtr() {
        if (!ptr_) return;

        if (ptr_->unview()) {
            delete ptr_;
        }
    }

    void view() {
        ptr_->view();
    }

    void unview() {
        ptr_->unview();
    }

    DataT &grab() {
        ptr_->unview();
        return ptr_->data_;

        // if (ptr_->unview()) {
        //     auto data = std::move(ptr_->data_);
        //     delete ptr_;
        //     return std::move(ptr_->data_);
        // } else {
        //     auto copy = ptr_->data_;
        //     return std::move(copy);
        // }
    }

    bool is_last() {
        return ptr_->viewers_ == 1;
    }

    DataT operator*() const {
        return ptr_->data_;
    }

    DataT* operator->() const {
        return &ptr_->data_;
    }

    SharedData<DataT> *get_ptr() {
        return ptr_;
    }
};


template <typename CharT, template <typename U>  typename Allocator = allocator::Simple>
class StringCore {
private:
    friend SharedDataPtr<StringCore>;
    friend SharedData<StringCore>;

    constexpr static size_t CHAR_SIZE = sizeof(CharT);
    constexpr static size_t MIN_CAPACITY = 4;
    constexpr static size_t SMALL_CAPACITY = sizeof(CharT*);

    Allocator<CharT> allocator_;

    union {
        CharT *data_;
        CharT small_data_[SMALL_CAPACITY];
        SharedDataPtr<StringCore> view_data_;
    };

    enum DataState {
        owner,
        small,
        view
    };
    DataState state_ = DataState::owner;

    size_t capacity_;
    size_t size_;

    void set_capacity(size_t new_capacity) {
        grab_if_view();

        CharT *new_data = allocator_.allocate(new_capacity);

        if (!new_data) {
            throw std::overflow_error("Can't allocate growed buffer for StringCore");
        }

        if (data_) {
            size_t to_copy = std::min(capacity_, new_capacity);
            memcpy(new_data, is_small() ? small_data_ : data_, to_copy * CHAR_SIZE);

            for (size_t i = to_copy; i < new_capacity; ++i) {
                new_data[i] = '\0';
            }

            if (!is_small()) {
                allocator_.deallocate(data_, capacity_);
            }
        }

        data_ = new_data;
        capacity_ = new_capacity;
        state_ = DataState::owner;
    }

    void grow_capacity(float coef=2) {
        grab_if_view();

        if (!capacity_) {
            capacity_ = SMALL_CAPACITY;
        } else {
            size_t new_capacity = capacity_ * coef;
            set_capacity(new_capacity);
        }
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

    inline bool can_be_small() const {
        return capacity_ <= SMALL_CAPACITY;
    }

    inline bool is_owner() const {
        return state_ == DataState::owner;
    }

    inline bool is_small() const {
        return state_ == DataState::small;
    }

    inline bool is_view() const {
        return state_ == DataState::view;
    }

    void alloc_capacity_data() {
        data_ = allocator_.allocate(capacity_);
        if (!data_) throw std::bad_alloc();
    }

    void dealloc_capacity_data() {
        if (is_small()) return;

        allocator_.deallocate(data_, capacity_);
        data_ = nullptr;
    }

    void copy_content_from(const StringCore &other) {
        if (!is_small() && other.state_ == DataState::view) {
            state_ = other.state_;
            view_data_ = const_cast<SharedDataPtr<StringCore>&>(other.view_data_);
        } else {
            for (size_t i = 0; i < size_; ++i) {
                data_at(i) = other.data_at(i);
            }
        }
    }

    void copy_from(const SharedDataPtr<StringCore> &view_data) {
        capacity_ = view_data->capacity_;
        size_ = view_data->size_;
        allocator_ = view_data->allocator_;

        if (!can_be_small()) {
            alloc_capacity_data();
            state_ = DataState::owner;
        } else {
            state_ = DataState::small;
        }

        for (size_t i = 0; i < size_; ++i) {
            data_at(i) = view_data->data_at(i);
        }
    }

    void move_data_from(StringCore &other) {
        if (other.state_ == DataState::view) {
            view_data_ = std::move(other.view_data_);
        } else {
            data_ = std::move(other.data_);
            other.data_ = nullptr;
        }
    }

    void become_view() {
        if (is_owner()) {
            auto shared_data = new SharedData<StringCore>(std::move(*this));
            new(&view_data_) SharedDataPtr<StringCore>(shared_data);
            
            view_to(view_data_, false);
        }
    }

    void view_to(SharedDataPtr<StringCore> view_data, bool to_copy_ptr=true) {
        state_ = DataState::view;
        
        if (to_copy_ptr) {
            view_data_ = view_data;
        }

        size_ = view_data_->size();
        capacity_ = view_data_->capacity();
    }

    void grab_if_view() {
        if (!is_view()) {
            return;
        }

        SharedDataPtr<StringCore> view_data = std::move(view_data_);
        view_data_.~SharedDataPtr<StringCore>();

        if (view_data.is_last()) {
            (*this) = std::move(view_data.grab());
        }

        copy_from(view_data);
    }

protected:
    StringCore() :
        data_(nullptr),
        state_(DataState::small),
        capacity_(0),
        size_(0)
    {}

    explicit StringCore(size_t length, const CharT &elem={}) :
        data_(nullptr),
        capacity_(length),
        size_(length)
    {
        if (!can_be_small()) {
            alloc_capacity_data();
        } else {
            state_ = DataState::small;
        }

        for (size_t i = 0; i < size_; ++i) {
            data_at(i) = (CharT) elem;
        }
    }

    explicit StringCore(std::initializer_list<CharT> list) :
        data_(nullptr),
        capacity_(list.size()),
        size_(list.size())
    {
        if (!can_be_small()) {
            alloc_capacity_data();
        } else {
            state_ = DataState::small;
        }

        size_t i = 0;
        for (auto it = list.begin(); it != list.end(); ++it, ++i) {
            data_at(i) = *it;
        }
    }

    StringCore(StringCore &other) :
        data_(nullptr),
        state_(other.state_),
        capacity_(other.capacity_),
        size_(other.size_)
    {
        if (!can_be_small()) {
            other.become_view();
            view_to(other.view_data_);
            return;
        } else {
            state_ = DataState::small;
        }

        copy_content_from(other);
    }

    StringCore(const StringCore &other) :
        data_(nullptr),
        state_(other.state_),
        capacity_(other.capacity_),
        size_(other.size_)
    {
        if (!can_be_small()) {
            alloc_capacity_data();
        } else {
            state_ = DataState::small;
        }

        copy_content_from(other);
    }

    StringCore &operator=(StringCore &other) {
        if (&other == this) return *this;

        clear();

        capacity_ = other.capacity_;
        state_ = other.state_;
        size_ = other.size_;

        if (!can_be_small()) {
            other.become_view();
            view_to(other.view_data_);
            return *this;
        } else {
            state_ = DataState::small;
        }

        copy_content_from(other);

        return *this;
    }

    StringCore &operator=(const StringCore &other) {
        if (&other == this) return *this;

        clear();

        capacity_ = other.capacity_;
        state_ = other.state_;
        size_ = other.size_;

        if (!can_be_small()) {
            alloc_capacity_data();
        } else {
            state_ = DataState::small;
        }

        copy_content_from(other);

        return *this;
    }

    StringCore(StringCore &&other) :
        data_(nullptr),
        state_(other.state_),
        capacity_(other.capacity_),
        size_(other.size_)
    {
        if (&other == this) return;

        if (!can_be_small()) {
            move_data_from(other);
        } else {
            state_ = DataState::small;
            copy_content_from(other);
        }

        other.clear();
    }

    StringCore &operator=(StringCore &&other) {
        if (&other == this) return *this;

        clear();

        capacity_ = other.capacity_;
        size_     = other.size_;

        if (!can_be_small()) {
            move_data_from(other);
        } else {
            state_ = DataState::small;
            copy_content_from(other);
        }

        other.data_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;

        return *this;
    }

    ~StringCore() {
        clear();
    }

    const StringCore &c() const {
        return *this;
    }

    CharT *data_ptr() {
        grab_if_view();

        if (is_small()) {
            return (CharT*) &small_data_;
        } else if (is_view()) {
            return view_data_->data_ptr();
        } else {
            return data_;
        }
    }

    const CharT *data_ptr() const {
        if (is_small()) {
            return (const CharT*) &small_data_;
        } else if (is_view()) {
            return view_data_->data_ptr();
        } else {
            return data_;
        }
    }

    CharT &data_at(size_t i) {
        grab_if_view();

        if (is_small()) {
            return small_data_[i];
        } else {
            return data_[i];
        }
    }

    const CharT &data_at(size_t i) const {
        if (is_small()) {
            return small_data_[i];
        } else if (is_view()) {
            return view_data_->data_at(i);
        } else {
            return data_[i];
        }
    }

// ============================================================================ capacity

    inline size_t size() const {
        return size_;
    }
    
    inline size_t capacity() const {
        return capacity_;
    }

    inline bool full() const {
        return !capacity_ || size_ >= capacity_ - 1;
    }

    void reserve(size_t capacity) {
        grab_if_view();

        if (capacity_ >= capacity || capacity < SMALL_CAPACITY) return;
        set_capacity(capacity);
    }

    bool shrink_to_small() {
        if (size() >= SMALL_CAPACITY - 1) {
            return false;
        }

        CharT tmp_data[SMALL_CAPACITY] = {};
        for (size_t i = 0; i < SMALL_CAPACITY - 1; ++i) {
            tmp_data[i] = c().data_at(i);
        }

        if (is_owner()) {
            dealloc_capacity_data();
        }
        state_ = DataState::small;

        memcpy(small_data_, tmp_data, SMALL_CAPACITY);
        return true;
    }

    void shrink_to_fit(bool enforced=false) {
        if (shrink_to_small() || (is_view() && !enforced)) {
            return;
        }
        grab_if_view();

        set_capacity(size());
    }

// ============================================================================ iterators

    CharT *begin()  {
        grab_if_view();

        return data_;
    }

    CharT *end() {
        grab_if_view();

        return data_ + size();
    }

    CharT *begin() const {
        return data_;
    }

    CharT *end() const {
        return data_ + size();
    }

// ============================================================================ modifirers

    inline CharT *expand_one() {
        grab_if_view();

        int placement_i = increment_size() - 1;
        return &data_at(placement_i);
    }

    inline void extract_one() {
        grab_if_view();

        decrement_size();
        data_at(size()) = '\0';
    }

    void clear() {
        if (is_owner()) {
            dealloc_capacity_data();
        } else if (is_view()) {
            view_data_.~SharedDataPtr<StringCore>();
        }

        state_ = DataState::small;
        capacity_ = 0;
        size_ = 0;
    }

    void resize(size_t new_size, const CharT &fill_elem={}) {
        while (size() > new_size) {
            extract_one();
        }

        while (size() < new_size) {
            *expand_one() = fill_elem;
        }
    }

    void swap(StringCore<CharT, Allocator> &other) {
        grab_if_view();

        std::swap(allocator_, other.allocator_);
        std::swap(data_, other.data_);
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }
};

} // namespace kctf::storage
