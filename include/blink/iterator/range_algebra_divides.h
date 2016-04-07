//
//=======================================================================
// Copyright 2016
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// Implement the operator/ overloads for range_algebra_wrappers

#ifndef BLINK_ITERATOR_RANGE_ALGEBRA_DIVIDES_H_AHZ
#define BLINK_ITERATOR_RANGE_ALGEBRA_DIVIDES_H_AHZ

#include <blink/iterator/range_algebra_transform.h>
#include <blink/iterator/range_algebra_wrapper.h>
#include <blink/iterator/transform_range.h>
#include <functional>

namespace blink {
  namespace iterator {
    namespace divides_operator {

      using fun = std::divides<>;

      template<class R> using wrap = range_algebra_wrapper<R>;
      template<class T> using dec = special_decay_t<T>;

      template<class A, class B>
      using trans1 = range_algebra_transform<fun, A, B>;

      template<class A, class B>
      using trans2 = transform_range<fun, A, B>;

      using std::move;
      using std::ref;
      using std::forward;

      template<class R, class T>
      wrap< trans1< wrap<R>, dec<T> > >
        operator/(wrap<R>&& r, T&& v)
      {
          return range_algebra_function(fun{}, move(r), forward<T>(v));
      }

      template<class R, class T>
      wrap< trans1< wrap<R>&, dec<T> > >
        operator/(wrap<R>& r, T&& v)
      {
          return range_algebra_function(fun{}, ref(r), forward<T>(v));
      }

      template<class R, class T>
      wrap< trans1< dec<T>, wrap<R> > >
        operator/(T&& v, wrap<R>&& r)
      {
          return range_algebra_function(fun{}, forward<T>(v), move(r));
      }

      template<class R, class T>
      wrap< trans1< dec<T>, wrap<R>& > >
        operator/(T&& v, wrap<R>& r)
      {
          return range_algebra_function(fun{}, forward<T>(v), ref(r));
      }

      template<class R1, class R2>
      wrap< trans2< wrap<R1>, wrap<R2> > >
        operator/(wrap<R1>&& r1, wrap<R2>&& r2)
      {
          return range_algebra(
            make_transform_range(fun{}, move(r1), move(r2)));
      }

      template<class R1, class R2>
      wrap< trans2< wrap<R1>, wrap<R2>& > >
        operator/(wrap<R1>&& r1, wrap<R2>& r2)
      {
          return range_algebra(make_transform_range(fun{}, move(r1), ref(r2)));
      }

      template<class R1, class R2>
      wrap< trans2< wrap<R1>&, wrap<R2> > >
        operator/(wrap<R1>& r1, wrap<R2>&& r2)
      {
          return range_algebra(make_transform_range(fun{}, ref(r1), move(r2)));
      }
      template<class R1, class R2>
      wrap< trans2< wrap<R1>&, wrap<R2>& > >
        operator/(wrap<R1>& r1, wrap<R2>& r2)
      {
          return range_algebra(make_transform_range(fun{}, ref(r1), ref(r2)));
      }
    }
    using divides_operator::operator/;
  }
}
#endif