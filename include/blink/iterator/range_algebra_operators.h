//
//=======================================================================
// Copyright 2016
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// MACRO to provide operator overloads for range_algebra_wrappers

#ifndef BLINK_ITERATOR_RANGE_ALGEBRA_OPERATOR_MACRO_H_AHZ
#define BLINK_ITERATOR_RANGE_ALGEBRA_OPERATOR_MACRO_H_AHZ

#include <blink/iterator/range_algebra_transform.h>
#include <blink/iterator/range_algebra_wrapper.h>
#include <blink/iterator/transform_range.h>
#include <functional>

// Overload raster_algebra operator "op" in namespace "space" and ties it to 
// function "func". The operato "op" is subsequently pulled into the current 
// namespace, which MUST be blink::iterator.

// This is an implementation detail. Not part of the public interface

#define BLINK_ITERATOR_RA_BINARY_OP(space, op,func)                           \
  namespace space {                                                           \
                                                                              \
    using fun = func;                                                         \
                                                                              \
    template<class R> using wrap = range_algebra_wrapper<R>;                  \
    template<class T> using dec = special_decay_t<T>;                         \
                                                                              \
    template<class A, class B>                                                \
    using trans1 = range_algebra_transform<fun, A, B>;                        \
                                                                              \
    template<class A, class B>                                                \
    using trans2 = transform_range<fun, A, B>;                                \
                                                                              \
    using std::move;                                                          \
    using std::ref;                                                           \
    using std::forward;                                                       \
                                                                              \
    template<class R, class T>                                                \
    wrap< trans1< wrap<R>, dec<T> > >                                         \
      operator op(wrap<R>&& r, T&& v)                                         \
    {                                                                         \
      return range_algebra_function(fun{}, move(r), forward<T>(v));           \
    }                                                                         \
                                                                              \
    template<class R, class T>                                                \
    wrap< trans1< wrap<R>&, dec<T> > >                                        \
    operator op(wrap<R>& r, T&& v)                                            \
    {                                                                         \
      return range_algebra_function(fun{}, ref(r), forward<T>(v));            \
    }                                                                         \
                                                                              \
    template<class R, class T>                                                \
    wrap< trans1< dec<T>, wrap<R> > >                                         \
    operator op(T&& v, wrap<R>&& r)                                           \
    {                                                                         \
      return range_algebra_function(fun{}, forward<T>(v), move(r));           \
    }                                                                         \
    template<class R, class T>                                                \
    wrap< trans1< dec<T>, wrap<R>& > >                                        \
      operator op(T&& v, wrap<R>& r)                                          \
    {                                                                         \
      return range_algebra_function(fun{}, forward<T>(v), ref(r));            \
    }                                                                         \
                                                                              \
    template<class R1, class R2>                                              \
    wrap< trans2< wrap<R1>, wrap<R2> > >                                      \
    operator op(wrap<R1>&& r1, wrap<R2>&& r2)                                 \
    {                                                                         \
      return range_algebra(make_transform_range(fun{}, move(r1), move(r2)));  \
    }                                                                         \
                                                                              \
    template<class R1, class R2>                                              \
    wrap< trans2< wrap<R1>, wrap<R2>& > >                                     \
      operator op(wrap<R1>&& r1, wrap<R2>& r2)                                \
    {                                                                         \
      return range_algebra(make_transform_range(fun{}, move(r1), ref(r2)));   \
    }                                                                         \
                                                                              \
    template<class R1, class R2>                                              \
    wrap< trans2< wrap<R1>&, wrap<R2> > >                                     \
      operator op(wrap<R1>& r1, wrap<R2>&& r2)                                \
    {                                                                         \
      return range_algebra(make_transform_range(fun{}, ref(r1), move(r2)));   \
    }                                                                         \
                                                                              \
    template<class R1, class R2>                                              \
    wrap< trans2< wrap<R1>&, wrap<R2>& > >                                    \
      operator op(wrap<R1>& r1, wrap<R2>& r2)                                 \
    {                                                                         \
      return range_algebra(make_transform_range(fun{}, ref(r1), ref(r2)));    \
    }                                                                         \
  }                                                                           \
  using space::operator op;


#define BLINK_ITERATOR_RA_UNARY_OP(space, op,func)                            \
  namespace space {                                                           \
                                                                              \
    template<class R> using wrap = range_algebra_wrapper<R>;                  \
    using fun = func;                                                         \
                                                                              \
    template<class R>                                                         \
    range_algebra_wrapper< transform_range<fun, wrap<R>& > >                  \
      operator op(wrap<R>& r)                                                 \
    {                                                                         \
      return range_algebra(make_transform_range(fun{}, std::ref(r)));         \
    }                                                                         \
                                                                              \
    template<class R>                                                         \
    range_algebra_wrapper< transform_range<fun, wrap<R> > >                   \
      operator op(wrap<R>&& r)                                                \
    {                                                                         \
      return range_algebra(make_transform_range(fun{}, std::move(r)));        \
    }                                                                         \
  }                                                                           \
  using space::operator op;

namespace blink {
  namespace iterator {

    BLINK_ITERATOR_RA_BINARY_OP(plus_space, +, std::plus<>)
    BLINK_ITERATOR_RA_BINARY_OP(minus_space, -, std::minus<>)
    BLINK_ITERATOR_RA_BINARY_OP(divides_space, / , std::divides<>)
    BLINK_ITERATOR_RA_BINARY_OP(modulus_space, %, std::modulus<>)
    BLINK_ITERATOR_RA_BINARY_OP(multiplies_space, *, std::multiplies<>)
    BLINK_ITERATOR_RA_BINARY_OP(logical_and_space, &&, std::logical_and<>)
    BLINK_ITERATOR_RA_BINARY_OP(logical_or_space, || , std::logical_or<>)
    BLINK_ITERATOR_RA_BINARY_OP(greater_space, > , std::greater<>)
    BLINK_ITERATOR_RA_BINARY_OP(greater_equal_space, >= , std::greater_equal<>)
    BLINK_ITERATOR_RA_BINARY_OP(less_space, < , std::less<>)
    BLINK_ITERATOR_RA_BINARY_OP(less_equal_space, <= , std::less_equal<>)
    BLINK_ITERATOR_RA_BINARY_OP(equal_to_space, == , std::equal_to<>)
    BLINK_ITERATOR_RA_BINARY_OP(not_equal_to_space, != , std::not_equal_to<>)

    BLINK_ITERATOR_RA_UNARY_OP(negate_space, -, std::negate<>)
    BLINK_ITERATOR_RA_UNARY_OP(logical_not_space, ! , std::logical_not<>)
  }
}

#endif