#ifndef RW_TOPN_H
#define RW_TOPN_H

#include "meta_rw.h"

#include <array>

template <long N>
constexpr std::string TOP_DATA_PREFIX() {
        stringstream stream;
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
            o <<
                self.bid[i] << ',' <<
                self.ask[i] << ',' <<
                self.bidvol[i] << ',' <<
                self.askvol[i] << ',';
                o << hex_dump(self.bid[i]) << ',';
                o << hex_dump(self.ask[i]) << ',';
        }

        o <<
            self.bid[N - 1] << ',' <<
            self.ask[N - 1] << ',' <<
            self.bidvol[N - 1] << ',' <<
            self.askvol[N - 1] << ',';
            o << hex_dump(self.bid[N - 1]) << ',';
            o << hex_dump(self.ask[N - 1]); // Note the missing comma

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
                stringstream stream;
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
class TopWriter : public MetaWriter< TopWriter<N> >, TopBase<N> {

    public:
        TopWriter(const std::string & rel_contract, ShmSession & session)
            : MetaBase(rel_contract, TOP_DATA_PREFIX<N>(), session),
              MetaWriter< TopWriter<N> >(rel_contract, TOP_DATA_PREFIX<N>(), session), 
              TopBase<N>(rel_contract, TOP_DATA_PREFIX<N>(), session)
        {}

        friend class MetaReader<TopWriter<N>>;
        void write_derived(const MetaData * d) {
            const TopData<N> & data(*static_cast<const TopData<N> *>(d));

            for(long i = 0; i != N; ++i) {
                *TopBase<N>::p_bids[i] = data.bids[i];
                *TopBase<N>::p_asks[i] = data.asks[i];
                *TopBase<N>::p_bidvols[i] = data.bidvols[i];
                *TopBase<N>::p_askvols[i] = data.askvols[i];
            }
        }
};

template <long N>
class TopReader : public MetaReader< TopReader<N> >, TopBase<N> {

    public:
        TopReader(const std::string & rel_contract, ShmSession & session)
            : MetaBase(rel_contract, TOP_DATA_PREFIX<N>(), session),
              MetaReader< TopReader<N> >(rel_contract, TOP_DATA_PREFIX<N>(), session), 
              TopBase<N>(rel_contract, TOP_DATA_PREFIX<N>(), session)
        {}

    protected:
        friend class MetaReader<TopReader<N>>;
        void read_derived(MetaData * d) {
            TopData<N> & data(*static_cast<TopData<N> *>(d));

            for(long i = 0; i != N; ++i) {
                data.bids[i] = *TopBase<N>::p_bids[i];
                data.asks[i] = *TopBase<N>::p_asks[i];
                data.bidvols[i] = *TopBase<N>::p_bidvols[i];
                data.askvols[i] = *TopBase<N>::p_askvols[i];
            }
        }

};

#endif // RW_TOPN_H

