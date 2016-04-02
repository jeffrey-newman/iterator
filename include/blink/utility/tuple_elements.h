//
//=======================================================================
// Copyright 2015
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// get a number of elements (classes or values) from a tuple

#ifndef BLINK_TUPLE_ELEMENTS_H_AHZ
#define BLINK_TUPLE_ELEMENTS_H_AHZ

#include <blink/utility/index_sequence.h> 
#include <blink/iterator/detail.h>
#include <tuple> 
#include <cstddef> //size_t

namespace blink {
  namespace utility {
    using blink::iterator::apply_to_pack_t;
    using blink::iterator::add_lvalue_reference_t;


    template<class Tuple, std::size_t I>
    using tuple_element_t = typename std::tuple_element<I, Tuple>::type;

    template<class Tuple, class IntegerSequence>
    struct tuple_elements{};

    template<class Tuple, template<std::size_t...> class IS, std::size_t... S>
    struct tuple_elements<Tuple, IS<S...> >
    {
      
      using type = std::tuple < tuple_element_t<Tuple, S>... >;
      using ref_type = apply_to_pack_t < add_lvalue_reference_t, type >;
      
      static type get(Tuple& t)
      {
        return std::make_tuple(std::get<S>(t)...);
      }
      static type get_move(Tuple& t)
      {
        return std::make_tuple(std::move(std::get<S>(t))...);
      }

      static ref_type get_refer(Tuple& t)
      {
        //return std::make_tuple(std::ref(std::get<S>(t)))...);
        return ref_type(std::get<S>(t)...);
      }

    };

    template<class Tuple, class IntegerSequence>
    using tuple_elements_t = typename tuple_elements<Tuple, IntegerSequence>::type;
    
    template<class Tuple, class IntegerSequence>
    tuple_elements_t<Tuple, IntegerSequence > get_elements(Tuple& t, IntegerSequence)
    {
       return tuple_elements<Tuple, IntegerSequence>::get(t);
    }
    
    template<class Tuple, class IntegerSequence>
    tuple_elements_t<Tuple, IntegerSequence > move_elements(Tuple& t, IntegerSequence)
    {
      return tuple_elements<Tuple, IntegerSequence>::get_move(t);
    }

    template<class Tuple, class IntegerSequence>
    tuple_elements_t<apply_to_pack_t<add_lvalue_reference_t,Tuple>, IntegerSequence > refer_elements(Tuple& t, IntegerSequence)
    {
      return tuple_elements<Tuple, IntegerSequence>::get_refer(t);
    }
  } //utility
} //blink
#endif
