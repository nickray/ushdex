#ifndef RED_RW_H
#define RED_RW_H

#include "meta_rw.h"

namespace ush {

const char * const RED_DATA_PREFIX = "RedData::";

struct RedData : public MetaData {

    double bid;
    double ask;

    RedData() : MetaData(), bid(0.), ask(0.) {}

    friend std::ostream & operator<< (std::ostream & o, const RedData & self) {
        o << static_cast<const MetaData &>(self);

        o << ',' << self.bid;
        o << ',' << hex_dump(self.bid);
        o << ',' << self.ask;
        o << ',' << hex_dump(self.ask);
        return o;
    }

    long serialize(const std::string& rel_contract, char * buffer) {

        long offset = MetaData::serialize(rel_contract, buffer);

        void * const start(static_cast<char * const>(static_cast<void * const>(this)) + sizeof(MetaData));
        long num = sizeof(*this) - sizeof(MetaData);
        memcpy(buffer + offset, start, num);
        offset += num;

        return offset;

    };

    long deserialize(const char * const buffer, std::string & rel_contract) {

        long offset = MetaData::deserialize(buffer, rel_contract);

        void * start(static_cast<char *>(static_cast<void *>(this)) + sizeof(MetaData));
        long num = sizeof(*this) - sizeof(MetaData);
        memcpy(start, buffer + offset, num);
        offset += num;

        return offset;
    }

};

class RedBase : public virtual MetaBase {

    protected:
        RedBase(const std::string & rel_contract, const std::string & prefix)
            : MetaBase(rel_contract, prefix)
        {
            p_bid = locate_double_entry("bid");
            p_ask = locate_double_entry("ask");
        }

        // pointers
        double * p_bid;
        double * p_ask;

};

class RedWriter : public MetaWriter<RedWriter, RedData>, RedBase {

    public:
        RedWriter(const std::string & rel_contract)
            : MetaBase(rel_contract, RED_DATA_PREFIX),
              MetaWriter<RedWriter, RedData>(rel_contract, RED_DATA_PREFIX),
              RedBase(rel_contract, RED_DATA_PREFIX)
        {}

    protected:
        friend class MetaWriter<RedWriter, RedData>;
        void write_derived(const RedData & data) {
            *p_bid = data.bid;
            *p_ask = data.ask;
        }

};

class RedReader : public MetaReader<RedReader, RedData>, RedBase {

    public:
        RedReader(const std::string & rel_contract)
            : MetaBase(rel_contract, RED_DATA_PREFIX),
              MetaReader<RedReader, RedData>(rel_contract, RED_DATA_PREFIX), 
              RedBase(rel_contract, RED_DATA_PREFIX)
        {}

    protected:
        friend class MetaReader<RedReader, RedData>;
        void read_derived(RedData & data) {
            data.bid = *p_bid;
            data.ask = *p_ask;
        }

};

} // namespace ush

#endif // RED_RW_H
