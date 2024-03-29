namespace MySTL{
    template<typename T>
    class list{
        private:
            
            struct BaseNode{
                BaseNode* next;
                BaseNode* prev;
                BaseNode(BaseNode*,BaseNode*);
            };

            struct Node: BaseNode{
                T value;
                Node(const T&,BaseNode*,BaseNode*);
            };
            
            BaseNode FakeNode;
            size_t sz;

        public:    

            list();
            list(size_t,const T&);
            list(const MySTL::list<T>&);

            T& front();
            T& back();

            const T& front() const;
            const T& back() const;

            void push_front(const T&);
            void push_back(const T&);

            void pop_front();
            void pop_back();

            bool empty() const;
            size_t size() const;
    };
};