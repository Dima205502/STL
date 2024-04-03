#include"vector.hpp"

////////////////////////////     VECTOR<T>     /////////////////////////////


//           Member functions

template<typename T,typename Alloc>
MySTL::vector<T,Alloc>::vector() = default;

template<typename T, typename Alloc>
MySTL::vector<T,Alloc>::vector(size_t n, const T& value) {

    arr = alloc.allocate(2*(n+1));

    sz = n;
    cp = 2 * (sz + 1);

    size_t i = 0;
    try {
        for (; i < sz; ++i) {
            alloc.construct(arr + i, value);
        }
    }
    catch (...) {

        for (size_t j = 0; j < i; ++j) {
            alloc.destroy(arr + j);
        }
        
        alloc.deallocate(arr,cp);

        arr = nullptr;
        sz = cp = 0;

        throw;
    }
}


template<typename T, typename Alloc>
MySTL::vector<T,Alloc>::vector(const MySTL::vector<T>& other) {
    sz = other.sz;
    cp = other.cp;

    arr = alloc.allocate(cp);

    for (size_t i = 0; i < sz; ++i) {
        alloc.construct(arr + i, other.arr[i]);
    }
}

template<typename T, typename Alloc>
MySTL::vector<T,Alloc>::~vector() {
    for (size_t i = 0; i < sz; ++i) {
        alloc.destroy(arr + i);
    }

    if (arr != nullptr) alloc.deallocate(arr,cp);
}





//          Capacity

template<typename T, typename Alloc>
void MySTL::vector<T,Alloc>::reserve(size_t new_cap) {
    if (new_cap <= cp) return;

    T* new_arr = alloc.allocate(new_cap);

    size_t i = 0;
    try {
        for (; i < sz; ++i) {
            alloc.construct(new_arr + i, arr[i]);
        }
    }
    catch (...) {
        for (size_t j = 0; j < i; ++j) {
            alloc.destroy(new_arr + i);
        }
        throw;
    }

    for (size_t i = 0; i < sz; ++i) {
        alloc.destroy(arr + i);
    }

    if (arr != nullptr) alloc.deallocate(arr, cp);

    arr = new_arr;
    cp = new_cap;
};

template<typename T, typename Alloc>
size_t MySTL::vector<T,Alloc>::capacity() const {
    return cp;
}


template<typename T, typename Alloc>
size_t MySTL::vector<T,Alloc>::size() const {
    return sz;
}

template<typename T, typename Alloc>
bool MySTL::vector<T,Alloc>::empty() const {
    return sz == 0;
}


//          Modifiers

template<typename T, typename Alloc>
void MySTL::vector<T,Alloc>::resize(size_t n, const T& value) {
    if (n < sz) {
        for (size_t i = n; i < sz; ++i) {
            alloc.destroy(arr+i);
        }

        sz = n;
        return;
    }

    if (n > sz) reserve(2 * (n + 1));

    size_t i = sz;
    try {
        for (; i < n; ++i) {
           alloc.construct(arr+i,value);
        }
    }
    catch (...) {
        for (size_t j = sz; j < i; ++j) {
            alloc.destroy(arr + j);
        }

        throw;
    }

    sz = n;
}

template<typename T, typename Alloc>
void MySTL::vector<T,Alloc>::push_back(const T& value) {
    if (sz == cp) reserve(2 * (sz + 1));

    try {
        alloc.construct(arr + sz, value);
    }
    catch (...) {
        alloc.destroy(arr + sz);
        throw;
    }

    sz++;
}

template<typename T, typename Alloc>
void MySTL::vector<T,Alloc>::pop_back() {
    if (sz == 0) return;
    sz--;
    alloc.destroy(arr + sz);
}






//           Element access

template<typename T, typename Alloc>
T& MySTL::vector<T,Alloc>::operator[](size_t i) {
    return arr[i];
}

template<typename T, typename Alloc>
const T& MySTL::vector<T,Alloc>::operator[](size_t i) const {
    return arr[i];
}

template<typename T, typename Alloc>
T& MySTL::vector<T,Alloc>::at(size_t i) {
    if (i < sz) return arr[i];

    throw std::out_of_range("incorrect call method .at");
}

