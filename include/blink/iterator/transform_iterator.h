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


//JN forward slash (backward slash incompatible with macos clang)
#include <blink/iterator/detail.h>
#include <blink/utility/index_sequence.h>
#include <blink/iterator/zip_iterator.h>

#include <boost/iterator/iterator_facade.hpp>

#include <tuple>
#include <type_traits>
#include <utility>

namespace blink {
  namespace iterator {

    template<class Function, class... Iterators>
    using transform_iterator_reference_t = decltype(std::declval<remove_reference_t<Function>>()(
      std::declval<get_reference_t<remove_reference_t<Iterators> > >()...));

    template<class Function, class... Iterators>
    struct transform_iterator_value_type
    {
      using reference = transform_iterator_reference_t<Function, Iterators...>;
      using type = remove_reference_t<reference>;
    };
    
    template<class Function, class... Iterators>
    using transform_iterator_value_type_t = 
      typename transform_iterator_value_type<Function, Iterators...>::type;

    template < class Function, class... Iterators >
    class transform_iterator : public 
      boost::iterator_facade 
      < transform_iterator<Function, Iterators...>
      , transform_iterator_value_type_t<Function, Iterators...>
      , boost::random_access_traversal_tag
      , transform_iterator_reference_t<Function, Iterators...> >
    {
      using iterators = std::tuple < Iterators... > ;
      static const std::size_t N = std::tuple_size<iterators>::value;
      using tuple_indices = blink::utility::make_index_sequence <N> ;

      template<std::size_t I>
      using selected_iterator = tuple_element_t<I, iterators>;


    public:
      using reference = transform_iterator_reference_t<Function, Iterators...>;
      using value_type = transform_iterator_value_type_t<Function, Iterators...>;

      transform_iterator(const transform_iterator& it) 
        : m_iterators(it.m_iterators), m_f(it.m_f)
      { }

      transform_iterator(transform_iterator&& it) 
        : m_iterators(std::move(it.m_iterators)), m_f(std::move(it.m_f))
      { }

      
      template<class InFunction, class... InIterators
        , typename = enable_if_t
        < std::is_same< special_decay_t<InFunction>, Function >::value > 
        , typename = enable_if_t
        < std::is_same
        < std::tuple<special_decay_t<InIterators>... >
        , std::tuple<Iterators... > >::value>
      >
      transform_iterator(InFunction&& f, InIterators&&... its)
        : m_f(std::forward<InFunction>(f))
        , m_iterators(std::forward<InIterators>(its)...)
      { }
      
      transform_iterator& operator=(const transform_iterator& it)
      {
        m_iterators = it.m_iterators;
        m_f = it.m_f;
        return *this;
      }

      transform_iterator& operator=(transform_iterator&& it)
      {
        m_iterators = std::move(it.m_iterators);
        m_f = std::move(it.m_f);
        return *this;
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
      friend boost::iterator_core_access;

      template<template<std::size_t...> class Pack, std::size_t ...S>
      value_type dereference(Pack<S...>) const
      {
        return m_f(*std::get<S>(m_iterators)...);

      }
      value_type dereference() const
      {
        return dereference(tuple_indices());
      }
      
      template<template<std::size_t...> class Pack, std::size_t ...S>
      void increment(Pack<S...>)
      {
        do_nothing(++std::get<S>(m_iterators)...);
      }

      void increment()
      {
        increment(tuple_indices());
      }

      template<template<std::size_t...> class Pack, std::size_t ...S>
      void decrement(Pack<S...>)
      {
        do_nothing(--get<S>()...);
      }

      void decrement()
      {
        decrement(tuple_indices());
      }

      template<template<std::size_t...> class Pack, std::size_t ...S> 
      void advance(Pack<S...>, std::ptrdiff_t n)
      {
        do_nothing((get<S>() += n)...);
      }

      void advance(std::ptrdiff_t n)
      {
        advance(tuple_indices(), n);
      }

      template<class OtherFunction, class... OtherIterators >
      std::ptrdiff_t distance_to(const transform_iterator<OtherFunction, OtherIterators...>& that) const
      {
        static_assert(std::is_same < std::tuple< decay_t<OtherIterators>...>
          , std::tuple< decay_t <Iterators>... > > ::value, "incompatible iterators");

        return std::distance(std::get<0>(m_iterators), that.get<0>());
      }

      template<class OtherFunction, class... OtherIterators >
      bool equal(const transform_iterator<OtherFunction, OtherIterators...>& that) const
      {
        static_assert(std::is_same < std::tuple< decay_t<OtherIterators>...>
          , std::tuple< decay_t <Iterators>... > > ::value, "incompatible iterators");

        return std::get<0>(m_iterators) == that.get<0>();
      }

    private:
      iterators m_iterators;
      Function m_f;
    };

    template<class Function, class...Iterators>  
    transform_iterator < 
      special_decay_t<Function>,
      special_decay_t<Iterators>... >
      make_transform_iterator(Function&& fn, Iterators&&... its)
    {
      return transform_iterator < special_decay_t<Function>,
        special_decay_t< Iterators>... >
        (std::forward<Function>(fn), std::forward<Iterators>(its)...);
    }
  }
}
 
#endif
