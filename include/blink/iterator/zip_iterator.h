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
// The reference type is not equal to the value type, therefore at best this is a 
// forward iterator, even if it can be decremented, advanced, etc. However, using 
// the forward_tag in boost::iterator facade would make it disable all of those.

#ifndef BLINK_ITERATOR_ZIP_ITERATOR_H_AHZ
#define BLINK_ITERATOR_ZIP_ITERATOR_H_AHZ

#include <blink\iterator\detail.h>
#include <boost\iterator\iterator_facade.hpp>

#include <tuple>
#include <type_traits>
#include <utility>

namespace blink {
  namespace iterator {

    template < class... Iterators >
    class zip_iterator : public  //detail::zip_iterator_helper < Iterators... >::facade
      boost::iterator_facade < zip_iterator<Iterators...>,
      std::tuple<get_type<get_value_type<get_type < std::remove_reference<Iterators> > > >...>,
      boost::random_access_traversal_tag,
      std::tuple<get_type<get_reference<get_type< std::remove_reference<Iterators> > > >...> >
    {
      // get_reference instead of ::reference is workaround for a Visual Studio bug
      // http://stackoverflow.com/questions/23347287/parameter-pack-expansion-fails

      using iterators = std::tuple < Iterators... > ;
      using tuple_indices = blink::utility::make_index_sequence < std::tuple_size<iterators>::value > ;

      template<std::size_t I>
      using selected_iterator = get_type < std::tuple_element<I, iterators> > ;


    public:
      zip_iterator(const zip_iterator& it) : m_iterators(it.m_iterators)
      { }

      template<class... InIterators>
      explicit zip_iterator(InIterators&&... its) : m_iterators(std::forward<InIterators>(its)...)
      { }

      zip_iterator& operator=(const zip_iterator& it)
      {
        m_iterators = it.m_iterators;
        return *this;
      }
    private:
      friend boost::iterator_core_access;
      template<std::size_t ...S>  reference dereference(blink::utility::index_sequence<S...>) const
      {
        return reference(*get<S>()...);

      }
      reference dereference() const
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
        static_assert(std::is_same < pack< get_type<std::decay<OtherIterators> >...>
          , pack< get_type < std::decay <Iterators> >... > > ::value, "incompatible iterators");

        return std::distance(std::get<0>(m_iterators), that.get<0>());
      }

      template<class... OtherIterators >
      bool equal(const zip_iterator<OtherIterators...>& that) const
      {
        static_assert(std::is_same < pack< get_type<std::decay<OtherIterators> >...>
          , pack< get_type < std::decay <Iterators> >... > > ::value, "incompatible iterators");

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
    };

    template<class...Iterators>  
    zip_iterator < unwrap < get_type < std::remove_reference < Iterators> > >... >  
      make_zip_iterator(Iterators&&... its)
    {
      return zip_iterator < unwrap < get_type < std::remove_reference < Iterators> > >... >(std::forward<Iterators>(its)...);
    }
  }
}
 
#endif