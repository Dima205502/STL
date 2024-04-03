#include"unordered_map.hpp"

template<typename Key,typename Value,typename Hash,typename KeyEqual>
MySTL::unordered_map<Key,Value,Hash,KeyEqual>::BaseNode::BaseNode(BaseNode* prev,BaseNode* next):prev(prev),next(next)
{}

template<typename Key,typename Value,typename Hash,typename KeyEqual>
MySTL::unordered_map<Key,Value,Hash,KeyEqual>::Node::Node(BaseNode* prev,BaseNode* next,value_type kv,size_t cache):BaseNode(prev,next),kv(kv),cache(cache)
{}

//         Member functions;


template<typename Key,typename Value,typename Hash,typename KeyEqual>
MySTL::unordered_map<Key,Value,Hash,KeyEqual>::unordered_map(): FakeNode{&FakeNode,&FakeNode}, sz(0), bucket_cnt(1), max_load_factor(1.0){
   buckets = new Node*[1];
   bucket_sz =  new size_t[1];

   buckets[0]=nullptr;
}

template<typename Key,typename Value,typename Hash,typename KeyEqual>
MySTL::unordered_map<Key,Value,Hash,KeyEqual>::unordered_map(size_t bucket_count): FakeNode{&FakeNode,&FakeNode}, sz(0), bucket_cnt(bucket_count),max_load_factor(1.0){
   buckets = new Node*[bucket_count];
   bucket_sz = new size_t[bucket_count];

   for(size_t i=0;i<bucket_count;++i){
      buckets[i]=nullptr;
   }
}

template<typename Key,typename Value,typename Hash,typename KeyEqual>
MySTL::unordered_map<Key,Value,Hash,KeyEqual>::unordered_map(const unordered_map& other): FakeNode{&FakeNode,&FakeNode}{
   buckets = new Node*[other.bucket_cnt];
   bucket_sz = new size_t[other.bucket_cnt];

   for(size_t i=0;i<other.bucket_cnt;++i){
      buckets[i]=nullptr;
   }

   for(size_t i=0;i<other.bucket_cnt;++i){
      bucket_sz[i] = other.buckets_size[i];
   }

   if(other.empty()) return;

   Node* other_cur=static_cast<Node*>(other.FakeNode.next);
   
   Node* cur = new Node(Node(&FakeNode,&FakeNode,other_cur->kv,other_cur->cache));
   buckets[other_cur->cache] = cur;
   other_cur = static_cast<Node*>(other_cur->next);

   FakeNode.next = cur;
   FakeNode.prev = cur;


   for (size_t i = 1; i < other.sz; ++i) {
      cur->next = new Node(cur,&FakeNode,other_cur->kv,other_cur->cache);

      if(cur->cache!=other_cur->cache){
            buckets[other_cur->cache] = cur->next;
      }

      cur = static_cast<Node*>(cur->next);
      other_cur = static_cast<Node*>(other_cur->next);
   }

   FakeNode.prev = cur;

   max_load_factor = other.max_load_factor;
   bucket_cnt = other.bucket_cnt;
   sz=other.sz;
}




template<typename Key,typename Value,typename Hash,typename KeyEqual>
MySTL::unordered_map<Key,Value,Hash,KeyEqual>::~unordered_map(){
   Node* cur=static_cast<Node*>(FakeNode.next);

   while(cur->next!=(&FakeNode)){
      Node* tmp=cur;
      cur=static_cast<Node*>(cur->next);
      delete tmp;
   }

   if(cur!=(&FakeNode)) delete cur;
   delete[] buckets;
   delete[] bucket_sz;
}



//            Capacity

template<typename Key,typename Value,typename Hash,typename KeyEqual>
bool MySTL::unordered_map<Key,Value,Hash,KeyEqual>::empty() const{
   return sz==0;
}

template<typename Key,typename Value,typename Hash,typename KeyEqual>
size_t MySTL::unordered_map<Key,Value,Hash,KeyEqual>::size() const{
   return sz;
}




//           Modifieres