template<typename T, typename Alloc>
const T& MySTL::vector<T,Alloc>::at(size_t i) const {
    if (i < sz) return arr[i];

    throw std::out_of_range("incorrect call method .at");
}

template<typename T, typename Alloc>
T& MySTL::vector<T,Alloc>::front() {
    return arr[0];
}

template<typename T, typename Alloc>
T& MySTL::vector<T,Alloc>::back() {
    return arr[sz - 1];
}

template<typename T, typename Alloc>
const T& MySTL::vector<T,Alloc>::front() const {
    return arr[0];
}

template<typename T, typename Alloc>
const T& MySTL::vector<T,Alloc>::back() const {
    return arr[sz - 1];
}




//        Iterators

template<typename T, typename Alloc>
typename MySTL::vector<T,Alloc>::iterator MySTL::vector<T,Alloc>::begin() {
    return arr;
}

template<typename T, typename Alloc>
typename MySTL::vector<T,Alloc>::iterator MySTL::vector<T,Alloc>::end() {
    return arr + sz;
}


template<typename T, typename Alloc>
typename MySTL::vector<T,Alloc>::const_iterator MySTL::vector<T,Alloc>::begin() const {
    return arr;
}

template<typename T, typename Alloc>
typename MySTL::vector<T,Alloc>::const_iterator MySTL::vector<T,Alloc>::end() const {
    return arr + sz;
}

template<typename T, typename Alloc>
typename MySTL::vector<T,Alloc>::const_iterator MySTL::vector<T,Alloc>::cbegin() const {
    return arr;
}

template<typename T, typename Alloc>
typename::MySTL::vector<T,Alloc>::const_iterator MySTL::vector<T,Alloc>::cend() const {
    return arr + sz;
}

template<typename T, typename Alloc>
typename::MySTL::vector<T,Alloc>::reverse_iterator MySTL::vector<T,Alloc>::rbegin() {
    return reverse_iterator(end());
}

template<typename T, typename Alloc>
typename MySTL::vector<T,Alloc>::reverse_iterator MySTL::vector<T,Alloc>::rend() {
    return reverse_iterator(begin());
}

template<typename T, typename Alloc>
typename MySTL::vector<T,Alloc>::const_reverse_iterator MySTL::vector<T,Alloc>::rbegin() const {
    return const_reverse_iterator(end());
}

template<typename T, typename Alloc>
typename MySTL::vector<T,Alloc>::const_reverse_iterator MySTL::vector<T,Alloc>::rend() const {
    return const_reverse_iterator(begin());
}

template<typename T, typename Alloc>
typename MySTL::vector<T,Alloc>::const_reverse_iterator MySTL::vector<T,Alloc>::crbegin() const {
    return const_reverse_iterator(end());
}

template<typename T, typename Alloc>
typename MySTL::vector<T,Alloc>::const_reverse_iterator MySTL::vector<T,Alloc>::crend() const {
    return const_reverse_iterator(begin());
}




////////////////////////////     VECTOR<BOOL>     /////////////////////////////



//                 BitReference

MySTL::vector<bool>::BitReference::BitReference(uint8_t* ptr,uint8_t pos)
: ptr(ptr),pos(pos) {};

MySTL::vector<bool>::BitReference MySTL::vector<bool>::BitReference::operator=(bool value){
    if(value){
        *ptr |= ((uint8_t)1<<pos);
    } else {
        *ptr &= ~((uint8_t)1<<pos);
    }

    return *this;
}

MySTL::vector<bool>::BitReference::operator bool()const{
    return (*ptr)&((uint8_t)1<<pos);
}






//               Member functions

MySTL::vector<bool>::vector()=default;

MySTL::vector<bool>::vector(size_t n,bool value){
    arr=new uint8_t[(n+7)/8];
    
    sz=n;
    cp=2*(sz+1);

    for(size_t i=0;i<sz;++i){
        BitReference(arr+i/8,i%8)=value;
    }
}

MySTL::vector<bool>::vector(const MySTL::vector<bool>& other){
    arr=new uint8_t[other.cp];

    sz=other.sz;
    cp=other.cp;

    for(size_t i=0;i<(sz+7)/8;++i){
        arr[i]=other.arr[i];
    }
}

MySTL::vector<bool>::~vector(){
    delete[] arr;
}


//            Capacity


