#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "DynamicPrime.h"

namespace DRSA::Algorithm {

    class DRSAContext {
    public:
    private:
        void generatePrimes();
        void encrypt( const std::string &data );
    private:
        DRSA::Dynamic::UInt<uint32_t> P, Q;

        DRSA::Dynamic::UInt<uint32_t> N;
        DRSA::Dynamic::UInt<uint32_t> F;
        DRSA::Dynamic::UInt<uint32_t> E;
        DRSA::Dynamic::UInt<uint32_t> D;
    };
}


#endif // ALGORITHM_H
