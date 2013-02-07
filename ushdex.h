#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/pair.hpp>
#include <boost/interprocess/containers/string.hpp>

//#include <functional>
//#include <utility>

using namespace boost::interprocess;


typedef int KeyType; // want to change this later to pair<string, string>

typedef std::pair<const KeyType, long> LongValueType;
typedef std::pair<const KeyType, double> DoubleValueType;

// todo: replace this and the following with template<T>

typedef allocator<void, managed_shared_memory::segment_manager>
  VoidShmAllocator;

typedef allocator<LongValueType, managed_shared_memory::segment_manager>
  LongShmAllocator;

typedef allocator<DoubleValueType, managed_shared_memory::segment_manager>
  DoubleShmAllocator;

typedef map<KeyType, long, std::less<KeyType>, LongShmAllocator>
  LongDataExchange;

typedef map<KeyType, double, std::less<KeyType>, DoubleShmAllocator>
  DoubleDataExchange;



//typedef basic_string<char, std::char_traits<char>, char_allocator> char_string;
