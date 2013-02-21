#ifndef RW_META_H
#define RW_META_H

#include "ushdex.h"
#include "storeload.h"

// Are the next three lines needed?!
#include <sstream>
#include <string>
using std::stringstream;

char * hex_dump(const double d) {
    static char buffer[32];
    sprintf(buffer, "%a", d);
    return buffer;
}

struct MetaData {

    /*
     * to do: replace with this (discuss?)
     *

    long exch_id; // coming from FAST seq #
    long exch_ts; // coming from UDP arrival at FCN
    long prod_id; // currently ctr/2
    long prod_ts; // set by interface (slow?)
    */

    long input_id;
    long timestamp;
    long output_id;

    friend std::ostream & operator<< (std::ostream & o, const MetaData & self) {
        o << self.input_id << ',' << self.timestamp << ',' << self.output_id;
        return o; 
    }

};

class MetaBase {

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

        MetaBase(const std::string & rel_contract, const std::string & prefix, ShmSession & session)
            : rel_contract(rel_contract), prefix(prefix), session(session)
        {
            // Remark: In the current implementation,
            // we (currently) have output_id == ctr/2.
            // But this is an implemenation detail, let's not hardcode it!
            
            p_ctr = locate_long_entry("ctr");
            p_input_id = locate_long_entry("input_id");
            p_timestamp = locate_long_entry("timestamp");
            p_output_id = locate_long_entry("output_id");
        }

        // general variables
        const std::string rel_contract;
        const std::string prefix;
        ShmSession & session;
        
        // pointers
        volatile long *p_ctr;
        long *p_input_id;
        long *p_timestamp;
        long *p_output_id;

};

class MetaWriter : public virtual MetaBase {

    public:

        MetaWriter(const std::string & rel_contract, const std::string & prefix, ShmSession & session) 
            : MetaBase(rel_contract, prefix, session), ctr(*MetaBase::p_ctr)
        {}

        void write(const MetaData & data) {

            store<long>(p_ctr, ++ctr);

            *p_input_id = data.input_id;
            *p_timestamp = data.timestamp;
            *p_output_id = data.output_id;

            write_derived(&data);

            store<long>(p_ctr, ++ctr);
        }

    protected:
        virtual void write_derived(const MetaData * data) = 0;
        long ctr;

};

class MetaReader : public virtual MetaBase {

    public:

        MetaReader(const std::string & rel_contract, const std::string & prefix, ShmSession & session) 
            : MetaBase(rel_contract, prefix, session), previous_ctr(0)
        {}

        bool read(MetaData * d) {
            MetaData & data(*d);
            do {
                prior_ctr = load<long>(p_ctr);

                data.input_id = *p_input_id;
                data.timestamp = *p_timestamp;
                data.output_id = *p_output_id;

                read_derived(&data);

                posterior_ctr = load<long>(p_ctr);
             } while ((posterior_ctr != prior_ctr) || (posterior_ctr & 1));

            //data.id = posterior_ctr >> 1;

            if(posterior_ctr != previous_ctr) {
                previous_ctr = posterior_ctr;
                return true;
            } else
                return false;
        }

        void read_next(MetaData * data) {
            do {
                if(read(data))
                    return;
                // supposedly, this helps... but perhaps it's just FUD ;-)
                asm volatile ("pause" ::: "memory");
            } while (true);
        }

    protected:
        virtual void read_derived(MetaData * data) = 0;
        long previous_ctr;
        long prior_ctr, posterior_ctr;

};

#endif // RW_META_H
