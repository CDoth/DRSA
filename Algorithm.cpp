#include "Algorithm.h"


namespace DRSA::Algorithm {

    void DRSAContext::generatePrimes() {
        //----------------------
        P.setRand( DRSA::Dynamic::__get_rand, 4 );
        DRSA::Dynamic::MRTestD( P, 10 );
        Q.setRand( DRSA::Dynamic::__get_rand, 4 );
        DRSA::Dynamic::MRTestD( Q, 10 );
        //----------------------
        N = P;
        N.mulMe( Q );
        //----------------------
        auto __P = P;
        __P.decreaseByOne();
        auto __Q = Q;
        __Q.decreaseByOne();
        F = __P;
        F.mulMe( __Q );
        //----------------------
        E.setExponent( 1 );
        //----------------------
        D = F;
        D.increaseByOne();
        D.divideMe( E );
        //----------------------
    }
    void DRSAContext::encrypt(const std::string &data) {
        DRSA::Dynamic::UInt<uint32_t> Message;
        DRSA::Dynamic::UInt<uint32_t> Code;
        DRSA::Dynamic::powmod( Message, E, N, Code );
    }
}

