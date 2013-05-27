#ifndef USH_SESSION_H
#define USH_SESSION_H

#include "types.h"

namespace ush {

// standard location /dev/shm/SHM_NAME
static const char * const standard_shm_name = "MD.EXCHANGE";
static const std::size_t default_shm_size = 524288;

struct SessionKey;

struct connect_only_t {};
static const connect_only_t connect_only = connect_only_t();
struct recreate_t {};
static const recreate_t recreate = recreate_t();

struct ShmSession {

    ShmSession(recreate_t, const std::size_t size=default_shm_size, const std::string & name=standard_shm_name)
        : name(name), size(size)
    {
        ipc::shared_memory_object::remove(name.c_str());

        segment.reset(new ipc::managed_shared_memory(ipc::create_only, name.c_str(), size));
        allocator.reset(new void_allocator(segment->get_segment_manager()));

        ddex = segment->construct<DoubleDataExchange>("DoubleDataExchange") (KeyLess(), *allocator);
        ldex = segment->construct<LongDataExchange>("LongDataExchange") (KeyLess(), *allocator);
    }

    ShmSession(connect_only_t, const std::size_t size=default_shm_size, const std::string & name=standard_shm_name)
        : name(name), size(size)
    {
        segment.reset(new ipc::managed_shared_memory(ipc::open_only, name.c_str()));
        allocator.reset(new void_allocator(segment->get_segment_manager()));
        ddex = segment->find<DoubleDataExchange>("DoubleDataExchange").first;
        ldex = segment->find<LongDataExchange>("LongDataExchange").first;
    }

    DoubleDataExchange & doubles() { return *ddex; }
    LongDataExchange & longs() { return *ldex; }

    const std::string name;
    const std::size_t size;

    std::size_t free_memory() { 
        return segment->get_free_memory();
    }

    std::unique_ptr<ipc::managed_shared_memory> segment;
    std::unique_ptr<void_allocator> allocator;

    protected:

        // these don't need to be deleted
        DoubleDataExchange * ddex;
        LongDataExchange * ldex;

};

struct SessionKey : public Key {

    SessionKey(const std::string & rel_contract, const std::string & data, 
               ShmSession & session)
        : Key(rel_contract, data, *session.allocator)
    {}
};

} // namespace ush

#endif // USH_SESSION_H
