#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <algorithm>
#include <type_traits>
#include "allocate.h"
#include "iterator.h"
namespace SmallSTL{
	template<class T,class Alloc=allocator<T> >
	class vector{       
		private:
            T *start_;
            T *finish_;
            T *end_of_storage;
            typedef Alloc dataAllocator;
        public:
            typedef T value_type;
            typedef T* iterator;
            typedef const T* const_iterator;
            typedef T& reference;
            typedef const T& const_reference;
            typedef size_t size_type;
            typedef ptrdiff_t difference_type;
        public:
            //构造，复制，析构等函数
            vecotor():start_(0),finish_(0),end_of_storage(0){}
            explicit vector(const size_type n);
            vector(const size_type n,const type_value &value);
            template <class InputIterator>
            vector(InputIterator first,InputIerator end);
            vector(const vecot &v);
            vector& operate=(const vector &v);
            ~vector();
            //比较相关函数
            bool operator==(const vecoter &v)const;
            bool operator!=(const vector &v)const;
            //迭代器
            iterator begin(){return start_;}
            const_iterator begin(){return start_;}
            const_iterator cbegin(){return start_;}
            iterator end(){return finish_;}
            const_iterator end(){return finish_;}
            const_iterator cend(){return finish_;}
            //与容量相关
            difference_type size()const {return finish_-start_;}
            difference_type capacity()const {return end_of_storage-start_;}
            bool empty(){return start_==finish_;}
            void resize(size_type n,value_type val=value_type());
            void reserve(size_type n);
            //访问元素相关
            reference operator[](const difference_type i){return *(begin()+i);}
            const_reference operator[](const difference_type i)const {return *(cbegin()+i);}
            reference front(){return *(begin());}
            reference back(){return *(end()-1);}
            pointer data(){return start_;}
            //修改容器相关
            void clear();
            void swap(vector &v);
            void push_back(const value_type &value);
            void pop_back();
            iterator insert(iterator position,const type_value &value);
            void insert(iterator position,const value_type &n,const value_type &value);
            template <class InputIterator>
            void insert(iterator position,InputIterator first,InputIterator last);
            iterator erase(iterator position);
            iterator erase(iterator first,iterator last);
        private:
            void destroyAndDeallocateALL();
            void allocateAndFillN(const size_type n,const value_type &value);
            template<class InputIterator>
			void allocateAndCopy(InputIterator first,InputIterator last);
			template<class InputIterator>
			void vector_aux(InputIterator first,InputIterator last,std::false_type);
			template<class Integer>
			void vector_aux(Integer n,const value_type& value,std::true_type);
			template<class InputIterator>
			void insert_aux(iterator position,InputIterator first,InputIterator last,std::false_type);
			template<class Integer>
			void insert_aux(iterator position,Integer n,const value_type& value,std::true_type);
			template<class InputInterator>
			void reallocateAndCopy(iterator position,InputIterator first,InputIterator last);
			void reallocateAndFillN(iterator position,const size_type &n,const value_type &val);
			size_type getNewCapacity(size_type len)const;
		public:
			template<class T,class Alloc>
			friend bool operator==(const vector<T,Alloc>& v1,const vector<T,Alloc>& v2);
			template<class,class Alloc>
			friend bool operator!=(const vector<T,Alloc>& v1,const vector<T,Alloc>& v2);
	};
}
#endif
