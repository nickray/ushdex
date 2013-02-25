#ifndef USH_TYPES_H
#define USH_TYPES_H

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/pair.hpp>
#include <boost/interprocess/containers/string.hpp>

#include <ostream>

using namespace boost::interprocess;

namespace ush {

// basic types
typedef managed_shared_memory::segment_manager segment_manager_t;
typedef allocator<void, segment_manager_t> void_allocator;

// interprocess version of string
typedef allocator<char, segment_manager_t> char_allocator;
typedef basic_string<char, std::char_traits<char>, char_allocator> IpcString;

// basic key type, e.g. ("CL.F.GLOB.0", "last_traded_price")
struct Key {

    IpcString rel_contract;
    IpcString data;

    Key(const std::string & rel_contract, const std::string & data, 
            const void_allocator & allocator)
        : rel_contract(rel_contract.c_str(), allocator),
          data(data.c_str(), allocator)
    {}

    friend std::ostream & operator<< (std::ostream & o, const Key & self) {
        o << "('" << self.rel_contract << "', '" << self.data << "')";
        return o;
    }

};

// necessary due to implementation of sets
struct KeyLess : std::binary_function <Key, Key, bool> {
  bool operator() (const Key& x, const Key& y) const {
      if(x.rel_contract < y.rel_contract)
          return true;
      if(x.rel_contract == y.rel_contract)
          return x.data < y.data;
      return false;
  }
};

#if __GNUC_PREREQ(4, 7)
/* template aliases are available */
template <typename T> using ValueType = std::pair<const Key, T>;
template<typename T> using ValueTypeAllocator = allocator<ValueType<T>, segment_manager_t>;
template<class T> using DataExchange = map<Key, T, KeyLess, ValueTypeAllocator<T>>;

typedef DataExchange<double> DoubleDataExchange;
typedef DataExchange<long> LongDataExchange;
#else
// our two main types to exchange double and long data
typedef std::pair<const Key, double> DoubleValueType;
typedef allocator<DoubleValueType, segment_manager_t> DoubleValueTypeAllocator;
typedef map<Key, double, KeyLess, DoubleValueTypeAllocator> DoubleDataExchange;

typedef std::pair<const Key, long> LongValueType;
typedef allocator<LongValueType, segment_manager_t> LongValueTypeAllocator;
typedef map<Key, long, KeyLess, LongValueTypeAllocator> LongDataExchange;

/*
 * This is supposed to be the standard workaround,
 * but it doesn't quite work!
 *
template <typename T>
struct ValueType {
    typedef std::pair<const Key, T> type;
};

template <typename T>
struct ValueTypeAllocator {
    allocator<ValueType::type, segment_manager_t> type;
};

template <typename T>
struct DataExchange {
    map<Key, T, KeyLess, ValueTypeAllocator::type> type;
};

typedef DataExchange<double>::type DoubleDataExchange;
typedef DataExchange<long>::type LongDataExchange;
*/
#endif

} // namespace ush

#endif // USH_TYPES_H
