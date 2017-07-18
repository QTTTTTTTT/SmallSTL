#include "alloc.h"
namspace SmallSTL{
	char *alloc::start_free=0;
	char *alloc::end_free=0;
	size_t alloc::heap_size=0;
	alloc::obj alloc::free_list[alloc::NFREELISTS]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
	void *alloc::allocate(size_t bytes){
		if(bytes>=alloc::MAXBYTES)
			return malloc(bytes);
		obj *list=free_list+allc::FREELIST_INDEX(bytes);
		obj *result=list;
		if(list){
			list=result->free_list_link;
			return result;
		}
		else{
			alloc::refill(alloc::ROUAND_UP(bytes));
		}
	}
	void alloc::deallocate(void *ptr,size_t bytes){
		if(bytes>=alloc::MAXBYTES){
			free(ptr);
		}
		else{
			obj *list=free_list+alloc::FREELIST_INDEX(bytes);
			obj *p=(obj *)ptr;
			p->free_list_link=list;
			list=p;
		}
	}
	void *alloc::reallocate(void *ptr,size_t old_sz,size_t new_sz){
		alloc::deallocate(ptr,old_sz);
		ptr=alloc::allocate(new_sz);
		return ptr;
	}
	void *alloc::refill(size_t n){
		size_t nobjs=alloc::NOBJS;
		char *chunk=chunk_alloc(n,nobjs);
		obj **list,*result,*current,*next_obj;
		int i;
		if(1==nobjs){
			return chunk;
		}
		*list=free_list+alloc::FREELIST_INDEX(n);
		result=(obj *)chunk;
		*list=next_obj=(obj *)(chunk+n);
		for(i=1;;i++){
			current=next_obj;
			next_obj=(obj *)((char *)next_obj+n);
			if(nobjs-1==i){
				current->free_list_link=0;
				break;
			}
			else{
				current->free_list_link=next_obj;
			}
		}
		return result;
	}
	//假设bytes已调整至8的倍数 
	void *alloc::chunk_alloc(size_t bytes,size_t &nobjs){
		char *result=0;
		size_t total_bytes=bytes*nobjs;
		size_t bytes_left=end_free-start_free;
		if(total_bytes<=bytes_left){//完全满足需求 
			result=start_free;
			start_free+=total_bytes;
			return result;
		}
		else if(bytes_left>0){//内存池空间不能完全满足需求，但足够提供至少一个以上的区块 
			nobjs=bytes_left/bytes;
			total_bytes=nobjs*bytes;
			result=start_free;
			start_free+=total_bytes;
			return result;
		}
		else{//内存池无剩余空间 
			size_t byte_to_get=2*total_bytes+ROUND_UP(heap_size>>4);
			if(bytes_left>0){
				obj **my_list=free_list+alloc::FREELIST_INDEX(bytes_left);
				((obj *)start_free)->free_list_link=*my_list;
				*my_list=(obj *)start_free;
			}
			start_free=malloc(bytes_to_get);
			if(start_free==0){
				obj **my_list,*p;
				for(int i=bytes;i<=alloc::MAXBYTES;i+=alloc::ALIGN){
					my_list=free_list+FREELIST_INDEX(i);
					p=*my_list;
					if(p!=0){
						*my_list=p->next;
						start_free=(char *)p;
						end_free=start_free+i;
						return chunk_alloc(bytes,nobjs); 
					}
				}
				end_free=0;
			}
			heap_size+=bytes_to_get;
			end_free=start_free+bytes_to_get;
			return chunk_alloc(bytes,nobjs); 
		}
	}
}
