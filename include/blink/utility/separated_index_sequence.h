//
//=======================================================================
// Copyright 2015
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//

#ifndef BLINK_UTILITY_FILTER_INDEX_SEQUENCE_H_AHZ
#define BLINK_UTILITY_FILTER_INDEX_SEQUENCE_H_AHZ

#include<blink/utility/index_sequence.h>
#include <cstddef> //size_t

namespace blink {
  namespace utility {
    namespace detail {
      template<template<class> class Predicate, class... Args>
      struct filter_index_sequence
      {

        using arg_tuple = std::tuple<Args...>;

        template<std::size_t I>
        struct include
        {
          static const bool value = Predicate< std::tuple_element<I, arg_tuple>::type >::value;
        };

        template<std::size_t N, bool Include, std::size_t ...S>
        struct make_index_sequence_helper{};

        template<std::size_t N, std::size_t ...S>
        struct make_index_sequence_helper<N, true, S...>
          : make_index_sequence_helper < N - 1, include<N - 1>::value, N, S... >
        {};

        template<std::size_t N, std::size_t ...S>
        struct make_index_sequence_helper<N, false, S...>
          : make_index_sequence_helper < N - 1, include<N - 1>::value, S... >
        {};

        template<std::size_t ...S>
        struct make_index_sequence_helper < 0, true, S... >
        {
          using type = index_sequence < 0, S... > ;
        };

        template<std::size_t ...S>
        struct make_index_sequence_helper < 0, false, S... >
        {
          using type = index_sequence <S... > ;
        };

        using type = typename make_index_sequence_helper < sizeof...(Args), false >::type;
      };
    }

    template<template<class> class Predicate, class...Args>
    using make_filter_index_sequence = typename detail::filter_index_sequence<Predicate, Args...>::type;
    
  }
}
#endif