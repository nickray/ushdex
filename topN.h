#ifndef TOPN_H
#define TOPN_H

#include "ushdex.h"
#include "storeload.h"

#include <sstream>
#include <string>
using std::stringstream;

template <long N>
struct TopData {

    long timestamp;
    double bid[N], ask[N];
    long bidvol[N], askvol[N];

    friend std::ostream & operator<< (std::ostream & o, const TopData<N> & self) {
        o << self.timestamp << ',';

        char buffer[32];
        for(unsigned long i = 0; i != N - 1; ++i) {
            o <<
                self.bid[i] << ',' <<
                self.ask[i] << ',' <<
                self.bidvol[i] << ',' <<
                self.askvol[i] << ',';

                sprintf(buffer, "%a", self.bid[i]);
                o <<  buffer << ',';
                sprintf(buffer, "%a", self.ask[i]);
                o <<  buffer << ',';
        }

        o <<
            self.bid[N - 1] << ',' <<
            self.ask[N - 1] << ',' <<
            self.bidvol[N - 1] << ',' <<
            self.askvol[N - 1] << ',';

            sprintf(buffer, "%a", self.bid[N - 1]);
            o <<  buffer << ',';
            sprintf(buffer, "%a", self.ask[N - 1]);
            o <<  buffer; // Note the missing comma

        return o;
    }

};

template <long N>
class TopBase {
    protected:
        long * locate_long_entry(const std::string & name) {
            stringstream stream; 
            stream << prefix << name;
            return &session.longs()[SessionKey(rel_contract, stream.str(), session)];
        }

        double * locate_double_entry(const std::string & name) {
            stringstream stream;
            stream << prefix << name;
            return &session.doubles()[SessionKey(rel_contract, stream.str(), session)];
        }

        TopBase(const std::string & rel_contract, ShmSession & session)
            : rel_contract(rel_contract), session(session)
        {
            {
                stringstream stream; stream << "Top" << N << "Data::";
                prefix = stream.str();
            }

            p_ctr = locate_long_entry("ctr");
            p_timestamp = locate_long_entry("timestamp");

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

        // general variables
        const std::string rel_contract;
        ShmSession & session;
        std::string prefix;

        // pointers
        volatile long *p_ctr;
        long *p_timestamp;
        double *p_bid[N], *p_ask[N];
        long *p_bidvol[N], *p_askvol[N];

};

template <long N>
class TopWriter : public TopBase<N> {

    public:
        TopWriter(const std::string & rel_contract, ShmSession & session)
            : TopBase<N>(rel_contract, session), ctr(*TopBase<N>::p_ctr)
        {}

        void write(const TopData<N> & data) {
            // One of the great joys of C++ template syntax ;-)
            //
            // A simple p_ctr lookup would not work, even though 
            // it's completely obvious that it can't depend on N.
            store<long>(TopBase<N>::p_ctr, ++ctr);
            *TopBase<N>::p_timestamp = data.timestamp;
            for(long i = 0; i != N; ++i) {
                *TopBase<N>::p_bid[i] = data.bid[i];
                *TopBase<N>::p_ask[i] = data.ask[i];
                *TopBase<N>::p_bidvol[i] = data.bidvol[i];
                *TopBase<N>::p_askvol[i] = data.askvol[i];
            }
            store<long>(TopBase<N>::p_ctr, ++ctr);
        }

    protected:
        long ctr;
};

template <long N>
class TopReader : public TopBase<N> {

    public:

        TopReader(const std::string & rel_contract, ShmSession & session)
            : TopBase<N>(rel_contract, session), previous_ctr(-1),
              prior_ctr(-1), posterior_ctr(-1)
        {}

        bool read(TopData<N> & data) {
            do {
                prior_ctr = load<long>(TopBase<N>::p_ctr);

                data.timestamp = *TopBase<N>::p_timestamp;
                for(long i = 0; i != N; ++i) {
                    data.bid[i] = *TopBase<N>::p_bid[i];
                    data.ask[i] = *TopBase<N>::p_ask[i];
                    data.bidvol[i] = *TopBase<N>::p_bidvol[i];
                    data.askvol[i] = *TopBase<N>::p_askvol[i];
                }

                posterior_ctr = load<long>(TopBase<N>::p_ctr);
             } while ((posterior_ctr != prior_ctr) || (posterior_ctr & 1));

            if(posterior_ctr != previous_ctr) {
                previous_ctr = posterior_ctr;
                return true;
            } else
                return false;
        }

        void read_next(Top1Data & data) {
            do {
                if(read(data))
                    return;
                // supposedly, this helps... but perhaps it's just FUD ;-)
                asm volatile ("pause" ::: "memory");
            } while (true);
        }

    protected:
        long previous_ctr;
        long prior_ctr, posterior_ctr;
};

#endif
