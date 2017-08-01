#include "string.h"
#include <iostream>
namespace SmallSTL{
	const size_t string::npos;
	void string::moveData(string& str){
		start_ = str.start_;
		finish_ = str.finish_;
		endOfStorage_ = str.endOfStorage_;
		str.start_ = str.finish_ = str.endOfStorage_ = 0;
	}
	string::string(size_t n, char c){
		allocateAndFillN(n, c);
	}
	string::string(const char* s){
		allocateAndCopy(s, s + strlen(s));
	}
	string::string(const char* s, size_t n){
		allocateAndCopy(s, s + n);
	}
	string::string(const string& str){
		allocateAndCopy(str.start_, str.finish_);
	}
	string::string(string&& str){
		moveData(str);
	}
	string::string(const string& str, size_t pos, size_t len){
		len = changeVarWhenEuqalNPOS(len, str.size(), pos);
		allocateAndCopy(str.start_ + pos, str.start_ + pos + len);
	}
	size_t string::changeVarWhenEuqalNPOS(size_t var, size_t minuend, size_t minue)const{
		return (var == npos ? minuend - minue : var);
	}
	string::~string(){ 
		destroyAndDeallocate(); 
	}
	string& string::operator= (const string& str){
		if (this != &str){
			destroyAndDeallocate();
			allocateAndCopy(str.start_, str.finish_);
		}
		return *this;
	}
	string& string::operator= (string&& str){
		if (this != &str){
			moveData(str);
		}
		return *this;
	}
	string& string::operator= (const char* s){
		destroyAndDeallocate();
		allocateAndCopy(s, s + strlen(s));
		return *this;
	}
	string& string::operator= (char c){
		destroyAndDeallocate();
		allocateAndFillN(1, c);
		return *this;
	}
	void string::resize(size_t n){
		resize(n, value_type());
	}
	string::size_type string::getNewCapacity(size_type len)const{
		size_type oldCapacity = endOfStorage_ - start_;
		auto res = SmallSTL::max(oldCapacity, len);
		auto newCapacity = oldCapacity + res;
		return newCapacity;
	}
	void string::resize(size_t n, char c){
		if (n < size()){
			dataAllocator::destroy(start_ + n, finish_);
			finish_ = start_ + n;
		}
		else if (n > size() && n <= capacity()){
			auto lengthOfInsert = n - size();
			finish_ = SmallSTL::uninitialized_fill_n(finish_, lengthOfInsert, c);
		}
		else if (n > capacity()){
			auto lengthOfInsert = n - size();
			iterator newStart = dataAllocator::allocate(getNewCapacity(lengthOfInsert));
			iterator newFinish = SmallSTL::uninitialized_copy(begin(), end(), newStart);
			newFinish = SmallSTL::uninitialized_fill_n(newFinish, lengthOfInsert, c);
			destroyAndDeallocate();
			start_ = newStart;
			finish_ = newFinish;
			endOfStorage_ = start_ + n;
		}
	}
	string& string::insert(size_t pos, const string& str){
		insert(start_ + pos, str.begin(), str.end());
		return *this;
	}
	string& string::insert(size_t pos, const char* s){
		insert(begin() + pos, s, s + strlen(s));
		return *this;
	}
	string& string::insert(size_t pos, const char* s, size_t n){
		insert(begin() + pos, s, s + n);
		return *this;
	}
	string::iterator string::insert_aux_filln(iterator p, size_t n, value_type c){
		auto newCapacity = getNewCapacity(n);
		iterator newStart = dataAllocator::allocate(newCapacity);
		iterator newFinish = TinySTL::uninitialized_copy(start_, p, newStart);
		newFinish = TinySTL::uninitialized_fill_n(newFinish, n, c);
		auto res = newFinish;
		newFinish = TinySTL::uninitialized_copy(p, finish_, newFinish);

		destroyAndDeallocate();
		start_ = newStart;
		finish_ = newFinish;
		endOfStorage_ = start_ + newCapacity;
		return res;
	}
	string::iterator string::insert(iterator p, size_t n, char c){
		auto lengthOfLeft = capacity() - size();
		if (n <= lengthOfLeft){
			for (iterator it = finish_ - 1; it >= p; --it){
				*(it + n) = *(it);
			}
			TinySTL::uninitialized_fill_n(p, n, c);
			finish_ += n;
			return (p + n);
		}
		else{
			return insert_aux_filln(p, n, c);
		}
	}
	string::iterator string::insert(iterator p, char c){
		return insert(p, 1, c);
	}
	string& string::operator+= (const string& str){
		insert(size(), str);
		return *this;
	}
	string& string::operator+= (const char* s){
		insert(size(), s);
		return *this;
	}
	string& string::operator+= (char c){
		insert(end(), c);
		return *this;
	}
	string::iterator string::erase(iterator first, iterator last){
		size_t lengthOfMove = finish_ - last;
		for (auto i = 0; i != lengthOfMove; ++i){
			*(first + i) = *(last + i);
		}
		dataAllocator::destroy(first + lengthOfMove, finish_);
		finish_ = first + lengthOfMove;
		return first;

	}
	string::iterator string::erase(iterator p){
		return erase(p, p+1);
	}
	size_t string::find_aux(const_iterator cit, size_t pos, size_t lengthOfS, size_t cond)const{
		size_t i, j;
		for (i = pos; i != cond; ++i){
			for (j = 0; j != lengthOfS; ++j){
				if (*(begin() + i + j) != cit[j])
					break;
			}
			if (j == lengthOfS)
				return i;
		}
		return npos;
	}
	size_t string::find(const char* s, size_t pos, size_t n) const{
		size_t lenghtOfS = strlen(s);
		return return find_aux(s, pos, lenghtOfS, pos + n);
	}
	size_t string::find(const string& str, size_t pos) const{
		size_t lengthOfS = str.size();
		if (size() - pos < lengthOfS)
			return npos;
		return find_aux(str.cbegin(), pos, lengthOfS, size());
	}
	size_t string::find(const char* s, size_t pos) const{
		return find(s, pos, size() - pos);
	}
	size_t string::find(char c, size_t pos) const{
		for (auto cit = cbegin() + pos; cit != cend(); ++cit){
			if (*cit == c)
				return cit - cbegin();
		}
		return npos;
	}
	std::ostream& operator <<(std::ostream& os, const string&str){
		for (const auto ch : str){
			os << ch;
		}
		return os;
	}
	std::istream& operator >> (std::istream& is, string& str){
		char ch;
		string::size_type oldSize = str.size(), index = 0;
		bool hasPrevBlank = false;
		while (is.get(ch)){
			if (isblank(ch) || ch == '\n')
				hasPrevBlank = true;
			else
				break;
		}
		is.putback(ch);
		str.clear();
		while (is.get(ch)){
			if (ch != EOF && !isblank(ch) && ch != '\n'){
				str.push_back(ch);
			}
			else
				break;
		}
		return is;
	}
	string operator+ (const string& lhs, const string& rhs){
		string res(lhs);
		return res += rhs;
	}
	string operator+ (const string& lhs, const char* rhs){
		string res(lhs);
		return res += rhs;
	}
	string operator+ (const char* lhs, const string& rhs){
		string res(lhs);
		return res += rhs;
	}
	string operator+ (const string& lhs, char rhs){
		string res(lhs);
		return res += rhs;
	}
	string operator+ (char lhs, const string& rhs){
		string res(1, lhs);
		return res += rhs;
	}
	bool operator== (const string& lhs, const string& rhs){
		if (lhs.size() == rhs.size()){
			for (auto cit1 = lhs.cbegin(), cit2 = rhs.cbegin();
				cit1 != lhs.cend() && cit2 != rhs.cend();
				++cit1, ++cit2){
				if (*cit1 != *cit2)
					return false;
			}
			return true;
		}
		return false;
	}
	bool operator== (const char*   lhs, const string& rhs){
		return rhs == lhs;
	}
	bool operator== (const string& lhs, const char*   rhs){
		size_t len = strlen(rhs);
		if (lhs.size() == len){
			const char *p = rhs;
			for (string::const_iterator cit = lhs.cbegin();
				cit != lhs.cend() && p != rhs + len;
				++cit, ++p){
				if (*cit != *p)
					return false;
			}
			return true;
		}
		return false;
	}
	bool operator!= (const string& lhs, const string& rhs){
		return !(lhs == rhs);
	}
	bool operator!= (const char*   lhs, const string& rhs){
		return !(lhs == rhs);
	}
	bool operator!= (const string& lhs, const char*   rhs){
		return !(lhs == rhs);
	}
	bool operator<  (const string& lhs, const string& rhs){
		return !(lhs >= rhs);
	}
	bool operator<  (const char*   lhs, const string& rhs){
		return !(lhs >= rhs);
	}
	bool operator<  (const string& lhs, const char*   rhs){
		return !(lhs >= rhs);
	}
	template<class Iterator1, class Iterator2>
		bool lessEqual_aux(Iterator1 first1, Iterator1 last1, Iterator2 first2, Iterator2 last2){
			for (; first1 != last1 && first2 != last2; ++first1, ++first2){
				if (*first1 < *first2)
					return true;
				else if (*first1 > *first2)
					return false;
			}
			if ((first1 == last1 && first2 == last2)// ==
				|| (first1 == last1))// <
				return true;
			else
				return false;
		}
	bool operator<= (const string& lhs, const string& rhs){
		return lessEqual_aux(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
	}
	bool operator<= (const char*   lhs, const string& rhs){
		return lessEqual_aux(lhs, lhs + strlen(lhs), rhs.cbegin(), rhs.cend());
	}
	bool operator<= (const string& lhs, const char*   rhs){
		return lessEqual_aux(lhs.cbegin(), lhs.cend(), rhs, rhs + strlen(rhs));
	}
	bool operator>  (const string& lhs, const string& rhs){
		return !(lhs <= rhs);
	}
	bool operator>  (const char*   lhs, const string& rhs){
		return !(lhs <= rhs);
	}
	bool operator>  (const string& lhs, const char*   rhs){
		return !(lhs <= rhs);
	}
	template<class Iterator1, class Iterator2>
		bool greaterEqual_aux(Iterator1 first1, Iterator1 last1, Iterator2 first2, Iterator2 last2){
			for (; first1 != last1 && first2 != last2; ++first1, ++first2){
				if (*first1 > *first2)
					return true;
				else if (*first1 < *first2)
					return false;
			}
			if ((first1 == last1 && first2 == last2)// ==
				|| (first2 == last2))// >
				return true;
			else
				return false;
		}
	bool operator>= (const string& lhs, const string& rhs){
		return greaterEqual_aux(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
	}
	bool operator>= (const char*   lhs, const string& rhs){
		return greaterEqual_aux(lhs, lhs + strlen(lhs), rhs.cbegin(), rhs.cend());
	}
	bool operator>= (const string& lhs, const char*   rhs){
		return greaterEqual_aux(lhs.cbegin(), lhs.cend(), rhs, rhs + strlen(rhs));
	}
	void string::allocateAndFillN(size_t n, char c){
		start_ = dataAllocator::allocate(n);
		finish_ = SmallSTL::uninitialized_fill_n(start_, n, c);
		endOfStorage_ = finish_;
	}
	void string::string_aux(size_t n, char c, std::true_type){
		allocateAndFillN(n, c);
	}
	void string::destroyAndDeallocate(){
		dataAllocator::destroy(start_, finish_);
		dataAllocator::deallocate(start_, endOfStorage_ - start_);
	}
}
