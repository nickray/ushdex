#ifndef TOP1_H
#define TOP1_H

/*
 * Old crufty legacy class -- use TopData<1> or RedData in production!
 *
 */

#include "session.h"
#include "util.h"

namespace ush {

const long N = 1000000;

struct Top1Data {

    long id;
    long timestamp;

    double bid1, ask1;
    long bid1vol, ask1vol;

    friend std::ostream & operator<< (std::ostream & o, const Top1Data & self) {
        o << 
            self.timestamp << ',' <<
            self.bid1 << ',' <<
            self.ask1 << ',' <<
            self.bid1vol << ',' <<
            self.ask1vol << ',';

        char buffer[32];
        sprintf(buffer, "%a", self.bid1);
        o <<  buffer << ',';
        sprintf(buffer, "%a", self.ask1);
        o <<  buffer;

        return o;
    }

};

class Top1Writer {

    public:

        const std::string rel_contract;
        Top1Data data;

        Top1Writer(const std::string & rel_contract, ShmSession & session)
            : rel_contract(rel_contract), session(session)
        {
            p_ctr = &session.longs()[SessionKey(rel_contract, "Top1Data::ctr", session)];
            p_timestamp = &session.longs()[SessionKey(rel_contract, "Top1Data::timestamp", session)];
            p_bid1 = &session.doubles()[SessionKey(rel_contract, "Top1Data::bid1", session)];
            p_ask1 = &session.doubles()[SessionKey(rel_contract, "Top1Data::ask1", session)];
            p_bid1vol = &session.longs()[SessionKey(rel_contract, "Top1Data::bid1vol", session)];
            p_ask1vol = &session.longs()[SessionKey(rel_contract, "Top1Data::ask1vol", session)];
            ctr = *p_ctr;
        }

        void write(const Top1Data & data) {
            store<long>(p_ctr, ++ctr);
            
            *p_timestamp = data.timestamp;
            *p_bid1 = data.bid1;
            *p_ask1 = data.ask1;
            *p_bid1vol = data.bid1vol;
            *p_ask1vol = data.ask1vol;

            store<long>(p_ctr, ++ctr);
        }

    protected:

        ShmSession & session;

        long ctr;
        long *p_ctr;

        long *p_timestamp;
        
        double *p_bid1, *p_ask1;
        long *p_bid1vol, *p_ask1vol;

};

class Top1Reader {

    public:

        const std::string rel_contract;
        Top1Data data;

        Top1Reader(const std::string & rel_contract, ShmSession & session)
            : rel_contract(rel_contract), session(session)
        {
            p_ctr = &session.longs()[SessionKey(rel_contract, "Top1Data::ctr", session)];
            p_timestamp = &session.longs()[SessionKey(rel_contract, "Top1Data::timestamp", session)];
            p_bid1 = &session.doubles()[SessionKey(rel_contract, "Top1Data::bid1", session)];
            p_ask1 = &session.doubles()[SessionKey(rel_contract, "Top1Data::ask1", session)];
            p_bid1vol = &session.longs()[SessionKey(rel_contract, "Top1Data::bid1vol", session)];
            p_ask1vol = &session.longs()[SessionKey(rel_contract, "Top1Data::ask1vol", session)];
        }

        bool read(Top1Data & data) {
            do {
                prior_ctr = load<long>(p_ctr);

                data.timestamp = *p_timestamp;
                data.bid1 = *p_bid1;
                data.ask1 = *p_ask1;
                data.bid1vol = *p_bid1vol;
                data.ask1vol = *p_ask1vol;

                posterior_ctr = load<long>(p_ctr);
             } while ((posterior_ctr != prior_ctr) || (posterior_ctr & 1));

            data.id = posterior_ctr >> 1;

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

        ShmSession & session;

        volatile long *p_ctr; // check: should this be "volatile const long *"?
        long previous_ctr;
        long prior_ctr, posterior_ctr;

        const long *p_timestamp;
        
        const double *p_bid1, *p_ask1;
        const long *p_bid1vol, *p_ask1vol;

};

}

#endif
