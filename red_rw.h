#ifndef RW_RED_H
#define RW_RED_H

#include "meta_rw.h"

const std::string RED_CLASS_PREFIX = "Red::";

struct RedData : public MetaData {

    double bid;
    double ask;

    RedData() : MetaData() {}

    friend std::ostream & operator<< (std::ostream & o, const RedData & self) {
        o << static_cast<const MetaData &>(self) << ',';

        o << self.bid << ',';
        o << self.ask << ',';
        o << hex_dump(self.bid) << ',';
        o << hex_dump(self.ask);

        return o;
    }

    bool operator==(const RedData & other) const {
        return (
                MetaData::operator==(other) and
                ( bid == other.bid ) and
                ( ask == other.ask ) );
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

class RedWriter : public MetaWriter, RedBase {

    public:
        RedWriter(const std::string & rel_contract, ShmSession & session)
            : MetaBase(rel_contract, RED_CLASS_PREFIX, session),
              MetaWriter(rel_contract, RED_CLASS_PREFIX, session), 
              RedBase(rel_contract, RED_CLASS_PREFIX, session)
        {}

    protected:
        void write_derived(const MetaData * d) {
            const RedData & data(*static_cast<const RedData *>(d));

            *p_bid = data.bid;
            *p_ask = data.ask;
        }

};

class RedReader : public MetaReader, RedBase {

    public:
        RedReader(const std::string & rel_contract, ShmSession & session)
            : MetaBase(rel_contract, RED_CLASS_PREFIX, session),
              MetaReader(rel_contract, RED_CLASS_PREFIX, session), 
              RedBase(rel_contract, RED_CLASS_PREFIX, session)
        {}

    protected:
        void read_derived(MetaData * d) {
            RedData & data(*static_cast<RedData *>(d));

            data.bid = *p_bid;
            data.ask = *p_ask;
        }

};

#endif // RW_RED_H
