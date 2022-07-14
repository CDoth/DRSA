#ifndef DYNAMICMONG_H
#define DYNAMICMONG_H

#include "DynamicUInt.h"
namespace DRSA::Dynamic {

    template <class WordType> void reduce(const UInt<WordType> &a, const UInt<WordType> &b, const UInt<WordType> &m, UInt<WordType> &result) {

        result.clear();
        int n = m.bits();

//        std::cout << " >>> DYNAMIC REDUCE (0) "
//                  << " A: " << a.toUInt64()
//                  << " B: " << b.toUInt64()
//                  << " m: " << m.toUInt64()
//                  << " P: " << result.toUInt64()
//                  << std::endl
//                     ;

        UInt<WordType> A = a;
        FOR_VALUE( n, i ) {
            if( !A.isEven() ) {
                result.addToMe( b );
            }
            if( !result.isEven() ) {
                result.addToMe( m );
            }
            A.halfMe();
            result.halfMe();
        }
//        std::cout << " >>> DYNAMIC REDUCE (1) "
//                  << " A: " << a.toUInt64()
//                  << " B: " << b.toUInt64()
//                  << " m: " << m.toUInt64()
//                  << " P: " << result.toUInt64()
//                  << std::endl
//                     ;

        if( result >= m ) {
            result.substractFromMe( m );
        }
    }
    template <class WordType> void reduce(const UInt<WordType> &single, const UInt<WordType> &m, UInt<WordType> &result) {

        result = single;
        int n = m.bits();
        FOR_VALUE( n, i ) {
            if( !result.isEven() ) {
                result.addToMe( m );
            }
            result.halfMe();
        }


        bool comp = bool(result >= m );
//        std::cout << " >>> DYNAMIC REDUCE"
//                  << " A: " << result.toUInt64()
//                  << " m: " << m.toUInt64()
//                  << " comp: " << comp
//                  << std::endl
//                     ;
        if( result >= m ) {

//            std::cout << " >>> DYNAMIC substract m (0)"
//                      << " A: " << result.toUInt64()
//                      << " m: " << m.toUInt64()
//                      << std::endl
//                         ;

            result.substractFromMe( m );

//            std::cout << " >>> DYNAMIC substract m (1)"
//                      << " A: " << result.toUInt64()
//                      << " m: " << m.toUInt64()
//                      << std::endl
//                         ;
        }


    }
    template <class WordType> void powmod(const UInt<WordType> &a, const UInt<WordType> &e, const UInt<WordType> &m, UInt<WordType> &result) {

        int Mn = m.bits();
        int En = e.bits();
        UInt<WordType> A, X, R;
        R.setExponent( m.bits() );

        A = a;
        A.mulMe( R );
        A.modMe( m );

        X = R;
        X.modMe( m );

        UInt<WordType> __X;

//        std::cout << "DYNAMIC powmod1: "
//                  << "a: " << a.toUInt64()
//                  << " e: " << e.toUInt64()
//                  << " m: " << m.toUInt64()
//                  << " r: " << R.toUInt64()
//                  << " A: " << A.toUInt64()
//                  << " X: " << X.toUInt64()
//                  << " Mn: " << Mn
//                  << " En: " << En
//                  << std::endl
//                     ;

        FOR_VALUE( En, i ) {

//            std::cout << "DYNAMIC iteration: " << i
//                      << " step: 0"
//                      << " X: " << X.toUInt64()
//                      << " A: " << A.toUInt64()
//                      << " e bit: " << (e.fastbit( i ))
//                      << std::endl;

            reduce( X, X, m, __X );
            X = __X;

//            std::cout << "DYNAMIC iteration: " << i
//                      << " step: 1"
//                      << " X: " << X.toUInt64()
//                      << " A: " << A.toUInt64()
//                      << " e bit: " << (e.fastbit( i ))
//                      << std::endl;

            if( e.fastbit( i ) ) {
                reduce( X, A, m, __X );
                X = __X;
            }
//            std::cout << "DYNAMIC iteration: " << i
//                      << " step: 2"
//                      << " X: " << X.toUInt64()
//                      << " A: " << A.toUInt64()
//                      << " e bit: " << (e.fastbit( i ))
//                      << std::endl;

        }
        reduce( X, m, result );

//        std::cout << "DYNAMIC LAST REDUCE: "
//                  << "X: " << X.toUInt64()
//                  << " P: " << result.toUInt64()
//                  << std::endl;
    }

}




#endif // DYNAMICMONG_H
