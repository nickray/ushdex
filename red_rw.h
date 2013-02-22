#ifndef RED_RW_H
#define RED_RW_H

#include "meta_rw.h"

namespace ush {

const char * const RED_DATA_PREFIX = "RedData::";

struct RedData : public MetaData {

    double bid;
    double ask;

    RedData() : MetaData() {}

    friend std::ostream & operator<< (std::ostream & o, const RedData & self) {
        o << static_cast<const MetaData &>(self) << ',';

        o << self.bid << ',';
        o << hex_dump(self.bid) << ',';
        o << self.ask << ',';
        o << hex_dump(self.ask);
        return o;
    }

    bool operator==(const RedData & other) const {
        return (MetaData::operator==(other) &&
               ( bid == other.bid ) &&
               ( ask == other.ask ) &&
                true);
    }

};

class RedBase : public virtual MetaBase {
    protected:
        RedBase(const std::string & rel_contract, const std::string & prefix, ShmSession & session)
            : MetaBase(rel_contract, prefix, session)
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
        RedWriter(const std::string & rel_contract, ShmSession & session)
            : MetaBase(rel_contract, RED_DATA_PREFIX, session),
              MetaWriter(rel_contract, RED_DATA_PREFIX, session), 
              RedBase(rel_contract, RED_DATA_PREFIX, session)
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
        RedReader(const std::string & rel_contract, ShmSession & session)
            : MetaBase(rel_contract, RED_DATA_PREFIX, session),
              MetaReader(rel_contract, RED_DATA_PREFIX, session), 
              RedBase(rel_contract, RED_DATA_PREFIX, session)
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