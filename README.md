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

- 反馈帧（方向：本地->上位机）
     序号 | 变量名 | 数据类型 | 备注
     :---: | --- | --- | ---
     1 | head | uint8_t | 数据帧头，固定为0xA9：1010 1001
     2 | status | unsigned char | 状态码
     3 | data | uint8_t [5] | 数据区，长度8bits x5
     5 | tail | uint8_t | 数据帧尾，固定为0xA6：1010 0110

    帧结构实现为：

    ```C
    typedef struct{
          uint8_t head; // 帧头（固定为0xA9：1010 1001）
          uint8_t status;     // 状态码
          uint8_t data[5];    // 数据
          uint8_t tail; // 帧尾（固定为0xA6：1010 0110）
     }localMsg;
    ```

#### 上位机命令

本项目支持的上位机命令有：

命令 | ASCII码 | 参数1 | 参数2 | 备注
:---: | :--- | ---: | ---: | :---:
当前垃圾投入类型 | 'g' | 垃圾类码 | 保留
反馈满载信息 | 'f' | 保留 | 保留 | 触发下位机发送数据

---

传送给上位机的数据有：

状态类型 | ASCII码 | 参数1 | 参数2 | 参数3 | 参数4 | 参数5 | 备注
:---: | :--- | ---: | ---: | ---: | ---: | ---: | :---:
满载检测 | 'f' | 垃圾桶1 | 桶2 | 桶3 | 桶4 | 保留 | 传输的每个值都是逻辑量（`true`&`false`）
位置检测 | 'p' |
