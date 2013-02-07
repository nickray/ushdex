#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include <boost/interprocess/containers/map.hpp>

#include <boost/interprocess/containers/pair.hpp>
#include <boost/interprocess/containers/string.hpp>

#include <functional>
//#include <utility>

using namespace boost::interprocess;

const char SHM_NAME[] = "MD.EXCHANGE";

/*
typedef int KeyType; // want to change this later to pair<string, string>

typedef std::pair<const KeyType, long> LongValueType;
typedef std::pair<const KeyType, double> DoubleValueType;

// todo: replace this and the following with template<T>

//typedef allocator<void, managed_shared_memory::segment_manager>
//  VoidShmAllocator;

typedef allocator<LongValueType, managed_shared_memory::segment_manager>
  LongShmAllocator;

typedef allocator<DoubleValueType, managed_shared_memory::segment_manager>
  DoubleShmAllocator;

typedef map<KeyType, long, std::less<KeyType>, LongShmAllocator>
  LongDataExchange;

typedef map<KeyType, double, std::less<KeyType>, DoubleShmAllocator>
  DoubleDataExchange;

//typedef basic_string<char, std::char_traits<char>, char_allocator> char_string;
*/


typedef managed_shared_memory::segment_manager segment_manager_t;

typedef allocator<char, segment_manager_t> char_allocator;
typedef allocator<void, segment_manager_t> void_allocator;

typedef basic_string<char, std::char_traits<char>, char_allocator> String;

struct KeyType {
    String rel_contract;
    String data;

    KeyType(const std::string & rel_contract, const std::string & data, 
            const void_allocator & allocator)
        : rel_contract(rel_contract.c_str(), allocator),
          data(data.c_str(), allocator)
    {}
};

struct key_less : std::binary_function <KeyType, KeyType, bool>
{
  bool operator() (const KeyType& x, const KeyType& y) const
      {return x.rel_contract < y.rel_contract || x.data < y.data;}
};

/*
template<class T> using ValueType = std::pair<const KeyType, T>;

template<class T> using ValueTypeAllocator = typedef allocator<ValueType, segment_manager_t>;

template<class T> using DataExchange = map<KeyType, T, key_less, ValueTypeAllocator>;
*/

typedef std::pair<const KeyType, double> DoubleValueType;
typedef allocator<DoubleValueType, segment_manager_t> DoubleValueTypeAllocator;
typedef map<KeyType, double, key_less, DoubleValueTypeAllocator> DoubleDataExchange;

typedef std::pair<const KeyType, long> LongValueType;
typedef allocator<LongValueType, segment_manager_t> LongValueTypeAllocator;
typedef map<KeyType, long, key_less, LongValueTypeAllocator> LongDataExchange;
