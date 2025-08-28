#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__


#define MIN(a, b) ((a) < (b) ? (a) : (b))

#ifndef NULL
#define NULL ((void *)0)
#endif



/**
 * @brief 读写镜像默认只会在索引从末尾回到开始时镜像一次翻转
 *        强制写入操作时，如果写入的大小大于剩余数据空间的大小，则强制镜像一次
 * 
 */
struct ringbuffer
{
    unsigned char *buffer;            // 缓冲区指针

    unsigned int read_mirror : 1;     // 读镜像     
    unsigned int read_index : 31;     // 读索引
    unsigned int write_mirror : 1;    // 写镜像
    unsigned int write_index : 31;    // 写索引

    unsigned int buffer_size;         // 缓冲区大小
};

enum ringbuffer_status
{
    RINGBUFFER_EMPTY = 0,
    RINGBUFFER_FULL,
    RINGBUFFER_NOT_FULL,
};


enum ringbuffer_status ringbuffer_status_get(struct ringbuffer *rb);
unsigned int ringbuffer_data_len(struct ringbuffer *rb);

unsigned char ringbuffer_init(struct ringbuffer *rb, unsigned char *buffer, int size);

unsigned int ringbuffer_write(struct ringbuffer *rb, char *data, int size);
unsigned int ringbuffer_write_force(struct ringbuffer *rb, char *data, int size);
unsigned char ringbuffer_write_char(struct ringbuffer *rb, char data);
unsigned char ringbuffer_write_char_force(struct ringbuffer *rb, char data);

unsigned int ringbuffer_read(struct ringbuffer *rb, char *data, int size);
unsigned char ringbuffer_readchar(struct ringbuffer *rb, char *data);

void ringbuffer_clear(struct ringbuffer *rb);


#define ringbuffer_get_space_len(rb) ((rb)->buffer_size - ringbuffer_data_len(rb))

#endif

