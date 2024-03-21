#include <includes.h>

#define TASK_STK_SIZE    128
#define TASK_PRIO_GAME   6
#define TASK_PRIO_INPUT  7
#define TASK_PRIO_PRINT_TBL  5// ������������ȼ�



OS_STK TaskStk_Game[TASK_STK_SIZE];
OS_STK TaskStk_Input[TASK_STK_SIZE];
OS_STK TaskStk_PrintTable[TASK_STK_SIZE]; // ��������Ķ�ջ



INT8U secret_number = 10;
INT8U guess;
INT8U countdown = 0;
INT8U i;
INT8U Pri_printed = 1; // ���ڱ���Ƿ��Ѿ�������������ȼ���
INT8U Fin_printed = 1; // ���ڱ���Ƿ��Ѿ���������������
INT8U result;// ���ڼ�������Ƿ�Ϊ���ֵı���
char c = '1';

static void Task_Game(void *p_arg);
static void Task_Input(void *p_arg);
static void Task_PrintTable(void *p_arg); // ����������



// �ֶ�ʵ�ֵĺ���������������ȼ��ж�����Ϣ
void OutputPriorityTable(void) {
    INT8U i;
    OS_TCB *ptcb;
    char *task_name;

    printf("���ȼ��ж�����Ϣ��\n");
    for (i = 0; i < OS_LOWEST_PRIO + 1; i++) {
        printf("���ȼ� %d �ϵ�����\n", i);
        ptcb = OSTCBPrioTbl[i];
        while (ptcb != (OS_TCB *)0) {
            task_name = ptcb->OSTCBTaskName; // ��ȡ��������ָ��
            if (task_name != NULL) {
                printf("  ������: %s\n", task_name); // �����������
            } else {
                printf("  ������: (No Name)\n"); // �����������Ϊ�գ������ (No Name)
            }
            ptcb = ptcb->OSTCBNext;
        }
    }
}

// �ֶ���д���������ĺ���
void OutputReadyTaskTable(void) {

    OS_TCB *ptcb;

    printf("�����������Ϣ��\n");
    for (i = 0; i < OS_LOWEST_PRIO + 1; i++) {
        printf("���ȼ� %d �ϵ�����\n", i);
        ptcb = OSTCBPrioTbl[i];
        while (ptcb != (OS_TCB *)0) {
            printf("  ����ID: %d\n", ptcb->OSTCBPrio); // ����������ȼ�
            printf("  ����״̬: %d\n", ptcb->OSTCBStat); // �������״̬
            // �������������Ϣ�����ջ��Ϣ��
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
	
	strcpy(OSTCBPrioTbl[TASK_PRIO_GAME]->OSTCBTaskName, "GameTask");// �����񴴽����ֶ�������������

    
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


    
    // ������ȼ��ж���
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
            printf("����ʱ���ѣ��Ѿ���ȥ %d ����\n", countdown*10);
        }
        
        
    }
}


static void Task_Input(void *p_arg) {
    p_arg = p_arg;

    while (1) {
        //OutputPriorityTable();
        printf("��������Ĳ²⣨1-100����");

        // ���Զ�ȡ���������
        result = scanf("%hhu", &guess);

        // ����ɹ���ȡ��һ�����֣��������һ������������ʾ��������
        if (result == 1) {
            if (guess == secret_number) {
                printf("�ߺ����¶���\n");
                break;
            } else if (guess < secret_number) {
                printf("��С����\n");
            } else {
                printf("�´�����\n");
            }
        } else {
            // ������뻺��������ֹ����ѭ��
            while (getchar() != '\n');
            printf("�������������һ����Ч������\n");
        }
    }
}



static void Task_PrintTable(void *p_arg) {
    p_arg = p_arg;

    while(1)
	{OutputPriorityTable(); // ��ӡ�������ȼ���
	OutputReadyTaskTable(); 
       
        
	OSTimeDlyHMSM(0, 0, 15, 0);}
}

