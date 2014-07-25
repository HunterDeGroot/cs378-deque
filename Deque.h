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
#include <iostream>  // for prints

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
        while (b != e) {
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
    BI p = b;
    try {
        while (b != e) {
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
         * returns true if deques are equal
         */
        friend bool operator == (const my_deque& lhs, const my_deque& rhs) {
            if(lhs._size == rhs._size)
		return equal(lhs.begin(),lhs.end(),rhs.begin());
            return false;}

        // ----------
        // operator <
        // ----------

        /**
         * compares deques lexicographically
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
                 * returns true if iterators point to same deque at same index
                 */
                friend bool operator == (const iterator& lhs, const iterator& rhs) {
                    return (lhs._p == rhs._p) && (lhs._index == rhs._index);
                    return true;}

                /**
                 * returns false if iterators point to same deque at same index
                 */
                friend bool operator != (const iterator& lhs, const iterator& rhs) {
                    return !(lhs == rhs);}

                // ----------
                // operator +
                // ----------

                /**
                 * + index of iterator
                 */
                friend iterator operator + (iterator lhs, difference_type rhs) {
                    return lhs += rhs;}

                // ----------
                // operator -
                // ----------

                /**
                 * - index of iterator
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
                 * constructor given a deque and index
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
                 * returns value in deque at index
                 */
                reference operator * () const {
                    return (*_p)[_index];}

                // -----------
                // operator ->
                // -----------

                /**
                 * derefence shorthand for iterator
                 */
                pointer operator -> () const {
                    return &**this;}

                // -----------
                // operator ++
                // -----------

                /**
                 * ++ index of iterator pre
                 */
                iterator& operator ++ () {
                    ++_index;
                    return *this;}

                /**
                 * ++ index of iterator post
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
                 * -- index of iterator pre
                 */
                iterator& operator -- () {
                    if(_index != 0)
                        --_index;
                    return *this;}

                /**
                 * -- index of iterator post
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
                 * += index of iterator
                 */
                iterator& operator += (difference_type d) {
                    _index += d;
                    assert(valid());
                    return *this;}

                // -----------
                // operator -=
                // -----------

                /**
                 * -= index of iterator
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
                 * returns true if the iterators point to the same index of same deque
                 */
                friend bool operator == (const const_iterator& lhs, const const_iterator& rhs) {
                    return (lhs._p == rhs._p) && (lhs._index == rhs._index);}


                /**
                 * returns false if the iterators point to the same index of same deque
                 */
                friend bool operator != (const const_iterator& lhs, const const_iterator& rhs) {
                    return !(lhs == rhs);}

                // ----------
                // operator +
                // ----------

                /**
                 * + the index of const iterator
                 */
                friend const_iterator operator + (const_iterator lhs, difference_type rhs) {
                    return lhs += rhs;}

                // ----------
                // operator -
                // ----------

                /**
                 * - the index of const iterator
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
                    return true;}//(_index >= 0 && _index <= _p->size());}

            public:
                // -----------
                // constructor
                // -----------

                /**
                 * constructor that sets the iterator according to an index
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
                 * returns the value of the deque that the const iterator is pointing to
                 */
                reference operator * () const {
                    return (*_p)[_index];}

                // -----------
                // operator ->
                // -----------

                /**
                 * derefrence shorthand for const iterator
                 */
                pointer operator -> () const {
                    return &**this;}

                // -----------
                // operator ++
                // -----------

                /**
                 * pre ++ the index of const iterator
                 */
                const_iterator& operator ++ () {
                    ++_index;
                    assert(valid());
                    return *this;}

                /**
                 * post ++ the index of const iterator
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
                 * pre -- the index of const iterator
                 */
                const_iterator& operator -- () {
                    --_index;
                    assert(valid());
                    return *this;}

                /**
                 * post -- the index of const iterator
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
                 * += the index of const iterator
                 */
                const_iterator& operator += (difference_type d) {
                    _index += d;
                    assert(valid());
                    return *this;}

                // -----------
                // operator -=
                // -----------

                /**
                 * -= the index of const iterator
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
         * default constructor
         */
        explicit my_deque (const allocator_type& a = allocator_type()) :
                _a (a) {
            _pa = std::allocator<T*>();
            _cont = _pa.allocate(1);
            _cont[0] = _a.allocate(10);
            _b = _e = _cont[0];
            _bi = _cbi = _cei = _ei = _cont;
            _size = 0;
            assert(valid());}

        /**
         * returns a deque with s elements initialized to value v
         */
        explicit my_deque (size_type s, const_reference v = value_type(), const allocator_type& a = allocator_type()) :
                _a (a) {
            if(s == 0){
                _pa = std::allocator<T*>();
                _cont = _pa.allocate(1);
                _cont[0] = _a.allocate(10);
                _b = _e = _cont[0];
                _bi = _cbi = _cei = _ei = _cont;
                _size = 0;
            }
            else{
               _pa = std::allocator<T*>();
                size_type numBlocks = (s/10)*3;
                if(numBlocks == 0){
                    numBlocks = 3;
                }
                size_type halfBlock = numBlocks / 3;
                _cont = _pa.allocate(numBlocks);
                size_type i;
                for(i = 0; i < numBlocks; ++i)
                    _cont[i] = _a.allocate(10);

                _cbi = _cont;
                _bi = &_cont[halfBlock];
                _b = _cont[halfBlock];
                _cei = &_cont[numBlocks - 1];
                _ei = &_cont[halfBlock + s/10];
                _size = s;
                _e = &*uninitialized_fill(_a, begin(), begin() + s, v) - 1;
            }
            assert(valid());}

        /**
         * (my_deque) constructor
         */
        my_deque (const my_deque& that) :
                _a (that._a) {
            _pa = std::allocator<T*>();
            if(that._size == 0){
                _cont = _pa.allocate(1);
                _cont[0] = _a.allocate(10);
                _b = _e = _cont[0];
                _bi = _cbi = _cei = _ei = _cont;
                _size = 0;
            }
            else{
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
            }
            assert(valid());}

        // ----------
        // destructor
        // ----------

        /**
         * destructor of deque
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
         * sets this deque equal to the right hand deque
         */
        my_deque& operator = (const my_deque& rhs) {
            size_type capacity = _cei-_cbi;
            if (this == &rhs)
                return *this;
            if (rhs.size() == size())
                std::copy(rhs.begin(), rhs.end(), begin());
            else if (rhs.size() < size()) {
                std::copy(rhs.begin(), rhs.end(), begin());
                resize(rhs.size());}
            else if (rhs.size() <= ((capacity+1)*10)) {
                if(_size == 1){
                    std::copy(rhs.begin(), rhs.begin() + 1, begin());
                    _e = &*uninitialized_copy(_a, rhs.begin()+1, rhs.end(), end()) - 1;

                }else{
                    std::copy(rhs.begin(), rhs.begin() + size(), begin());
                     _e = &*uninitialized_copy(_a, rhs.begin() + size(), rhs.end(), end()) - 1;

                }
                _size = rhs.size();
            }
            else {
                clear();
                size_type numBlocks = (rhs.size()-1)/10 +1;
                _cont = _pa.allocate(numBlocks);
                size_type i;
                for(i = 0; i < numBlocks; ++i)
                    _cont[i] = _a.allocate(10);
                _bi = _cbi = _cont;
                _b = _cont[0];
                _e = _cont[numBlocks-1] + (rhs.size()-1)%10;
                _cei = _ei = &_cont[numBlocks-1];
                _size = rhs.size();
                _e = &*uninitialized_copy(_a, rhs.begin(), rhs.end(), begin()) - 1;}
            assert(valid());
            return *this;
        }


        // -----------
        // operator []
        // -----------

        /**
         * returns ref to element at index
         */
        reference operator [] (size_type index) {
         
            if(_b + index < *_bi + 10)
                return *(_b + index);
            else{
                size_type temp = _bi - _cbi;
                int offs = 10 - (_b - *_bi);
                index -= offs;
                return _cont[temp + index/10 + 1][index%10];}}

        /**
         * returns const ref to element at index
         */
        const_reference operator [] (size_type index) const {
            return const_cast<my_deque*>(this)->operator[](index);}

        // --
        // at
        // --

        /**
         * returns element at index and checks if in range
         */
        reference at (size_type index) {
            if (index >= size())
                throw std::out_of_range("deque");
            return (*this)[index];}

        /**
         * returns const checked reference
         */
        const_reference at (size_type index) const {
            return const_cast<my_deque*>(this)->at(index);}


        // ----
        // back
        // ----

        /**
         * <returns an iterator pointing to the end of the deque
         */
        reference back () {
            return (*this)[size()-1];}

        /**
         * returns a const iterator pointing to the end of the deque
         */
        const_reference back () const {
            return const_cast<my_deque*>(this)->back();}

        // -----
        // begin
        // -----

        /**
         * returns an iterator pointing to the beginning of the deque
         */
        iterator begin () {
            return iterator(this,0);}

        /**
         * returns a const iterator pointing to the beginning of the deque
         */
        const_iterator begin () const {
            return const_iterator(const_cast<my_deque*>(this),0);}

        // -----
        // clear
        // -----

        /**
         * removes all elements in the deque
         */
        void clear () {
	    resize(0);            
            assert(valid());}

        // -----
        // empty
        // -----

        /**
         * returns true if the deque is empty
         */
        bool empty () const {
            return !size();}

        // ---
        // end
        // ---

        /**
         * returns the last element
         */
        iterator end () {
            return iterator(this,_size);}

        /**
         * return const last element
         */
        const_iterator end () const {
            return const_iterator(this,_size);}

        // -----
        // erase
        // -----

        /**
         * removes an the element pointed to by the iterator
         */
        iterator erase (iterator it) {
            if(it == begin()){
                destroy(_a,begin(),begin()+1);
                --_size;
                if(*_bi+9 == _b){
                    ++_bi;
                    _b = *_bi;
                }else{
                ++_b;
                }
             }else{
            while(it != end() - 1){
		        *it = *(it+1);
                ++it;}
	        destroy(_a,end()-1,end());
            --_e;
            if(_e < *_ei){
                --_ei;
                _e = *_ei + 9;}
	        --_size;
        }
            assert(valid());
            return it;

        }

        // -----
        // front
        // -----

        /**
         * returns the first element
         */
        reference front () {
            return (*this)[0];}

        /**
         * return const first element
         */
        const_reference front () const {
            return const_cast<my_deque*>(this)->front();}

        // ------
        // insert
        // ------

        /**
         * inserts a value at the ith position in the deque
         */
        iterator insert (iterator i, const_reference v) {

            if(empty()){          
              *_b = v;
                _size = 1;
            } else if(i == begin()){
                if(_b == *_bi){
                    resize_front(size());
                    --i;
                    ++_size;
                    --_bi;
                    _b = *_bi+9;
                    *_b = v;
                }
                else{
                    --i;
                    ++_size;
                    --_b;
                    *_b = v;
                }
            }else if(i == end()){
                resize(size()+1,v);
            }
            else{
        	     iterator it = end() - 1;
                 if(i == end() - 1){
                    it = end();
                    *it = *i;
                    *i = v;
                 }else{
            	     while(it != i){
                		*it = *(it-1);
                        --it;
                    }
                	*it = v;
                    ++_e;
                }
                if(_e == *_ei + 10){
                    ++_ei;
                    _e = *_ei;
                }
                ++_size;
            }
        assert(valid());
        return i;}

        // ---
        // pop
        // ---

        /**
         * removes last element
         */
        void pop_back () {
            assert(!empty());
	    resize(size() - 1);
            assert(valid());}

        /**
         * removes first element
         */
        void pop_front () {
            assert(!empty());
            erase(begin());
            assert(valid());}

        // ----
        // push
        // ----

        /**
         * adds element to back
         */
        void push_back (const_reference v) {
            resize(size() + 1, v);
            assert(valid());}

        /**
         * adds element to front
         */
        void push_front (const_reference v) {
            insert(begin(), v);
            assert(valid());}


        // ----
        // print
        // ----

        /**
         * gives great visual of the current state of the deque
         */

         void print_deque(){
            size_type s = _cei - _cbi;
            size_type i = 0;
            size_type offs = 0;
            size_type index = 0;
            for(i = 0; i < s; ++i){

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
         * sets the number of elements in the deque and adds capacity if needed
         */
        void resize (size_type s, const_reference v = value_type()) {
            size_type wholeCap = _cei - _cbi + 1;
            size_type endCap = _cei - _ei;
            size_type from_EIto_BI = _ei - _bi;
            size_type temp = (10*endCap + (9-(_e - *_ei)));

            T** i;
            if(s == _size)
                return;
            if(s < _size) {
                _e = &*destroy(_a, begin() + s, end());
                for(i = _cont + 0; i < _cei; ++i)
                    if((_e - *i) >= 0 && (_e - *i) <= 9) break;
                _ei = i;
                _size = s;
            }
            else if(_ei != 0 && (s - _size <  temp)){
                size_type endTemp = _size;
                _size = s;
                if(_e == *_ei+8){
                 _e = &*uninitialized_fill(_a, iterator(this,endTemp), end(), v);
                }
                else{
                _e = &*uninitialized_fill(_a, iterator(this,endTemp), end(), v) - 1;
                }
                for(i = _bi; i <= _cei; ++i){
                    if((_e - *i) >= 0 && (_e - *i) <= 9){
                        break;}}
                 _ei = i;

            } else{
                T** newCont;
                _pa = std::allocator<T*>();
                

                size_type numBlocks = wholeCap*3;
                if(numBlocks == 0)
                    numBlocks = s*3;
                size_type halfBlock = numBlocks/3;
                newCont = _pa.allocate(numBlocks);
                size_type ind;
                size_type counter = 0;
                for(ind = 0; ind < numBlocks; ++ind){
                    if(_cei != 0 && (ind >= halfBlock) && (ind < halfBlock + wholeCap)){
                        newCont[ind] = _cont[counter];
                        ++counter;
                    }
                    else{
                        newCont[ind] = _a.allocate(10);}}
                size_type tempo = (_bi - _cbi);
                _bi = &newCont[halfBlock + tempo];
                _ei = &newCont[halfBlock + from_EIto_BI + tempo];
                if(counter == 0){
                    _b = _e = *_bi;
                }
                if(wholeCap != 0)
                    _pa.deallocate(_cont,wholeCap);
                _cont = newCont;   
                _cbi = _cont;
                _cei = &_cont[numBlocks-1];
                resize(s,v);
            }
            
            assert(valid());}

            void resize_front (size_type s, const_reference v = value_type()) {
            size_type from_EIto_BI = _ei - _bi;
           
                T** newCont;
                _pa = std::allocator<T*>();
                size_type wholeCap = _cei - _cbi + 1;
                size_type numBlocks = wholeCap*3;
                size_type halfBlock = numBlocks/3;
                newCont = _pa.allocate(numBlocks);
                size_type ind;
                size_type counter = 0;
                for(ind = 0; ind < numBlocks; ++ind){
                    if(_cei != 0 && (ind >= halfBlock) && (ind < halfBlock + wholeCap)){
                        newCont[ind] = _cont[counter];
                        ++counter;
                    }
                    else{
                        newCont[ind] = _a.allocate(10);
                    }
                }
                size_type tempo = (_bi - _cbi);
                _bi = &newCont[halfBlock + tempo];
                _ei = &newCont[halfBlock + from_EIto_BI + tempo];
                if(counter == 0){
                    _b = _e = *_bi;
                }
                if(wholeCap != 0)
                    _pa.deallocate(_cont,wholeCap);
                _cont = newCont;   
                _cbi = _cont;
                _cei = &_cont[numBlocks-1];
                assert(valid());}

        // ----
        // size
        // ----

        /**
         * returns the size of the deque
         */
        size_type size () const {
            return _size;}

        // ----
        // swap
        // ----

        /**
         * swaps the values of the deques
         */
        void swap (my_deque& that) {
            // if (_a == that._a) {
            //     std::swap(_cbi , that._cbi);
            //     std::swap(_cei , that._cei);
            //     std::swap(_bi , that._bi);
            //     std::swap(_ei , that._ei);
            //     std::swap(_cei , that._cei);
            //     std::swap(_cont , that._cont);
            //     std::swap(_b, that._b);
            //     std::swap(_e, that._e);
            //     std::swap(_size , that._size);
            // }
            // else {
                my_deque x(*this);
                *this = that;
                that = x;
            // }
            assert(valid());}};

#endif // Deque_h
