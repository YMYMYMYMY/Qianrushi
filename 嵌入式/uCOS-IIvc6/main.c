#include <includes.h>

#define TASK_STK_SIZE    128
#define TASK_PRIO_GAME   6
#define TASK_PRIO_INPUT  7
#define TASK_PRIO_PRINT_TBL  5// 新增任务的优先级



OS_STK TaskStk_Game[TASK_STK_SIZE];
OS_STK TaskStk_Input[TASK_STK_SIZE];
OS_STK TaskStk_PrintTable[TASK_STK_SIZE]; // 新增任务的堆栈



INT8U secret_number = 10;
INT8U guess;
INT8U countdown = 0;
INT8U i;
INT8U Pri_printed = 1; // 用于标记是否已经输出过任务优先级表
INT8U Fin_printed = 1; // 用于标记是否已经输出过任务就绪表
INT8U result;// 用于检测输入是否为数字的变量
char c = '1';

static void Task_Game(void *p_arg);
static void Task_Input(void *p_arg);
static void Task_PrintTable(void *p_arg); // 新增任务函数



// 手动实现的函数，用于输出优先级判定表信息
void OutputPriorityTable(void) {
    INT8U i;
    OS_TCB *ptcb;
    char *task_name;

    printf("优先级判定表信息：\n");
    for (i = 0; i < OS_LOWEST_PRIO + 1; i++) {
        printf("优先级 %d 上的任务：\n", i);
        ptcb = OSTCBPrioTbl[i];
        while (ptcb != (OS_TCB *)0) {
            task_name = ptcb->OSTCBTaskName; // 获取任务名称指针
            if (task_name != NULL) {
                printf("  任务名: %s\n", task_name); // 输出任务名称
            } else {
                printf("  任务名: (No Name)\n"); // 如果任务名称为空，则输出 (No Name)
            }
            ptcb = ptcb->OSTCBNext;
        }
    }
}

// 手动编写任务就绪表的函数
void OutputReadyTaskTable(void) {

    OS_TCB *ptcb;

    printf("任务就绪表信息：\n");
    for (i = 0; i < OS_LOWEST_PRIO + 1; i++) {
        printf("优先级 %d 上的任务：\n", i);
        ptcb = OSTCBPrioTbl[i];
        while (ptcb != (OS_TCB *)0) {
            printf("  任务ID: %d\n", ptcb->OSTCBPrio); // 输出任务优先级
            printf("  任务状态: %d\n", ptcb->OSTCBStat); // 输出任务状态
            // 输出其他任务信息，如堆栈信息等
            ptcb = ptcb->OSTCBNext;
        }
    }
}

void main(void) {
    OSInit();
    
    OSTaskCreateExt(Task_Game,
                    (void *)0,
                    (OS_STK *)&TaskStk_Game[TASK_STK_SIZE - 1],
                    TASK_PRIO_GAME,
                    TASK_PRIO_GAME,
                    (OS_STK *)&TaskStk_Game[0],
                    TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
	
	strcpy(OSTCBPrioTbl[TASK_PRIO_GAME]->OSTCBTaskName, "GameTask");// 在任务创建后手动设置任务名称

    
    OSTaskCreateExt(Task_Input,
                    (void *)0,
                    (OS_STK *)&TaskStk_Input[TASK_STK_SIZE - 1],
                    TASK_PRIO_INPUT,
                    TASK_PRIO_INPUT,
                    (OS_STK *)&TaskStk_Input[0],
                    TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
	strcpy(OSTCBPrioTbl[TASK_PRIO_INPUT]->OSTCBTaskName, "InputTask");


	OSTaskCreateExt(Task_PrintTable,
                    (void *)0,
                    (OS_STK *)&TaskStk_PrintTable[TASK_STK_SIZE - 1],
                    TASK_PRIO_PRINT_TBL,
                    TASK_PRIO_PRINT_TBL,
                    (OS_STK *)&TaskStk_PrintTable[0],
                    TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
	strcpy(OSTCBPrioTbl[TASK_PRIO_PRINT_TBL]->OSTCBTaskName, "PriTableTask");


    
    // 输出优先级判定表
    //OutputPriorityTable();
    
    OSStart();
}

static void Task_Game(void *p_arg) {
    p_arg = p_arg;

    srand((unsigned int)OSTimeGet());
    secret_number = rand() % 100 + 1;
    
    while (1) {
        countdown++;
        OSTimeDlyHMSM(0, 0, 10, 0);
        if (countdown % 1 == 0) {
            printf("倒计时提醒：已经过去 %d 秒了\n", countdown*10);
        }
        
        
    }
}


static void Task_Input(void *p_arg) {
    p_arg = p_arg;

    while (1) {
        //OutputPriorityTable();
        printf("请输入你的猜测（1-100）：");

        // 尝试读取输入的数字
        result = scanf("%hhu", &guess);

        // 如果成功读取了一个数字，则进行下一步处理，否则提示重新输入
        if (result == 1) {
            if (guess == secret_number) {
                printf("芜湖！猜对了\n");
                break;
            } else if (guess < secret_number) {
                printf("猜小了捏\n");
            } else {
                printf("猜大了捏\n");
            }
        } else {
            // 清空输入缓冲区，防止无限循环
            while (getchar() != '\n');
            printf("输入错误，请输入一个有效的数字\n");
        }
    }
}



static void Task_PrintTable(void *p_arg) {
    p_arg = p_arg;

    while(1)
	{OutputPriorityTable(); // 打印任务优先级表
	OutputReadyTaskTable(); 
       
        
	OSTimeDlyHMSM(0, 0, 15, 0);}
}

