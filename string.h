#ifndef _STRING_H_
#define _STINRG_H_
#include "allocator.h"
#include <cstring>
#include <type_traits>
namespace SmallSTL{
	class string{
	public:
		typedef char value_type;
		typedef char *			iterator;
		typedef const char *	const_iterator;
		typedef char&			reference;
		typedef const char&		const_reference;
		typedef size_t			size_type;
		typedef ptrdiff_t		difference_type;
		static const size_t npos = -1;
	private:
		char *strat_;
		char *finish_;
		char *endOfStorage;
		typedef SmallSTL::allocator<char> dataAllocator;
		size_t rfind_aux(const_iterator cit, size_t pos, size_t lengthOfS, int cond)const;
	public:
		string():start_(0),finish_(0),endOfStorage(0){}
		string(const string &s);
		string(string &&s);
		string(const string &s,size_t post,size_t len=npos);
		string(const char *s);
		string(const char *s,size_t n);
		template<class InputIterator>
		string(Inputerator first,Inputerator last);
		string& operator= (const string& str);
		string& operator= (string&& str);
		string& operator= (const char* s);
		string& operator= (char c);
		~string();
		iterator begin(){ return start_; }
		const_iterator begin() const{ return start_; }
		iterator end(){ return finish_; }
		const_iterator end() const{ return finish_; }
		size_t size() const{ return finish_ - start_; }
		size_t length() const{ return size(); }
		size_t capacity() const{ return endOfStorage_ - start_; }
		void clear(){
			dataAllocator::destroy(start_, finish_);
			start_ = finish_;
		}
		bool empty() const{ return begin() == end(); }
		void resize(size_t n);
		void resize(size_t n, char c);
		void reserve(size_t n = 0);
		char& operator[] (size_t pos){ return *(start_ + pos); }
		const char& operator[] (size_t pos) const{ return *(start_ + pos); }
		char& back(){ return *(finish_ - 1); }
		const char& back() const{ return *(finish_ - 1); }
		char& front(){ return *(start_); }
		const char& front() const{ return *(start_); }
		void push_back(char c){ insert(end(), c); }
		string& insert(size_t pos, const string& str);
		string& insert(size_t pos, const char* s);
		string& insert(size_t pos, const char* s, size_t n);
		template <class InputIterator>
		iterator insert(iterator p, InputIterator first, InputIterator last);
		string& operator+= (const string& str);
		string& operator+= (const char* s);
		string& operator+= (char c);
		void pop_back(){ erase(end() - 1, end()); }
		string& erase(size_t pos = 0, size_t len = npos);
		iterator erase(iterator p);
		iterator erase(iterator first, iterator last);
		size_t copy(char* s, size_t len, size_t pos = 0) const{
			auto ptr = SmallSTL::uninitialized_copy(begin() + pos, begin() + pos + len, s);
			return (size_t)(ptr - s);
		}
		size_t find(const string& str, size_t pos = 0) const;
		size_t find(const char* s, size_t pos = 0) const;
		size_t find(const char* s, size_t pos, size_t n) const;
		size_t find(char c, size_t pos = 0) const;
		void moveData(string& str);
		size_t changeVarWhenEuqalNPOS(size_t var, size_t minuend, size_t minue)const;
		//插入时空间不足的情况
		template<class InputIterator>
		iterator insert_aux_copy(iterator p, InputIterator first, InputIterator last);
		//插入时空间不足的情况
		iterator insert_aux_filln(iterator p, size_t n, value_type c);
		void allocateAndFillN(size_t n, char c);
		template<class InputIterator>
		void allocateAndCopy(InputIterator first, InputIterator last);
		void string_aux(size_t n, char c, std::true_type);
		template<class InputIterator>
		void string_aux(InputIterator first, InputIterator last, std::false_type);
		void destroyAndDeallocate();
		friend std::ostream& operator <<(std::ostream& os, const string&str);
		friend std::istream& operator >> (std::istream& is, string& str);
		friend string operator+ (const string& lhs, const string& rhs);
		friend string operator+ (const string& lhs, const char* rhs);
		friend string operator+ (const char* lhs, const string& rhs);
		friend string operator+ (const string& lhs, char rhs);
		friend string operator+ (char lhs, const string& rhs);
		friend bool operator== (const string& lhs, const string& rhs);
		friend bool operator== (const char*   lhs, const string& rhs);
		friend bool operator== (const string& lhs, const char*   rhs);
		friend bool operator!= (const string& lhs, const string& rhs);
		friend bool operator!= (const char*   lhs, const string& rhs);
		friend bool operator!= (const string& lhs, const char*   rhs);
		friend bool operator<  (const string& lhs, const string& rhs);
		friend bool operator<  (const char*   lhs, const string& rhs);
		friend bool operator<  (const string& lhs, const char*   rhs);
		friend bool operator<= (const string& lhs, const string& rhs);
		friend bool operator<= (const char*   lhs, const string& rhs);
		friend bool operator<= (const string& lhs, const char*   rhs);
		friend bool operator>  (const string& lhs, const string& rhs);
		friend bool operator>  (const char*   lhs, const string& rhs);
		friend bool operator>  (const string& lhs, const char*   rhs);
		friend bool operator>= (const string& lhs, const string& rhs);
		friend bool operator>= (const char*   lhs, const string& rhs);
		friend bool operator>= (const string& lhs, const char*   rhs);
		size_type getNewCapacity(size_type len)const;
	};
	template<class InputIterator>
	string::string(InputIterator first, InputIterator last){
		//处理指针和数字间的区别的函数
		string_aux(first, last, typename std::is_integral<InputIterator>::type());
	}
	template <class InputIterator>
	string::iterator string::insert_aux_copy(iterator p, InputIterator first, InputIterator last){
		size_t lengthOfInsert = last - first;
		auto newCapacity = getNewCapacity(lengthOfInsert);
		iterator newStart = dataAllocator::allocate(newCapacity);
		iterator newFinish = TinySTL::uninitialized_copy(start_, p, newStart);
		newFinish = TinySTL::uninitialized_copy(first, last, newFinish);
		auto res = newFinish;
		newFinish = TinySTL::uninitialized_copy(p, finish_, newFinish);

		destroyAndDeallocate();
		start_ = newStart;
		finish_ = newFinish;
		endOfStorage_ = start_ + newCapacity;
		return res;
	}
	template <class InputIterator>
	string::iterator string::insert(iterator p, InputIterator first, InputIterator last){
		auto lengthOfLeft = capacity() - size();
		size_t lengthOfInsert = distance(first, last);
		if (lengthOfInsert <= lengthOfLeft){
			for (iterator it = finish_ - 1; it >= p; --it){
				*(it + lengthOfInsert) = *(it);
			}
			SmallSTL::uninitialized_copy(first, last, p);
			finish_ += lengthOfInsert;
			return (p + lengthOfInsert);
		}
		else{
			return insert_aux_copy(p, first, last);
		}
	}
	template<class InputIterator>
	void string::allocateAndCopy(InputIterator first, InputIterator last){
		start_ = dataAllocator::allocate(last - first);
		finish_ = SmallSTL::uninitialized_copy(first, last, start_);
		endOfStorage_ = finish_;
	}
	template<class InputIterator>
	void string::string_aux(InputIterator first, InputIterator last, std::false_type){
		allocateAndCopy(first, last);
	}
}
#endif 
