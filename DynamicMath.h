#ifndef DYNAMICMATH_H
#define DYNAMICMATH_H
#include "__drsa_global.h"
#include <DArray.h>

namespace DRSA::Dynamic {

    //======================================================================================================== add:
    template <class T>
    void add(DArray<T> &to, const DArray<T> &v) {

        typedef typename std::conditional<sizeof(T) == 1, uint16_t,
        typename std::conditional<sizeof(T) == 2, uint32_t, uint64_t>::type
        >::type
        DoubleWord;

        enum {bitsize = sizeof(T) * 8};

        if( to.empty() ) {
            to.copyArray(v);
            return;
        }
        if( v.empty() ) {
            return;
        }
        if(v.size() > to.size() ) {
            to.appendNs(v.size() - to.size());
        }
        auto b = v.constBegin();
        auto e = v.constEnd();
        auto d = to.begin();

        DoubleWord carry = 0;
        while (b != e ) {
            *d = carry = (DoubleWord)*d + (DoubleWord)*b + (carry >> bitsize);
            ++d; ++b;
        }
        if( carry >> bitsize == 0 ) return;

        auto bc = to.begin() + v.size();
        auto ec = to.end();
        while(bc != ec ) {
            *bc = carry = (DoubleWord)*bc + (carry >> bitsize);
            ++bc;
        }
        if( carry >> bitsize ) {
            to.append(1);
        }
    }
    template <>
    void add(DArray<uint64_t> &to, const DArray<uint64_t> &v) {

        if( to.empty() ) {
            to.copyArray(v);
            return;
        }
        if( v.empty() ) {
            return;
        }
        if(v.size() > to.size() ) {
            to.appendNs(v.size() - to.size());
        }
        auto b = v.constBegin();
        auto e = v.constEnd();
        auto d = to.begin();

        uint64_t save = 0;
        uint8_t carry = 0;

        while (b != e ) {
            save = *d;
            *d = *d + *b + carry;
            carry = save > *d ? 1 : 0;

            ++d; ++b;
        }
        if( carry == 0 ) return;

        auto bc = to.begin() + v.size();
        auto ec = to.end();

        while(bc != ec ) {
            save = *bc;
            *bc = *bc + carry;
            carry = save > *d ? 1 : 0;
            ++bc;
        }
        if( carry ) {
            to.append(1);
        }
    }
    //======================================================================================================== substract:
    template <class T>
    void substract(DArray<T> &from, const DArray<T> &v) {

        if( from.empty() || v.empty() ) {
            return;
        }
        typedef typename std::conditional<sizeof(T) == 1, uint16_t,
        typename std::conditional<sizeof(T) == 2, uint32_t, uint64_t>::type
        >::type
        DoubleWord;

        enum {bitsize = sizeof(T) * 8,
              double_bitsize = sizeof(DoubleWord) * 8,
              double_bitsize_1 = double_bitsize - 1};

        auto b = v.constBegin();
        auto e = v.constEnd();
        auto d = from.begin();

        DoubleWord carry = 0;
        while (b != e ) {
            *d = carry = (DoubleWord)*d - (DoubleWord)*b - (carry >> (double_bitsize_1));
            ++d; ++b;
        }

        if( carry >> bitsize == 0 ) {
            return;
        }

        auto bc = from.begin() + v.size();
        auto ec = from.end();
        while(bc != ec ) {
            *bc = carry = (DoubleWord)*bc - (carry >> (double_bitsize_1));
            ++bc;
        }
    }
    template <>
    void substract(DArray<uint64_t> &from, const DArray<uint64_t> &v) {

        auto b = v.constBegin();
        auto e = v.constEnd();
        auto d = from.begin();

        uint64_t save = 0;
        uint8_t carry = 0;
        while (b != e ) {
            save = *d;
            *d = *d - *b - carry;
            carry = save > *d ? 0 : 1;
            ++d; ++b;
        }

        if( carry == 0 ) {
            return;
        }

        auto bc = from.begin() + v.size();
        auto ec = from.end();
        while(bc != ec ) {
            save = *bc;
            *bc = *bc - carry;
            carry = save > *bc ? 0 : 1;
            ++bc;
        }
    }
    //======================================================================================================== increase:
    template <class T>
    void increase(DArray<T> &value) {

        if( value.empty() ) {
            value.append(1);
            return;
        }

        typedef typename std::conditional<sizeof(T) == 1, uint16_t,
        typename std::conditional<sizeof(T) == 2, uint32_t, uint64_t>::type
        >::type
        DoubleWord;

        enum {bitsize = sizeof(T) * 8};

        DoubleWord carry = DoubleWord(1) << bitsize;
        auto b = value.begin();
        auto e = value.end();
        while( carry >> bitsize) {
            if( b == e ) {
                value.append(1);
                return;
            }
            *b = carry = (DoubleWord)*b + (carry >> bitsize);
            ++b;
        }

    }
    template <>
    void increase(DArray<uint64_t> &value) {


        if( value.empty() ) {
            value.append(1);
            return;
        }

        uint64_t save = 0;
        uint8_t carry = 1;
        auto b = value.begin();
        auto e = value.end();
        while( carry ) {
            if( b == e ) {
                value.append(1);
                return;
            }
            save = *b;
            *b = *b + carry;
            carry = save > *b ? 1 : 0;
            ++b;
        }
    }
    //======================================================================================================== reduce:

}



#endif // DYNAMICMATH_H
