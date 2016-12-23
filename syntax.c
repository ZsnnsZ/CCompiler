#include "word.h"
#include "syntax.h"

char input[255];// 输入缓冲区
char token[55] = "";// 单词缓冲区
int p_input;// 输入缓冲区指针
int p_token;// 单词缓冲区指针
int flag = 0;// 1表示刚读取一个变量或常数，"+/-"为运算符;0反之，"+/-"可能为数值符号
char ch;// 当前读入字符
char *rwtab[10] = {"main", "int", "float", "double", "char", "if", "else", "do", "while", _KEY_WORD_END};
double sum;// 存放数值
int input_f;// 用于判断oneword是否要输出

int type;// 存放当前的typenum

int successflag = 0;
int failflag = 0;

void main()
{
	int over = 1;
	WORD *oneword = NULL;
	printf("Enter Your words(end with #):\n");
	scanf("%[^#]s", input);
	printf("\n");
	p_input = 0;
	flag = 0;
	pre_do();
	printf("Your words after pre_do:\n%s\n\n", input);
	printf("The results:\n");
	while(over < 1000)
	{
		oneword = scaner();
		if(input_f)
			printf("(%d , %s)\n", oneword->typenum, oneword->word);
			over = oneword->typenum;
	}
	// 调用语法分析程序
	anlyse_syntax();
}

//语法分析程序
void anlyse_syntax()
{
	p_input = 0;
	printf("\nbegin anlysing the syntax:\n");
	sscaner();
	// statement_strand();
	program();
	//if_statement();
	//do_statement();
	if(type == 1000)
		successflag = 1;
	else
	{
		// printf("\nanlyse unexpected ending\n");
		// error();
		failflag = 1;
	}
	if(failflag == 1)
	{
		printf("\nfail in anlysing syntax\n");
	}

	if(successflag == 1 && failflag == 0)
		printf("\nsuccess in anlysing syntax\n");
}

//扫描
void sscaner()
{
	WORD *sword = NULL;
	sword = scaner();
	if(input_f)
		type = sword->typenum;
	else
		sscaner();
	printf("current typenum: %d\n", type);
}

//出错处理
void error()
{
	printf("Syntax error around %d\n\n",type);
}

//<程序> ::= main()<语句块>
void program()
{
	if(type == 1)
	{
		sscaner();
		if(type == 26)
		{
			sscaner();
			if(type == 27)
			{
				sscaner();
				block();
			}
			else
			{
				printf("\nprogram(): main() lack of ')'\n");
				error();
			}
		}
		else
		{
			printf("\nprograme(): main() lack of '('\n");
			error();
		}
	}
	else
	{
		printf("\nprograme(): main() lack of 'main'\n");
		error();
	}
}

//<语句块> ::= ‘{‘<语句串>’}’
void block()
{
	if(type == 28)
	{
		sscaner();
		statement_strand();
		if(type == 29)
			sscaner();
		else
		{
			printf("\nblock(): lack of '}'\n");
			error();
		}
	}
	else
	{
		printf("\nblock(): lack of '{'\n");
		error();
	}
}

//<语句串>::=<语句>{;<语句>}
void statement_strand()
{
	statement();
	if(type == 31)
		sscaner();
	else
	{
		printf("\nstatement_strand()1: lack of ';' or ';' is not necessary\n");
		error();
	}
	while(type != 29)
	{
		statement();
		if(type == 31)
			sscaner();
		else
		{
			printf("\nstatement_strand()2: lack of ';'\n");
			error();
		}
	}
	return;
}

//<语句>::=<赋值语句>|<条件语句>|<循环语句>
void statement()
{
	if(type == 10)
		assign_statement();
	else if(type == 6)
		if_statement();
	else if(type == 8)
		do_statement();
	else
	{
		printf("\nstatement(): unexpected statement\n");
		error();
	}
	return;
}

//<赋值语句>::=ID=<表达式>
void assign_statement()
{
	if(type == 10)
	{
		sscaner();
		if(type == 21)
		{
			sscaner();
			expression();
		}
		else
		{
			printf("\nassign_statement(): lack of '='\n");
			error();
		}
	}
	else
	{
		printf("\nassign_statement(): lack of 'ID'\n");
		error();
	}
	return;
}

//<条件语句>::=if<条件><语句块>[else <语句块>]
//<条件语句>::=if'('<条件>')'<语句块>[else <语句块>]
void if_statement()
{
	if(type == 6)
	{
		sscaner();
		if(type == 26)
		{
			sscaner();
			condition();
			if(type == 27)
			{
				sscaner();
				block();
				if(type == 7)
				{
					sscaner();
					block();
				}
			}
			else
			{
				printf("\nif_statement(): lack of ')'\n");
				error();
			}
		}
		else
		{
			printf("\nif_statement(): lack of '('\n");
			error();
		}
	}
	else
	{
		printf("\nif_statement(): lack of 'if'\n");
		error();
	}
	return;
}

//<条件>::=<表达式><关系运算符><表达式>
void condition()
{
	expression();
	operator();
	expression();
	return;
}

//<关系运算符> ::= <|<=|>|>=|==|!=
void operator()
{
	if(type == 32 || type == 33 || type == 34 || type == 35 || type == 36 || type == 37)
		sscaner();
	else
	{
		printf("\noperator(): unexpected operator\n");
		error();
	}
	return;
}

//<循环语句>::=do <语句块>while <条件>
void do_statement()
{
	if(type == 8)
	{
		sscaner();
		block();
		if(type == 9)
		{
			sscaner();
			if(type == 26)
			{
				sscaner();
				condition();
				if(type == 27)
				{
					sscaner();
				}
				else
				{
					printf("\ndo_statement(): lack of ')'\n");
					error();
				}
			}
			else
			{
				printf("\ndo_statement(): lack of '('\n");
				error();
			}
		}
		else
		{
			printf("\ndo_statement(): lack of 'while'\n");
			error();
		}
	}
	else
	{	
		printf("\ndo_statement(): lack of 'do'\n");
		error();
	}
	return;
}

//<表达式> ::= <项>{+<项>|-<项>}
void expression()
{
    term();
    while(type == 22 || type == 23)
    {
    	sscaner();
    	term();
    }
    return;
}

//<项> ::= <因子>{*<因子>|/<因子>}
void term()
{
    factor();
    while(type == 24 || type == 25)
    {
    	sscaner();
    	factor();
    }
    return;
}

//<因子> ::=ID|num|(<表达式>)
void factor()
{
    if(type == 10 || type == 20)//ID|num
    	sscaner();
    else if (type == 26)//'('
    {
        sscaner();
        expression();
        if (type == 27)//')'
        	sscaner();
        else 
        { 
            failflag = 1;
            printf("\nfactor(): lack of ')'\n");
            error();
        }
    }
    else { 
        failflag = 1;
        printf("\nfator(): expression error\n");
        error();
    }
    return;
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
						p_input = p_input + 2;
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
	else switch(ch){
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
