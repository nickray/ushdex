#ifndef USH_SESSION_H
#define USH_SESSION_H

#include "types.h"

namespace ush {

// standard location /dev/shm/SHM_NAME
const char * SHM_NAME = "MD.EXCHANGE";

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

} // namespace ush

#endif // USH_SESSION_H
