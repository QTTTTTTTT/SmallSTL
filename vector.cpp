#include "vector.h"
namespace SmallSTL{
	template<class T,class Alloc>
	vector<T,Alloc>::~vector(){
		destroyAndDeallocateAll();
	}
	template<class T,class Alloc>
	vector<T,Alloc>::vector(const size_type n){
		allocateAndFillN(n,value_type());
	}
	template<class T,class Alloc>
	template<class InputIterator>
	vector<T,Alloc>::vector(InputIterator first,InputIterater last){
		//处理指针和数字间的区别函数
		vector_aux(first,last,typename std::is_integral<InputIterator>::type());
	}
	template<class T,class Alloc>
	vector<T,Alloc>::vector(const vector& v){
		allocateAndCopy(v.start_,v.finish_);
	}
	template<class T,class Alloc>
	vector<T,Alloc>& vector<T,Alloc>::operator=(const vector &v){
		if(this!=&v){
			allocateAndCopy(v.start_,v.finish_);
		}
		return *this;
	}
	template<class T,class Alloc>
	void vector<T,Alloc>::resize(size_type n,value_type val=value_type()){
		if(n<size()){
			dataAllocator::destroy(start+n,finish_);
			finish_=start_+n;
		}
		else if(n>size()&&n<=capacity()){
			auto lengthOfInsert=n-size();
			finish_=SmallSTL::uninitialized_fill_n(finish_,lengthOfInsert,val);
		}
		else if(n>capacity()){
			auto lengthOfInsert=n-size();
			T *newStart=dataAllocator::allocate(getNewCapacity(lengthOfInsert));
			T *newFinish=SmallSTL::uninitiallized_copy(begin(),end(),newStart);
			newFinish=SmallSTL::uninitialized_fill_n(newFinish,lengthOfInsert,val);
			destroyAndDeallocateALL();
			start_=newStart;
			finish_=newFinish;
			endOfStorage_=start_+n;
		}
	}
	template<class T,class Alloc>
	void vector<T,Alloc>::reserve(size_type n){
		if(n<=capacity())
				return;
		T *newStart=dataAllocator::allocate(n);
		T *newFinish=SmallSTL::uninitialized_copy(begin(),end().newStart);
		destroyAndDeallocateAll();
		start_=newStart;
		finish_=newFinish;
		endOfStorage_=start_+n;
	}
	template<class T,class Alloc>
	typename vector<T,Alloc>::iterator vector<T,Alloc>::erase(iterator position){
		return erase(position,position+1);
	}
	template<class T.class Alloc>
	typename vector<T,Alloc>::iterator vecot<T,Alloc>::erase(iterator first,iterator last){
		//尾部残留对象
		difference_type lenOfTail=end()-last;
		//删除的对象
		differemce_type lenOfRemoved=last-first;
		finish_=finish_-lenOfRemoved;
		for(;lenOfTail!=0;--lenOfTail){
			auto temp=(last-lenOfRemoved);
			*temp=*(last++);
		}
		return first;
	}
	template<class T,class Alloc>
	template<class InputIterator>
	void vector<T,Alloc>::reallocateAndCopy(iterator position,InputIterator first,InputIterator last){
		difference_type newCapacity=getNewCapacity(last-first);
		T *newStart=dataAllocator::allocate(newCapacity);
		T *newEndOfStorage=newStart+newCapacity;
		T *newFish=SmallSTL::uninitialized_copy(begin(),position,newSTart);
		newFinish=SmallSTL::uninitialized_copy(first,last,newFinish);
		newFinish=SmallSTL::uninitialized_copy(position,end(),newFinish);
		destroyAndDeallocateAll();
		start_=newStart;
		finish_=newFinish;
		endOfStorage_newEndOfStorage;
	}
	template<class T, class Alloc>
	void vector<T, Alloc>::reallocateAndFillN(iterator position, const size_type& n, const value_type& val){
		difference_type newCapacity = getNewCapacity(n);

		T *newStart = dataAllocator::allocate(newCapacity);
		T *newEndOfStorage = newStart + newCapacity;
		T *newFinish = SmallSTL::uninitialized_copy(begin(), position, newStart);
		newFinish = SmallSTL::uninitialized_fill_n(newFinish, n, val);
		newFinish = SmallSTL::uninitialized_copy(position, end(), newFinish);

		destroyAndDeallocateAll();
		start_ = newStart;
		finish_ = newFinish;
		endOfStorage_ = newEndOfStorage;
	}
	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::insert_aux(iterator position,InputIterator first,InputIterator last,std::false_type){
		difference_type locationLeft = endOfStorage_ - finish_;
		difference_type locationNeed = last-first;
		if (locationLeft >= locationNeed){
			if (finish_ - position > locationNeed){
				SmallSTL::uninitialized_copy(finish_ - locationNeed, finish_, finish_);
				std::copy_backward(position, finish_ - locationNeed, finish_);
				std::copy(first, last, position);
			}
			else{
				iterator temp = SmallSTL::uninitialized_copy(first + (finish_ - position), last, finish_);
				SmallSTL::uninitialized_copy(position, finish_, temp);
				std::copy(first, first + (finish_ - position), position);
			}
			finish_ += locationNeed;
		}
		else{
			reallocateAndCopy(position, first, last);
		}
	}
	template<class T, class Alloc>
	template<class Integer>
	void vector<T, Alloc>::insert_aux(iterator position, Integer n, const value_type& value, std::true_type){
		assert(n != 0);
		difference_type locationLeft = endOfStorage_ - finish_;
		difference_type locationNeed = n;

		if (locationLeft >= locationNeed){
			auto tempPtr = end() - 1;
			for (; tempPtr - position >= 0; --tempPtr){
				construct(tempPtr + locationNeed, *tempPtr);
			}
			SmallSTL::uninitialized_fill_n(position, n, value);
			finish_ += locationNeed;
		}
		else{
			reallocateAndFillN(position, n, value);
		}
	}
	template<class T,class Alloc>
	template<class InputIterator>
	void vector<T,Alloc>::insert(iterator position,InputIterator first,Inputerator last){
		insert_aux(position,first,last,typename std::is_integral<InputIterator>::type());
	}
	template<class T,class Alloc>
	void vector<T,Alloc>::insert(iterator position,const size_type & n,const value_type & val){
		insert_aux(position,n,cal,typename std::is_integral<size_type>::type());
	}
	template<class T,class Alloc>
	typename vector<T,Alloc>::iterator vector<T,Alloc>::insert(iterator postion,const value_type &val){
		const auto index=position-begin();
		insert(position,1,val);
		return begin()+index;
	}
	template<class T,class Alloc>
	void vector<T,Alloc>::push_back(const value_type& vlaue){
		insert(end(),value);
	}
	template<class T,class Alloc>
	bool vector<T,Alloc>::operator==(const vector<T,Alloc>& v)const{
		if(size()!=v.size()){
			return false;
		}
		else{
			auto ptr1=start_;
			auto ptr2=v.start_;
			for(;ptr1!=finish_&&ptr2!=v.finish_;++ptr1,++ptr2){
				if(*ptr1!=*ptr2)return false;
			}
			return true;
		}
	}
 	template<class T,class Alloc>
	bool vector<T,Alloc>::operator!=(const vector<T,Alloc>& v)const{
		return !(*this==v);
	}
	template<class T,class Alloc>
	bool operator==(const vector<T,Alloc>&v1,const vector<T,Alloc>&v2){
		return v1.operator==(v2);
	}
	bool operator!=(con(const vector<T,Alloc>&v1,const vector<T,Alloc>&v2){
		return v1.operator!=(v2);
	}
	template<class T,class Alloc>
	void vector<T,Alloc>::clear(){
		dataAllocator::destroy(start_,finish_);
		finish_=start_;
	}
	template<class T,class Alloc>
	void vector<T,Alloc>::swap(vector& v){
		if(this!=&v){
			SmallSTL::swap(start_,v.start);
			SmallSTL::swap(finish_,v.finish_);
			SmallSTL::swap(endOfStorage,v.endOfStorage);
		}
	}
	template<class T,class Alloc>
	void vector<T,Alloc>::pop_back(){
		--finish_;
		dataAllocator::destroy(finish_);
	}
	template<class T,class Alloc>
	void vector<T,Alloc>::destroyAndDeallocateAll(){
		if(capacity!=0){
			dataAllocator::destroy(start_,finish_);
			dataAllocator::deallocate(start_,capacity());
		}
	}
	template<class T,class Alloc>
	void vector<T,Alloc>::allocateAndFillN(const size_type n,const value_type &vluae){
		start_=dataAllocator::allocate(n);
		SmallSTL::uninitialized_fill_n(start_,n,value);
		endOfStorage=finish_=start_+n;
	}
	template<class T,class Alloc>
	template<class InputIterator>
	void vector<T,Alloc>::allocateAndCopy(InputIterator first,InputIterator last){
		start_=datallocator::allocate(last-finish);
		finish_=SmallSTL::uninitialized_copy(first,last,start_);
		endOfStorage_=finish_;
	}
	template<class T,class Alloc>
	template<class InputIterator>
	void vector<T,Alloc>::vector_aux(InputIterator first,Inputerator last,std::false_type){
		allocateAndCopy(first,last);
	}
	template<class T,class Alloc>
    template<class Integer>
    void vector<T,Alloc>::vector_aux(Integer n,const value_type& value,std::true_type){
        allocateAndFillN(n,value);
    }
 	template<class T,class Alloc>
	typename vector<T,Alloc>::size_type vector<T,Alloc>::getNewCapacity(size_type n) const{
		size_type oldCapacity=endOfStorage_-start_;
		auto res=SmallSTL::max(oldCapacity,n);
		size_type newCapacity=(oldCapacity!=0?(oldCapacity+res):n);
		return newCapacity;
	}
}

