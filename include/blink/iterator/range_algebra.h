//
//=======================================================================
// Copyright 2015
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// range_algebra is a light wrapper around ranges that signals operators to use 
// operators in lazy element-by-element fashion
// 

namespace blink
{
  namespace iterator {
    template<class A, class B>
    struct plus
    {
      using result_type = decltype(std::declval<A>() + std::declval<B>());
      
      result_type operator()(const A& a, const B& b)
      {
        return a + b;
      }
    }

  
      template<class Range>
    class range_algebra
    {
      using A = typename Range::iterator::value_type;
      using interator = Range::iterator;
      iterator begin() const
      {
        return m_range.begin();
      }

      iterator end() const
      {
        return m_range.end();
      }
  
      template<class OtherRange>
      struct result
      {
        using B = typename Range::OtherRange::value_type;
        using plus_value_type = plus<A, B>::result_type;
        using plus_range = transform_range<
          plus<A, result<OtherRange>::B>,
          result<OtherRange>::plus_value_type>,
          Range,
          OtherRange>
      }
      template<class OtherRange>
      result<OtherRange>::plus_range
        operator+(const OtherRange& other) const 
      {
          return make_transform_range(plus<A, result<OtherRange>::B>(), )


      {
      
      const Range& m_range;
};