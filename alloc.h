#ifndef _ALLOC_H_
#define _ALLOC_H_
#include <cstdlib>
namespace SmallSTL{
	class alloc{
		private:
			enum {__ALIGN = 8};     // С��������ϵ��߽�
            enum {__MAX_BYTES = 128};   // С�����������
            enum {__NFREELISTS = __MAX_BYTES/__ALIGN};      // free-lists ����
            enum { NOBJS = 20};//ÿ�����ӵĽڵ���
            //free-lists�Ľ�㹹�� 
            union obj{
            	union obj* free_list_link;
            	char client[1];
			};
			//ROUND_UP��bytes��ֵ�ϵ�8�ı���
			static size_t ROUND_UP(size_t bytes){
				return (bytes+__ALIGN-1)&~(__ALIGN-1);
			} 
			static obj *free_list[__NFREELISTS];
			static char *start_free;//�ڴ�ص���ʼλ��
			static char *end_free//�ڴ�ؽ�����λ��
			static size_t heap_size;
			//��������Ĵ�С������ʹ�õڼ���free-lists 
			static size_t FREELIST_INDEX(size_t bytes){
				return (bytes+__ALIGN-1)/(__ALIGN-1);
			} 
			//����һ����СΪn�Ķ��󣬲����ܼ����СΪn��free-lists 
			static void *refill(size_t n);
			//����һ���ռ䣬������nobjs����СΪsize������
			//�������nobjs��������������,nobjs���ܻή�� 
			static char *chunk_alloc(size_t size,sizt_t &nobjs);
		public:
			static void *allocate(size_t bytes);
			static void deallocate(void *ptr,size_t bytes);
			static void reallocate(void *ptr,size_t old_sz,size_t new_sz);
	};
} 
#endif