bool MySTL::vector<bool>::empty() const{
    return sz==0;
}

size_t MySTL::vector<bool>::size() const{
    return sz;
}

size_t MySTL::vector<bool>::capacity() const{
    return cp;
}



void MySTL::vector<bool>::reserve(size_t n){
    if(cp<=n) return;

    uint8_t* new_arr=new uint8_t[(n+7)/8];

    for(size_t i=0;i<sz;++i){
        BitReference(new_arr+i/8,i%8)=BitReference(arr+i/8,i%8);
    }
    
    delete[] arr;

    arr=new_arr;
    cp=n;
}



//            Modifieres


void MySTL::vector<bool>::push_back(const bool& value){
      if(sz==cp) reserve(2*(sz+1));

      BitReference(arr+sz/8,sz%8)=value;

      sz++;
}

void MySTL::vector<bool>::pop_back(){
     sz--;
}

void MySTL::vector<bool>::resize(size_t n,bool value){
    if(n<=sz){
        sz=n;
        return;
    }

    if(n>sz) reserve(2*(n+1));

    for(size_t i=sz;i<n;++i){
        BitReference(arr+i/8,i%8)=value;
    }

    sz=n;
}



//           Element access

MySTL::vector<bool>::BitReference MySTL::vector<bool>::operator[](size_t i){
    return BitReference(arr+i/8,i%8);
}

const MySTL::vector<bool>::BitReference MySTL::vector<bool>::operator[](size_t i) const{
    return BitReference(arr+i/8,i%8);
}


MySTL::vector<bool>::BitReference MySTL::vector<bool>::at(size_t i){
    if(i<sz) return BitReference(arr+i/8,i%8);

    throw std::out_of_range("incorrect call method .at");
}

const MySTL::vector<bool>::BitReference MySTL::vector<bool>::at(size_t i) const{
    if(i<sz) return BitReference(arr+i/8,i%8);

    throw std::out_of_range("incorrect call method .at");
}

MySTL::vector<bool>::BitReference MySTL::vector<bool>::front(){
    return BitReference(arr,0);
}

MySTL::vector<bool>::BitReference MySTL::vector<bool>::back(){
    return BitReference(arr+(sz-1)/8,(sz-1)%8);
}

const MySTL::vector<bool>::BitReference MySTL::vector<bool>::front() const{
    return BitReference(arr,0);
}

const MySTL::vector<bool>::BitReference MySTL::vector<bool>::back() const{
    return BitReference(arr+(sz-1)/8,(sz-1)%8);
}



//         Iterators

typename MySTL::vector<bool>::iterator    MySTL::vector<bool>::begin(){
    return BitReference(arr,0);
}


typename MySTL::vector<bool>::iterator    MySTL::vector<bool>::end(){
    return BitReference(arr+sz/8,sz%8);
}


typename MySTL::vector<bool>::const_iterator   MySTL::vector<bool>::begin() const{
    return BitReference(arr,0);
}


typename MySTL::vector<bool>::const_iterator   MySTL::vector<bool>::end() const{
    return BitReference(arr+sz/8,sz%8);
}

typename MySTL::vector<bool>::const_iterator   MySTL::vector<bool>::cbegin() const{
    return BitReference(arr,0);
}

typename::MySTL::vector<bool>::const_iterator  MySTL::vector<bool>::cend() const{
    return BitReference(arr+sz/8,sz%8);
}

typename::MySTL::vector<bool>::reverse_iterator MySTL::vector<bool>::rbegin(){
    return reverse_iterator(end());
}

typename MySTL::vector<bool>::reverse_iterator  MySTL::vector<bool>::rend(){
    return reverse_iterator(begin());
}

typename MySTL::vector<bool>::const_reverse_iterator   MySTL::vector<bool>::rbegin() const{
    return const_reverse_iterator(end());
}

typename MySTL::vector<bool>::const_reverse_iterator   MySTL::vector<bool>::rend() const{
    return const_reverse_iterator(begin());
}

typename MySTL::vector<bool>::const_reverse_iterator   MySTL::vector<bool>::crbegin() const{
    return const_reverse_iterator(end());
}

typename MySTL::vector<bool>::const_reverse_iterator   MySTL::vector<bool>::crend() const{
    return const_reverse_iterator(begin());
}