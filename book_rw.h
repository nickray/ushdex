#ifndef BOOK_RW_H
#define BOOK_RW_H

#include "meta_rw.h"
#include "book_types.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <sstream>
#include <vector>

namespace ush {

const char * const BOOK_DATA_PREFIX = "BookData::";

struct BookData : public MetaData {

    long depth;
    prices_t bids, asks;
    volumes_t bidvols, askvols;

    BookData(const long depth)
      : MetaData(), depth(depth)
      , bids(depth), asks(depth), bidvols(depth), askvols(depth)
    {
#ifdef USE_EIGEN
        bids.setZero(); asks.setZero(); bidvols.setZero(); askvols.setZero();
#endif
    }

    friend std::ostream & operator<< (std::ostream & o, const BookData & self) {
        o << static_cast<const MetaData &>(self);

        const long depth(self.depth);
        o << ',' << depth;

        for(long i = 0; i != depth; ++i) {
            o << ',';
            o << self.bids[i] << ',';
            o << hex_dump(self.bids[i]) << ',';
            o << self.asks[i] << ',';
            o << hex_dump(self.asks[i]) << ',';
            o << self.bidvols[i] << ',';
            o << self.askvols[i];
        }

        return o;
    }

    long serialize(const std::string& rel_contract, char * buffer) {

        long offset = MetaData::serialize(rel_contract, buffer);

        long num = sizeof(long);
        memcpy(buffer + offset, &depth, num);
        offset += num;

        num = depth*sizeof(double);
        memcpy(buffer + offset, bids.data(), num);
        offset += num;
        memcpy(buffer + offset, asks.data(), num);
        offset += num;

        num = depth*sizeof(long);
        memcpy(buffer + offset, bidvols.data(), num);
        offset += num;
        memcpy(buffer + offset, askvols.data(), num);
        offset += num;

        return offset;

    };

    long deserialize(const char * const buffer, std::string & rel_contract) {

        long offset = MetaData::deserialize(buffer, rel_contract);

        long num = sizeof(long);
        memcpy(&depth, buffer + offset, num);
        offset += num;

        num = depth*sizeof(double);
        memcpy(bids.data(), buffer + offset, num);
        offset += num;
        memcpy(asks.data(), buffer + offset, num);
        offset += num;

        num = depth*sizeof(long);
        memcpy(bidvols.data(), buffer + offset, num);
        offset += num;
        memcpy(askvols.data(), buffer + offset, num);
        offset += num;

        return offset;
    }
};

class BookBase : public virtual MetaBase {

    public:
        long depth;

    protected:

        BookBase(const long depth, const std::string & rel_contract, const std::string & prefix)
          : MetaBase(rel_contract, prefix), depth(depth)
        {
            p_depth = locate_long_entry(std::string("depth"));
            lookup_pointers();
        }

        BookBase(const std::string & rel_contract, const std::string & prefix)
          : MetaBase(rel_contract, prefix)
        {
            p_depth = locate_long_entry(std::string("depth"));
            depth = load<long>(p_depth);
            lookup_pointers();
        }

        void lookup_pointers() {
            p_bids.resize(depth); p_asks.resize(depth);
            p_bidvols.resize(depth); p_askvols.resize(depth);
            for(long i = 0; i != depth; ++i) {
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
        long *p_depth;
        std::vector<double *> p_bids, p_asks;
        std::vector<long *> p_bidvols, p_askvols;
};

class BookWriter : public MetaWriter<BookWriter, BookData>, public BookBase {

    public:
        // Note that BookWriter.write can process BookData with
        // BookWriter.depth <= BookData.depth, if only that much is required
        BookWriter(const long depth, const std::string & rel_contract)
          : MetaBase(rel_contract, BOOK_DATA_PREFIX)
          , MetaWriter<BookWriter, BookData>(rel_contract, BOOK_DATA_PREFIX)
          , BookBase(depth, rel_contract, BOOK_DATA_PREFIX)
        {
            store<long>(p_depth, depth);
        }

        friend class MetaReader<BookWriter, BookData>;
        void write_derived(const BookData & data) {
            for(long i = 0; i != depth; ++i) {
                *p_bids[i] = data.bids[i];
                *p_asks[i] = data.asks[i];
                *p_bidvols[i] = data.bidvols[i];
                *p_askvols[i] = data.askvols[i];
            }
        }
};

class BookReader : public MetaReader<BookReader, BookData>, public BookBase {

    public:
        // Use this if you want as much depth as MD.EXCHANGE offers
        BookReader(const std::string & rel_contract)
          : MetaBase(rel_contract, BOOK_DATA_PREFIX)
          , MetaReader<BookReader, BookData>(rel_contract, BOOK_DATA_PREFIX)
          , BookBase(rel_contract, BOOK_DATA_PREFIX)
        {}

        // Use this if you want (possibly) less depth than MD.EXCHANGE offers
        BookReader(const long depth, const std::string & rel_contract)
          : MetaBase(rel_contract, BOOK_DATA_PREFIX)
          , MetaReader<BookReader, BookData>(rel_contract, BOOK_DATA_PREFIX)
          , BookBase(rel_contract, BOOK_DATA_PREFIX)
        {
            // make sure offered depth (read from shm) is big enough
            assert(depth <= this->depth);
            this->depth = depth;
        }

    protected:
        friend class MetaReader<BookReader, BookData>;
        void read_derived(BookData & data) {
            for(long i = 0; i != depth; ++i) {
                data.bids[i] = *p_bids[i];
                data.asks[i] = *p_asks[i];
                data.bidvols[i] = *p_bidvols[i];
                data.askvols[i] = *p_askvols[i];
            }
        }

};

} // namespace ush

#endif // BOOK_RW_H
