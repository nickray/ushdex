#ifndef ADAPTORS_H
#define ADAPTORS_H

#include "book_rw.h"

namespace ush {

struct BestAdaptor {

    BestAdaptor(BookData & data) : data(data) {}

    void apply() {
        bid = data.best_bid();
        ask = data.best_ask();
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
