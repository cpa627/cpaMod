#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__


#define MIN(a, b) ((a) < (b) ? (a) : (b))

#ifndef NULL
#define NULL ((void *)0)
#endif



/**
 * @brief ��д����Ĭ��ֻ����������ĩβ�ص���ʼʱ����һ�η�ת
 *        ǿ��д�����ʱ�����д��Ĵ�С����ʣ�����ݿռ�Ĵ�С����ǿ�ƾ���һ��
 * 
 */
struct ringbuffer
{
    unsigned char *buffer;            // ������ָ��

    unsigned int read_mirror : 1;     // ������     
    unsigned int read_index : 31;     // ������
    unsigned int write_mirror : 1;    // д����
    unsigned int write_index : 31;    // д����

    unsigned int buffer_size;         // ��������С
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

