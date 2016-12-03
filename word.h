#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define _KEY_WORD_END "zzzzzz"//关键字结束标志

typedef struct
{
	int typenum;
	char *word;
}WORD;

//从输入缓冲区获得单个字符
char m_getch();
//去掉空白符号
void getbc();
//拼接单词
void contact();
//判断是否是字母
int letter();
int letter2(char c);
//判断是否是数字
int digit();
int digit2(char c);
//搜索关键字表;若它是一个关键字, 则回送它的编码，否则回送标识符的种别码10。
int reserve();
//回退一个字符
void retract();
//数字转换，将token中的数字串转换成二进制数值表示
char *dtb();
//词法扫描函数
WORD *scaner();
//预处理
void pre_do();
