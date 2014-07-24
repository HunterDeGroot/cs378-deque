// ----------------------
// projects/deque/Deque.h
// Copyright (C) 2014
// Glenn P. Downing
// ----------------------

#ifndef Deque_h
#define Deque_h

// --------
// includes
// --------

#include <algorithm> // copy, equal, lexicographical_compare, max, swap
#include <cassert>   // assert
#include <iterator>  // iterator, bidirectional_iterator_tag
#include <memory>    // allocator
#include <stdexcept> // out_of_range
#include <utility>   // !=, <=, >, >=
#include <iostream>

// -----
// using
// -----

using namespace std;
using std::rel_ops::operator!=;
using std::rel_ops::operator<=;
using std::rel_ops::operator>;
using std::rel_ops::operator>=;

// -------
// destroy
// -------

template <typename A, typename BI>
BI destroy (A& a, BI b, BI e) {
    while (b != e) {
        --e;
        a.destroy(&*e);}
    return b;}

// ------------------
// uninitialized_copy
// ------------------

template <typename A, typename II, typename BI>
BI uninitialized_copy (A& a, II b, II e, BI x) {
    BI p = x;
    try {
        int i = 0;
        while (b != e) {
            ++i;
            a.construct(&*x, *b);
            ++b;
            ++x;}}
    catch (...) {
        destroy(a, p, x);
        throw;}
    return x;}

// ------------------
// uninitialized_fill
// ------------------

template <typename A, typename BI, typename U>
BI uninitialized_fill (A& a, BI b, BI e, const U& v) {
    int i = 0;
    BI p = b;
    try {
        while (b != e) {
            //cout << i << " " << v << endl;
            ++i;
            a.construct(&*b, v);
            ++b;}}
    catch (...) {
        destroy(a, p, b);
        throw;}
    return e;}

// -------
// my_deque
// -------

template < typename T, typename A = std::allocator<T> >
class my_deque {
    public:
        // --------
        // typedefs
        // --------

        typedef A                                        allocator_type;
        typedef typename allocator_type::value_type      value_type;

        typedef typename allocator_type::size_type       size_type;
        typedef typename allocator_type::difference_type difference_type;

        typedef typename allocator_type::pointer         pointer;
        typedef typename allocator_type::const_pointer   const_pointer;

        typedef typename allocator_type::reference       reference;
        typedef typename allocator_type::const_reference const_reference;

    public:
        // -----------
        // operator ==
        // -----------

        /**
         * <your documentation>
         */
        friend bool operator == (const my_deque& lhs, const my_deque& rhs) {
            if(lhs._size == rhs._size)
		return equal(lhs.begin(),lhs.end(),rhs.begin());
            return false;}

        // ----------
        // operator <
        // ----------

