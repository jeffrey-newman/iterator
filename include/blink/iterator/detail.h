//
//=======================================================================
// Copyright 2015
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// Implements a zip range, it is probably not fully correct in providing all
// required range and iterator traits, but since these are ill-conceived 
// anyway I am not bothering about those.

#ifndef BLINK_ITERATOR_DETAIL_H_AHZ
#define BLINK_ITERATOR_DETAIL_H_AHZ

#include <utility>

namespace blink {
  namespace iterator {

    // some basic Meta Programming for which I do not have a good place
    namespace detail {

      // TODO use std::iterator_traits? requires full set of traits for indicators, including pointer and iterator_category
      template<class Iterator> struct get_reference { using type = typename Iterator::reference; };
      template<class Iterator> struct get_value_type { using type = typename Iterator::value_type; };
      template<class Range> struct get_iterator { using type = typename Range::iterator; };

      template<class ...Ts> struct pack{};

      template<class T> using get_type = typename T::type;
      template<class T> struct unwrap_ { using type = T; };
      template<class T> struct unwrap_ < std::reference_wrapper<T> > { using type = T&; };
      template<class T> using unwrap = get_type < detail::unwrap_<T> > ;

      template<class T> struct unwrap_unref_ { using type = typename std::remove_reference<unwrap<T> >::type; };
      template<class T> using unwrap_unref = get_type < detail::unwrap_unref_<T> > ;
      template<class T>
      using unwrap_decay = get_type < std::decay< unwrap<T> > > ;

      template<typename...Args> void do_nothing(const Args&...)
      {}

    }

    using detail::unwrap;
    using detail::unwrap_unref;
    using detail::unwrap_decay;
    using detail::do_nothing;
    using detail::get_type;
    using detail::pack;
    using detail::get_iterator;
    using detail::get_value_type;
    using detail::get_reference;

  }
}

#endif