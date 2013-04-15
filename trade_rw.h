#ifndef TRADE_RW_H
#define TRADE_RW_H

#include "meta_rw.h"

namespace ush {

const char * const TRADE_DATA_PREFIX = "TradeData::";

struct TradeData : public MetaData {

    double price;
    long volume;
    long aggressor;
    long type;
    long cum_volume;

    TradeData() : MetaData(), price(0.), volume(0), aggressor(0), type(0), cum_volume(0) {}

    friend std::ostream & operator<< (std::ostream & o, const TradeData & self) {
        o << static_cast<const MetaData &>(self);

        o << ',' << self.price;
        o << ',' << hex_dump(self.price);
        o << ',' << self.volume;
        o << ',' << self.aggressor;
        o << ',' << self.type;
        o << ',' << self.cum_volume;
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

class TradeBase : public virtual MetaBase {

    protected:
        TradeBase(const std::string & rel_contract, const std::string & prefix)
            : MetaBase(rel_contract, prefix)
        {
            p_price = locate_double_entry("price");
            p_volume = locate_long_entry("volume");
            p_aggressor = locate_long_entry("aggressor");
            p_type = locate_long_entry("type");
            p_cum_volume = locate_long_entry("cum_volume");
        }

        // pointers
        double * p_price;
        long * p_volume;
        long * p_aggressor;
        long * p_type;
        long * p_cum_volume;

};

class TradeWriter : public MetaWriter<TradeWriter, TradeData>, TradeBase {

    public:
        TradeWriter(const std::string & rel_contract)
            : MetaBase(rel_contract, TRADE_DATA_PREFIX),
              MetaWriter<TradeWriter, TradeData>(rel_contract, TRADE_DATA_PREFIX),
              TradeBase(rel_contract, TRADE_DATA_PREFIX)
        {}

    protected:
        friend class MetaWriter<TradeWriter, TradeData>;
        void write_derived(const TradeData & data) {
            *p_price = data.price;
            *p_volume = data.volume;
            *p_aggressor = data.aggressor;
            *p_type = data.type;
            *p_cum_volume = data.cum_volume;
        }

};

class TradeReader : public MetaReader<TradeReader, TradeData>, TradeBase {

    public:
        TradeReader(const std::string & rel_contract)
            : MetaBase(rel_contract, TRADE_DATA_PREFIX),
              MetaReader<TradeReader, TradeData>(rel_contract, TRADE_DATA_PREFIX), 
              TradeBase(rel_contract, TRADE_DATA_PREFIX)
        {}

    protected:
        friend class MetaReader<TradeReader, TradeData>;
        void read_derived(TradeData & data) {
            data.price = *p_price;
            data.volume = *p_volume;
            data.aggressor = *p_aggressor;
            data.type = *p_type;
            data.cum_volume = *p_cum_volume;
        }

};

} // namespace ush

#endif // TRADE_RW_H
