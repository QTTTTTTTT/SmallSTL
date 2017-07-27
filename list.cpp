#include "list.h"
namespace SmallSTL{
	template<class T>
	listIterator<T>& listIterator<T>::operator++(){
		p=p->next;
		return *this;
	}
	template<class T>
	listIterator<T> listIterator<T>::operator++(int){
		auto temp=*this;
		++*this;
		return temp;
	}
	template<class T>
	listIterator<T>& listIterator<T>::operator--(){
		p=p->pre;
		return *this;
	}
	template<class T>
	listIterator<T> listIterator<T>::operator--(int){
		auto temp=*this;
		--*this;
		return temp;
	}
	template<class T>
	bool operator==(const listIterator<T>& lhs,const listIterator<T> &rhs){
		return lhs.p==rhs.p;
	}
	template<class T>
    bool operator!=(const listIterator<T>& lhs,const listIterator<T> &rhs){
        return lhs.p!=rhs.p;
    }
	template<class T>
	void list<T>::insert_aux(iterator position,InputIterator first,InputIterator last,std::false_type){
		for(--last;first!=last;--last){
			position=insert(position,*last);
		}
		insert(position,*last);
	}
	template<class T>
	void list<T>::insert_aux(iterator position,size_type n,const T& val,std::true_type){
		for(auto i=n;i!=0;--i){
			position=insert(position,val);
		}
	}
	template<class T>
	typename list<T>::nodeptr list<T>::newNode(const T &val=T()){
		nodeptr res=nodeAllocator::allocate();
		nodeAllocator::construct(res,node<T>(val,nullptr,nullptr,this));
		return res;
	}
	template<class T>
	void list<T>::deleteNode(nodeptr p){
		p->pre=p->next=nullptr;
		nodeAllocator::destroy(p);
		nodeAllocator::deallocate(p);
	}
	template<class T>
	void list<T>::ctorAux(size_type n,const value_type& val,std::true_type){
		head.p=newNode();
		tail.p=head.p;
		while(n--)
			push_back(val);
	}
	template<class T>
	template<class InputIterator>
	void list<T>::ctorAux(InputIterator first,InputIterator last,std::false_type){
		head.p=newNode();
		tail.p=head.p;
		for(;first!=last;++first)
			push_back(*first);
	}
	template<class T>
	typename list<T>::size_type list<T>::size()const{
		size_type length=0;
		for(auto h=head;h!=tail;++h){
			++length;
		}
		return length;
	}
	template<class T>
	list<T>::list(){
		head.p=newNode();
		tail.p=head.p;
	}
	template<class T>
	list<T>::list(size_type n,const value_type& val=value_type()){
		ctorAux(n,val,std::is_integral<value_type>());
	}
	template<class T>
	template<class InputIterator>
	list<T>::list(InputIterator first,InputIterator last){
		ctorAux(first,last,std::is_integral<InputIterator>());
	}
	template<class T>
	list<T>::list(const list &l){
		head.p=newNode();
		tail.p=head.p;
		for(auto node=l.head.p;node!=l.tail.p;node=node->next){
			push_back(node->data);
		}
	}
	template<class T>
	void list<T>::push_back(const value_type& val){
		auto node=newNode();
		tail.p->data=val;
		tail.p->next=node;
		node->pre=tail.p;
		tail.p=node;
	}
	template<class T>
	void list<T>::pop_back(){
		auto newTail=tail.p->pre;
		newTail->next=nullptr;
		tail.p=newTail;
		nodeAllocator::destroy(tail.p);
	}
	template<class T>
	void list<T>::push_front(const value_type& val){
		auto node =newNode();
		head.p->pre=node;
		node->next=head.p;
		head.p=node;
	}
	template<class T>
	void list<T>::pop_front(){
		auto oldNode=head.p;
		head.p=oldNode->next;
		head.p->pre=nullptr;
		deleteNode(oldNode);
	}
	template<class T>
	list<T>::~list(){
		for(;head!=tail;){
			auto temp=head++;
			nodeAllocator::destroy(temp.p);
			nodeAllocator::deallocate(temp.p);
		}
		nodeallocator::deallocate(tail.p)
	}
	template<class T>
	typename list<T>::iterator list<T>::insert(iterator position,const value_type &val){
		if(position==begin()){
			push_front(val);
			return begin();
		}
		else if(position==end()){
			auto temp=position;
			push_back(val);
			return temp;
		}
		auto node=newNode(val);
		auto pre=position.p->pre;
		node->next=position;
		node->pre=pre;
		position.p->pre=node;
		pre->next=node;
		return iterator(node);
	}
	template<class T>
	void list<T>::insert(iterator position,size_type n,const value_type& val){
		insert_aux(position,n,val,typename std::is_integral<InputIterator>::type());
	}
	template<class T>
	template<class InputIterator>
	void list<T>::insert(iterator position,InputIterator first,InputIterator last){
		insert_aux(position,first,last,typename std::is_integreal<InputIterator>::type());
	}
	template<class T>
	typename list<T>::iterator list<T>::erase(iterator position){
		if(position==head){
			pop_front();
			return head;
		}
		else{
			auto pre=position.p->pre;
			pre->next=postion.p->next;
			posiont.p->next->pre=pre;
			deleteNode(position.p);
			return iterator(pre->next);
		}
	}
	template<class T>
	typename list<T>::iterator list<T>::erase(iterator first,iterator last){
		typename list<T>::iterator temp;
		for(;first!=last;){
			auto t=first++;
			temp=erase(t);
		}
		return temp;
	}
	template<class T>
	void list<T>::clear(){
		erase(begin(), end());
	}
	template<class T>
	typename list<T>::iterator list<T>::begin(){
		return head;
	}
	template<class T>
	typename list<T>::iterator list<T>::end(){
		return tail;
	}
	template<class T>
	void list<T>::reverse(){
		if(empty()||head.p->next==tail.p)return;
		auto cur=head.p;
		head.p=tail.p->pre;
		head.p->pre=nullptr;
		do{
			auto next=cur->next;
			cur->next=head.p->next;
			head.p->next->pre=cur;
			head.p->next=cur;
			cur->pre=head.p;
			cur=next;
		}while(cur!=head.p)
	}
	template<class T>
	void list<T>::remove(const value_type& val){
		for(auto it=begin();it!=end();){
			if(*it==val)
				it=erase(it);
			else ++it;
		}
	}
	template<class T>
	void list<T>::swap(list& x){
		SmallSTL::swap(head.p, x.head.p);
		SmallSTL::swap(tail.p, x.tail.p);
	}
	template<class T>
	void swap(list<T>& x, list<T>& y){
		x.swap(y);
	}
	template<class T>
	void list<T>::unique(){
		nodeptr cur=head.p;
		while(cur!=tail.p){
			nodeptr next=cur->next;
			if(cur->data==next->data){
				if(next==tail.p){
					cur->next=nullptr;
					tail.p=cur;
				}
				else{
					cur->next=next->next;
					next->next->pre=cur;
				}
				deleteNode(next);
			}
			else cur=next;
		}
	}
	template<class T>
	void list<T>::splice(iterator position, list& x){
		this->insert(position, x.begin(), x.end());
		x.head.p = x.tail.p;
	}
	template<class T>
	void list<T>::merge(list &x){
		auto it1=begin(),it2=x.begin();
		while(it1!=end()&&it2!=x.end()){
			if(*it1<=*it2)++it1;
			else{
				auto temp=it2++;
				this->splice(it1,temp);
			}
		}
		while(it2!=end()){
			this->insert(it2++);
		}
	}
	template<class T>
	bool operator==(const list<T>& lhs,const list<T>& rhs){
		auto node1=lhs.head.p,node2=rhs.head.p;
		for(;node1!=lhs.tail.p&&node2!=rhs.tail.p;node1 = node1->next, node2 = node2->next){
			if (node1->data != node2->data)
				break;
		}
		if (node1 == lhs.tail.p && node2 == rhs.tail.p)
			return true;
		return false;
	}
	template <class T>
	bool operator!= (const list<T>& lhs, const list<T>& rhs){
		return !(lhs == rhs);
	}
	
	void list<T>::sort(){
		sort(SmallSTL::less<T>());
	}
}
