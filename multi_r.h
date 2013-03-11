#ifndef MULTI_R_H
#define MULTI_R_H

#include "boost/dynamic_bitset.hpp"
#include <vector>

namespace ush {

template <typename Reader, typename Data>
class MultiReader {
public:
    MultiReader(std::vector<Reader> readers) : readers(readers) {
        for(auto it = readers.begin(); it != readers.end(); ++it) {
            datas.push_back(Data(*it));
        }
        changed.resize(datas.size(), false);
        changed_again.resize(datas.size());
    }

    const boost::dynamic_bitset<> & read_next() {

        // get at least one update
        do {
            for(unsigned long i = 0; i != readers.size(); ++i)
                changed[i] = readers[i].read(datas[i]);
        } while (!changed.any());

        // make sure we didn't skip one
        do {
            for(unsigned long i = 0; i != readers.size(); ++i)
                changed[i] |= changed_again[i] = readers[i].read(datas[i]);
        } while (changed_again.any());

        // N.B. this can be misleading if any of the writers 
        //      cheat by writing data twice
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

};

} // namespace ush

#endif // MULTI_R_H
