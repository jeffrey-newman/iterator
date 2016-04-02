//
//=======================================================================
// Copyright 2015
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// The transform iterator is a zip iterator that transforms the N zipped values
// using a N-ary function.

#ifndef BLINK_ITERATOR_TRANSFORM_ITERATOR_H_AHZ
#define BLINK_ITERATOR_TRANSFORM_ITERATOR_H_AHZ

#include <blink\iterator\detail.h>
#include <blink\utility\index_sequence.h>

#include <boost\iterator\iterator_facade.hpp>

#include <tuple>
#include <type_traits>
#include <utility>

namespace blink {
  namespace iterator {

    template<class Function, class... Ranges> struct transform_helper
    {
      //TODO: should use range_traits for value_type instead?
      using value_type = decltype(std::declval<Function>()(typename std::declval<
      remove_reference_t<special_decay_t<Ranges> > ::value_type>()...));
    };

    template < class Function, class Value, class... Iterators >
    class transform_iterator : public 
      boost::iterator_facade < transform_iterator<Function, Value, Iterators...>,
      Value,
      boost::random_access_traversal_tag,
      Value >
    {
      using iterators = std::tuple < Iterators... > ;
      using tuple_indices = blink::utility::make_index_sequence < std::tuple_size<iterators>::value > ;

      template<std::size_t I>
      using selected_iterator = tuple_element_t<I, iterators>;


    public:
      using reference = Value;

      transform_iterator(const transform_iterator& it) : m_iterators(it.m_iterators), m_f(it.m_f)
      { }

      template<class... InIterators>
      explicit transform_iterator(Function f, InIterators&&... its) 
        : m_f(f), m_iterators(std::forward<InIterators>(its)...)
      { }

      transform_iterator& operator=(const transform_iterator& it)
      {
        m_iterators = it.m_iterators;
        m_f = it.m_f;
        return *this;
      }
    private:
      friend boost::iterator_core_access;
      template<std::size_t ...S>
      Value dereference(blink::utility::index_sequence<S...>) const
      {
        return m_f(*get<S>()...);

      }
      Value dereference() const
      {
        return dereference(tuple_indices());
      }


      template<std::size_t ...S> void increment(blink::utility::index_sequence<S...>)
      {
        do_nothing(++get<S>()...);
      }

      void increment()
      {
        increment(tuple_indices());
      }

      template<std::size_t ...S> void decrement(blink::utility::index_sequence<S...>)
      {
        do_nothing(--get<S>()...);
      }

      void decrement()
      {
        decrement(tuple_indices());
      }

      template<std::size_t ...S> void advance(blink::utility::index_sequence<S...>, std::ptrdiff_t n)
      {
        do_nothing((get<S>() += n)...);
      }

      void advance(std::ptrdiff_t n)
      {
        advance(tuple_indices(), n);
      }

      template<class... OtherIterators >
      std::ptrdiff_t distance_to(const zip_iterator<OtherIterators...>& that) const
      {
        static_assert(std::is_same < std::tuple< decay_t<OtherIterators>...>
          , std::tuple< decay_t <Iterators>... > > ::value, "incompatible iterators");

        return std::distance(std::get<0>(m_iterators), that.get<0>());
      }

      template<class... OtherIterators >
      bool equal(const transform_iterator<Function, Value, OtherIterators...>& that) const
      {
        static_assert(std::is_same < std::tuple< decay_t<OtherIterators>...>
          , std::tuple< decay_t <Iterators>... > > ::value, "incompatible iterators");

        return std::get<0>(m_iterators) == that.get<0>();
      }


    public:
      // should really be implemented by facade, but it only compares iterator against const_iterator
      template<class... OtherIterators > //OtherIterators can be different in type of references, etc
      bool operator!=(const zip_iterator<OtherIterators...>& that) const
      {
        return !equal(that);
      }
      template<class... OtherIterators > //OtherIterators can be different in type of references, etc
      bool operator==(const zip_iterator<OtherIterators...>& that) const
      {
        return equal(that);
      }

      template<std::size_t I>
      selected_iterator<I>& get()
      {
        return std::get<I>(m_iterators);
      }

      template<std::size_t I>
      const selected_iterator<I>& get() const
      {
        return std::get<I>(m_iterators);
      }

    private:
      iterators m_iterators;
      Function m_f;
    };

   
    template<class Function, class...Iterators>
    using get_value_type2
      = decltype(std::declval(Function)(std::declval(Iterators::value_type)...));


    template<class Function, class...Iterators>  
    transform_iterator < 
      Function,
      get_value_type2<Function, Iterators...>,
      special_decay_t< Iterators>... >
      make_transform_iterator(Function fn, Iterators&&... its)
    {
      return transform_iterator <
        Function,
        get_value_type2<Function, Iterators...>,
        special_decay_t< Iterators>... >(fn, std::forward<Iterators>(its)...);
    }
  }
}
 
#endif