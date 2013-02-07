#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <functional>
#include <utility>

using namespace boost::interprocess;

typedef int    KeyType;
typedef float  MappedType;
typedef std::pair<const int, float> ValueType;

typedef allocator<ValueType, managed_shared_memory::segment_manager>
  ShmemAllocator;

typedef map<KeyType, MappedType, std::less<KeyType>, ShmemAllocator>
  MyMap;

