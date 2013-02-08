#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/pair.hpp>
#include <boost/interprocess/containers/string.hpp>

using namespace boost::interprocess;


// standard location in /dev/shm/MD.EXCHANGE
const char SHM_NAME[] = "MD.EXCHANGE";


// basic types
typedef managed_shared_memory::segment_manager segment_manager_t;
typedef allocator<void, segment_manager_t> void_allocator;

// interprocess version of string
typedef allocator<char, segment_manager_t> char_allocator;
typedef basic_string<char, std::char_traits<char>, char_allocator> String;

// basic key type, e.g. ("CL.F.GLOB.0", "last_traded_price")
struct Key{

    String rel_contract;
    String data;

    Key(const std::string & rel_contract, const std::string & data, 
            const void_allocator & allocator)
        : rel_contract(rel_contract.c_str(), allocator),
          data(data.c_str(), allocator)
    {}
};

// necessary due to implementation of sets
struct key_less : std::binary_function <Key, Key, bool>
{
  bool operator() (const Key& x, const Key& y) const
      {return x.rel_contract < y.rel_contract || x.data < y.data;}
};

// our two main types to exchange double and long data
typedef std::pair<const Key, double> DoubleValueType;
typedef allocator<DoubleValueType, segment_manager_t> DoubleValueTypeAllocator;
typedef map<Key, double, key_less, DoubleValueTypeAllocator> DoubleDataExchange;

typedef std::pair<const Key, long> LongValueType;
typedef allocator<LongValueType, segment_manager_t> LongValueTypeAllocator;
typedef map<Key, long, key_less, LongValueTypeAllocator> LongDataExchange;

/*
 * What we'd really like to do here is use template aliases:
 *
 * template <typename T>
 * using ValueType = std::pair<const Key, T>
 * template<class T> using ValueTypeAllocator = typedef allocator<ValueType, segment_manager_t>;
 * template<class T> using DataExchange = map<Key, T, key_less, ValueTypeAllocator>;
 *
 * But this is only supported in GCC 4.7, and we're stuck on 4.4
 *
 */
