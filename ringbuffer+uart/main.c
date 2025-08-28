#include "ringbuffer.h"
#include "string_utils.h"

#include <stdio.h>

// ringbuffer ʵ��
unsigned char rxbuffer[20] = {0};

// ���ջ���
unsigned char test_buffer[100] = {0};

struct ringbuffer ringbuffer_tsk = {0};

#define NORMAL_WRITE_TEST      0        // һ��Ǹ���д��
#define FORCE_WRITE_TEST       1


int main(void)
{

    // ��ʼ��
    ringbuffer_init(&ringbuffer_tsk, rxbuffer, sizeof(rxbuffer));

    #if NORMAL_WRITE_TEST
    // �Ǹ���д��������֤ -- ģ�⴮�ڽ������ݣ�д��rxbuffer
    ringbuffer_write(&ringbuffer_tsk, "1234567890", 10);
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);
    ringbuffer_write(&ringbuffer_tsk, "hello world", 11);
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);

    // ��ȡ������֤ -- ������test_buffer��
    ringbuffer_read(&ringbuffer_tsk, test_buffer, 20);
    printf("test_buffer: %s\n", test_buffer);

    // ��ȡrxbuffer������֤ -- Ŀǰ��û�и���д��������
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);

    // ׷��д������֤
    printf("append write bytes test:\r\n");
    ringbuffer_write(&ringbuffer_tsk, "aaaaabbbbbcccccddddd", 20);  // ǰ�����20bytes,
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);

    // ��ͨ׷��д�ֽھͻ�ʧ��
    printf("append write byte test:\r\n");
    ringbuffer_write_char(&ringbuffer_tsk, 'p');
    ringbuffer_write_char(&ringbuffer_tsk, '?');
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);

    ringbuffer_read(&ringbuffer_tsk, rxbuffer, 2);
    ringbuffer_write_char(&ringbuffer_tsk, 'p');
    ringbuffer_write_char(&ringbuffer_tsk, '?');
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);

    #endif

    #if FORCE_WRITE_TEST
    // ����д��������֤
    ringbuffer_write_force(&ringbuffer_tsk, "1234567890", 10);
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);
    ringbuffer_write_force(&ringbuffer_tsk, "hello world", 11);
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);

    // ��ȡ������֤
    ringbuffer_read(&ringbuffer_tsk, test_buffer, 20);
    printf("test_buffer: %s\n", test_buffer);

    // ��ȡʣ��������֤
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);

    // ׷��д������֤
    printf("append write bytes test:\r\n");
    ringbuffer_write_force(&ringbuffer_tsk, "aaaaabbbbbcccccdddddeeeee", 25);  // ǰ�����20bytes,
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);

    // ǿ��׷��д�ֽ�
    printf("append write byte test:\r\n");
    ringbuffer_write_char_force(&ringbuffer_tsk, 'p');
    ringbuffer_write_char_force(&ringbuffer_tsk, '?');
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);

    #endif
    

    while(1)
    {

    }

    return 0;
}
