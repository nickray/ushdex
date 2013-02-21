#ifndef RW_TOPN_H
#define RW_TOPN_H

#include "meta_rw.h"

#include <array>

template <long N>
constexpr std::string TopPrefix() {
        stringstream stream;
        stream << "Top" << N << "Data::";
        return stream.str();
}

template <long N>
struct TopData : public MetaData {

    std::array<double, N> bid, ask;
    std::array<long, N> bidvol, askvol;

    TopData() : MetaData(), bid(), ask(), bidvol(), askvol() {}

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

                p_bid[i] = locate_double_entry(std::string("bid") + postfix1);
                p_ask[i] = locate_double_entry(std::string("ask") + postfix1);
                p_bidvol[i] = locate_long_entry(std::string("bid") + postfix2);
                p_askvol[i] = locate_long_entry(std::string("ask") + postfix2);
            }

        }
        // pointers
        std::array<double *, N> p_bid, p_ask;
        std::array<long *, N> p_bidvol, p_askvol;

};

template <long N>
class TopWriter : public MetaWriter, TopBase<N> {

    public:
        TopWriter(const std::string & rel_contract, ShmSession & session)
            : MetaBase(rel_contract, TopPrefix<N>(), session),
              MetaWriter(rel_contract, TopPrefix<N>(), session), 
              TopBase<N>(rel_contract, TopPrefix<N>(), session)
        {}

        void write_derived(const MetaData * d) {
            const TopData<N> & data(*static_cast<const TopData<N> *>(d));

            for(long i = 0; i != N; ++i) {
                *TopBase<N>::p_bid[i] = data.bid[i];
                *TopBase<N>::p_ask[i] = data.ask[i];
                *TopBase<N>::p_bidvol[i] = data.bidvol[i];
                *TopBase<N>::p_askvol[i] = data.askvol[i];
            }
        }
};

template <long N>
class TopReader : public MetaReader, TopBase<N> {

    public:

        TopReader(const std::string & rel_contract, ShmSession & session)
            : MetaBase(rel_contract, TopPrefix<N>(), session),
              MetaReader(rel_contract, TopPrefix<N>(), session), 
              TopBase<N>(rel_contract, TopPrefix<N>(), session)
        {}

        void read_derived(MetaData * d) {
            TopData<N> & data(*static_cast<TopData<N> *>(d));

            for(long i = 0; i != N; ++i) {
                data.bid[i] = *TopBase<N>::p_bid[i];
                data.ask[i] = *TopBase<N>::p_ask[i];
                data.bidvol[i] = *TopBase<N>::p_bidvol[i];
                data.askvol[i] = *TopBase<N>::p_askvol[i];
            }
        }

};

#endif // RW_TOPN_H

