#include "ringbuffer.h"

#include "string_utils.h"

/**
 * @brief 获取ringbuffer空间状态
 * 
 * @param rb 
 * @return enum ringbuffer_status 
 */
enum ringbuffer_status ringbuffer_status_get(struct ringbuffer *rb)
{
    if(rb->read_index == rb->write_index)
    {
        // 写位置 = 读位置且缓冲区状态标志一致时为 “空”
        if (rb->read_mirror == rb->write_mirror)
            return RINGBUFFER_EMPTY;
        else
            return RINGBUFFER_FULL;
    }
    return RINGBUFFER_NOT_FULL;
}

/**
 * @brief 获取环形缓冲区的数据长度
 * 
 * @param rb 
 * @return unsigned int 
 */
unsigned int ringbuffer_data_len(struct ringbuffer *rb)
{
    switch(ringbuffer_status_get(rb))
    {
        case RINGBUFFER_EMPTY:
            return 0;
        case RINGBUFFER_FULL:   // 只关心读写镜像位的相对关系（相同或不同）
                                // 持续强制写入的情况下，状态会在"满"和"非满"之间动态变化
            return rb->buffer_size;
        case RINGBUFFER_NOT_FULL:
        default:
        {
            unsigned int wi = rb->write_index, ri = rb->read_index;
            if(wi > ri)
                return wi - ri;
            else
                return rb->buffer_size - (ri - wi);
        }
    }
}


/**
 * @brief 初始化环形缓冲区
 * @param rb 环形缓冲区结构体指针
 * @param buffer 缓冲区指针
 * @param size 缓冲区大小
 * @return 是否成功
 */
unsigned char ringbuffer_init(struct ringbuffer *rb, unsigned char *buffer, int size)
{

    rb->read_mirror = rb->read_index = 0;
    rb->write_mirror = rb->write_index = 0;

    rb->buffer = buffer;        // 缓冲区初始化
    rb->buffer_size = size;     // 缓冲区大小初始化

}

/**
 * @brief 向环形缓冲区写入数据，如果写入大小超过缓冲区剩余空间，则丢弃超出部分数据
 * @param rb 环形缓冲区结构体指针
 * @param data 要写入的数据指针
 * @param size 希望写入的数据大小
 * @return 实际写入的数据大小
 */
unsigned int ringbuffer_write(struct ringbuffer *rb, char *data, int size)
{

    int len = 0;

    len = ringbuffer_get_space_len(rb);
    
    /* 没有空间 */
    if(len == 0)
    {
        return 0;
    }
    /* 空间不足，进行裁剪。裁剪后保证最后写的位置和读的位置重合*/
    if(size > len)
    {
        size = len;
    }
    
    // 如果总大小 - 写位置 > size 则表示右侧剩余空间可以写入
    if(rb->buffer_size - rb->write_index > size)
    {
        mymemcpy(&rb->buffer[rb->write_index], data, size);
        rb->write_index += size;
        return size;
    }


    // 运行到这里表示右侧空间不足以写入，需要回到左侧进行写入
    mymemcpy(&rb->buffer[rb->write_index], 
            data, 
            rb->buffer_size - rb->write_index);
    mymemcpy(&rb->buffer[0], 
            &data[rb->buffer_size - rb->write_index], 
            size - (rb->buffer_size - rb->write_index));    //左侧写入空间大小 = 总大小 - 右侧写入空间大小
    
    // 写超过了缓冲区大小，返回开头后进行镜像翻转一次
    rb->write_mirror = ~rb->write_mirror;   
    rb->write_index = size - (rb->buffer_size - rb->write_index);

    // 这种情况不会出现超过的部分，因为被限制了

    return size;
}

/**
 * @brief 强制写入环形缓冲区，若出现数据写入量超过缓冲区大小的情况，覆盖写入
 * 
 * @param rb 
 * @param data 
 * @param size 
 * @return unsigned int 
 */
