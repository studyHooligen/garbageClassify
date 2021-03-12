#include "taskAPI.h"
#include "BSP.h"

int systemInit(void)
{
    
    steerEngine_Init();
    /* 垃圾投入线程初始化 */
    rt_mailbox_t gbgTrMb = threadInit_GarbageThrow();
    
    /* 上位机命令解析伺服线程 */
    threadInit_CmdAnalysis(gbgTrMb);
    
    /* 满载检测线程初始化 */
    threadInit_heightDetect();
    
    /* 初始化完成 */
    return 0;
}

INIT_APP_EXPORT(systemInit);    //开机自动初始化
