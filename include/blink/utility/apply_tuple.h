//
//=======================================================================
// Copyright 2015
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// Check here for a simpler solution (but C++14): 
// http://www.cppsamples.com/common-tasks/apply-tuple-to-function.html

#ifndef BLINK_UTILITY_APPLY_TUPLE_H_AHZ
#define BLINK_UTILITY_APPLY_TUPLE_H_AHZ

#include <blink/utility/index_sequence.h> 
#include <tuple> 
#include <cstddef> //size_t

namespace blink {
  namespace utility {

    namespace detail {

      template<class F, class T, class S> struct return_type_helper{};

      template<class F, class T, std::size_t... S>
      struct return_type_helper<F, T, index_sequence<S...> >
      {
        using type = decltype(std::declval<std::decay_t<F> >()
          (std::declval< typename std::tuple_element<S, std::decay_t<T> >::type >()...));
      };

      template<class F, class T>
      using return_type = typename return_type_helper < F, T, make_index_sequence
        < std::tuple_size<std::decay_t<T> >::value> >::type;

      template<class F, class T, std::size_t... S>
      return_type<F, T> apply(F&& f, T&& t, index_sequence<S...>)
      {
        return f(std::get<S>(t)...);
      }

      template<class F, class T>
      return_type<F, T> apply(F&& f, T&& t)
      {
        return apply(std::forward<F>(f), std::forward<T>(t)
          , make_index_sequence < std::tuple_size<std::decay_t<T> >::value>{});
      }
    } // detail
    template<class F, class Tuple>
    using apply_tuple_return_type = typename detail::return_type<F, Tuple>;
    
    template<class F, class Tuple>
    apply_tuple_return_type<std::decay_t<F>, std::decay_t<Tuple>>
      apply_tuple(F&& f, Tuple&& t)
    {
      return detail::apply(std::forward<F>(f), std::forward<Tuple>(t));
    }
  } //utility
} //blink
#endif
