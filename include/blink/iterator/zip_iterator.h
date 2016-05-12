//
//=======================================================================
// Copyright 2015
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// Implements a zip iterato, it is probably not fully correct in providing all
// required iterator traits, but since these are ill-conceived 
// anyway I am not bothering about those.
// The zip_iterator can contain references to other iterators, but then it is not
// default constructible, so depending on your use will be either input or output 
// iterator, not even forward iterator.
// The reference type is not equal to value_type&, therefore at best this is a 
// forward iterator.

#ifndef BLINK_ITERATOR_ZIP_ITERATOR_H_AHZ
#define BLINK_ITERATOR_ZIP_ITERATOR_H_AHZ

#include <blink/iterator/detail.h>
#include <blink/utility/index_sequence.h>

#include <boost/iterator/iterator_facade.hpp>

#include <tuple>
#include <type_traits>
#include <utility>

namespace blink {
  namespace iterator {

    template < class... Iterators >
    class zip_iterator : public 
      boost::iterator_facade < zip_iterator<Iterators...>,
      std::tuple<get_value_type_t<remove_reference_t<Iterators> >...>,
      boost::random_access_traversal_tag,
      std::tuple<get_reference_t<remove_reference_t<Iterators> >...> >
    {
// get_reference_t instead of ::reference is workaround for a Visual Studio bug
// http://stackoverflow.com/questions/23347287/parameter-pack-expansion-fails

      using iterators = std::tuple < Iterators... > ;
      using tuple_indices = blink::utility::make_index_sequence < 
        std::tuple_size<iterators>::value > ;

      template<std::size_t I>
      using selected_iterator = tuple_element_t<I, iterators> ;


    public:
      using reference = 
        std::tuple<get_reference_t<remove_reference_t<Iterators> >...>;
      
      zip_iterator(const zip_iterator& it) : m_iterators(it.m_iterators)
      { }

      // This constructor was only made to satisfy the is_convertible type_trait.
      // Which in turn enables comparison operators in boost facade
      // It is not really intended to be used...
      
      template < typename... InIterators, typename = enable_if_t<
        std::is_same
        < std::tuple<decay_t<InIterators>... >
        , std::tuple<Iterators... > >::value>>
      zip_iterator(const zip_iterator<InIterators...>& that)
        : m_iterators(that.m_iterators)
      {
        assert(false); 
        // TODO: remove this assert, if we find that this constructor actually
        // has a valid use.
      }

      template < typename... InIterators, typename = enable_if_t<
        std::is_same 
        < std::tuple<special_decay_t<InIterators>... >
        , std::tuple<Iterators... > >::value> >
      zip_iterator(InIterators&&... its) 
        : m_iterators(std::forward<InIterators>(its)...)
      { }

      zip_iterator& operator=(const zip_iterator& it)
      {
        m_iterators = it.m_iterators;
        return *this;
      }

      zip_iterator& operator=(zip_iterator&& it)
      {
        m_iterators = std::move(it.m_iterators);
        return *this;
      }
    private:
      friend boost::iterator_core_access;
      template <class...> friend class zip_iterator;
      
      template<template<std::size_t...> class Pack, std::size_t ...S>
      reference dereference(Pack<S...>) const
      {
        return reference(*get<S>()...);

      }
      reference dereference() const
      {
        return dereference(tuple_indices());
      }


      template<template<std::size_t...> class Pack, std::size_t ...S> 
      void increment(Pack<S...>)
      {
        do_nothing(++get<S>()...);
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

      template<class... OtherIters >
      std::ptrdiff_t distance_to(const zip_iterator<OtherIters...>& that) const
      {
        static_assert(std::is_same < std::tuple< decay_t<OtherIters>...>
          , std::tuple<decay_t<Iterators>... > > ::value
          , "incompatible iterators");

        return std::distance(std::get<0>(m_iterators), that.get<0>());
      }

      template<class... OtherIters >
      bool equal(const zip_iterator<OtherIters...>& that) const
      {
        return equal_first(that);
      }


      template<class... OtherIters >
      bool equal_first(const zip_iterator<OtherIters...>& that) const
      {
        static_assert(std::is_same < std::tuple< decay_t<OtherIters>...>
          , std::tuple < decay_t <Iterators>... > > ::value
          , "incompatible iterators");

        return std::get<0>(m_iterators) == that.get<0>();
      }
      /*
      template<class... OtherIters >
      bool equal_any(const zip_iterator<OtherIters...>& that) const
      {
        static_assert(std::is_same < std::tuple< decay_t<OtherIters>...>
          , std::tuple < decay_t <Iterators>... > > ::value
          , "incompatible iterators");

        // return std::get<0>(m_iterators) == that.get<0>();
      }
      */


    public:

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
    };

    template<class...Iterators>  
    zip_iterator < special_decay_t < Iterators>... >  
      make_zip_iterator(Iterators&&... its)
    {
      return zip_iterator < special_decay_t < Iterators>... >(
        std::forward<Iterators>(its)...);
    }
  }
}
 
#endif