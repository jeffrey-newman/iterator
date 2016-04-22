//
//=======================================================================
// Copyright 2015
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// index_sequence is now part of C++14

#ifndef BLINK_UTILITY_INDEX_SEQUENCE_H_AHZ
#define BLINK_UTILITY_INDEX_SEQUENCE_H_AHZ

#include <cstddef> //size_t
namespace blink {
  namespace utility {

    template<std::size_t ...>
    struct index_sequence { };

    namespace detail {
      template<std::size_t N, std::size_t ...S>
      struct make_index_sequence_helper : make_index_sequence_helper < N - 1, N - 1, S... >
      { };

      template<std::size_t ...S>
      struct make_index_sequence_helper < 0, S... >
      {
        using type = index_sequence < S... > ;
      };
    }

    template<std::size_t N>
    using make_index_sequence = typename detail::make_index_sequence_helper<N>::type;
  }
}
#endif