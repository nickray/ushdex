#ifndef FEEDSTRUCTS_H
#define FEEDSTRUCTS_H

#include "ushdex.h"

const long N = 1000000;

struct Top1Data {

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
            p_ctr = &session.longs()[SessionKey(rel_contract, "Top1Data_ctr", session)];
            p_timestamp = &session.longs()[SessionKey(rel_contract, "Top1Data_timestamp", session)];
            p_bid1 = &session.doubles()[SessionKey(rel_contract, "Top1Data_bid1", session)];
            p_ask1 = &session.doubles()[SessionKey(rel_contract, "Top1Data_ask1", session)];
            p_bid1vol = &session.longs()[SessionKey(rel_contract, "Top1Data_bid1vol", session)];
            p_ask1vol = &session.longs()[SessionKey(rel_contract, "Top1Data_ask1vol", session)];
        }

        void write(const Top1Data & data) {
            ++*p_ctr;
            asm volatile("sfence" ::: "memory");
            *p_timestamp = data.timestamp;
            *p_bid1 = data.bid1;
            *p_ask1 = data.ask1;
            *p_bid1vol = data.bid1vol;
            *p_ask1vol = data.ask1vol;
            ++*p_ctr;
        }

    protected:

        ShmSession & session;

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
            p_ctr = &session.longs()[SessionKey(rel_contract, "Top1Data_ctr", session)];
            p_timestamp = &session.longs()[SessionKey(rel_contract, "Top1Data_timestamp", session)];
            p_bid1 = &session.doubles()[SessionKey(rel_contract, "Top1Data_bid1", session)];
            p_ask1 = &session.doubles()[SessionKey(rel_contract, "Top1Data_ask1", session)];
            p_bid1vol = &session.longs()[SessionKey(rel_contract, "Top1Data_bid1vol", session)];
            p_ask1vol = &session.longs()[SessionKey(rel_contract, "Top1Data_ask1vol", session)];
        }

        void read(Top1Data & data) {
            do {
                prior_ctr = *p_ctr;
                data.timestamp = *p_timestamp;
                data.bid1 = *p_bid1;
                data.ask1 = *p_ask1;
                data.bid1vol = *p_bid1vol;
                data.ask1vol = *p_ask1vol;

                asm volatile("lfence" ::: "memory");
                posterior_ctr = *p_ctr;
             } 
                while ((posterior_ctr != prior_ctr) || (posterior_ctr & 1));
        }

    protected:

        ShmSession & session;

        volatile long *p_ctr; // check: should this be "volatile const long *"?
        long prior_ctr, posterior_ctr;

        const long *p_timestamp;
        
        const double *p_bid1, *p_ask1;
        const long *p_bid1vol, *p_ask1vol;

};

#endif
