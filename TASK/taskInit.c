#include "taskAPI.h"

void systemInit(void)
{
    rt_mailbox_t gbgTrMb = threadInit_GarbageThrow();
    threadInit_CmdAnalysis(gbgTrMb);
}

INIT_APP_EXPORT(systemInit);    //开机自动初始化
