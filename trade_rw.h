#ifndef RW_TRADE_H
#define RW_TRADE_H

#include "meta_rw.h"

const char * const TRADE_DATA_PREFIX = "TradeData::";

struct TradeData : public MetaData {

    double price;
    long volume;

    long aggressor; // +1 for buyer, -1 for seller, 0 for unknown
    long type;

    long cum_volume;

    TradeData() : MetaData() {}

    friend std::ostream & operator<< (std::ostream & o, const TradeData & self) {
        o << static_cast<const MetaData &>(self) << ',';

        o << self.price << ',';
        o << hex_dump(self.price) << ',';
        o << self.volume << ',';
        o << self.aggressor << ',';
        o << self.type << ',';
        o << self.cum_volume;

        return o;
    }

    bool operator==(const TradeData & other) const {
        return (
                MetaData::operator==(other) &&
                ( price == other.price ) &&
                ( volume == other.volume ) &&
                ( type == other.type ) &&
                ( aggressor == other.aggressor ) &&
                ( cum_volume == other.cum_volume ) );
    }

};

class TradeBase : public virtual MetaBase {
    protected:
        TradeBase(const std::string & rel_contract, const std::string & prefix, ShmSession & session)
            : MetaBase(rel_contract, prefix, session)
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

class TradeWriter : public MetaWriter<TradeWriter>, TradeBase {

    public:
        TradeWriter(const std::string & rel_contract, ShmSession & session)
            : MetaBase(rel_contract, TRADE_DATA_PREFIX, session),
              MetaWriter(rel_contract, TRADE_DATA_PREFIX, session), 
              TradeBase(rel_contract, TRADE_DATA_PREFIX, session)
        {}

    protected:
        friend class MetaWriter<TradeWriter>;
        void write_derived(const MetaData * d) {
            const TradeData & data(*static_cast<const TradeData *>(d));

            *p_price = data.price;
            *p_volume = data.volume;
            *p_aggressor = data.aggressor;
            *p_type = data.type;
            *p_cum_volume = data.cum_volume;
        }

};

class TradeReader : public MetaReader<TradeReader>, TradeBase {

    public:
        TradeReader(const std::string & rel_contract, ShmSession & session)
            : MetaBase(rel_contract, TRADE_DATA_PREFIX, session),
              MetaReader(rel_contract, TRADE_DATA_PREFIX, session), 
              TradeBase(rel_contract, TRADE_DATA_PREFIX, session)
        {}


    protected:
        friend class MetaReader<TradeReader>;
        void read_derived(MetaData * d) {
            TradeData & data(*static_cast<TradeData *>(d));

            data.price = *p_price;
            data.volume = *p_volume;
            data.aggressor = *p_aggressor;
            data.type = *p_type;
            data.cum_volume = *p_cum_volume;
        }

};

#endif // RW_TRADE_H
