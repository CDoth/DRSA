#include "__drsa_global.h"
#include "DRand.h"

namespace DRSA::Demo {

    uint64_t randomOdd() {
        uint64_t r = 0;
        uint8_t *r8 = reinterpret_cast<uint8_t*>(&r);
        FOR_VALUE( 8, i ) { r8[i] = rand(); }
        return isOdd(r) ? r : r + 1;
    }
    bool isOdd(uint64_t v) {
        return v & 1;
    }
    bool DRSADemo::generateKeys() {
        if( (P = randomPrime()) == 0 ) {
            return false;
        }
        if( (Q = randomPrime()) == 0 ) {
            return false;
        }
        N = P * Q;
        F = (P - 1) * (Q - 1);
        E = getPublicExp();
        D = (2 * F + 1) / E;

        publicKey.exp = E;
        publicKey.mod = N;

        privateKey.exp = D;
        privateKey.mod = N;

        return true;
    }
    bool DRSADemo::encode(const void *data, int n) {

        if( data == nullptr || n < 1 ) {
            return false;
        }
        DArray<uint8_t> buffer;

        int size = 0;

        size = sizeof(n) + n;
        int rem = size % sizeof(uint64_t);
        if( rem ) {
            size += (sizeof(uint64_t) - rem );
        }
        buffer.reformUp( size );
        zero_mem( buffer.changableData(), size );


        int *w = (int*)buffer.changableData();
        (*w++) = n;
        copy_mem( (uint8_t*)w, (uint8_t*)data, n);

        uint64_t *D = (uint64_t*)buffer.changableData();
        int N = buffer.size() / sizeof(uint64_t);

        DArray<uint64_t> code;
        while(N--) {
            uint64_t C = DRSA::Math::pow_mod( *D, publicKey.exp, publicKey.mod );
            code.append( C );
        }

        encodeBuffer.clear();
        encodeBuffer.appendData( (uint8_t*)code.constData(), code.size() * 8 );


        return true;
    }
    bool DRSADemo::decode(const DArray<uint64_t> &d) {
        if( d.empty() ) return false;

        DArray<uint64_t> __dec;
        FOR_VALUE( d.size(), i ) {
            uint64_t e = DRSA::Math::pow_mod( d[i], privateKey.exp, privateKey.mod );
            __dec.append( e );
        }

        int dec_bytes = __dec.size() * 8;

        int *w = (int*)__dec.constData();
        int size = (*w++);
        if( size > (dec_bytes - (int)sizeof(int)) ) return false;

        uint8_t *__src = (uint8_t*)w;
        DArray<uint8_t> source_data;
        FOR_VALUE( size, i ) {
            source_data.append( __src[i] );
        }

        decodeBuffer = source_data;


        return true;
    }
    uint64_t DRSADemo::randomPrime() const {
        uint64_t r = randomOdd();
        int n = 1024 * 1024;
        while( n-- ) {
            if( DRSA::Math::MRtest( r, 3 ) ) return r;
            r += 2;
        }
        return 0;
    }
    int DRSADemo::getPublicExp() const {
        return 65537;
    }





}
namespace DRSA::Mong {
    uint64_t reduce1(uint64_t a, uint64_t b, uint64_t m){
        uint64_t P = 0;
        int n = bits(m);

//        std::cout << " >>> STATIC REDUCE (0) "
//                  << " A: " << a
//                  << " B: " << b
//                  << " m: " << m
//                  << " P: " << P
//                  << std::endl
//                     ;

        FOR_VALUE( n, i ) {
            P += (a&1) * b;
            a >>= 1;
            if( (P&1) == 1 ) {
                P += m;
            }
            P >>= 1;
        }

//        std::cout << " >>> STATIC REDUCE (1) "
//                  << " A: " << a
//                  << " B: " << b
//                  << " m: " << m
//                  << " P: " << P
//                  << std::endl
//                     ;

        if( P >= m ) P -= m;
        return P;
    }
    uint64_t reduce2(uint64_t A, uint64_t m) {
        int i;
        int mn = bits(m);

        for (i = 0; i < mn; i++) {
            if ((A & 1) == 1) {
                A += m;
            }
            A = A >> 1;

        }

//        std::cout << " >>> STATIC REDUCE"
//                  << " A: " << A
//                  << " m: " << m
//                  << std::endl
//                     ;

        if (A >= m) {
            A -= m;
        }


        return A;
    }
    uint64_t powmod1(uint64_t a, uint64_t e, uint64_t m) {

        int Mn = bits(m);
        int En = bits(e);

        uint64_t r = (1ULL << Mn);


        uint64_t A = (a * r) % m;
        uint64_t X = (1 * r) % m;

        uint64_t P = 0;


//        std::cout << "STATIC powmod1: "
//                  << "a: " << a
//                  << " e: " << e
//                  << " m: " << m
//                  << " r: " << r
//                  << " A: " << A
//                  << " X: " << X
//                  << " Mn: " << Mn
//                  << " En: " << En
//                  << std::endl
//                     ;

        FOR_VALUE( En, i ) {

//            std::cout << "STATIC iteration: " << i
//                      << " step: 0"
//                      << " X: " << X
//                      << " A: " << A
//                      << " e bit: " << (e&1)
//                      << std::endl;

//            X = reduce2(X * X, m);
            X = reduce1(X, X, m);

//            std::cout << "STATIC iteration: " << i
//                      << " step: 1"
//                      << " X: " << X
//                      << " A: " << A
//                      << " e bit: " << (e&1)
//                      << std::endl;

            if( (e & 1) ) {
//                X = reduce2(X*A, m);
                X = reduce1(X, A, m);
            }

//            std::cout << "STATIC iteration: " << i
//                      << " step: 2"
//                      << " X: " << X
//                      << " A: " << A
//                      << " e bit: " << (e&1)
//                      << std::endl;

            e >>= 1;
        }

//        P = reduce2(X * 1, m);
        P = reduce1(X, 1, m);

//        std::cout << "STATIC LAST REDUCE: "
//                  << "X: " << X
//                  << " P: " << P
//                  << std::endl;
        return P;
    }



}
namespace DRSA::Math {

    uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t m) {
        uint64_t r = 1;

        uint64_t mul = 0;
        while (e > 0) {
            if (e & 1) {
                mul = r * a;
                r = mul % m;
            }
            mul = a * a;
            a = mul % m;
            e >>= 1;
        }

        return r;
    }

    bool MRtest(uint64_t number, int r) {

        if( number % 2 == 0 || number < 3 ) return false;

        uint64_t t = number - 1;
        int s = 0;
        while(true) {
            t >>= 1;
            ++s;
            if( t % 2 ) break;
        }

    //    qDebug() << "MR prepare: number:" << number << "s:" << s << "t:" << t;

        FOR_VALUE( r, r_i ) {

            uint64_t __rv = rand();
            uint64_t a = (__rv % ( number - 2)) + 2;

            uint64_t __classic = DRSA::Math::pow_mod(a, t, number);
            uint64_t __mong = DRSA::Mong::powmod1(a, t, number);

            std::cout << "mrtest powmod:"
                      << " a: " << a
                      << " t: " << t
                      << " number: " << number
                      << " classic: " << __classic
                      << " mong: " << __mong
                      << std::endl;

//            uint64_t x = DRSA::Math::pow_mod(a, t, number);
            uint64_t x = DRSA::Mong::powmod1(a, t, number);
            bool f = true;



            bool check1 = x == 1;
            bool check2 = x == number - 1;

    //        qDebug() << "R loop: a" << a << "x:" << x
    //                 << "check1:" << check1 << "check2:" << check2;

            if( check1 || check2 ) {
                continue;
            }
            FOR_VALUE( s, i ) {


                x = DRSA::Math::pow_mod(x, 2, number );
//                x = DRSA::Mong::powmod1(x, 2, number );

                check1 = bool(x == 1);
                check2 = bool(x == (number - 1));

    //            qDebug() << "S loop:" << "x:" << x << "number:" << number
    //                     << "check1:" << check1 << "check2:" << check2 << "Nd:" << number-1;


                if( check1 ) {
                    return false;
                }
                if( check2 ) {
                    f = false;
                    break;
                }
            }
            if(f) return false;
        }
        return true;
    }

}

