#ifndef TOPN_RW_H
#define TOPN_RW_H

#include "meta_rw.h"
#include "topN_types.h"

#include <algorithm>
#include <sstream>
#include <vector>

namespace ush {

inline std::string TOP_DATA_PREFIX(long N) {
        std::stringstream stream;
        stream << "Top" << N << "Data::";
        return stream.str();
}

struct TopData : public MetaData {

    const long N;
    prices_t bids, asks;
    volumes_t bidvols, askvols;
    prices_t implied_bids, implied_asks;
    volumes_t implied_bidvols, implied_askvols;

    TopData(const long N) : MetaData(), N(N), bids(N), asks(N), bidvols(N), askvols(N),
        implied_bids(N), implied_asks(N), implied_bidvols(N), implied_askvols(N)
    {}

    friend std::ostream & operator<< (std::ostream & o, const TopData & self) {
        o << static_cast<const MetaData &>(self) << ',';

        const long N(self.N);
        o << N << ',';

        for(long i = 0; i != N; ++i) {
            o << self.bids[i] << ',';
            o << hex_dump(self.bids[i]) << ',';
            o << self.asks[i] << ',';
            o << hex_dump(self.asks[i]) << ',';
            o << self.bidvols[i] << ',';
            o << self.askvols[i] << ',';
        }

        for(long i = 0; i != N - 1; ++i) {
            o << self.implied_bids[i] << ',';
            o << hex_dump(self.implied_bids[i]) << ',';
            o << self.implied_asks[i] << ',';
            o << hex_dump(self.implied_asks[i]) << ',';
            o << self.implied_bidvols[i] << ',';
            o << self.implied_askvols[i] << ',';
        }

        o << self.implied_bids[N - 1] << ',';
        o << hex_dump(self.implied_bids[N - 1]) << ',';
        o << self.implied_asks[N - 1] << ',';
        o << hex_dump(self.implied_asks[N - 1]) << ',';
        o << self.implied_bidvols[N - 1] << ',';
        o << self.implied_askvols[N - 1]; // Note the missing comma

        return o;
    }

    double best_bid() { return std::max(
            asks[0] > 0         ? asks[0]         : -INFINITY,
            implied_asks[0] > 0 ? implied_asks[0] : -INFINITY); }

    double best_ask() { return std::min(
            asks[0] > 0         ? asks[0]         : INFINITY,
            implied_asks[0] > 0 ? implied_asks[0] : INFINITY); }

};

class TopBase : public virtual MetaBase {
    protected:

        TopBase(const long N, const std::string & rel_contract, const std::string & prefix)
            : MetaBase(rel_contract, prefix), N(N),
              p_bids(N), p_asks(N), p_bidvols(N), p_askvols(N),
              p_implied_bids(N), p_implied_asks(N), 
              p_implied_bidvols(N), p_implied_askvols(N)
        {
            for(long i = 0; i != N; ++i) {
                std::stringstream stream;
                stream << (i + 1);
                auto postfix1 = stream.str();
                stream << "vol";
                auto postfix2 = stream.str();

                p_bids[i] = locate_double_entry(std::string("bid") + postfix1);
                p_asks[i] = locate_double_entry(std::string("ask") + postfix1);
                p_bidvols[i] = locate_long_entry(std::string("bid") + postfix2);
                p_askvols[i] = locate_long_entry(std::string("ask") + postfix2);
                p_implied_bids[i] = locate_double_entry(std::string("implied_bid") + postfix1);
                p_implied_asks[i] = locate_double_entry(std::string("implied_ask") + postfix1);
                p_implied_bidvols[i] = locate_long_entry(std::string("implied_bid") + postfix2);
                p_implied_askvols[i] = locate_long_entry(std::string("implied_ask") + postfix2);
            }

        }

        const long N;

        // pointers
        std::vector<double *> p_bids, p_asks;
        std::vector<long *> p_bidvols, p_askvols;
        std::vector<double *> p_implied_bids, p_implied_asks;
        std::vector<long *> p_implied_bidvols, p_implied_askvols;

};

class TopWriter : public MetaWriter<TopWriter, TopData>, TopBase {

    public:
        TopWriter(const long N, const std::string & rel_contract)
            : MetaBase(rel_contract, TOP_DATA_PREFIX(N)),
              MetaWriter<TopWriter, TopData>(rel_contract, TOP_DATA_PREFIX(N)), 
              TopBase(N, rel_contract, TOP_DATA_PREFIX(N))
        {}

        friend class MetaReader<TopWriter, TopData>;
        void write_derived(const TopData & data) {
            for(long i = 0; i != N; ++i) {
                *p_bids[i] = data.bids[i];
                *p_asks[i] = data.asks[i];
                *p_bidvols[i] = data.bidvols[i];
                *p_askvols[i] = data.askvols[i];
                *p_implied_bids[i] = data.implied_bids[i];
                *p_implied_asks[i] = data.implied_asks[i];
                *p_implied_bidvols[i] = data.implied_bidvols[i];
                *p_implied_askvols[i] = data.implied_askvols[i];
            }
        }
};

class TopReader : public MetaReader<TopReader, TopData>, TopBase {

    public:
        TopReader(const long N, const std::string & rel_contract)
            : MetaBase(rel_contract, TOP_DATA_PREFIX(N)),
              MetaReader<TopReader, TopData>(rel_contract, TOP_DATA_PREFIX(N)), 
              TopBase(N, rel_contract, TOP_DATA_PREFIX(N))
        {}

    protected:
        friend class MetaReader<TopReader, TopData>;
        void read_derived(TopData & data) {
            for(long i = 0; i != N; ++i) {
                data.bids[i] = *p_bids[i];
                data.asks[i] = *p_asks[i];
                data.bidvols[i] = *p_bidvols[i];
                data.askvols[i] = *p_askvols[i];
                data.implied_bids[i] = *p_implied_bids[i];
                data.implied_asks[i] = *p_implied_asks[i];
                data.implied_bidvols[i] = *p_implied_bidvols[i];
                data.implied_askvols[i] = *p_implied_askvols[i];
            }
        }

};

} // namespace ush

#endif // TOPN_RW_H
