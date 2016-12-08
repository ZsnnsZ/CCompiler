#include "word.h"
#include "pre_syntax.h"

char input[255];//输入缓冲区
char token[55] = "";//单词缓冲区
int p_input;//输入换缓冲区指针
int p_token;//单词缓冲区指针
int flag = 0;//1表示刚读取一个变量或常数，"+/-"为运算符;0反之，"+/-"可能为数值符号
char ch;//当前读入字符
char *rwtab[10] = {"main", "int", "float", "double", "char", "if", "else", "do", "while", _KEY_WORD_END};
double sum;//存放数值
int input_f;//用于判断oneword是否要输出

/*pre_syntax*/
char cur1;     //存放当前栈顶的字符
int cur2;      //存放当前待比较的种别编码

int successflag = 1;
int i, j;//（10*i+j）用于查询预测分析表
//预测分析表
int table[5][10] = {
	{1,1,0,0,0,0,1,0,0,0},
	{0,0,1,1,0,0,0,1,1,1},
	{1,1,0,0,0,0,1,0,0,0},
	{0,0,1,1,1,1,0,1,1,1},
	{1,1,0,0,0,0,1,0,0,0}
};

int t_table[100];//存放待识别种别编码

/*	A=E'  B=T'
	预测分析表查询产生式时：
	非终结符：0:E  1:A  2:T  3:B  4:F 
	终结符  ：0:i  1:n  2:+  3:-  4:*  5:/  6:(  7:)  8:$	*/

void main()
{
	int over = 1;
	WORD *oneword = NULL;
	//pre_syntax
	int t_i = 0;//用于构造t_table[]
	printf("Enter Your words(end with #):\n");
	scanf("%[^#]s", input);
	printf("\n");
	p_input = 0;
	pre_do();
	printf("Your words after pre_do:\n%s\n\n", input);
	printf("The results:\n");
	while(over < 1000)
	{
		oneword = scaner();
		if(input_f)
			printf("(%d , %s)\n", oneword->typenum, oneword->word);
			over = oneword->typenum;
		t_table[t_i] = oneword->typenum;
		t_i++;
	}
    /*初始化栈，将$和E入栈*/
	base = malloc(sizeof(struCH));
	base->next = NULL;
	base->ch = '$';
	temp = malloc(sizeof(struCH));
	temp->next = base;
	temp->ch = 'E';
	top = temp;

	printf("\nbegin anlysing the syntax:\n");
	identify();

	if(!successflag)
	{
		printf("\nfail in anlysing\n");
	}
	else
		printf("\nsuccess in anlysing syntax\n");
}

void push(char ch)
{
	temp = malloc(sizeof(struCH));
	temp->ch = ch;
	temp->next = top;
	top = temp;
}
 
void pop(void)
{
	cur1 = top->ch;
	if(top->ch != '$')
		top = top->next;
}
 
void doforpush(int t)
{
	switch(t){
		case 0:push('A');push('T');break;
		case 1:push('A');push('T');break;
		case 6:push('A');push('T');break;
		case 12:push('A');push('T');push('+');break;
		case 13:push('A');push('T');push('-');break;
		case 20:push('B');push('F');break;
		case 21:push('B');push('F');break;
		case 26:push('B');push('F');break;
		case 34:push('B');push('F');push('*');break;
		case 35:push('B');push('F');push('/');break;
		case 40:push('i');break;
		case 41:push('n');break;
		case 46:push(')');push('E');push('(');
	}
}
 
void checkforpush()
{
	switch(cur1)          	/*非终结符:栈顶*/
	{
		case 'A':i=1;break;
		case 'B':i=3;break;
		case 'E':i=0;break;
		case 'T':i=2;break;
		case 'F':i=4;
	}
	switch(cur2)            /*终结符:待识别的表达式中*/
	{
		case 10:j=0;break;// i
		case 20:j=1;break;// n
		case 22:j=2;break;// +
		case 23:j=3;break;// -
		case 24:j=4;break;// *
		case 25:j=5;break;// /
		case 26:j=6;break;// (
		case 27:j=7;break;// )
		case 1000:j=8;// \0
	}
}

int isN(char cur)
{
	if(cur == 'A' || cur == 'B' || cur == 'E' || cur == 'T' || cur == 'F' || cur == '$')
		return 1;
	else
		return 0;
}

