#ifndef RW_TRADE_H
#define RW_TRADE_H

#include "meta_rw.h"

const std::string TRADE_CLASS_PREFIX = "Trade::";

struct TradeData : public MetaData {

    double last_traded_price;
    long last_traded_volume;
    long cum_traded_volume;

    TradeData() : MetaData() {}

    friend std::ostream & operator<< (std::ostream & o, const TradeData & self) {
        o << static_cast<const MetaData &>(self) << ',';

        o << self.last_traded_price << ',';
        o << hex_dump(self.last_traded_price) << ',';
        o << self.last_traded_volume << ',';
        o << self.cum_traded_volume;

        return o;
    }

    bool operator==(const TradeData & other) const {
        return (
                MetaData::operator==(other) &&
                ( last_traded_price == other.last_traded_price ) &&
                ( last_traded_volume == other.last_traded_volume ) &&
                ( cum_traded_volume == other.cum_traded_volume ) );
    }

};

class TradeBase : public virtual MetaBase {
    protected:
        TradeBase(const std::string & rel_contract, const std::string & prefix, ShmSession & session)
            : MetaBase(rel_contract, prefix, session)
        {
            p_last_traded_price = locate_double_entry("last_traded_price");
            p_last_traded_volume = locate_long_entry("last_traded_volume");
            p_cum_traded_volume = locate_long_entry("cum_traded_volume");
        }

        // pointers
        double * p_last_traded_price;
        long * p_last_traded_volume;
        long * p_cum_traded_volume;
};

class TradeWriter : public MetaWriter, TradeBase {

    public:
        TradeWriter(const std::string & rel_contract, ShmSession & session)
            : MetaBase(rel_contract, TRADE_CLASS_PREFIX, session),
              MetaWriter(rel_contract, TRADE_CLASS_PREFIX, session), 
              TradeBase(rel_contract, TRADE_CLASS_PREFIX, session)
        {}

    protected:
        void write_derived(const MetaData * d) {
            const TradeData & data(*static_cast<const TradeData *>(d));

            *p_last_traded_price = data.last_traded_price;
            *p_last_traded_volume = data.last_traded_volume;
            *p_cum_traded_volume = data.cum_traded_volume;
        }

};

class TradeReader : public MetaReader, TradeBase {

    public:
        TradeReader(const std::string & rel_contract, ShmSession & session)
            : MetaBase(rel_contract, TRADE_CLASS_PREFIX, session),
              MetaReader(rel_contract, TRADE_CLASS_PREFIX, session), 
              TradeBase(rel_contract, TRADE_CLASS_PREFIX, session)
        {}

    protected:
        void read_derived(MetaData * d) {
            TradeData & data(*static_cast<TradeData *>(d));

            data.last_traded_price = *p_last_traded_price;
            data.last_traded_volume = *p_last_traded_volume;
            data.cum_traded_volume = *p_cum_traded_volume;
        }

};

#endif // RW_TRADE_H