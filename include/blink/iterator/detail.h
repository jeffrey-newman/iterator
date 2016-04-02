//
//=======================================================================
// Copyright 2015
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// some basic Meta Programming for which I do not have a good place
// quite some is obsolete with c++14
//

#ifndef BLINK_ITERATOR_DETAIL_H_AHZ
#define BLINK_ITERATOR_DETAIL_H_AHZ

#include <tuple>
#include <utility>

namespace blink {
  namespace iterator {

    // some basic Meta Programming for which I do not have a good place
    namespace detail {

      // TODO use std::iterator_traits? requires full set of traits for indicators, including pointer and iterator_category
      template<class Iterator> struct get_reference { using type = typename Iterator::reference; };
      template<class Iterator> using get_reference_t = typename get_reference<Iterator>::type;
      template<class Iterator> struct get_value_type { using type = typename Iterator::value_type; };
      template<class Iterator> using get_value_type_t = typename get_value_type<Iterator>::type;

      template<class Range> struct get_iterator { using type = typename Range::iterator; };
      template<class Range> using get_iterator_t = typename get_iterator<Range>::type;
      
      template<class T> using get_type = typename T::type;

      template<class T> 
      using remove_reference_t = typename std::remove_reference<T>::type;

      template<class T>
      using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;

      template<class T>
      using decay_t = typename std::decay<T>::type;

      template<typename...Args> void do_nothing(const Args&...)
      {}

      template<std::size_t I, class Tuple>
      using tuple_element_t = typename std::tuple_element<I, Tuple>::type;

      template <class T>
      struct unwrap_refwrapper
      {
        using type = T;
      };

      template <class T>
      struct unwrap_refwrapper<std::reference_wrapper<T>>
      {
        using type = T&;
      };

      template <class T>
      using special_decay_t = typename unwrap_refwrapper<typename std::decay<T>::type>::type;

      template<template<class...> class, class>  struct apply_to_pack{};
      template<template<class...> class F, template<class...> class Pack, class... Elements>
      struct apply_to_pack < F, Pack<Elements...> >
      {
        using type = Pack < F<Elements>... > ;
      };

      template<template<class...> class F, class Pack>
      using apply_to_pack_t = typename apply_to_pack < F, Pack >::type;


    }
    using detail::do_nothing;
    using detail::special_decay_t;
    using detail::decay_t;
    using detail::get_iterator_t;
    using detail::get_reference_t;
    using detail::get_value_type_t;
    using detail::remove_reference_t;
    using detail::tuple_element_t;
    using detail::add_lvalue_reference_t;
    using detail::apply_to_pack_t;
  }
}

#endif