int curequal(char cur1, int cur2)
{
	switch(cur2)
	{
		case 10:if(cur1 == 'i')
					return 1;// i
		case 20:if(cur1 == 'n')
					return 1;// n
		case 22:if(cur1 == '+')
					return 1;// +
		case 23:if(cur1 == '-')
					return 1;// -
		case 24:if(cur1 == '*')
					return 1;// *
		case 25:if(cur1 == '/')
					return 1;// /
		case 26:if(cur1 == '(')
					return 1;// (
		case 27:if(cur1 == ')')
					return 1;// )
		default:return 0;
	}
}

void identify()
{
	int t;//寻找产生式的索引值
	int t_f = 0;
	int e_flag = 0;//输出控制
	for(;;)
	{
		pop();									/*读取栈顶的字符存cur1中*/
		cur2 = t_table[t_f];					/*读取数组中的一个字符存cur2*/
		if(!e_flag)
			printf("keep  trying: %c--%d\n", cur1, cur2);
		e_flag = 0;
		if(isN(cur1))//若当前出栈的为非终结符或者'$'
		{
			if(cur1 != '$')
			{
			    checkforpush();
			    if(table[i][j])
			    {
					t = 10*i + j;               /*计算产生式在数组中的位置*/
					doforpush(t);         		/*找对应t的产生式，并入栈*/
					continue;
			    }
			    else{
					successflag = 0;
					printf("there is no such syntax\n");
					break;
			    }
			}
			else//cur1 == '$'
			{
			    if(cur2 == 1000)				/*cur1=='$' && cur2==1000*/
			    	break;						//success
			    else
				{
					successflag = 0;
					printf("unexpected ending\n");
					break;
			    }
			}
		}
		else/*若当前字符为终结符*/
		{
			if(curequal(cur1, cur2))
			{
				printf("\nmatching one: %c--%d\n\n", cur1, cur2);
				t_f++;
				e_flag = 1;
				continue;
			}
			else
			{
				successflag = 0;
				printf("unmatching character\n");
			    break;
			}
		}
	}
}

char m_getch()
{
	ch = input[p_input];
	p_input++;
	return(ch);
}

void getbc()
{
	while(ch == ' ' || ch == 10)
	{
		ch = input[p_input++];
	}
}

void contact()
{
	token[p_token] = ch;
	p_token++;
	token[p_token] = '\0';
}

