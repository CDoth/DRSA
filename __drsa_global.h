#ifndef __DRSA_GLOBAL_H
#define __DRSA_GLOBAL_H
#include <stdint.h>
#include <DArray.h>

namespace DRSA {
    namespace Mong {}
    namespace Math {}
    namespace Dynamic {}
    namespace Algorithm {}
    namespace Demo {}

}

namespace DRSA {
    template <class WordType> int bits( WordType v ) {
        int b = 0;
        while( v>0 ) { ++b; v >>= 1; }
        return b;
    }
}
namespace DRSA::Mong {

    uint64_t reduce1(uint64_t a, uint64_t b, uint64_t m);
    uint64_t reduce2(uint64_t A, uint64_t m);
    uint64_t powmod1(uint64_t a, uint64_t e, uint64_t m);
}
namespace DRSA::Math {
    uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t m);
    bool MRtest(uint64_t number, int r);
}
namespace DRSA::Demo {
    bool isOdd( uint64_t v );
    uint64_t randomOdd();
    struct Key {
        uint64_t mod;
        uint64_t exp;
    };
    class DRSADemo {
    public:
        bool generateKeys();
        bool encode( const void *data, int n );
        bool decode( const DArray<uint64_t> &d );

        const DArray<uint8_t> & encoded() const { return encodeBuffer; }
        const DArray<uint8_t> & decoded() const { return decodeBuffer; }
    private:
        uint64_t randomPrime() const;
        int getPublicExp() const;
    private:
        DArray<uint8_t> encodeBuffer;
        DArray<uint8_t> decodeBuffer;
    private:
        uint64_t P, Q;
        uint64_t N;
        uint64_t F;
        uint64_t E;
        uint64_t D;
        Key publicKey;
        Key privateKey;
    };
}


#endif // __DRSA_GLOBAL_H
