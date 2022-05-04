#pragma once

#include "helpers.h"

namespace kctf {

template <typename T>
class Function;

template<typename ReturnT, typename ... ArgsTs>
class Function<ReturnT (ArgsTs...)> {
    _impl::CallableBase<ReturnT, ArgsTs...> *callable_;

    using FunctorT = ReturnT (ArgsTs ...);

    void clear() {
        if (callable_) {
            delete callable_;
            callable_ = nullptr;
        }
    }

    Function copy() const {
        return Function(*callable_);
    }

public:
    Function() : callable_(nullptr) {
    }

    template<typename FuncT>
    Function(FuncT &&functor)
        : callable_(new _impl::Callable<FuncT, ReturnT, ArgsTs...>(std::forward<FuncT>(functor)))
    {}

    ~Function() {
        delete callable_;
    }

    Function(const Function &other) : callable_(nullptr) {
        *this = other.copy();
    }

    Function(const Function &&other) : callable_(nullptr) {
        std::swap(callable_, other.callable_);
    }

    Function &operator=(const Function &other) {
        if (this == &other) {
            return *this;
        }

        clear();
        *this = std::move(other.copy());

        return *this;
    }

    Function &operator=(Function &&other) {
        if (this == &other) {
            return *this;
        }

        std::swap(callable_, other.callable_);

        return *this;
    }

// ============================================================================ operators

    operator bool() {
        return callable_;
    }

    bool operator==(const Function &/*other*/) {
        return !(*this); 
    }

    ReturnT operator()(ArgsTs... args) const {
        if (!callable_) {
            throw std::bad_function_call();
        }

        return (*callable_)(std::forward<ArgsTs>(args)...);
    }

    const std::type_info &target_type() {
        if (!callable_) {
            return typeid(void);
        }

        return callable_->target_type();
    }

    template <typename MaybeTargetT>
    FunctorT *target() {
        if (typeid(MaybeTargetT) == target_type()) {
            return callable_;
        } else {
            return nullptr;
        }
    }

// ============================================================================

    void swap(Function<ReturnT (ArgsTs...)> &other) {
        printf("!!!\n");
        std::swap(callable_, other.callable_);
    }

};

template<typename ReturnT, typename ... ArgsTs>
Function(ReturnT (*)(ArgsTs...)) -> Function<ReturnT (ArgsTs...)>;

template <typename F>
Function(F f) -> Function<typename _impl::FunctorTypeDeductionHelper<decltype(&F::operator())>::type>;

}