        /**
         * <your documentation>
         */
        friend bool operator < (const my_deque& lhs, const my_deque& rhs) {
            return lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());}

    private:
        // ----
        // data
        // ----

        allocator_type _a;
        std::allocator<T*> _pa;

        size_t _size;

        // start data
        pointer _b;

        // end data
        pointer _e;

        // pointers to the begin/end (data) blocks
        T** _bi;
        T** _ei;

        // pointers to the begin/end (capacity) blocks
        T** _cbi;
        T** _cei;

        T** _cont;

    private:
        // -----
        // valid
        // -----

        bool valid () const { 
            return (!_b && !_e && !_cei) || (_bi <= _ei);}

    public:
        // --------
        // iterator
        // --------

        class iterator {
            public:
                // --------
                // typedefs
                // --------

                typedef std::bidirectional_iterator_tag   iterator_category;
                typedef typename my_deque::value_type      value_type;
                typedef typename my_deque::difference_type difference_type;
                typedef typename my_deque::pointer         pointer;
                typedef typename my_deque::reference       reference;

            public:
                // -----------
                // operator ==
                // -----------

                /**
                 * <your documentation>
                 */
                friend bool operator == (const iterator& lhs, const iterator& rhs) {
                    return (lhs._p == rhs._p) && (lhs._index == rhs._index);
                    return true;}

                /**
                 * <your documentation>
                 */
                friend bool operator != (const iterator& lhs, const iterator& rhs) {
                    return !(lhs == rhs);}

                // ----------
                // operator +
                // ----------

                /**
                 * <your documentation>
                 */
                friend iterator operator + (iterator lhs, difference_type rhs) {
                    return lhs += rhs;}

                // ----------
                // operator -
                // ----------

                /**
                 * <your documentation>
                 */
                friend iterator operator - (iterator lhs, difference_type rhs) {
                    return lhs -= rhs;}

            private:
                // ----
                // data
                // ----

                my_deque* _p;
                size_t _index;

            private:
                // -----
                // valid
                // -----

                bool valid () const {
                    return (_index >= 0 && _index <= _p->size());}

            public:
                // -----------
                // constructor
                // -----------

                /**
                 * <your documentation>
                 */
                iterator (my_deque* p, size_t index) :

                    _p(p),
                    _index(index)

                    {
                    assert(valid());}

                // Default copy, destructor, and copy assignment.
                // iterator (const iterator&);
                // ~iterator ();
                // iterator& operator = (const iterator&);

                // ----------
                // operator *
                // ----------

                /**
                 * <your documentation>
                 */
                reference operator * () const {
                    return (*_p)[_index];}

                // -----------
                // operator ->
                // -----------

                /**
                 * <your documentation>
                 */
                pointer operator -> () const {
                    return &**this;}

                // -----------
                // operator ++
                // -----------

                /**
                 * <your documentation>
                 */
                iterator& operator ++ () {
                    ++_index;
                    return *this;}

                /**
                 * <your documentation>
                 */
                iterator operator ++ (int) {
                    iterator x = *this;
                    ++(*this);
                    assert(valid());
                    return x;}

                // -----------
                // operator --
                // -----------

                /**
                 * <your documentation>
                 */
                iterator& operator -- () {
                    --_index;
                    return *this;}

                /**
                 * <your documentation>
                 */
                iterator operator -- (int) {
                    iterator x = *this;
                    --(*this);
                    assert(valid());
                    return x;}

                // -----------
                // operator +=
                // -----------

                /**
                 * <your documentation>
                 */
                iterator& operator += (difference_type d) {
                    _index += d;
                    assert(valid());
                    return *this;}

                // -----------
                // operator -=
                // -----------

                /**
                 * <your documentation>
                 */
                iterator& operator -= (difference_type d) {
                    _index -= d;
                    assert(valid());
                    return *this;}};

    public:
        // --------------
        // const_iterator
        // --------------

        class const_iterator {
            public:
                // --------
                // typedefs
                // --------

                typedef std::bidirectional_iterator_tag   iterator_category;
                typedef typename my_deque::value_type      value_type;
                typedef typename my_deque::difference_type difference_type;
                typedef typename my_deque::const_pointer   pointer;
                typedef typename my_deque::const_reference reference;

            public:
                // -----------
                // operator ==
                // -----------

                /**
                 * <your documentation>
                 */
                friend bool operator == (const const_iterator& lhs, const const_iterator& rhs) {
                    return (lhs._p == rhs._p) && (lhs._index == rhs._index);}


                /**
                 * <your documentation>
                 */
                friend bool operator != (const const_iterator& lhs, const const_iterator& rhs) {
                    return !(lhs == rhs);}

                // ----------
                // operator +
                // ----------

                /**
                 * <your documentation>
                 */
                friend const_iterator operator + (const_iterator lhs, difference_type rhs) {
                    return lhs += rhs;}

                // ----------
                // operator -
                // ----------

                /**
                 * <your documentation>
                 */
                friend const_iterator operator - (const_iterator lhs, difference_type rhs) {
                    return lhs -= rhs;}

            private:
                // ----
                // data
                // ----

                size_type _index;
                const my_deque* _p;

            private:
                // -----
                // valid
                // -----

                bool valid () const {
                    return (_index >= 0 && _index <= _p->size());}

            public:
                // -----------
                // constructor
                // -----------

                /**
                 * <your documentation>
                 */
                const_iterator (const my_deque* p, size_t index) :

                    _index(index),
                    _p(p)

                    {
                    assert(valid());}

                // Default copy, destructor, and copy assignment.
                // const_iterator (const const_iterator&);
                // ~const_iterator ();
                // const_iterator& operator = (const const_iterator&);

                // ----------
                // operator *
                // ----------

                /**
                 * <your documentation>
                 */
                reference operator * () const {
                    return (*_p)[_index];}

                // -----------
                // operator ->
                // -----------

                /**
                 * <your documentation>
                 */
                pointer operator -> () const {
                    return &**this;}

                // -----------
                // operator ++
                // -----------

                /**
                 * <your documentation>
                 */
                const_iterator& operator ++ () {
                    ++_index;
                    assert(valid());
                    return *this;}

                /**
                 * <your documentation>
                 */
                const_iterator operator ++ (int) {
                    const_iterator x = *this;
                    ++(*this);
                    assert(valid());
                    return x;}

                // -----------
                // operator --
                // -----------

                /**
                 * <your documentation>
                 */
                const_iterator& operator -- () {
                    --_index;
                    assert(valid());
                    return *this;}

                /**
                 * <your documentation>
                 */
                const_iterator operator -- (int) {
                    const_iterator x = *this;
                    --(*this);
                    assert(valid());
                    return x;}

                // -----------
                // operator +=
                // -----------

                /**
                 * <your documentation>
                 */
                const_iterator& operator += (difference_type d) {
                    _index += d;
                    assert(valid());
                    return *this;}

                // -----------
                // operator -=
                // -----------

                /**
                 * <your documentation>
                 */
                const_iterator& operator -= (difference_type d) {
                    _index -= d;
                    assert(valid());
                    return *this;}};

    public:
        // ------------
        // constructors
        // ------------

        /**
         * <your documentation>
         */
        explicit my_deque (const allocator_type& a = allocator_type()) :
                _a (a) {
            _pa = std::allocator<T*>();

            _b = _e = 0;
            _bi = _cbi = _cei = _ei = 0;
            _size = 0;
            assert(valid());}

        /**
         * <your documentation>
         */
        explicit my_deque (size_type s, const_reference v = value_type(), const allocator_type& a = allocator_type()) :
                _a (a) {
           _pa = std::allocator<T*>();
            size_type numBlocks = (s-1)/10 + 1;

            _cont = _pa.allocate(numBlocks);
            size_type i;
            for(i = 0; i < numBlocks; ++i){
                _cont[i] = _a.allocate(10);
            }

            _bi = _cbi = _cont;
            _b = _cont[0];
            _e = _cont[numBlocks-1] + (s-1)%10;
            _cei = _ei = &_cont[numBlocks-1];
            _size = s;
            uninitialized_fill(_a, begin(), end(), v);
            assert(valid());}

        /**
         * <your documentation>
         */
        my_deque (const my_deque& that) :
                _a (that._a) {
            _pa = std::allocator<T*>();
            size_type numBlocks = (that.size()-1)/10 +1;
            _cont = _pa.allocate(numBlocks);

            size_type i;
            for(i = 0; i < numBlocks; ++i)
                _cont[i] = _a.allocate(10);

            _bi = _cbi = _cont;
            _b = _cont[0];
            _e = _cont[numBlocks-1] + (that.size()-1)%10;
            _cei = _ei = &_cont[numBlocks-1];
            _size = that.size();
           
            uninitialized_copy(_a, that.begin(), that.end(), begin());
            assert(valid());}

        // ----------
        // destructor
        // ----------

        /**
         * <your documentation>
         */
        ~my_deque () {
            if (_b) {
                clear();
                iterator b = begin();
                iterator e = end();

                destroy (_a, b, e);

                typename std::allocator<T*>::pointer index = _cbi;
                size_type st = 10;
                while(index <= _cei){
                    _a.deallocate(*index,st);
                    ++index;
                }
                st = _cei - _cbi;
                _pa.deallocate(_cont, st);}
            assert(valid());}


        // ----------
        // operator =
        // ----------

        /**
         * <your documentation>
         */
        my_deque& operator = (const my_deque& rhs) {
            *this = my_deque(rhs);
            assert(valid());
            return *this;}

        // -----------
        // operator []
        // -----------

        /**
         * <your documentation>
         */
        reference operator [] (size_type index) {
         


            if(_b + index < *_bi + 10)
                return *(_b + index);
            else{
                int offs = 10 - (_b - *_bi);
                index -= offs;
                return (*(_bi + index/10 + 1))[index%10];}}

        /**
         * <your documentation>
         */
        const_reference operator [] (size_type index) const {
            return const_cast<my_deque*>(this)->operator[](index);}

        // --
        // at
        // --

        /**
         * <your documentation>
         */
        reference at (size_type index) {
            if (index >= size())
                throw std::out_of_range("deque");
            return (*this)[index];}

        /**
         * <your documentation>
         */
        const_reference at (size_type index) const {
            return const_cast<my_deque*>(this)->at(index);}


        // ----
        // back
        // ----

        /**
         * <your documentation>
         */
        reference back () {
            return const_cast<my_deque*>(this)->back();}

        /**
         * <your documentation>
         */
        const_reference back () const {
            return const_cast<my_deque*>(this)->back();}

        // -----
        // begin
        // -----

        /**
         * <your documentation>
         */
        iterator begin () {
            return iterator(this,0);}

        /**
         * <your documentation>
         */
        const_iterator begin () const {
            return const_iterator(const_cast<my_deque*>(this),0);}

        // -----
        // clear
        // -----

        /**
         * <your documentation>
         */
        void clear () {
	    resize(0);            
            assert(valid());}

        // -----
        // empty
        // -----

        /**
         * <your documentation>
         */
        bool empty () const {
            return !size();}

        // ---
        // end
        // ---

        /**
         * <your documentation>
         */
        iterator end () {
            return iterator(this,_size);}

        /**
         * <your documentation>
         */
        const_iterator end () const {
            return const_iterator(this,_size);}

        // -----
        // erase
        // -----

        /**
         * <your documentation>
         */
        iterator erase (iterator it) {
            while(it != end() - 1)
		*it = *(it+1);
	    *it = T();
	    --_size;
            assert(valid());
            return iterator();}

        // -----
        // front
        // -----

        /**
         * <your documentation>
         */
        reference front () {
            return *this[0];}

        /**
         * <your documentation>
         */
        const_reference front () const {
            return const_cast<my_deque*>(this)->front();}

        // ------
        // insert
        // ------

        /**
         * <your documentation>
         */
        iterator insert (iterator i, const_reference v) {
	    resize(size+1);
	    iterator it = end() - 1;
	    while(it != i)
		*it = *(it-1);
	    *it = v;
            assert(valid());
            return iterator();}

        // ---
        // pop
        // ---

        /**
         * <your documentation>
         */
        void pop_back () {
            assert(!empty());
	    resize(size() - 1);
            assert(valid());}

        /**
         * <your documentation>
         */
        void pop_front () {
            assert(!empty());
            erase(begin());
            assert(valid());}

        // ----
        // push
        // ----

        /**
         * <your documentation>
         */
        void push_back (const_reference v) {
            resize(size() + 1, v);
            assert(valid());}

        /**
         * <your documentation>
         */
        void push_front (const_reference v) {
            insert(begin(), v);
            assert(valid());}


         void print_deque(){

            size_type s = _cei - _cbi;
            size_type i;
            size_type offs = 0;
            size_type index = 0;
            for(i = 0; i <= s; ++i){

                size_type x;
                for(x = 0; x < 10; ++x){
                    offs = x;
                    if(&_cont[i][x] == _b) break;
                    cout << 0;
                }
                index = i;
                if(&_cont[i][x] == _b) break;
                cout << endl;
            }

            iterator b = begin();
            iterator e = end();

            while(b!=e){
                cout << *b;
                ++offs;
                if(offs % 10 == 0){
                    cout << endl;
                    ++index; 
                }
                ++b;
            }
            offs %= 10;
            for(i = index; i <= s; ++i){

                size_type x;
                for(x = offs; x < 10; ++x){
                    cout << 0;
                }
                offs = 0;
                cout << endl;}}

        // ------
        // resize
        // ------

        /**
         * <your documentation>
         */
        void resize (size_type s, const_reference v = value_type()) {
            size_type cap = _cei - _ei;
            size_type newEndCap = _cei - _ei + 1;
            size_type from_EIto_BI = _ei - _bi;
            T** i;
            if(s == _size)
                return;
            if(s < _size) {
                _e = &*destroy(_a, begin() + s, end());
                for(i = _cont + 0; i < _cei; ++i)
                    if((_e - *i) >= 0 && (_e - *i) <= 9) break;
                _ei = i;
                _size = s;
            } else if(s - _size <= (newEndCap * 10)+from_EIto_BI){
                size_type endTemp = _size;
                // cout << "endTemp: " << endTemp << endl;
                _size = s;
                // cout << "start fill" << endl;
                // cout << "Containers given to fill" << endl;
                // for(i = _cbi; i <= _cei; ++i){
                //     cout << *i << endl;
                // }
                // cout << "begin index is : " << _bi - _cont << endl;
                _e = &*uninitialized_fill(_a, iterator(this,endTemp), begin() + s, v);
                // cout << "_e is : " << _e << endl;

                for(i = _bi; i <= _cei; ++i){
                    // cout << "in for loop" << endl;
                    // cout << *i << endl;
                    if((_e - *i) >= 0 && (_e - *i) <= 9){
                        // cout << "broke out of loop" << endl;
                        break;
                    }       
                }
                 _ei = i;
                // cout << "end index should be " << _ei - _cont  << endl;
            } else{
                // cout << "Original given to new" << endl;
                // for(i = _cbi; i <= _cei; ++i){
                //     cout << *i << endl;
                // }
                // cout << "|" << cap << "|\n";
                T** newCont;
                _pa = std::allocator<T*>();
                size_type wholeCap = _cei - _cbi + 1;
                // cout <<"\n\n"<< wholeCap << "\n\n";
                size_type numBlocks = std::max(wholeCap*3,(s/10)*3);
                size_type halfBlock = numBlocks/3;
                newCont = _pa.allocate(numBlocks);
                size_type ind;
                size_type counter = 0;
                // cout << "New Container Addresses" << endl;
                for(ind = 0; ind < numBlocks; ++ind){
                    if((ind >= halfBlock) && (ind < halfBlock + wholeCap)){
                        newCont[ind] = _cont[counter];
                        ++counter;
                    }
                    else
                        newCont[ind] = _a.allocate(10);
                    // cout << newCont[ind] << endl;
                }
                _bi = newCont + halfBlock;
                _ei = newCont + halfBlock + from_EIto_BI;
                _pa.deallocate(_cont,wholeCap);
                _cont = newCont;   
                _cbi = _cont;
                _cei = &_cont[numBlocks-1];
                //print_deque();
                resize(s,v);
            }
            assert(valid());}

        // ----
        // size
        // ----

        /**
         * <your documentation>
         */
        size_type size () const {
            return _size;}

        // ----
        // swap
        // ----

        /**
         * <your documentation>
         */
        void swap (my_deque&) {
            assert(valid());}};

#endif // Deque_h
