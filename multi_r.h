#ifndef MULTI_R_H
#define MULTI_R_H

#include "boost/dynamic_bitset.hpp"
#include <vector>

namespace ush {

template <class Reader, class Data>
class MultiReader {

public:
    MultiReader(std::vector<Reader> readers) : readers(readers) {
        setup();
    }

    MultiReader(const std::vector<std::string> & rel_contracts) : readers() {
        for(auto it = rel_contracts.cbegin(); it != rel_contracts.cend(); ++it)
            readers.emplace_back(Reader(*it));
        setup();
    }

    // N.B. we are not using coupled mode here,
    //      the second do loop would exhaust the writers.
    const boost::dynamic_bitset<> & read_next() {
        
        // get at least one update
        do {
            for(unsigned long i = 0; i != readers.size(); ++i)
                changed[i] = readers[i].read(datas[i]);
        } while (changed.none());

        // make sure we didn't skip one
        do {
            for(unsigned long i = 0; i != readers.size(); ++i)
                changed[i] |= changed_again[i] = readers[i].read(datas[i]);
        } while (changed_again.any());

        // N.B. this can be misleading if any of the writers 
        //      cheat by writing data twice
        return changed;
    }

    const boost::dynamic_bitset<> & read() {

        for(unsigned long i = 0; i != readers.size(); ++i)
            changed[i] = readers[i].read(datas[i]);
        return changed;
    }

    friend std::ostream & operator<< (std::ostream & o, const MultiReader<Reader, Data> & self) {
        if(self.datas.size())
            o << self.datas[0];
        for(unsigned long i = 1; i != self.datas.size(); ++i)
            o << ',' << self.datas[i];
        return o;
    }

    std::vector<Data> datas;

protected:
    std::vector<Reader> readers;
    boost::dynamic_bitset<> changed;
    boost::dynamic_bitset<> changed_again;

    inline void setup() {
        for(auto it = readers.cbegin(); it != readers.cend(); ++it)
            datas.emplace_back(Data(it->depth));
        changed.resize(datas.size(), false);
        changed_again.resize(datas.size());
    }

};

} // namespace ush

#endif // MULTI_R_H