template<typename Key,typename Value,typename Hash,typename KeyEqual>
std::pair< typename MySTL::unordered_map<Key,Value,Hash,KeyEqual>::iterator, bool > // return value   pair<iterator,bool>
MySTL::unordered_map<Key,Value,Hash,KeyEqual>::insert(std::pair<const Key,Value> X){
   if(1.0*sz/bucket_cnt > max_load_factor) rehash(next_prime(bucket_cnt*1.75));

   size_t hash=Hash{}(X.first)%bucket_cnt;

   if(buckets[hash]==nullptr){
      buckets[hash] = new Node(&FakeNode,FakeNode.next,X,hash);
      FakeNode.next->prev = buckets[hash];
      FakeNode.next = buckets[hash];

      bucket_sz[hash]++;
      sz++;

      return std::make_pair(iterator(buckets[hash]),true);
   }

   Node* cur = buckets[hash];
   while(cur->cache == hash){
      if(KeyEqual{}(cur->kv.first,X.first)){

          return std::make_pair(iterator(cur),false);
      }
      
      if(cur->next==(&FakeNode)){
         Node* new_node = new Node(cur,&FakeNode,X,hash);
         cur->next = new_node;
         FakeNode.prev = new_node;

         bucket_sz[hash]++;
         sz++;

         return std::make_pair(iterator(new_node),true);
      }

      cur=static_cast<Node*>(cur->next);
   }

   Node* new_node = new Node(cur->prev,cur,X,hash);
   cur->prev->next = new_node;
   cur->prev = new_node;

   bucket_sz[hash]++;
   sz++;

   return std::make_pair(iterator(new_node),true);
}  


template<typename Key,typename Value,typename Hash,typename KeyEqual>
size_t   MySTL::unordered_map<Key,Value,Hash,KeyEqual>::erase(const Key& key){
   size_t hash = Hash{}(key)%bucket_cnt;
   
   size_t cnt=0;

   if(buckets[hash]==nullptr){
      return cnt;
   }
   
   Node* cur = buckets[hash];
   while(cur->cache == hash){
      if(KeyEqual{}(cur->kv.first,key)){
         Node* tmp=cur;
         cur->prev->next=cur->next;
         cur->next->prev=cur->prev;
         cnt++;

         bucket_sz[hash]--;
         sz--;
   

         if(cur->next!=(&FakeNode)){
            cur=static_cast<Node*>(cur->next);
            delete tmp;
         } else{
            delete tmp;
            break;
         }

         continue;
      }
      
      if(cur->next==(&FakeNode)){
         break;
      }

      cur=static_cast<Node*>(cur->next);
   }

   return cnt;
}


template<typename Key,typename Value,typename Hash,typename KeyEqual>
typename MySTL::unordered_map<Key,Value,Hash,KeyEqual>::iterator     MySTL::unordered_map<Key,Value,Hash,KeyEqual>::erase(iterator iter){
   Node* node = iter.ptr;
   Node* next_node = static_cast<Node*>(node->next);
   
   node->prev->next = static_cast,Node*>(node->next);
   node->next->prev = static_cast<Node*>(node->prev);
   
   bucket_sz[node->cache]--;
   sz--;

   delete node;

   return next_node;
}


//               Element access

template<typename Key,typename Value,typename Hash,typename KeyEqual>
Value& MySTL::unordered_map<Key,Value,Hash,KeyEqual>::operator[](const Key& key){
   size_t hash=Hash{}(key)%bucket_cnt;
   
   if(buckets[hash]==nullptr){
      Node* new_node = new Node(&FakeNode,FakeNode.next,std::make_pair(key,Value()),hash);
      buckets[hash] = new_node;

      FakeNode.next->prev=new_node;
      FakeNode.next=new_node;
      
      bucket_sz[hash]++;
      sz++;

      return new_node->kv.second;
   }

   
   Node* cur = buckets[hash];

   while(cur->cache == hash){
      if(KeyEqual{}(cur->kv.first,key)){
           return cur->kv.second;
      }
      
      if(cur->next==(&FakeNode)){
         Node* new_node = new Node(cur,&FakeNode,std::make_pair(key,Value()),hash);
         cur->next = new_node;
         FakeNode.prev = new_node;

         bucket_sz[hash]++;
         sz++;

         return new_node->kv.second;
      }

      cur=static_cast<Node*>(cur->next);
   }

   Node* new_node = new Node(cur->prev,cur,std::make_pair(key,Value()),hash);
   cur->prev->next = new_node;
   cur->prev = new_node;

   bucket_sz[hash]++;
   sz++;

   return new_node->kv.second;
}

