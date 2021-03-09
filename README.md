# garbageClassify

这是一个智能垃圾分类垃圾桶的底层驱动控制嵌入式工程

## 硬件说明

### UART通信

通信用途 | 串口实例 | RX引脚 | TX引脚 | 说明
:---: | :--- | :--- | :--- | ---:
FinSH控制台 | USART3 | D9 | D8 | 见`./BSP/uartFinshCharIO.c`
上位机通信接口 | USART1 | A10 | A9 | 见`./BSP/uart4Host.c`

## 软件说明

### 上位机通信

本项目采用的通信帧结构为：

- 上位机命令帧（方向：上位机->本地）
     序号 | 变量名 | 数据类型 | 备注 |
     :---: | --- | --- | ---
     1 | head | uint8_t | 数据帧头，固定为0xA9：1010 1001
     2 | command | unsigned char | 命令
     3 | arg_1 | uint16_t | 参数1，长度16bit
     4 | arg_2 | uint16_t | 参数2，长度16bit
     5 | tail | uint8_t | 数据帧尾，固定为0xA6：1010 0110

    帧结构实现为：

    ```C
    typedef struct{
    uint8_t head; // 帧头（固定为0xA9：1010 1001）
    unsigned char command;   // 命令符号
    uint16_t arg_1;     // 参数1
    uint16_t arg_2;     // 参数2
    uint8_t tail; // 帧尾（固定为0xA6：1010 0110）
    }hostCommand;
     ```

#### 上位机命令

本项目支持的上位机命令有：

命令 | ASCII码 | 参数1 | 参数2 | 备注
:---: | :--- | ---: | ---: | :---:
当前垃圾投入类型 | 'g' | 垃圾类码 | 保留
