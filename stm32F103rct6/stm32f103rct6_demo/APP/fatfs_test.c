#include "fatfs_test.h"
/*
写文件测试，填充emmc空间
函数参考自 安富莱demo

*/


     
/* 用于测试读写速度 */
#define TEST_FILE_LEN			(10*1024*1024)	/* 用于测试的文件长度 */
#define BUF_SIZE				(8*1024)		/* 每次读写SD卡的最大数据长度 (4*1024)*/



#define  DEBUG_READ_FILE  1

char * p_FR_Table[]= {
	"FR_OK：成功",                                      /* (0) Succeeded */
	"FR_DISK_ERR：底层硬件错误",                      /* (1) A hard error occurred in the low level disk I/O layer */
	"FR_INT_ERR：断言失败",                              /* (2) Assertion failed */
	"FR_NOT_READY：物理驱动没有工作",                  /* (3) The physical drive cannot work */
	"FR_NO_FILE：文件不存在",                          /* (4) Could not find the file */
	"FR_NO_PATH：路径不存在",                          /* (5) Could not find the path */
	"FR_INVALID_NAME：无效文件名",                      /* (6) The path name format is invalid */
	"FR_DENIED：由于禁止访问或者目录已满访问被拒绝",  /* (7) Access denied due to prohibited access or directory full */
	"FR_EXIST：由于访问被禁止访问被拒绝",              /* (8) Access denied due to prohibited access */
	"FR_INVALID_OBJECT：文件或者目录对象无效",          /* (9) The file/directory object is invalid */
	"FR_WRITE_PROTECTED：物理驱动被写保护",              /* (10) The physical drive is write protected */
	"FR_INVALID_DRIVE：逻辑驱动号无效",                  /* (11) The logical drive number is invalid */
	"FR_NOT_ENABLED：卷中无工作区",                      /* (12) The volume has no work area */
	"FR_NO_FILESYSTEM：没有有效的FAT卷",              /* (13) There is no valid FAT volume */
	"FR_MKFS_ABORTED：由于参数错误f_mkfs()被终止",             /* (14) The f_mkfs() aborted due to any parameter error */
	"FR_TIMEOUT：在规定的时间内无法获得访问卷的许可",         /* (15) Could not get a grant to access the volume within defined period */
	"FR_LOCKED：由于文件共享策略操作被拒绝",                 /* (16) The operation is rejected according to the file sharing policy */
	"FR_NOT_ENOUGH_CORE：无法分配长文件名工作区",             /* (17) LFN working buffer could not be allocated */
	"FR_TOO_MANY_OPEN_FILES：当前打开的文件数大于_FS_SHARE", /* (18) Number of open files > _FS_SHARE */
	"FR_INVALID_PARAMETER：参数无效"                         /* (19) Given parameter is invalid */
};

void WriteFileTest(char *path, char *c_test_buf)
{
    /* 本函数使用的局部变量占用较多，请修改启动文件，保证堆栈空间够用 */
    FRESULT result;
//	FATFS fs;
    FIL file;
    uint8_t res = 0;
    uint32_t bw;
    uint32_t i;//,k;        
    char TestFileName[30];

    /* 打开文件 */
    sprintf(TestFileName, "%s", path);		
    result = f_open(&file, TestFileName, FA_CREATE_ALWAYS | FA_WRITE);
    if (result != FR_OK)
    {
        printf("打开文件 [%s] 失败 (%s)\r\n", TestFileName, p_FR_Table[result]);               
        f_close(&file);
        
        
        printf("FATFS ERROR(%s), Flash Disk Formatting...\r\n", p_FR_Table[res]);	//格式化FLASH
		res=f_mkfs("0:",1,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
		if(res==0){
			f_setlabel((const TCHAR *)"0:MYTEST");	//设置Flash磁盘的名字为：ALIENTEK
            printf("Flash Disk Format Finish \r\n"); //格式化完成			
		}else {
            printf("Flash Disk Format Error(%s) \r\n", p_FR_Table[result]);	//格式化失败
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
        printf("%s文件写失败(%s)\r\n", TestFileName, p_FR_Table[result]);
    }    
    f_close(&file);		/* 关闭文件*/

#if  DEBUG_READ_FILE
    /* 开始读文件测试 */
    result = f_open(&file, TestFileName, FA_OPEN_EXISTING | FA_READ);
    if (result !=  FR_OK)
    {
        printf("没有找到文件: %s\r\n", TestFileName);
        return;
    }

    printf("开始读文件[%s]...\r\n", TestFileName);
   

    result = f_read(&file, c_test_buf, sizeof(c_test_buf), &bw);
    if (result == FR_OK){
        printf("read: \r\n%s", c_test_buf);
    }else{
        printf("文件[%s]读失败(%s)\r\n", TestFileName, p_FR_Table[result]);
    }
    
    /* 关闭文件*/
    f_close(&file);
#endif
    /* 卸载文件系统 */
    // f_mount(NULL, "0:", 0);
}

