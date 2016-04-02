//
//=======================================================================
// Copyright 2015
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//index_in_filter

#ifndef BLINK_UTILITY_INDEX_IN_FILTER_TRANSFORM_H_AHZ
#define BLINK_UTILITY_INDEX_IN_FILTER_TRANSFORM_H_AHZ

#include<blink/utility/index_sequence.h>
#include <cstddef> //size_t

namespace blink {
  namespace utility {
  
    template<std::size_t First, std::size_t Second>
    struct index_pair
    {
      static const std::size_t first = First;
      static const std::size_t second = Second;
    };
    
    namespace detail {
      template<template<class> class  Predicate, class...Args>
      struct index_in_filter_transform
      {
        static const std::size_t N = sizeof...(Args);
        using arg_tuple = std::tuple < Args... > ;
        

        template<std::size_t I>
        struct include
        {
          static const bool value = Predicate < std::tuple_element<I, arg_tuple>::type >::value;
        };

        template<class Previous>
        struct next
        {
          static const std::size_t flat_index = Previous::flat_index + 1;
          static const std::size_t tuple_index = include<flat_index>::value ? 1 : 0;
          static const std::size_t next_first = Previous::next_first + 1 - tuple_index;
          static const std::size_t next_second = Previous::next_second + tuple_index;
          static const std::size_t next_index = include<flat_index>::value ? next_second : next_first;
          using pair = index_pair < tuple_index, next_index-1 >;
        };
               
        struct first
        {
          static const std::size_t flat_index = 0;
          static const std::size_t tuple_index = include<0>::value ? 1 : 0;
          static const std::size_t next_first = 0 + 1 - tuple_index;
          static const std::size_t next_second = 0 + tuple_index;
          using pair = index_pair < tuple_index, 0>;
        };

        template<std::size_t N, class Next, class...Pairs>
        struct index_in_filter_helper : public
          index_in_filter_helper < N - 1, next<Next>, Pairs..., typename Next::pair>
        {};
        
        template<class Next, class...Pairs>
        struct index_in_filter_helper <0, Next, Pairs... >
        {
          using type = std::tuple <Pairs... > ;
        };

        using type = typename index_in_filter_helper<N, first>::type;
      };
    }

    template<template<class> class Predicate, class...Args>
    using make_index_in_filter = typename detail::index_in_filter_transform<Predicate, Args...>::type;
  }
}
#endif