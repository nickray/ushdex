#ifndef BOOK_TYPES_H
#define BOOK_TYPES_H

#ifdef USE_EIGEN
#include <Eigen/Dense>
namespace Eigen {
    typedef Matrix<long, Dynamic, 1> VectorXl;
    typedef Matrix<long, 1, Dynamic> RowVectorXl;
    typedef Matrix<long, Dynamic, Dynamic> MatrixXl;
}
#else
#include <vector>
#endif

namespace ush {

#ifndef USE_EIGEN
    typedef std::vector<double> prices_t;
    typedef std::vector<long> volumes_t;
#else
    #ifndef USE_ROWS
    typedef Eigen::RowVectorXd prices_t;
    typedef Eigen::RowVectorXl volumes_t;
    #else
    typedef Eigen::VectorXd prices_t;
    typedef Eigen::VectorXl volumes_t;
    #endif
#endif

} // namespace ush

#endif // BOOK_TYPES_H
