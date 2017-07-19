#ifndef _ALLOC_H_
#define _ALLOC_H_
#include <cstdlib>
namespace SmallSTL{
	class alloc{
		private:
			enum {__ALIGN = 8};     // 小型区块的上调边界
            enum {__MAX_BYTES = 128};   // 小型区块的上限
            enum {__NFREELISTS = __MAX_BYTES/__ALIGN};      // free-lists 个数
            enum { NOBJS = 20};//每次增加的节点数
            //free-lists的结点构造 
            union obj{
            	union obj* free_list_link;
            	char client[1];
			};
			//ROUND_UP将bytes的值上调8的倍数
			static size_t ROUND_UP(size_t bytes){
				return (bytes+__ALIGN-1)&~(__ALIGN-1);
			} 
			static obj *free_list[__NFREELISTS];
			static char *start_free;//内存池的起始位置
			static char *end_free//内存池结束的位置
			static size_t heap_size;
			//根据区块的大小，返回使用第几号free-lists 
			static size_t FREELIST_INDEX(size_t bytes){
				return (bytes+__ALIGN-1)/(__ALIGN-1);
			} 
			//返回一个大小为n的对象，并可能加入大小为n的free-lists 
			static void *refill(size_t n);
			//配置一块大空间，可容纳nobjs个大小为size的区块
			//如果配置nobjs个区块有所不便,nobjs可能会降低 
			static char *chunk_alloc(size_t size,sizt_t &nobjs);
		public:
			static void *allocate(size_t bytes);
			static void deallocate(void *ptr,size_t bytes);
			static void reallocate(void *ptr,size_t old_sz,size_t new_sz);
	};
} 
#endif
