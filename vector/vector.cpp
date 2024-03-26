#include"vector.hpp"

////////////////////////////     VECTOR<T>     /////////////////////////////
template<typename T>
MySTL::vector<T>::vector()=default;

template<typename T>
MySTL::vector<T>::vector(size_t n,const T& value){

    arr=reinterpret_cast<T*>(new uint8_t[2*(n+1)*sizeof(T)]);
    
    sz=n;
    cp=2*(sz+1);

    size_t i=0;
    try{
        for(;i<sz;++i){
            new(arr+i) T(value);
        }
    } catch(...){

        for(size_t j=0;j<i;++j){
            (arr+j)->~T();
        }
        delete[] reinterpret_cast<uint8_t*>(arr);
        arr=nullptr;
        sz=cp=0;

        throw;
    }
}


template<typename T>
MySTL::vector<T>::vector(const MySTL::vector<T>& other){
    sz=other.sz;
    cp=other.cp;

    arr=reinterpret_cast<T*>(new int8_t(cp*sizeof(T)));

    for(size_t i=0;i<sz;++i){
        new (arr+i) T(other.arr[i]);
    }
}


template<typename T>
size_t MySTL::vector<T>::capacity() const{
    return cp;
}

template<typename T>
void MySTL::vector<T>::reserve(size_t n){
    if(n<=cp) return;

    T* new_arr=reinterpret_cast<T*>(new int8_t[n*sizeof(T)]);
            
    try{
        std::uninitialized_copy(arr,arr+sz,new_arr);
    } catch(...){
        delete[] reinterpret_cast<int8_t*>(new_arr);
        throw;
    }

    for(size_t i=0;i<sz;++i){
        (arr+i)->~T();
    }

    if(arr!=nullptr) delete[] reinterpret_cast<int8_t*>(arr);

    arr=new_arr;
    cp=n;
};

template<typename T>
size_t MySTL::vector<T>::size() const{
    return sz;
}

template<typename T>
void MySTL::vector<T>::resize(size_t n,const T& value){
    if(n<sz){
        for(size_t i=n;i<sz;++i){
            (arr+i)->~T();
        }

        sz=n;
        return;
    } 

    if(n>sz) reserve(2*(n+1));
    
    size_t i=sz;
    try{
       for(;i<n;++i){
          new (arr+i) T(value);
       } 
    } catch(...){
       for(size_t j=sz;j<i;++j){
          (arr+j)->T();
       }
    
       throw;
    }

    sz=n;
}

template<typename T>
void MySTL::vector<T>::push_back(const T& value){
    if(sz==cp) reserve(2*(sz+1));

    try{
       new (arr+sz) T(value);
    } catch(...){
       (arr+sz)->~T();
       throw;
    }

    sz++;
}

template<typename T>
void MySTL::vector<T>::pop_back(){
    if(sz==0) return;
    sz--;
    (arr+sz)->~T();
}

template<typename T>
T& MySTL::vector<T>::operator[](size_t i){
    return arr[i];   
}

template<typename T>
const T& MySTL::vector<T>::operator[](size_t i) const{
    return arr[i];
}

template<typename T>
T& MySTL::vector<T>::at(size_t i){
    if(i<sz) return arr[i];

    throw std::out_of_range("incorrect call method .at");
}

template<typename T>
const T& MySTL::vector<T>::at(size_t i) const{
    if(i<sz) return arr[i];

    throw std::out_of_range("incorrect call method .at");
}

template<typename T>
T& MySTL::vector<T>::front(){
    return arr[0];
}

template<typename T>
T& MySTL::vector<T>::back(){
    return arr[sz-1];
}

template<typename T>
const T& MySTL::vector<T>::front() const{
    return arr[0];
}

template<typename T>
const T& MySTL::vector<T>::back() const{
    return arr[sz-1];
}

template<typename T>
MySTL::vector<T>::~vector(){
    for(size_t i=0;i<sz;++i){
        (arr+i)->~T();
    }

    if(arr!=nullptr) delete[] reinterpret_cast<uint8_t*>(arr);
}

////////////////////////////     VECTOR<BOOL>     /////////////////////////////

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

size_t MySTL::vector<bool>::capacity() const{
    return cp;
}

void MySTL::vector<bool>::reserve(size_t n){
    if(cp<=n) return;

    uint8_t* new_arr=new uint8_t[n];

    for(size_t i=0;i<(sz+7)/8;++i){
        new_arr[i]=arr[i];
    }

    cp=n;
}

size_t MySTL::vector<bool>::size() const{
    return sz;
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

bool MySTL::vector<bool>::empty() const{
    return sz==0;
}

MySTL::vector<bool>::BitReference MySTL::vector<bool>::operator[](size_t i){
    return BitReference(arr+i/8,i%8);
}

MySTL::vector<bool>::BitReference MySTL::vector<bool>::at(size_t i){
    if(i<sz) return BitReference(arr+i/8,i%8);

    throw std::out_of_range("incorrect call method .at");
}

const MySTL::vector<bool>::BitReference MySTL::vector<bool>::operator[](size_t i) const{
    return BitReference(arr+i/8,i%8);
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

MySTL::vector<bool>::~vector(){
    delete[] arr;
}