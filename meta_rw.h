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

const char * readable(const long t)
{
    static char now_buffer[32 + 1]; // + 1 for '\0'
    enum { million = 1000000 };
    long seconds(t/million);
    strftime(now_buffer, 32, "%y%m%d.%H%M%S.", localtime(&seconds));
    snprintf(&now_buffer[14], 32 - 14, "%06ld", t % million);
    return now_buffer;
}


struct MetaData {

    long timestamp; // UDP packet arrival at FCN
    long input_id;  // FAST sequence number
    long output_id; // ctr/2

    friend std::ostream & operator<< (std::ostream & o, const MetaData & self) {
        o << readable(self.timestamp) << ',' << self.input_id << ',' << self.output_id;
        return o; 
    }

    bool operator==(const MetaData & other) const {
        return (( timestamp == other.timestamp ) &&
                ( input_id == other.input_id ));
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
            p_timestamp = locate_long_entry("timestamp");
            p_input_id = locate_long_entry("input_id");
            p_output_id = locate_long_entry("output_id");
        }

        // general variables
        const std::string rel_contract;
        const std::string prefix;
        ShmSession & session;
        
        // pointers
        volatile long *p_ctr;
        long *p_timestamp;
        long *p_input_id;
        long *p_output_id;

};

/*
 * For future readers: Before MetaWriter/MetaReader were templated on their
 * derived classes, write_derived/read_derived were pure virtual methods
 * and called in write/read directly. By using the "curiously recurring 
 * template pattern", this virtual table lookup can be avoided!
 *
 * Re. renaming write_derived simply as write, the problem is that
 * MetaWriter::write is public while Writer::derived_write is protected.
 *
 */

template <class Writer, class Data>
class MetaWriter : public virtual MetaBase {

    public:

        MetaWriter(const std::string & rel_contract, const std::string & prefix, ShmSession & session) 
            : MetaBase(rel_contract, prefix, session), ctr(*MetaBase::p_ctr)
        {
            // Recover from potentially crashed previous writer
            if(ctr & 1)
                store<long>(p_ctr, ++ctr);
        }

        void write(const Data & data) {

            store<long>(p_ctr, ++ctr);

            *p_timestamp = data.timestamp;
            *p_input_id = data.input_id;
            *p_output_id = (ctr + 1)/2;

            static_cast<Writer*>(this)->write_derived(data);

            store<long>(p_ctr, ++ctr);
        }

    protected:
        long ctr;

};

template <class Reader, class Data>
class MetaReader : public virtual MetaBase {

    public:

        MetaReader(const std::string & rel_contract, const std::string & prefix, ShmSession & session) 
            : MetaBase(rel_contract, prefix, session), previous_ctr(0)
        {}

        bool read(Data & data) {
            do {
                prior_ctr = load<long>(p_ctr);

                data.input_id = *p_input_id;
                data.timestamp = *p_timestamp;
                data.output_id = *p_output_id;

                static_cast<Reader*>(this)->read_derived(data);

                posterior_ctr = load<long>(p_ctr);
             } while ((posterior_ctr != prior_ctr) || (posterior_ctr & 1));

            if(posterior_ctr != previous_ctr) {
                previous_ctr = posterior_ctr;
                return true;
            } else
                return false;
        }

        void read_next(Data & data) {
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

#endif // RW_META_H
