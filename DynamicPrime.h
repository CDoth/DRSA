#ifndef DYNAMICPRIME_H
#define DYNAMICPRIME_H


#include "DynamicMong.h"

namespace DRSA::Dynamic {


    inline void __get_rand( uint64_t &v ) {
        v = rand();
    }
    template <class WT>
    bool MRTestD(const UInt<WT> &number, int rounds) {

        if( number.isEven() || number.empty() ) return false;

        UInt<WT> N_d;
        UInt<WT> T = number;
        T.decreaseByOne();
        N_d = number;
        N_d.decreaseByOne();
        int s = 0;

        while(true) {
            T.halfMe();
            ++s;
            if( !T.isEven() ) break;
            if( T.empty() ) return false;
        }

        UInt<WT> A, X;
    //    uint8_t MAXB = number.back() - 2;
    //    int backShift = 0;
    //    if( MAXB == 0 ) {
    //        MAXB = 255;
    //        backShift = 1;
    //    }

        FOR_VALUE( rounds, r_i ) {
            bool f = true;
            A.setRand( __get_rand, number.size() );


    //        int __rv = rand() % MAXB;
    //        if( A.size() == 1 && __rv < 3 ) __rv = 3;
    //        A.word(A.size() - 1 - backShift) = __rv;

            X = A;


            DRSA::Dynamic::powmod( X, T, number, X );
            bool check1 = X.isOne();
            bool check2 = X == N_d;


            if( check1 || check2 ) {
                continue;
            }

            FOR_VALUE( s, i ) {

//                X.powmodMe(2, number);

                check1 = X.isOne();
                check2 = X == N_d;

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
//---------------------------------------------------------

}

#endif // DYNAMICPRIME_H
