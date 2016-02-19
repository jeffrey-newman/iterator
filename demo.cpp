#include <blink\iterator\zip_range.h>
#include <blink\iterator\transform_range.h>
#include <blink\iterator\range_algebra.h>

#include <boost\iterator\iterator_facade.hpp>

#include <string>
#include <iostream>
#include <vector>

struct uncopyable_iterator : public boost::iterator_facade<uncopyable_iterator, int, boost::incrementable_traversal_tag >
{
public:
   uncopyable_iterator(std::vector<int>& data, int i = 0) : m_data(data), m_i(i)
  {}

  uncopyable_iterator(const uncopyable_iterator&) = delete;

  uncopyable_iterator(uncopyable_iterator&& that) : m_i(that.m_i), m_data(that.m_data)
  {}
  uncopyable_iterator& operator=(const uncopyable_iterator&) = delete;
  uncopyable_iterator& operator=(uncopyable_iterator&& that) = delete;
  
  void increment()
  {
    ++m_i;
  }

  bool equal(const uncopyable_iterator& that) const
  {
    return m_i == that.m_i;
  }

  int& dereference() const
  {
    return m_data[m_i];
  }

  int m_i;
  std::vector<int>& m_data;
};

struct uncopyable_range
{
  uncopyable_range(int size) : m_data(size)
  {
    for (int i = 0; i < size; ++i) m_data[i] = i + 1;
  }

  uncopyable_range(const uncopyable_range&) = delete;


  uncopyable_range(uncopyable_range&& that) : m_data(std::move(that.m_data))
  {}
  uncopyable_range& operator=(const uncopyable_range&) = delete;
  uncopyable_range& operator=(uncopyable_range&& that)
  {
    std::swap(m_data, that.m_data);
    that.m_data.clear(); // just to avoid confusion
    return *this;
  }

  typedef uncopyable_iterator iterator;

  iterator begin()
  {
    return iterator(m_data);
  }
  iterator end()
  {
    return iterator(m_data, static_cast<int>(m_data.size()));
  }

  std::vector<int> m_data;
};

using namespace blink::iterator;

int main()
{
  std::vector<int> a = { 1, 2, 3, 4, 5, 6 };
  std::vector<int> b = { 100, 200, 300, 400, 500, 600 };
  
  auto sum = make_transform_range<int>(std::plus<int>(), std::ref(a), std::ref(b));
  auto sum1 = range_algebra(a) + range_algebra(b);
  auto sum2 = 1000 + range_algebra(a) + 2000000 + 40000000;

  for (auto&& i : sum)
  {
    std::cout << i << std::endl;
  }

  for (auto&& i : sum1)
  {
    std::cout << i << std::endl;
  }

  for (auto&& i : sum2)
  {
    std::cout << i << std::endl;
  }
  
  std::cin.get();
  return 0;
  using iter =  std::vector<int>::iterator;
  using zip_iter = zip_iterator<iter, iter>;

  zip_iter begin(a.begin(), b.begin());
  zip_iter end(a.end(), b.end());
   
  for (zip_iter i = begin; i != end; i+=2) // strides in steps of 2
  {
    std::cout << std::get<0>(*i) + std::get<1>(*i) << std::endl;
  }

  std::cout << std::endl;
  
  std::vector<int> ints = { 1, 2, 3, 4 };
  std::vector<std::string> roman = { "I", "II", "III", "IV" };
  std::vector<std::string> strs = { "one", "two", "three", "four" };
  uncopyable_range ur(4);
  
  auto zip_range = make_zip_range(std::ref(ur), std::ref(roman), strs);
  
  auto i = *(zip_range.begin());
  std::tuple<int, std::string&, std::string> v = i;
  std::get<2>(v) = "One";
  i = v;
  std::get<1>(v) = "i";
  std::get<2>(v) = "one";

  for (auto& i : zip_range)
  {

    std::cout 
      << std::get<0>(i) << " " 
      << std::get<1>(i) << " " 
      << std::get<2>(i)
      << std::endl;
  }
  std::cout << std::endl;
  
  uncopyable_iterator ur_begin = ur.begin();
  uncopyable_iterator ur_end = ur.end();
  std::vector<int>::iterator ints_begin = ints.begin();
  std::vector<std::string>::iterator roman_begin = roman.begin();

  auto zip_i = make_zip_iterator(ints_begin, std::ref(roman_begin), std::move(ur_begin) );
  auto zip_end = make_zip_iterator(ints.end(), roman.end(), std::ref(ur_end));
  for (; zip_i != zip_end; ++zip_i)
 {
    std::cout << std::get<0>(*zip_i) << " "
      << std::get<1>(*zip_i) << " "
      << std::get<2>(*zip_i) << std::endl;
  }
 
  std::cin.get();
}

