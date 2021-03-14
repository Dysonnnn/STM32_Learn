#include "fatfs_test.h"
/*
д�ļ����ԣ����emmc�ռ�
�����ο��� ������demo

*/


     
/* ���ڲ��Զ�д�ٶ� */
#define TEST_FILE_LEN			(10*1024*1024)	/* ���ڲ��Ե��ļ����� */
#define BUF_SIZE				(8*1024)		/* ÿ�ζ�дSD����������ݳ��� (4*1024)*/



#define  DEBUG_READ_FILE  1

char * p_FR_Table[]= {
	"FR_OK���ɹ�",                                      /* (0) Succeeded */
	"FR_DISK_ERR���ײ�Ӳ������",                      /* (1) A hard error occurred in the low level disk I/O layer */
	"FR_INT_ERR������ʧ��",                              /* (2) Assertion failed */
	"FR_NOT_READY����������û�й���",                  /* (3) The physical drive cannot work */
	"FR_NO_FILE���ļ�������",                          /* (4) Could not find the file */
	"FR_NO_PATH��·��������",                          /* (5) Could not find the path */
	"FR_INVALID_NAME����Ч�ļ���",                      /* (6) The path name format is invalid */
	"FR_DENIED�����ڽ�ֹ���ʻ���Ŀ¼�������ʱ��ܾ�",  /* (7) Access denied due to prohibited access or directory full */
	"FR_EXIST�����ڷ��ʱ���ֹ���ʱ��ܾ�",              /* (8) Access denied due to prohibited access */
	"FR_INVALID_OBJECT���ļ�����Ŀ¼������Ч",          /* (9) The file/directory object is invalid */
	"FR_WRITE_PROTECTED������������д����",              /* (10) The physical drive is write protected */
	"FR_INVALID_DRIVE���߼���������Ч",                  /* (11) The logical drive number is invalid */
	"FR_NOT_ENABLED�������޹�����",                      /* (12) The volume has no work area */
	"FR_NO_FILESYSTEM��û����Ч��FAT��",              /* (13) There is no valid FAT volume */
	"FR_MKFS_ABORTED�����ڲ�������f_mkfs()����ֹ",             /* (14) The f_mkfs() aborted due to any parameter error */
	"FR_TIMEOUT���ڹ涨��ʱ�����޷���÷��ʾ�������",         /* (15) Could not get a grant to access the volume within defined period */
	"FR_LOCKED�������ļ��������Բ������ܾ�",                 /* (16) The operation is rejected according to the file sharing policy */
	"FR_NOT_ENOUGH_CORE���޷����䳤�ļ���������",             /* (17) LFN working buffer could not be allocated */
	"FR_TOO_MANY_OPEN_FILES����ǰ�򿪵��ļ�������_FS_SHARE", /* (18) Number of open files > _FS_SHARE */
	"FR_INVALID_PARAMETER��������Ч"                         /* (19) Given parameter is invalid */
};

void WriteFileTest(char *path, char *c_test_buf)
{
    /* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
    FRESULT result;
//	FATFS fs;
    FIL file;
    uint8_t res = 0;
    uint32_t bw;
    uint32_t i;//,k;        
    char TestFileName[30];

    /* ���ļ� */
    sprintf(TestFileName, "%s", path);		
    result = f_open(&file, TestFileName, FA_CREATE_ALWAYS | FA_WRITE);
    if (result != FR_OK)
    {
        printf("���ļ� [%s] ʧ�� (%s)\r\n", TestFileName, p_FR_Table[result]);               
        f_close(&file);
        
        
        printf("FATFS ERROR(%s), Flash Disk Formatting...\r\n", p_FR_Table[res]);	//��ʽ��FLASH
		res=f_mkfs("0:",1,4096);//��ʽ��FLASH,1,�̷�;1,����Ҫ������,8������Ϊ1����
		if(res==0){
			f_setlabel((const TCHAR *)"0:MYTEST");	//����Flash���̵�����Ϊ��ALIENTEK
            printf("Flash Disk Format Finish \r\n"); //��ʽ�����			
		}else {
            printf("Flash Disk Format Error(%s) \r\n", p_FR_Table[result]);	//��ʽ��ʧ��
        }
		HAL_Delay(1000);
        
        return;
    }
    

    result = f_write(&file, c_test_buf, sizeof(c_test_buf), &bw);
    if (result == FR_OK) {
        if (((i + 1) % 80) == 0) {
            printf(".");
        }
    } else {
        printf("%s�ļ�дʧ��(%s)\r\n", TestFileName, p_FR_Table[result]);
    }    
    f_close(&file);		/* �ر��ļ�*/

#if  DEBUG_READ_FILE
    /* ��ʼ���ļ����� */
    result = f_open(&file, TestFileName, FA_OPEN_EXISTING | FA_READ);
    if (result !=  FR_OK)
    {
        printf("û���ҵ��ļ�: %s\r\n", TestFileName);
        return;
    }

    printf("��ʼ���ļ�[%s]...\r\n", TestFileName);
   

    result = f_read(&file, c_test_buf, sizeof(c_test_buf), &bw);
    if (result == FR_OK){
        printf("read: \r\n%s", c_test_buf);
    }else{
        printf("�ļ�[%s]��ʧ��(%s)\r\n", TestFileName, p_FR_Table[result]);
    }
    
    /* �ر��ļ�*/
    f_close(&file);
#endif
    /* ж���ļ�ϵͳ */
    // f_mount(NULL, "0:", 0);
}