template<typename Key,typename Value,typename Hash,typename KeyEqual>
size_t MySTL::unordered_map<Key,Value,Hash,KeyEqual>::count(const Key& key) const{
   size_t hash=Hash{}(key)%bucket_cnt;
   
   if(buckets[hash]==nullptr){
      return 0;
   }

   
   Node* cur = buckets[hash];

   while(cur->cache == hash){
      if(KeyEqual{}(cur->kv.first,key)){
           return 1;
      }
      
      if(cur->next==(&FakeNode)){
         return 0;
      }

      cur=static_cast<Node*>(cur->next);
   }

   return 0;
}


template<typename Key,typename Value,typename Hash,typename KeyEqual>
typename MySTL::unordered_map<Key,Value,Hash,KeyEqual>::iterator  //     return iterator
MySTL::unordered_map<Key,Value,Hash,KeyEqual>::find(const Key& key){
   size_t hash=Hash{}(key)%bucket_cnt;
   
   if(buckets[hash]==nullptr){
      return end();
   }

   
   Node* cur = buckets[hash];

   while(cur->cache == hash){
      if(KeyEqual{}(cur->kv.first,key)){
           return iterator(cur);
      }
      
      if(cur->next==(&FakeNode)){
         return end();
      }

      cur=static_cast<Node*>(cur->next);
   }

   return end();
}



template<typename Key,typename Value,typename Hash,typename KeyEqual>
typename MySTL::unordered_map<Key,Value,Hash,KeyEqual>::const_iterator  //     return const_iterator
MySTL::unordered_map<Key,Value,Hash,KeyEqual>::find(const Key& key) const{
   size_t hash=Hash{}(key)%bucket_cnt;
   
   if(buckets[hash]==nullptr){
      return cend();
   }

   
   Node* cur = buckets[hash];

   while(cur->cache == hash){
      if(KeyEqual{}(cur->kv.first,key)){
           return const_iterator(cur);
      }
      
      if(cur->next==(&FakeNode)){
         return cend();
      }

      cur=static_cast<Node*>(cur->next);
   }

   return cend();
}



//            Bucket interface



template<typename Key,typename Value,typename Hash,typename KeyEqual>
typename MySTL::unordered_map<Key,Value,Hash,KeyEqual>::iterator       
MySTL::unordered_map<Key,Value,Hash,KeyEqual>::begin(){
   return iterator(FakeNode.next);
}


template<typename Key,typename Value,typename Hash,typename KeyEqual>
typename MySTL::unordered_map<Key,Value,Hash,KeyEqual>::iterator       
MySTL::unordered_map<Key,Value,Hash,KeyEqual>::end(){
   return iterator(&FakeNode);
}


template<typename Key,typename Value,typename Hash,typename KeyEqual>
typename MySTL::unordered_map<Key,Value,Hash,KeyEqual>::const_iterator       
MySTL::unordered_map<Key,Value,Hash,KeyEqual>::begin() const{
   return const_iterator(FakeNode.next);
}

template<typename Key,typename Value,typename Hash,typename KeyEqual>
typename MySTL::unordered_map<Key,Value,Hash,KeyEqual>::const_iterator       
MySTL::unordered_map<Key,Value,Hash,KeyEqual>::end() const{
   return const_iterator(&FakeNode);
}

template<typename Key,typename Value,typename Hash,typename KeyEqual>
size_t MySTL::unordered_map<Key,Value,Hash,KeyEqual>::bucket_count() const{
   return bucket_cnt;
}

template<typename Key,typename Value,typename Hash,typename KeyEqual>
size_t MySTL::unordered_map<Key,Value,Hash,KeyEqual>::bucket_size(size_t i) const{
   return bucket_sz[i];
}