int letter()
{
	if(ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z')
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int letter2(char c)
{
	if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int digit()
{
	if(ch >= '0' && ch <= '9')
		return 1;
	else
		return 0;
}

int digit2(char c)
{
	if(c >= '0' && c <= '9')
		return 1;
	else
		return 0;
}

int reserve()
{
	int i = 0;
	while(strcmp(rwtab[i], _KEY_WORD_END))
	{
		if(!strcmp(rwtab[i], token))
		{
			flag = 0;
			return i+1;
		}
		i++;
	}
	return 10;
}

void retract()
{
	p_input--;
}

void pre_do_note()
{
	int i = 0;
	char cc = input[i];
	while(cc != '\0')
	{
		if(cc == '/')
		{
			cc = input[++i];
			if(cc == '/')
			{
				cc = input[--i];
				while(cc != '\n')
				{
					input[i] = ' ';
					cc = input[++i];
				}
			}
			else if(cc == '*')
			{
				input[i] = ' ';
				cc = input[--i];
				while(cc != '*' && input[++i] != '/')
				{
					input[--i] = ' ';
					cc = input[++i];
				}
				input[i] = ' ';
				input[++i] = ' ';
			}
		}
		cc = input[++i];
	}
}

void pre_do_line()
{
	int i = 0;
	char cc = input[i];
	while(cc != '\0')
	{
		if(cc == '\n')
		{
			cc = input[++i];
			while(cc == '\n')
			{
				input[i] = ' ';
				cc = input[++i];
			}
		}
		cc = input[++i];
	}
}

void pre_do_blank()
{
	char *string_temp = input;
    int i, j;
    for (i = 0, j = 0; string_temp[i] != '\0'; ) 
    {
        input[j++] = string_temp[i++];
        while(string_temp[i] == ' ') 
        {
        	if(string_temp[++i] != ' ')
        	{
        		--i;
        		input[j++] = string_temp[i++];
        	}
        	else
        		continue;
        }
    }
    input[j] = '\0';
}

void pre_do()
{
	pre_do_note();
	pre_do_line();
	pre_do_blank();
}

WORD *scaner(){
	WORD *myword = (WORD *)malloc(sizeof(WORD));;
	myword->typenum = 10;
	myword->word = "";
	p_token = 0;
	sum = 0;
	input_f = 1;
	int e = 0;
	m_getch();
	getbc();
	if(letter()){
		while(letter() || digit()){
			contact();
			m_getch();
		}
		retract();
		myword->typenum = reserve();
		myword->word = token;
		if(myword->typenum == 10)
		{
			flag = 1;
		}
		return (myword);
	}
	else if(ch == '+' || ch == '-' || digit())
	{
		if((!digit()) && (flag == 1 || !digit2(input[p_input]) || input[p_input-2] == ')'))//+,-
		{
			if(ch == '+')
			{
				/*m_getch();
				if(ch == '+' && !digit2(input[p_input]) && !letter2(input[p_input]))
				{
					myword->typenum = 38;
					myword->word = "++";
					return(myword);
				}
				retract();*/
				myword->typenum = 22;
				myword->word = "+";
				flag = 0;
				return (myword);
			}
			else
			{
				/*m_getch();
				if(ch == '-' && !digit2(input[p_input]) && !letter2(input[p_input]))
				{
					myword->typenum = 39;
					myword->word = "--";
					return(myword);
				}
				retract();*/
				myword->typenum = 23;
				myword->word = "-";
				flag = 0;
				return (myword);
			}
		}
		else
		{
			int flag1 = 0;
			int flag2 = 0;
			if(ch == '+' || ch == '-')
			{	
				if(ch == '-')
					flag1 = 1;
				m_getch();
			}
			while(digit())
			{
				sum = sum*10 + ch - '0';
				m_getch();
			}
			int k = 10;
			if(ch == '.' && digit2(input[p_input]))
			{
				m_getch();
				while(digit())
				{
					double d = ch - '0';
					sum = sum + d/k;
					k = k*10;
					m_getch();
				}
			}
			if(ch == '.' && !digit2(input[p_input]))
			{
				input_f = 0;
			}
			if(ch == 'e' || ch == 'E')
			{
				char ch_temp = input[p_input];
				if(((ch_temp == '+' || ch_temp == '-') && digit2(input[p_input+1])) || digit2(ch_temp))
				{
					m_getch();
					if(!digit())
					{
						if(ch == '+')
							flag2 = 0;//e指数为正
						else
							flag2 = 1;//e指数为负
						m_getch();
					}
					while(digit())
					{
						e = e*10 + ch - '0';
						m_getch();
					}
					if(flag2)
						sum = sum*pow(10.0,-e); 
					else
						sum = sum*pow(10.0,e);
				}
				else
					{
						input_f = 0;
						p_input++;
					}
			}
			retract();
			if(flag1)
				sum = sum * (-1);
			flag = 1;//flag=1,刚读取一个数
			char word_temp[100];
			sprintf(word_temp,"%e",sum);
			myword->typenum = 20;
			myword->word = word_temp;
			return(myword);
		}
	}
	else switch(ch)
	{
		case'=':m_getch();
				if(ch == '='){
					myword->typenum = 36;
					myword->word= "==";
					return (myword);
					break;
				}
				retract();
				myword->typenum= 21;
				myword->word= "=";
				return (myword);
				break;
		case'*':myword->typenum = 24;
				myword->word = "*";
				return (myword);
				break;
		case'/':myword->typenum = 25;
				myword->word = "/";
				break;
		case'(':myword->typenum = 26;
				myword->word = "(";
				return (myword);
				break;
		case')':myword->typenum = 27;
				myword->word = ")";
				return (myword);
		case'{':myword->typenum = 28;
				myword->word = "{";
				return (myword);
				break;
		case'}':myword->typenum = 29;
				myword->word = "}";
				return (myword);
				break;
		case',':myword->typenum = 30;
				myword->word = ",";
				break;
		case';':myword->typenum = 31;
				myword->word = ";";
				return (myword);
				break;
		case'>':m_getch();
				if(ch == '='){
					myword->typenum = 33;
					myword->word= ">=";
					return (myword);
					break;
				}
				retract();
				myword->typenum= 32;
				myword->word= ">";
				return (myword);
				break;
		case'<':m_getch();
				if(ch == '='){
					myword->typenum = 35;
					myword->word= "<=";
					return (myword);
					break;
				}
				retract();
				myword->typenum= 34;
				myword->word= "<";
				return (myword);
				break;
		case'!':m_getch();
				if(ch == '='){
					myword->typenum = 37;
					myword->word= "!=";
					return (myword);
					break;
				}
				retract();
				myword->typenum= -1;
				myword->word= "ERROR";
				return (myword);
				break;
		case'\0':myword->typenum = 1000;
				myword->word = "OVER";
				return (myword);
				break;
		default:myword->typenum = -1;
				myword->word = "ERROR";
				return (myword);
	} 
}