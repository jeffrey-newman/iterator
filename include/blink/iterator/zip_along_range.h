#include <boost/iterator/iterator_facade.hpp>
namespace blink {
  namespace iterator {
    template<class Iterator>
    struct reference_iterator :
      public boost::iterator_facade<reference_iterator<Iterator>
      , typename Iterator::value_type
      , boost::bidirectional_traversal_tag
      , typename Iterator::reference
      , typename Iterator::difference_type>
    {
    public:
      using difference_type = typename Iterator::difference_type;
      using reference = typename Iterator::reference;
      reference_iterator(Iterator& iter, bool is_end = false) : m_iterator(iter), m_is_end(is_end)
      {}

    private:
      friend boost::iterator_core_access;
      void increment()
      {
        ++m_iterator;
      }
      void decrement()
      {
        --m_iterator;
      }

      reference dereference() const
      {
        return *m_iterator;
      }

      template<class T>
      bool equal(const T& that) const
      {
        return m_iterator == that.m_iterator && m_is_end == that.m_is_end;
      }

      Iterator& m_iterator;
      bool m_is_end;
    };

    template<class Iterator>
    struct zip_along_range
    {
      using iterator = reference_iterator<Iterator>;
      zip_along_range(Iterator& iter) : m_iterator(iter)
      {};

      reference_iterator<Iterator> begin()
      {
        return reference_iterator<Iterator>(m_iterator);
      }

      reference_iterator<Iterator> end()
      {
        return reference_iterator<Iterator>(m_iterator, true);
      }
    private:
      Iterator& m_iterator;
    };

    template<class Iterator>
    zip_along_range<Iterator> make_zip_along_range(Iterator& iter)
    {
        return zip_along_range<Iterator>(iter);
    }
  }
}

