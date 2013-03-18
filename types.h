#ifndef USH_TYPES_H
#define USH_TYPES_H

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/string.hpp>

#include <ostream>
#include <string>
#include <tuple>

namespace ush {

namespace ipc {
    using namespace boost::interprocess;
}

// basic types
typedef ipc::managed_shared_memory::segment_manager segment_manager_t;
typedef ipc::allocator<void, segment_manager_t> void_allocator;

// interprocess version of string
typedef ipc::allocator<char, segment_manager_t> char_allocator;
namespace ipc {
    typedef ipc::basic_string<char, std::char_traits<char>, char_allocator> string;
}

// basic key type, e.g. ("CL.F.GLOB.0", "last_traded_price")
struct Key {

    ipc::string rel_contract;
    ipc::string data;

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
      return std::tie(x.rel_contract, x.data) < std::tie(y.rel_contract, y.data);
  }
};

#if __GNUC_PREREQ(4, 7)
/* template aliases are available */
template <typename T> using ValueType = std::pair<const Key, T>;
template<typename T> using ValueTypeAllocator = ipc::allocator<ValueType<T>, segment_manager_t>;
template<class T> using DataExchange = ipc::map<Key, T, KeyLess, ValueTypeAllocator<T>>;

// our two main types to exchange double and long data
using DoubleDataExchange = DataExchange<double>;
using LongDataExchange = DataExchange<long>;
#else
typedef std::pair<const Key, double> DoubleValueType;
typedef ipc::allocator<DoubleValueType, segment_manager_t> DoubleValueTypeAllocator;
typedef ipc::map<Key, double, KeyLess, DoubleValueTypeAllocator> DoubleDataExchange;

typedef std::pair<const Key, long> LongValueType;
typedef ipc::allocator<LongValueType, segment_manager_t> LongValueTypeAllocator;
typedef ipc::map<Key, long, KeyLess, LongValueTypeAllocator> LongDataExchange;
#endif

} // namespace ush

#endif // USH_TYPES_H
