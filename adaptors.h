#ifndef ADAPTORS_H
#define ADAPTORS_H

 // The contents here are experimental -- not sure it's a good idea yet!

#include "book_rw.h"

namespace ush {

struct BestAdaptor {

    BestAdaptor(BookData & data) : data(data) {}

    void apply() {
        bid = data.bids[0];
        ask = data.asks[0];
    }

    friend std::ostream & operator<< (std::ostream & o, const BestAdaptor & self) {
        o << static_cast<const MetaData &>(self.data) << ',';

        o << self.bid << ',';
        o << hex_dump(self.bid) << ',';
        o << self.ask << ',';
        o << hex_dump(self.ask);

        return o;
    }

    double bid;
    double ask;

    BookData & data;

};

} // namespace ush

#endif // ADAPTORS_H
