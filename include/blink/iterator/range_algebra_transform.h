//
//
//=======================================================================
// Copyright 2016
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// range_algebra allows element-by-element operations on ranges
// So far only implemented for plus

#ifndef BLINK_ITERATOR_RANGE_ALGEBRA_TRANSFORM_H_AHZ
#define BLINK_ITERATOR_RANGE_ALGEBRA_TRANSFORM_H_AHZ

#include <blink/iterator/range_algebra_wrapper.h>
#include <blink/iterator/detail.h>
#include <blink/utility/index_in_filter_transform.h>
#include <blink/utility/filter_index_sequence.h>
#include <blink/utility/apply_tuple.h>
#include <blink/utility/tuple_elements.h>

namespace blink {
  namespace iterator {
    
    template<class T> struct is_not_range_algebra_wrapper
    {
      const static bool value = !is_range_algebra_wrapped<T>::value;
    };

    template<class T>
    struct argument_type
    {
      using input_type = typename std::decay< T >::type;
      const static bool is_range = is_range_algebra_wrapped < input_type >::value;
      
      using lazy_type = typename std::conditional<is_range, 
        detail::get_value_type<input_type>, std::identity<input_type> >::type;
      using type = typename lazy_type::type;
    };

    template<class T>
    using argument_type_t = typename argument_type<T>::type;
    
    template<class Function, class... Args>
    struct range_algebra_transform
    {
      static const std::size_t n = sizeof...(Args);
      using indices = blink::utility::make_index_sequence < n >;

      using variable_indices = blink::utility::make_filter_index_sequence < is_range_algebra_wrapped, decay_t<Args>... >;
      using constant_indices = blink::utility::make_filter_index_sequence < is_not_range_algebra_wrapper, decay_t<Args>... >;
      using sorting_hat = blink::utility::make_index_in_filter < is_range_algebra_wrapped, decay_t<Args>... >;
      
      using all_tuple = std::tuple < Args... > ;
      using range_tuple = blink::utility::tuple_elements_t< apply_to_pack_t< decay_t, all_tuple>, variable_indices >; // tuple of ranges
      using value_tuple = apply_to_pack_t< get_reference_t, apply_to_pack_t< decay_t, range_tuple> >; 
      using constants_tuple = blink::utility::tuple_elements_t< all_tuple, constant_indices >; // tuple of ranges
     
      using range_reference_tuple = apply_to_pack_t < add_lvalue_reference_t, range_tuple >;
      using constants_reference_tuple = apply_to_pack_t < add_lvalue_reference_t, constants_tuple >;
      using value_reference_tuple_t = apply_to_pack_t < add_lvalue_reference_t, value_tuple >;
      
      using argument_tuple = apply_to_pack_t < argument_type_t, all_tuple > ;

      using value_type = blink::utility::apply_tuple_return_type < Function, argument_tuple > ;
      using reference = value_type;

      struct applicator
      {
        applicator(range_algebra_transform* parent = nullptr) : m_parent(parent)
        {}

        template<class... InArgs> // should be the value_types of the ranges
        value_type operator()(InArgs&&... args) const
        {
          value_reference_tuple_t vars(args...);// not forward<> 
          return m_parent->apply(vars, indices{});
        }
        range_algebra_transform* m_parent;
      };

      template<class> struct transform_range_helper{};
      template<class... Ranges>
      struct transform_range_helper<std::tuple<Ranges...>>
      {
        using type = transform_range < applicator, remove_reference_t<Ranges>... > ;
        static const std::size_t nRanges = sizeof...(Ranges);
        using range_indices = blink::utility::make_index_sequence < nRanges >;
        
        template<template<std::size_t...> class Pack, std::size_t... S>
        type make(applicator& a, range_reference_tuple& ranges, Pack<S...>)
        {
          type(a, std::get<S>(ranges)...);
        }

        type make(applicator& a, range_reference_tuple& ranges)
        {
          return make(a, ranges, range_indices{});
        }

      };

      using transform_range_helper_t = transform_range_helper < range_tuple >;
      using transform_range = typename transform_range_helper_t::type;
      using iterator = get_iterator_t < transform_range > ;
     
      range_algebra_transform(const range_algebra_transform& that) 
        : m_f(that.m_f)
        , m_arguments(that.m_arguments)
        , m_ranges(blink::utility::refer_elements(m_arguments, variable_indices{}))
        , m_constant_arguments(blink::utility::refer_elements(m_arguments, constant_indices{}))
        , m_applicator(this)
        , m_transform_range(m_applicator, m_ranges)
      {
        assert(false);
      }

      range_algebra_transform(range_algebra_transform&& that)
        : m_f(std::move(that.m_f) )
        , m_arguments(std::move(that.m_arguments))
        , m_ranges(blink::utility::refer_elements(m_arguments, variable_indices{}))
        , m_constant_arguments(blink::utility::refer_elements(m_arguments, constant_indices{}))
        , m_applicator(this)
        , m_transform_range(m_applicator, m_ranges)
      {}

      template<class InFunction, class... InArgs>
      range_algebra_transform(InFunction&& f, InArgs&&... args) 
        : m_f(std::forward<InFunction>(f))
        , m_arguments(std::forward<InArgs>(args)...)
        , m_ranges(blink::utility::refer_elements(m_arguments, variable_indices{}))
        , m_constant_arguments(blink::utility::refer_elements(m_arguments, constant_indices{}))
        , m_applicator(this)
        , m_transform_range(m_applicator, m_ranges)
      {
      }

      iterator begin() 
      {
        return m_transform_range.begin();
      }

      iterator end()
      {
        return m_transform_range.end();
      }

      template <std::size_t I>
      tuple_element_t<I, argument_tuple> get_value(value_reference_tuple_t& variables)
      {
        static const std::size_t first = tuple_element_t<I, sorting_hat>::first;
        static const std::size_t second = tuple_element_t<I, sorting_hat>::second;
        auto combined = std::make_tuple(std::ref(m_constant_arguments), std::ref(variables));
        return std::get<second>(std::get<first>(combined));

      }
      
      template<template<std::size_t...> class Pack, std::size_t...S>
      value_type apply(value_reference_tuple_t& variables, Pack<S...>)
      {
        return m_f(get_value<S>(variables)...);
      }

      

      applicator m_applicator;
      Function m_f;
      all_tuple m_arguments;
      range_reference_tuple m_ranges;
      constants_reference_tuple m_constant_arguments;
      transform_range m_transform_range;

    };

    template<class Function, class...Args>
    range_algebra_transform <special_decay_t<Function>, special_decay_t<Args>...>
      make_range_algebra_transform(Function&& f, Args&&... a)
    { 
      using type =
        range_algebra_transform < special_decay_t<Function>, special_decay_t<Args>... >;

      return type(std::forward<Function>(f), std::forward<Args>(a)...);
    }
   
    template<class Function, class...Args>
    range_algebra_wrapper <
      range_algebra_transform <special_decay_t<Function>, special_decay_t<Args>...> >
      range_algebra_function(Function&& f, Args&&... a)
    {
      using type =
        range_algebra_transform < special_decay_t<Function>, special_decay_t<Args>... >;
      type rat(std::forward<Function>(f), std::forward<Args>(a)...);
      return range_algebra_val(std::move(rat));
    }
    
  }
}



#endif