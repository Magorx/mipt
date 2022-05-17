#pragma once

#include <concepts>
#include <typeinfo>

namespace kctf {

namespace _impl {

    template<typename ReturnT, typename ... ArgsTs>
    class CallableBase {
    public:
        virtual ~CallableBase() {}

        virtual ReturnT operator()(ArgsTs...) = 0;
        virtual const std::type_info &target_type() = 0;

        virtual void *copy() = 0;
    };

    template <typename FuncT, typename ReturnT, typename ... ArgsTs>
    class Callable : public CallableBase<ReturnT, ArgsTs...> {
        FuncT functor_;
    public:
        Callable(FuncT functor) : functor_(functor) {}

        virtual void *copy() override {
            return new Callable(functor_);
        }

        virtual ReturnT operator()(ArgsTs... args) override {
            return functor_(std::forward<ArgsTs>(args)...);
        }

        virtual const std::type_info &target_type() override {
            return typeid(FuncT);
        }
    };

    


// ============================================================================ Explicit deduction help


    template <typename F>
    struct FunctorTypeDeductionHelper;

    #define HelperGuideline_(modifiers)                                 \
    template <typename Cls, typename R, typename ... As>                \
    struct FunctorTypeDeductionHelper<R (Cls::*)(As...) modifiers> {    \
        using type = R (As...);                                         \
    };                                                                  \

    HelperGuideline_()
    HelperGuideline_(const)
    HelperGuideline_(noexcept)
    HelperGuideline_(const noexcept)

    HelperGuideline_(&)
    HelperGuideline_(const &)
    HelperGuideline_(& noexcept)
    HelperGuideline_(const & noexcept)

    #undef HelperGuideline_

}

}
