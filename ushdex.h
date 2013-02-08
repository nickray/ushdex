#ifndef USHDEX_H
#define USHDEX_H

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/pair.hpp>
#include <boost/interprocess/containers/string.hpp>

#include <ostream>

using namespace boost::interprocess;
using std::unique_ptr;

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

    friend std::ostream & operator<< (std::ostream & o, const Key & self) {
        o << "('" << self.rel_contract << "', '" << self.data << "')";
        return o;
    }

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

#include <iostream>
using namespace std;

struct ShmSession {

    unique_ptr<managed_shared_memory> segment;
    unique_ptr<void_allocator> allocator;

    // these don't need to be deleted
    DoubleDataExchange * ddex;
    LongDataExchange * ldex;

    ShmSession(bool recreate=false)
    {
        if(recreate) {
            shared_memory_object::remove(SHM_NAME);

            segment.reset(new managed_shared_memory(create_only, SHM_NAME, 65536));
            allocator.reset(new void_allocator(segment->get_segment_manager()));
 
            ddex = segment->construct<DoubleDataExchange>("DoubleDataExchange") (key_less(), *allocator);
            ldex = segment->construct<LongDataExchange>("LongDataExchange") (key_less(), *allocator);
        } else {
            segment.reset(new managed_shared_memory(open_only, SHM_NAME));
            allocator.reset(new void_allocator(segment->get_segment_manager()));
            ddex = segment->find<DoubleDataExchange>("DoubleDataExchange").first;
            ldex = segment->find<LongDataExchange>("LongDataExchange").first;
        }
    }


    DoubleDataExchange & doubles() { return *ddex; }
    LongDataExchange & longs() { return *ldex; }
};

struct SessionKey : public Key {

    SessionKey(const std::string & rel_contract, const std::string & data, 
               ShmSession & session)
        : Key(rel_contract, data, *session.allocator)
    {}
};
#endif
