#ifndef BOOK_RW_H
#define BOOK_RW_H

#include "meta_rw.h"
#include "book_types.h"

#include <algorithm>
#include <cassert>
#include <limits>
#include <sstream>
#include <vector>

namespace ush {

const char * const BOOK_DATA_PREFIX = "BookData::";
const double infinity = std::numeric_limits<double>::infinity();

class BookSize {

    public:
        BookSize(const long N, const long n) : N(N), n(n) {}
        long depth() { return N; }
        long implied_depth() { return n; }
    protected:
        long N;
        long n;
};


struct BookData : public BookSize, public MetaData {

    prices_t bids, asks;
    volumes_t bidvols, askvols;
    prices_t implied_bids, implied_asks;
    volumes_t implied_bidvols, implied_askvols;

    BookData(const long N, const long n) : BookSize(N, n), MetaData(), 
        bids(N), asks(N), bidvols(N), askvols(N),
        implied_bids(n), implied_asks(n), implied_bidvols(n), implied_askvols(n)
    {
        assert(N >= 1);
        assert(n >= 0);
    }

    BookData(BookSize & base)
     :  BookSize(base.depth(), base.implied_depth()),
        MetaData(),
        // TODO: check if we can use N, n here
        bids(base.depth()), asks(base.depth()), bidvols(base.depth()), askvols(base.depth()),
        implied_bids(base.implied_depth()), implied_asks(base.implied_depth()),
        implied_bidvols(base.implied_depth()), implied_askvols(base.implied_depth())
    {}

    friend std::ostream & operator<< (std::ostream & o, const BookData & self) {
        o << static_cast<const MetaData &>(self) << ',';

        const long N(self.N);
        const long n(self.n);
        o << N << ',';
        o << n << ',';

        for(long i = 0; i != N; ++i) {
            o << self.bids[i] << ',';
            o << hex_dump(self.bids[i]) << ',';
            o << self.asks[i] << ',';
            o << hex_dump(self.asks[i]) << ',';
            o << self.bidvols[i] << ',';
            o << self.askvols[i];
            if (i < N - 1) o << ',';
        }

        if (n != 0) {
            o << ',';
            for(long i = 0; i != n; ++i) {
                o << self.implied_bids[i] << ',';
                o << hex_dump(self.implied_bids[i]) << ',';
                o << self.implied_asks[i] << ',';
                o << hex_dump(self.implied_asks[i]) << ',';
                o << self.implied_bidvols[i] << ',';
                o << self.implied_askvols[i];
                if (i < n - 1) o << ',';
            }
        }

        return o;
    }

    double best_bid() { 
        if(n == 0)
            return bids[0];
        else
            return std::max(
                bids[0] > 0         ? bids[0]         : -infinity,
                implied_bids[0] > 0 ? implied_bids[0] : -infinity);
    }

    double best_ask() { 
        if(n == 0)
            return asks[0];
        else
            return std::min(
            asks[0] > 0         ? asks[0]         : infinity,
            implied_asks[0] > 0 ? implied_asks[0] : infinity); 
    }

};

class BookBase : public virtual MetaBase, public BookSize {
    protected:

        BookBase(const long N, const long n, const std::string & rel_contract, const std::string & prefix)
            : MetaBase(rel_contract, prefix), BookSize(N, n)
        {
            p_N = locate_long_entry(std::string("N"));
            p_n = locate_long_entry(std::string("n"));

            lookup_pointers();
        }

        void lookup_pointers() {
            p_bids.resize(N); p_asks.resize(N);
            p_bidvols.resize(N); p_askvols.resize(N);
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

            p_implied_bids.resize(n); p_implied_asks.resize(n);
            p_implied_bidvols.resize(n); p_implied_askvols.resize(n);
            for(long i = 0; i != n; ++i) {
                std::stringstream stream;
                stream << (i + 1);
                auto postfix1 = stream.str();
                stream << "vol";
                auto postfix2 = stream.str();

                p_implied_bids[i] = locate_double_entry(std::string("implied_bid") + postfix1);
                p_implied_asks[i] = locate_double_entry(std::string("implied_ask") + postfix1);
                p_implied_bidvols[i] = locate_long_entry(std::string("implied_bid") + postfix2);
                p_implied_askvols[i] = locate_long_entry(std::string("implied_ask") + postfix2);
            }
        }

        // pointers
        long *p_N;
        long *p_n;
        std::vector<double *> p_bids, p_asks;
        std::vector<long *> p_bidvols, p_askvols;
        std::vector<double *> p_implied_bids, p_implied_asks;
        std::vector<long *> p_implied_bidvols, p_implied_askvols;

};

class BookWriter : public MetaWriter<BookWriter, BookData>, public BookBase {

    public:
        BookWriter(const long N, const long n, const std::string & rel_contract)
            : MetaBase(rel_contract, BOOK_DATA_PREFIX),
              MetaWriter<BookWriter, BookData>(rel_contract, BOOK_DATA_PREFIX), 
              BookBase(N, n, rel_contract, BOOK_DATA_PREFIX)
        {
            store<long>(p_N, N);
            store<long>(p_n, n);
        }

        friend class MetaReader<BookWriter, BookData>;
        void write_derived(const BookData & data) {
            for(long i = 0; i != N; ++i) {
                *p_bids[i] = data.bids[i];
                *p_asks[i] = data.asks[i];
                *p_bidvols[i] = data.bidvols[i];
                *p_askvols[i] = data.askvols[i];
            }
            for(long i = 0; i != n; ++i) {
                *p_implied_bids[i] = data.implied_bids[i];
                *p_implied_asks[i] = data.implied_asks[i];
                *p_implied_bidvols[i] = data.implied_bidvols[i];
                *p_implied_askvols[i] = data.implied_askvols[i];
            }
        }
};

class BookReader : public MetaReader<BookReader, BookData>, public BookBase {

    public:
        BookReader(const std::string & rel_contract)
            : MetaBase(rel_contract, BOOK_DATA_PREFIX),
              MetaReader<BookReader, BookData>(rel_contract, BOOK_DATA_PREFIX), 
              BookBase(0, 0, rel_contract, BOOK_DATA_PREFIX)
        {
            N = load<long>(p_N);
            n = load<long>(p_n);
            lookup_pointers();
        }

        BookReader(const long N, const long n, const std::string & rel_contract)
            : MetaBase(rel_contract, BOOK_DATA_PREFIX),
              MetaReader<BookReader, BookData>(rel_contract, BOOK_DATA_PREFIX), 
              BookBase(0, 0, rel_contract, BOOK_DATA_PREFIX)
        {
            long that_N = load<long>(p_N);
            long that_n = load<long>(p_n);
            assert(N <= that_N);
            assert(n <= that_n);
            this->N = N;
            this->n = n;
            lookup_pointers();
        }

    protected:
        friend class MetaReader<BookReader, BookData>;
        void read_derived(BookData & data) {
            for(long i = 0; i != N; ++i) {
                data.bids[i] = *p_bids[i];
                data.asks[i] = *p_asks[i];
                data.bidvols[i] = *p_bidvols[i];
                data.askvols[i] = *p_askvols[i];
            }
            for(long i = 0; i != n; ++i) {
                data.implied_bids[i] = *p_implied_bids[i];
                data.implied_asks[i] = *p_implied_asks[i];
                data.implied_bidvols[i] = *p_implied_bidvols[i];
                data.implied_askvols[i] = *p_implied_askvols[i];
            }
        }

};

} // namespace ush

#endif // BOOK_RW_H
