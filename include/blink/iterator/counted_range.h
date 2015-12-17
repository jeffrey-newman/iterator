#include <boost/iterator/iterator_facade.hpp>
namespace blink {
  namespace iterator {
    template<class Integer>
    struct counted_iterator :
      public boost::iterator_facade<counted_iterator<Integer>
      , Integer
      , boost::bidirectional_traversal_tag
      , Integer
      , Integer>
    {
    public:
      counted_iterator(Integer index) : m_index(index)
      {}

    private:
      void increment()
      {
        ++m_index;
      }
      void decrement()
      {
        --m_index;
      }

      Integer dereference() const
      {
        return m_index;
      }

      template<class T>
      bool equal(const T& that) const
      {
        return m_index == that.m_index;
      }

      Integer m_index;
    };

    template<class Integer>
    struct counted_range
    {
      counted_range(Integer n) : m_iterator(iter)
      {};

      counted_iterator<Integer> begin()
      {
        return counted_iterator<Integer>(Integer());
      }

      counted_iterator<Integer> end()
      {
        return counted_iterator<Integer>(m_n);
      }
    private:
      Integer m_n;
    };

    template<class Integer>
    counted_range<Integer> make_counted_range(Integer n)
    {
      return counted_range<Integer>(n);
    }
  }
}