template<typename Key, typename Value, typename Hash, typename KeyEqual>
typename MySTL::unordered_map<Key, Value, Hash, KeyEqual>::iterator
MySTL::unordered_map<Key, Value, Hash, KeyEqual>::begin() {
    return iterator(FakeNode.next);
}


template<typename Key, typename Value, typename Hash, typename KeyEqual>
typename MySTL::unordered_map<Key, Value, Hash, KeyEqual>::iterator
MySTL::unordered_map<Key, Value, Hash, KeyEqual>::end() {
    return iterator(&FakeNode);
}


template<typename Key, typename Value, typename Hash, typename KeyEqual>
typename MySTL::unordered_map<Key, Value, Hash, KeyEqual>::const_iterator
MySTL::unordered_map<Key, Value, Hash, KeyEqual>::begin() const {
    return const_iterator(FakeNode.next);
}

template<typename Key, typename Value, typename Hash, typename KeyEqual>
typename MySTL::unordered_map<Key, Value, Hash, KeyEqual>::const_iterator
MySTL::unordered_map<Key, Value, Hash, KeyEqual>::end() const {
    return const_iterator(&FakeNode);
}


template<typename Key, typename Value, typename Hash, typename KeyEqual>
typename MySTL::unordered_map<Key, Value, Hash, KeyEqual>::const_iterator
MySTL::unordered_map<Key, Value, Hash, KeyEqual>::cbegin() const {
    return const_iterator(FakeNode.next);
}

template<typename Key, typename Value, typename Hash, typename KeyEqual>
typename MySTL::unordered_map<Key, Value, Hash, KeyEqual>::const_iterator
MySTL::unordered_map<Key, Value, Hash, KeyEqual>::cend() const {
    return const_iterator(&FakeNode);
}





//                 Hash policy


template<typename Key,typename Value,typename Hash,typename KeyEqual>
double  MySTL::unordered_map<Key,Value,Hash,KeyEqual>::loud_factor() const{
   return 1.0*sz/bucket_cnt;
}

template<typename Key,typename Value,typename Hash,typename KeyEqual>
double  MySTL::unordered_map<Key,Value,Hash,KeyEqual>::max_loud_factor() const{
   return max_load_factor;
}


template<typename Key,typename Value,typename Hash,typename KeyEqual>
void  MySTL::unordered_map<Key,Value,Hash,KeyEqual>::rehash(size_t n){
   Node** new_buckets = new Node* [n];
   size_t* new_bucket_sz = new size_t [n];

   for(size_t i=0;i<n;++i){
      new_buckets[i] = nullptr;
   }

   Node* cur = static_cast<Node*>(FakeNode.next);

   for(size_t i=0;i<sz;++i){
      size_t hash = Hash{}(cur->kv.first)%n;
      cur->cache = hash;

      Node* tmp = cur;

      cur=static_cast<Node*>(cur->next);
      
      bool first=true;
      if(new_buckets[hash]==nullptr){
         new_buckets[hash] = tmp;

         if(first){
            tmp->next = &FakeNode;
            tmp->prev = &FakeNode;
            FakeNode->next = tmp;
            FakeNode->prev = tmp;

            first = false;
         } else {
            tmp->next = FakeNode.next;
            FakeNode->next->prev = tmp;
            tmp->prev = &FakeNode;
            FakeNode->next = tmp;
         }

      } else {
         Node* new_cur = new_buckets[hash];
  
         new_cur->prev->next = static_cast<Node*>(tmp);
         tmp->prev = static_cast<Node*>(new_cur->prev);
         new_cur->prev = static_cast<Node*>(tmp);
         tmp->next = static_cast<Node*>(new_cur); 
      }
   }

   FakeNode.prev = cur;

   for(size_t i=0;i<bucket_cnt;++i){
      buckets[i] = nullptr;
   }

   delete[] buckets;
   delete[] bucket_sz;

   buckets = new_buckets;
   bucket_sz = new_bucket_sz;
   bucket_cnt = n;
}




template<typename Key,typename Value,typename Hash,typename KeyEqual>
void MySTL::unordered_map<Key,Value,Hash,KeyEqual>::reserve(size_t n){
   size_t need_bucket_count = n+sz;

   rehash(nextPrime(need_bucket_count));
}