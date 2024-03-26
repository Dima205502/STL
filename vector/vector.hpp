#pragma once
#include<memory>
#include<stdexcept>

namespace MySTL{

  template<typename T>
  class vector{

    private:
       T* arr=nullptr;
       size_t sz=0;
       size_t cp=0;

    public:
       
       vector();
       vector(size_t,const T&);
       vector(const MySTL::vector<T>&);

       size_t size() const;
       void resize(size_t,const T&);

       size_t capacity() const;
       void reserve(size_t);

       bool empty() const;

       void push_back(const T& value);
       void pop_back();

       T& operator[](size_t);
       const T& operator[](size_t) const;

       T& at(size_t);
       const T& at(size_t) const;

       T& back();
       T& front();

       const T& back() const;
       const T& front() const;

       ~vector();
  };


  template<>
  class vector<bool>{
    private:
      uint8_t* arr=nullptr;
      size_t sz=0;
      size_t cp=0;

      struct BitReference{
        uint8_t* ptr;
        uint8_t pos;

        BitReference(uint8_t*,uint8_t);

        BitReference operator=(bool);

        operator bool() const;
      };

    public:
     
      vector();
      vector(size_t,bool);
      vector(const MySTL::vector<bool>&);
      
      size_t size() const;
      void resize(size_t,bool);

      size_t capacity() const;
      void reserve(size_t);

      bool empty() const;

      void push_back();
      void pop_back();

      BitReference operator[](size_t);
      const BitReference operator[](size_t) const;

      BitReference at(size_t);
      const BitReference at(size_t) const;

      BitReference back();
      BitReference front();

      const BitReference back() const;
      const BitReference front() const;

      ~vector();
  };
};
