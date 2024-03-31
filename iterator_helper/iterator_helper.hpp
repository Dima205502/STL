#pragma once
#include<iterator>

namespace MySTL{
   
   //        Iterator tag

   struct input_iterator_tag {};

   struct output_iterator_tag {};

   struct forward_iterator_tag: input_iterator_tag {};

   struct bidirectional_iterator_tag: forward_iterator_tag {};

   struct random_access_iterator_tag: bidirectional_iterator_tag {};

   struct contiguous_iterator_tag: public random_access_iterator_tag {};

   
   //        Iterator
   
   template<typename Category, typename T,typename Distance = ptrdiff_t,
            typename Pointer = T*,typename Reference = T&>
   struct iterator {
        using value_type = T;
        using difference_type = Distance;
        using pointer = Pointer;
        using reference = Reference;
        using iterator_category = Category;
   };


   //             CONDITIONAL<B,U,V>

   template<bool B,typename U,typename V>
   struct conditional{
      using type=V;
   };

   template<typename U,typename V>
   struct conditional<true,U,V>{
      using type=U;
   };

   template<bool B,typename U,typename V>
   using conditional_t=typename conditional<B,U,V>::type;


   //            ADVANCE(Iterator,int)
   
   template<typename Iterator,typename IterCategory>
   void advance_helper(Iterator& iter,int n,IterCategory){
       if(n>0){
         for(int i=0;i<n;++i){
            ++iter;
         }
       } else {  
         for(int i=0;i<-n;++i){
             --iter;
         }
       }
   }

   template<typename Iterator>
   void advance_helper(Iterator& iter,int n,std::random_access_iterator_tag){
       iter+=n;
   }
   
   template<typename Iterator>
   void advance(Iterator& iter,int n){
       advance_helper(iter,n,typename std::iterator_traits<Iterator>::iterator_category());
   }



   //            DISTANCE(Iterator,Iterator)

   template<typename Iterator,typename IterCategory>
   int distance_helper(Iterator it1,Iterator it2,IterCategory){
    int result=0;

    while(it1!=it2){
        ++it1,++result;
    }

    return result;
   }

   template<typename Iterator>
   int distance_helper(Iterator it1,Iterator it2,std::random_access_iterator_tag){
      return it2-it1;
   }

   template<typename Iterator>
   int distance(Iterator it1,Iterator it2){
       return distance_helper(it1,it2,typename std::iterator_traits<Iterator>::iterator_category());  
   }
   

   //           REVERSE_ITERATOR<Iterator>

   template<typename Iterator>
   class reverse_iterator{
    Iterator iter;

    public:
       
       reverse_iterator(Iterator iter): iter(--iter) {};

       reverse_iterator<Iterator>& operator++(){
          --iter;
          return *this;
       }

       reverse_iterator<Iterator>& operator++(int){
          reverse_iterator<Iterator> temp=*this;
          --iter;
          return temp;
       }
       
       reverse_iterator<Iterator>& operator--(){
           ++iter;
           return iter;
       }

       reverse_iterator<Iterator>& operator--(int){
           reverse_iterator<Iterator> temp=*this;
           --iter;
           return temp;
       }

       reverse_iterator<Iterator>& operator+=(int n){
          iter-=n;
          return *this;
       }

       reverse_iterator<Iterator>& operator-=(int n){
        iter+=n;
        return *this;
       }
       
       std::iterator_traits<Iterator>::value_type operator*(){
        return *iter;
       }
  
       Iterator base() const{
        return iter;
       }
   };
   

   //          BACK_INSERT_ITERATOR<Container>

   template<typename Container>
   class back_insert_iterator: public iterator<output_iterator_tag,void,void,void,void>{
       Container& container;

       public:
        back_insert_iterator(Container& container) : container(container)
        {};
         
        back_insert_iterator<Container>& operator++(){
            return *this;
        }

        back_insert_iterator<Container>& operator*(){
            return *this;
        }

        typename Container::value_type& operator=(const typename Container::value_type& value){
            container.push_back(value);
            return container.back();
        }
   };
   

   //         BACK_INSERTER(Container)

   template<typename Container>
   back_insert_iterator<Container> back_inserter(Container& container){
      return back_insert_iterator<Container>(container);
   }
}