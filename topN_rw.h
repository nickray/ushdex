#ifndef TOPN_RW_H
#define TOPN_RW_H

#include "meta_rw.h"

#include <array>
#include <sstream>

namespace ush {

template <long N>
constexpr std::string TOP_DATA_PREFIX() {
        std::stringstream stream;
        stream << "Top" << N << "Data::";
        return stream.str();
}

template <long N>
struct TopData : public MetaData {

    std::array<double, N> bids, asks;
    std::array<long, N> bidvols, askvols;

    TopData() : MetaData(), bids(), asks(), bidvols(), askvols() {}

    friend std::ostream & operator<< (std::ostream & o, const TopData<N> & self) {
        o << static_cast<const MetaData &>(self) << ',';

        for(unsigned long i = 0; i != N - 1; ++i) {
            o << self.bids[i] << ',';
            o << hex_dump(self.bids[i]) << ',';
            o << self.asks[i] << ',';
            o << hex_dump(self.asks[i]) << ',';
            o << self.bidvols[i] << ',';
            o << self.askvols[i] << ',';
        }

        o << self.bids[N - 1] << ',';
        o << hex_dump(self.bids[N - 1]) << ',';
        o << self.asks[N - 1] << ',';
        o << hex_dump(self.asks[N - 1]) << ',';
        o << self.bidvols[N - 1] << ',';
        o << self.askvols[N - 1]; // Note the missing comma

        return o;
    }

};

template <long N>
class TopBase : public virtual MetaBase {
    protected:

        TopBase(const std::string & rel_contract, const std::string & prefix, ShmSession & session)
            : MetaBase(rel_contract, prefix, session)
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
            }

        }
        // pointers
        std::array<double *, N> p_bids, p_asks;
        std::array<long *, N> p_bidvols, p_askvols;

};

template <long N>
class TopWriter : public MetaWriter< TopWriter<N>, TopData<N> >, TopBase<N> {

    public:
        TopWriter(const std::string & rel_contract, ShmSession & session)
            : MetaBase(rel_contract, TOP_DATA_PREFIX<N>(), session),
              MetaWriter< TopWriter<N>, TopData<N> >(rel_contract, TOP_DATA_PREFIX<N>(), session), 
              TopBase<N>(rel_contract, TOP_DATA_PREFIX<N>(), session)
        {}

        friend class MetaReader< TopWriter<N>, TopData<N> >;
        void write_derived(const TopData<N> & data) {
            for(long i = 0; i != N; ++i) {
                *TopBase<N>::p_bids[i] = data.bids[i];
                *TopBase<N>::p_asks[i] = data.asks[i];
                *TopBase<N>::p_bidvols[i] = data.bidvols[i];
                *TopBase<N>::p_askvols[i] = data.askvols[i];
            }
        }
};

template <long N>
class TopReader : public MetaReader< TopReader<N>, TopData<N> >, TopBase<N> {

    public:
        TopReader(const std::string & rel_contract, ShmSession & session)
            : MetaBase(rel_contract, TOP_DATA_PREFIX<N>(), session),
              MetaReader< TopReader<N>, TopData<N> >(rel_contract, TOP_DATA_PREFIX<N>(), session), 
              TopBase<N>(rel_contract, TOP_DATA_PREFIX<N>(), session)
        {}

    protected:
        friend class MetaReader< TopReader<N>, TopData<N> >;
        void read_derived(TopData<N> & data) {
            for(long i = 0; i != N; ++i) {
                data.bids[i] = *TopBase<N>::p_bids[i];
                data.asks[i] = *TopBase<N>::p_asks[i];
                data.bidvols[i] = *TopBase<N>::p_bidvols[i];
                data.askvols[i] = *TopBase<N>::p_askvols[i];
            }
        }

};

} // namespace ush

#endif // TOPN_RW_H