unsigned int ringbuffer_write_force(struct ringbuffer *rb, char *data, int size)
{ 
    unsigned int space_len = 0;

    //获取剩余可用空间的长度
    space_len = ringbuffer_get_space_len(rb);

    //如果数据长度大于缓冲区大小，将待写入数组的前面的一部分截断（这部分最大为缓冲区大小）
    if(size > rb->buffer_size)
    {
        data = &data[size - rb->buffer_size];
        size = rb->buffer_size;
    }

    // 如果总大小 - 写位置 > size 则表示右侧剩余空间可以写入
    if(rb->buffer_size - rb->write_index > size)
    {
        mymemcpy(&rb->buffer[rb->write_index], data, size);
        rb->write_index += size;

        if(size > space_len)
            rb->read_index = rb->write_index;

        return size;
    }

    // 运行到这里表示剩余空间不足，需要将数据写入到左侧
    mymemcpy(&rb->buffer[rb->write_index], 
            &data[0], 
            rb->buffer_size - rb->write_index);
    mymemcpy(&rb->buffer[0], 
            &data[rb->buffer_size - rb->write_index], 
            size - (rb->buffer_size - rb->write_index));

    // 右侧空间不足就需要更新一次状态标识写满了一次
    rb->write_mirror = ~rb->write_mirror;
    rb->write_index = size - (rb->buffer_size - rb->write_index);

    // 如果写入的大小比剩余的空间大，需要更新读指针为写指针位置，也就丢弃了部分数据
    if(size > space_len)
    {
        if(rb->write_index <= rb->read_index)       // 套圈后才满足条件翻转
            rb->read_mirror = ~rb->read_mirror;     // 读镜像翻转一次--同步写镜像翻转过程
        rb->read_index = rb->write_index;
    }

    return size;
}


/**
 * @brief 按字节写入buffer，写满则写位置归0，写满后再写入会失败
 * 
 * @param rb 
 * @param data 
 * @return unsigned int 
 */
unsigned char ringbuffer_write_char(struct ringbuffer *rb, char data)
{
    // 剩余空间长度不足则写入失败
    if(!ringbuffer_get_space_len(rb))
        return 0;

    rb->buffer[rb->write_index] = data;
    
    if(rb->write_index == rb->buffer_size - 1)
    {
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = 0;
    }
    else
    {
        rb->write_index++;
    }

    return 1;       // 成功
}


unsigned char ringbuffer_write_char_force(struct ringbuffer *rb, char data)
{
    enum ringbuffer_status old_status;

    old_status = ringbuffer_status_get(rb);

    rb->buffer[rb->write_index] = data;

    if(rb->write_index == rb->buffer_size - 1)
    {
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = 0;
        if(old_status == RINGBUFFER_FULL)
        {
            rb->read_mirror = ~rb->read_mirror;
            rb->read_index = rb->write_index;
        }
    }else
    {
        rb->write_index++;
        if(old_status == RINGBUFFER_FULL)
            rb->read_index = rb->write_index;
    }

    return 1;
}



/**
 * @brief 读取环形缓冲区指定长度数据，若读取数据超过剩余数据大小，限制读取
 * 
 * @param rb 
 * @param data 
 * @param size 
 * @return unsigned int 
 */
unsigned int ringbuffer_read(struct ringbuffer *rb, char *data, int size)
{
    unsigned int data_len = 0;

    data_len = ringbuffer_data_len(rb);

    if(data_len == 0)
        return 0;

    if(data_len < size)
        size = data_len;

    if(rb->buffer_size - rb->read_index > size)
    {
        mymemcpy(data, &rb->buffer[rb->read_index], size);
        rb->read_index += size;

        return size;
    }

    mymemcpy(&data[0], 
            &rb->buffer[rb->read_index], 
            rb->buffer_size - rb->read_index);
    mymemcpy(&data[rb->buffer_size - rb->read_index],
            &rb->buffer[0],
            size - (rb->buffer_size - rb->read_index));     

    // 当读大小超过了缓冲区剩余空间，即读位置超过写位置，则限制读位置并且翻转一次缓冲区状态
    rb->read_mirror = ~rb->read_mirror;
    rb->read_index = size - (rb->buffer_size - rb->read_index);

    return size;
}



unsigned char ringbuffer_readchar(struct ringbuffer *rb, char *data)
{
    
    if(!ringbuffer_data_len(rb))
        return 0;

    *data = rb->buffer[rb->read_index];
    
    if(rb->read_index == rb->buffer_size - 1)
    {
        rb->read_mirror = ~rb->read_mirror;
        rb->read_index = 0;
    }
    else
    {
        rb->read_index++;    
    }
    
    return 1;
}

/**
 * @brief 清空ringbuffer配置
 * 
 * @param rb 
 */
void ringbuffer_clear(struct ringbuffer *rb)
{
    rb->read_index = 0;
    rb->write_index = 0;
    rb->read_mirror = 0;
    rb->write_mirror = 0;
}







