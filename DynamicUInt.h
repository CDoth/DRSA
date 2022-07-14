#ifndef DYNAMICUINT_H
#define DYNAMICUINT_H
#include "DynamicMath.h"
#include <algorithm>

namespace DRSA::Dynamic {

    template <class WordType> class UInt;
    template <class WordType> class UIntPool {
    public:
    typedef UInt<WordType> * u_t;

        inline UInt<WordType> * get() {
            return aPool.empty() ? new UInt<WordType>() : take() ;
        }
        inline void set( UInt<WordType> *v ) {
            aPool.append( v );
        }
    private:
        inline u_t take() {
            u_t u = aPool.back();
            aPool.pop_back();
            return u;
        }
    private:
        DArray<UInt<WordType>*> aPool;
    };
    template <class WordType>
    class UInt {
        typedef typename std::conditional<sizeof(WordType) == 1, uint16_t,
        typename std::conditional<sizeof(WordType) == 2, uint32_t, uint64_t>::type
        >::type
        DoubleWord;

        enum {
            bitsize = sizeof(WordType) * 8,
            halfbitsize = bitsize / 2,
            double_bitsize = sizeof(DoubleWord) * 8,
            double_bitsize_1 = double_bitsize - 1
        };
    public:
        UInt() {}
    public:
        inline bool empty() const {return aWords.empty();}
        inline int bits() const {
            if( aWords.empty() ) return 0;
            int p = DRSA::bits( aWords.constBack() );
            return ((aWords.size() - 1) * bitsize) + p;
        }
        inline bool isLessThanExp(int expPower) const {return bits() < expPower + 1;}
        inline bool isOne() const {return aWords.size() == 1 && aWords[0] == 1;}
        inline bool isEven() const {
            if( aWords.empty() ) return true;
            return uint8_t(aWords.constFront() & 1) != uint8_t(0b00000001);
        }
        int bit(int pos) const {

            if( aWords.empty() ) return 0;
            int __byte = 0;
            int __bitInByte = 0;
            __byte = pos / bitsize;
            __bitInByte = pos % bitsize;


            if( aWords.inRange(__byte) ) {
                return (aWords[__byte] >> __bitInByte) & 0b00000001;
            }
            return -1;
        }
        inline int fastbit(int pos) const { return (aWords[pos / bitsize] >> (pos % bitsize)) & 1;}
        inline int leftExponent() const { return bits() - 1; }
        inline int rightExponent() const { return bits(); }
        inline int size() const {return aWords.size();}
    public:
        bool operator< (const UInt &v) const {


//            std::cout << "operator<"
//                      << " size: " << aWords.size()
//                      << " v size: " << v.aWords.size()
//                      << std::endl;

            if( aWords.size() < v.aWords.size() ) return true;
            if( aWords.size() > v.aWords.size() ) return false;

            auto b = aWords.constBegin();
            auto e = aWords.constEnd();
            auto e2 = v.aWords.constEnd();

            while( b != e ) {
                --e; --e2;
                if( *e < *e2 ) {
                    return true;
                }
            }
            return false;
        }
        bool operator>(const UInt &o) const {

            int b = bits();
            int ob = o.bits();

            if( b > ob ) return true;
            else if ( b < ob ) return false;

            for( int i = aWords.size()-1; i > -1; --i ) {

                if( aWords[i] == o.aWords[i] ) continue;
                if( aWords[i] > o.aWords[i] ) return true;
                return false;
            }
            return false;
        }
        bool operator==(const UInt &o) const {

            if( aWords.size() != o.size() ) return false;
            FOR_VALUE( aWords.size(), i ) {
                if( aWords[i] != o.aWords[i] ) return false;
            }
            return true;
        }
        inline bool operator!=( const UInt &o ) const { return !operator==(o); }
        inline bool operator>=(const UInt &o ) const {
            int b = bits();
            int ob = o.bits();

    //        qDebug() << "DYNAMIC operator >= b:" << b << "ob:" << ob << "value:" << toInt() << "other:" << o.toInt();

            if( b > ob ) return true;
            else if ( b < ob ) return false;

            for( int i = aWords.size()-1; i > -1; --i ) {
                if( aWords[i] == o.aWords[i] ) continue;
                if( aWords[i] > o.aWords[i] ) return true;
                return false;
            }
            return true;
        }
        inline bool operator<=( const UInt &o ) const {}
    public:
        void setValue(uint64_t value) {
            int __bits = DRSA::bits(value);
            int __bytes = __bits / 8;
            if( __bits % 8 ) ++__bytes;

            int __words = __bytes / sizeof(WordType);
            if( __bytes % sizeof(WordType) ) ++__words;

            aWords.reform(__words);
            zero_mem(aWords.changableData(), aWords.size());

            uint8_t *d = reinterpret_cast<uint8_t*>(aWords.changableData());
            uint8_t *s = reinterpret_cast<uint8_t*>(&value);
            while(__bytes--) *d++ = *s++;
        }
        uint64_t toUInt64() const {
            if( aWords.empty() ) return 0;

            int start = aWords.size() / sizeof(uint64_t) ;
            int size = aWords.size() % sizeof(uint64_t);
            if( size == 0 ) { --start; size = sizeof(uint64_t);}
            start *= sizeof(uint64_t);
            size *= sizeof(WordType);

            uint64_t r = 0;
            uint8_t *__r = reinterpret_cast<uint8_t*>(&r);
            const uint8_t *__d = reinterpret_cast<const uint8_t*>(aWords.constData()) + start;
            while(size--) {
                *(__r++) = *(__d++);
            }

            return r;
        }
        void fillRand( void (*randCallback)(uint64_t &v) ) {
            std::for_each( aWords.begin(), aWords.end(), randCallback );
        }
        void setRand( void (*randCallback)(uint64_t &v), int n ) {
            clear();
            uint64_t v = 0;
            FOR_VALUE( n, i ) {
                randCallback( v );
                aWords.append( v );
            }
        }
        inline void clear() { aWords.drop(); }
        UInt & operator = (const UInt &o) {
            aWords.copyArray( o.aWords );
            return *this;
        }
        inline void copy(const UInt &o) {
            aWords.copyArray( o.aWords );
        }
    public:
        inline void halfMe() { shiftMeRight( 1 ); }
        inline void increaseByOne() { DRSA::Dynamic::increase( aWords ); }
        inline void decreaseByOne() {}
        inline void addToMe(const UInt &o) { DRSA::Dynamic::add( aWords, o.aWords ); }
        inline void substractFromMe( const UInt &o ) {
            if( empty() || o.empty() || *this < o ) {
                std::cout << "substractFromMe: can't substract"
                          << " e: " << bool(empty())
                          << " oe: " << bool(o.empty())
                          << " comp: " << bool(*this < o)
                          << std::endl;
                return;
            }
            DRSA::Dynamic::substract( aWords, o.aWords );
            compress();
        }
        void mulMe( const UInt &o ) {

            if( empty() || o.empty() ) { aWords.clear(); return; }
            if( o.isOne() ) return;

            UInt *special = iPool.get();

            special->copy( *this );

            aWords.drop();

            int __ob = o.bits();
            int last_e = 0;

            FOR_VALUE( __ob, i ) {

                if( o.fastbit(i) ) {
                    special->shiftMeLeft( i - last_e );
                    last_e = i;
                    addToMe( *special );
                }
            }
            iPool.set( special );
        }
        void divideMe( const UInt &o ) {

            UInt *special1 = iPool.get();
            UInt *special2 = iPool.get();

            special2->clear();

            int __re = o.rightExponent();
            while( true ) {

                special1->copy( *this ); // save current value
                special1->shiftMeRight( __re ); // divide value to right exp
                special2->addToMe( *special1 ); // add to result
                special1->mulMe( o );
                substractFromMe( *special1 );
                bool isLess = isLessThanExp(__re);
                if( isLess ) {
                    bool cmpM = (*this >= o);
                    if( cmpM ) special2->increaseByOne();
                    break;
                }
            }

            copy( *special2 );

//            compress();

            iPool.set( special1 );
            iPool.set( special2 );
        }
        void modMe(const UInt &M) {

            if( M > *this ) return;

            UInt *special = iPool.get();
            special->copy( *this );
            special->divideMe( M );
            special->mulMe( M );
            substractFromMe( *special );

//            compress();

            iPool.set( special );

        }
    public:
        void setExponent(int power ) {
            clear();
            aWords.append( 1 );
            shiftMeLeft( power );
        }
        void shiftMeRight(int bitsToShift) {

            if( bitsToShift == 0 ) return;
            int __bytes = bitsToShift / bitsize;
            int __bits = bitsToShift % bitsize;

            if( bitsToShift >= bits() ) {
                aWords.clear();
                return;
            }
            if( __bytes ) aWords.cutBegin(__bytes);
            if( __bits ) {
    //            typedef uint8_t shift_type;

                auto b = aWords.begin();
                auto e = aWords.end();

                WordType carry1 = 0;
                WordType carry2 = 0;

                while( b != e ) {
                    e--;
                    carry1 = *e << (bitsize - __bits);
                    *e >>= __bits;
                    *e |= carry2;
                    carry2 = carry1;
                }
            }

//            compress();
        }
        void shiftMeLeft(int bitsToShift) {

            if( bitsToShift == 0 ) return;
            int __bytes = bitsToShift / bitsize;
            int __bits = bitsToShift % bitsize;
            if( __bits ) {
                WordType mask2 = WordType(-1) << (bitsize-__bits);
                bool q = aWords.back() & (mask2);

                if( q ) aWords.append(0);

                auto b = aWords.begin();
                auto e = aWords.end();
                WordType carry = 0;
                WordType mask = WordType(-1) << (bitsize-__bits);
                WordType pv = 0;
                while( b != e ) {

                    pv = *b;
                    *b = *b << __bits;
                    *b |= carry;
                    carry = pv & mask;
                    carry = carry >> (bitsize-__bits);
                    ++b;
                }
            }
            if( __bytes ) aWords.shiftPart(0, __bytes);

//            compress();
        }
    private:
        void compress() {
            auto b = aWords.begin();
            auto e = aWords.end();
            int i = aWords.size();
            while( b != e ) {
                e--;
                if( *e ) break;
                --i;
            }
            if( i != aWords.size() ) aWords.reformDown(i);
        }
    private:
        static UIntPool<WordType> iPool;
        DArray<WordType> aWords;
    };

    template <class WordType>
    DRSA::Dynamic::UIntPool<WordType> DRSA::Dynamic::UInt<WordType>::iPool;
}


#endif // DYNAMICUINT_H
