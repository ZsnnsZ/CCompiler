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
char *curword;//存放当前的word
int successflag = 0;//语法分析成功标志
int failflag = 0;//语法分析失败标志
QUAD *quad;//存放四元式的数组
int p_quad = 1;//四元式编号
int n_newtemp;//语义分析 临时变量下标

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
	//printf("Your words before anlyse_syntax:\n%s\n\n", input);
	// 调用语法分析程序
	anlyse_syntax();
}

//语法分析程序
void anlyse_syntax()
{
	p_input = 0;//置输入缓冲区指针为0
	flag = 0;
	quad = (QUAD *)malloc(strlen(input)*sizeof(QUAD));
	printf("\nbegin anlysing the syntax:\n");

	program();

	if(type == 1000)
		successflag = 1;
	else
	{
		failflag = 1;
	}

	if(failflag == 1)
	{
		printf("\nfail in anlysing syntax\n");
	}

	if(successflag == 1 && failflag == 0)
	{
		printf("\nsuccess in anlysing syntax\n");
		// 打印语义分析结果
		printQuad();
	}
}

//语法语义扫描
void sscaner()
{
	WORD *sword = NULL;
	sword = scaner();
	if(input_f)
	{
		type = sword->typenum;
		curword = sword->word;
	}	
	else
		sscaner();
	printf("current typenum: %d\n", type);
}

//出错处理
void error()
{
	failflag = 1;
	printf("Syntax error around %d\n\n",type);
}

//产生四元式
void emit(char *op, char *arg1, char *arg2, char *res)
{
	sprintf(quad[p_quad].op, "%s", op);
	sprintf(quad[p_quad].arg1, "%s", arg1);
	sprintf(quad[p_quad].arg2, "%s", arg2);
	sprintf(quad[p_quad].res, "%s", res);
	p_quad++;
	return;
}

//产生临时变量
char *newtemp()
{
	char *tempID = (char *)malloc(MAXLENGTH);
	sprintf(tempID, "T%d", ++n_newtemp);
	return tempID;
}

//打印四元式
void printQuad()
{
	int n = 1;
	printf("\nthe sequence of quaternion:");
	// for(n=1;n<p_quad;n++)
	// {
	// 	printf("\n%2d:%7s %5s %5s %5s %5s", n, quad[n].res, "=", quad[n].arg1, quad[n].op, quad[n].arg2);
	// }
	while(n<p_quad)
	{
		if(!strcmp(quad[n].arg2, "="))//赋值
			printf("\n%2d:%7s %5s %5s %5s", n, quad[n].op, quad[n].arg1, quad[n].arg2, quad[n].res);
		else if(!(strcmp(quad[n].op, "==") && strcmp(quad[n].op, "<") && strcmp(quad[n].op, "<=") && strcmp(quad[n].op, ">") && strcmp(quad[n].op, ">=") && strcmp(quad[n].op, "!=")))//条件
			printf("\n%2d:     if%3s%3s%3s goto %5s", n, quad[n].arg1,quad[n].op, quad[n].arg2, quad[n].res);
		else if(!strcmp(quad[n].op, "goto"))
			printf("\n%2d:%7s %5s", n, quad[n].op, quad[n].res);
		else
			printf("\n%2d:%7s %5s %5s %5s", n, quad[n].res, quad[n].arg1, quad[n].op, quad[n].arg2);
		n++;
	}
}

//链接函数，将p1，p2为首的两条链合并为以p2为首的链
int merge(int p1, int p2)
{
	if(p2 == 0)
		return p1;
	else
	{
		int a,b;
		a = p2;//a放编号
		b = atoi(quad[p2].res);//b放值
		while(b)
		{
			a = b;
			b = atoi(quad[a].res);
		}
		sprintf(quad[a].res, "%d", p1);
	}
	return p2;
}

//回填过程
void backpatch(int p, int t)
{
	int w, q = p;
	while(q)
	{
		w = atoi(quad[q].res);
		sprintf(quad[q].res, "%d", t);
		q = w;
	}
	return;
}

//<程序> ::= main()<语句块>
void program()
{
	int waitq;
	sscaner();
	if(type == 1)
	{
		sscaner();
		if(type == 26)
		{
			sscaner();
			if(type == 27)
			{
				sscaner();
				waitq = block();
				backpatch(waitq,p_quad);
			}
			else
			{
				printf("\nprogram(): main() lack of ')'\n");
				error();
			}
		}
		else
		{
			printf("\nprogram(): main() lack of '('\n");
			error();
		}
	}
	else
	{
		printf("\nprogram(): main() lack of 'main'\n");
		error();
	}
}

//<语句块> ::= ‘{‘<语句串>’}’
//void block()
int block()
{
	int waitq;
	if(type == 28)
	{
		sscaner();
		waitq = statement_strand();
		if(type == 29)
			sscaner();
		else
		{
			printf("\nblock(): lack of '}'\n");
			//error();
		}
	}
	else
	{
		printf("\nblock(): lack of '{'\n");
		error();
	}
	return waitq;
}

//<语句串>::=<语句>{;<语句>}
//void statement_strand()
int statement_strand()
{
	//statement();
	int waitq;
	waitq = statement();
	if(type == 31)
		sscaner();
	else
	{
		printf("\nstatement_strand()1: lack of ';' or ';' is not necessary\n");
		error();
	}
	while(type != 29)
	{
		//statement();
		backpatch(waitq, p_quad);
		waitq = statement();
		if(type == 31)
			sscaner();
		else
		{
			printf("\nstatement_strand()2: lack of ';'\n");
			error();
		}
	}
	return waitq;
}

//<语句>::=<赋值语句>|<条件语句>|<循环语句>
//void statement()
int statement()
{
	int waitq;
	if(type == 10)
		//assign_statement();
		waitq = assign_statement();
	else if(type == 6)
		//if_statement();
		waitq = if_statement();
	else if(type == 8)
		//do_statement();
		waitq = do_statement();
	else
	{
		printf("\nstatement(): unexpected statement\n");
		error();
	}
	return waitq;
}

//<赋值语句>::=ID=<表达式>
//void assign_statement()
int assign_statement()
{
	char *as_temp = (char *)malloc(MAXLENGTH);
	char *as_place = (char *)malloc(MAXLENGTH);
	if(type == 10)
	{
		sprintf(as_temp, "%s", curword);
		sscaner();
		if(type == 21)
		{
			sscaner();
			as_place = expression();
			emit(as_place, "=", "", as_temp);
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
	return 0;
}

//<条件语句>::=if<条件><语句块>[else <语句块>]
//void if_statement()
int if_statement()
{
	int ntc, nfc;
	int waitq, waitq1, waitq2;
	if(type == 6)
	{
		sscaner();
		condition(&ntc, &nfc);
		backpatch(ntc, p_quad);//填真出口
		waitq1 = block();//语句块待填出口
		if(type == 7)//else
		{
			int j;
			sscaner();
			j = p_quad;//记录无条件转移四元式的编号
			emit("goto", "", "", "0");
			backpatch(nfc, p_quad);//填if的假出口
			waitq2 = block();//语句块待填出口
			waitq = merge(j, waitq2);
			waitq = merge(j, waitq1);
			return waitq;
		}
		else
		{
			waitq = merge(nfc, waitq1);
			return waitq;
		}
	}
	else
	{
		printf("\nif_statement(): lack of 'if'\n");
		error();
	}
	return waitq;
}

//<条件>::=<表达式><关系运算符><表达式>
//void condition()
void condition(int *etc, int *efc)//turecondition, false condition
{
	char *op, *optmp, *cplace1, *cplace2;
	op = (char *)malloc(MAXLENGTH);
	optmp = (char *)malloc(MAXLENGTH);
	cplace1 = (char *)malloc(MAXLENGTH);
	cplace2 = (char *)malloc(MAXLENGTH);
	cplace1 = expression();

	if(operator())
	{
		//op = curword; 
		sprintf(op, "%s", curword);
		sscaner();
		cplace2 = expression();
		*etc = p_quad;//记录emit(optmp, cplace1, cplace2, "0")的四元式编号
		*efc = p_quad + 1;//记录无条件转移的四元式编号
		//sprintf(optmp, "goto %s", op);
		emit(op, cplace1, cplace2, "0");
		emit("goto", "", "", "0");
	}
	else
	{
		printf("\noperator(): unexpected operator\n");
		error();
	}
	return;
}

//<关系运算符> ::= <|<=|>|>=|==|!=
int operator()
{
	if(type == 32 || type == 33 || type == 34 || type == 35 || type == 36 || type == 37)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//<循环语句>::=do <语句块>while <条件>
//void do_statement()
int do_statement()
{
	int ntc, nfc;
	int waitq;
	if(type == 8)
	{
		int p_quadtmp = p_quad;//记录do块的入口
		sscaner();
		waitq = block();
		if(type == 9)
		{
			sscaner();
			condition(&ntc, &nfc);
			backpatch(ntc, p_quadtmp);//回填while条件判断的真出口
			backpatch(waitq, ntc);
			return nfc;
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
	return nfc;
}

//<表达式> ::= <项>{+<项>|-<项>}
char * expression()
{
	char *op, *eplace1, *eplace2, *temp;//四元式
	char *eplace;
	char *tempt;//输出格式美化
	op = (char *)malloc(MAXLENGTH);
	eplace1 = (char *)malloc(MAXLENGTH);
	eplace2 = (char *)malloc(MAXLENGTH);
	temp = (char *)malloc(MAXLENGTH);
	eplace = (char *)malloc(MAXLENGTH);
	eplace1 = term();
	eplace = eplace1;
	tempt = (char *)malloc(MAXLENGTH);
    while(type == 22 || type == 23)
    {
    	//op = curword;
    	sprintf(op, "%s", curword);
    	sscaner();
    	eplace2 = term();
    	temp = newtemp();
    	sprintf(tempt, "%s =", temp);
    	emit(op, eplace1, eplace2, tempt);
    	eplace = temp;
    	eplace1 = temp;
    }

    return eplace;
}

//<项> ::= <因子>{*<因子>|/<因子>}
char * term()
{
    char *op, *tplace1, *tplace2, *temp;//四元式
	char *tplace;
	op = (char *)malloc(MAXLENGTH);
	tplace1 = (char *)malloc(MAXLENGTH);
	tplace2 = (char *)malloc(MAXLENGTH);
	temp = (char *)malloc(MAXLENGTH);
	tplace = (char *)malloc(MAXLENGTH);
	tplace1 = factor();
	tplace = tplace1;

    while(type == 24 || type == 25)
    {
    	//op = curword;
    	sprintf(op, "%s", curword);
    	sscaner();
    	tplace2 = factor();
		temp = newtemp();	
    	emit(op, tplace1, tplace2, temp);
    	tplace = temp;
    	tplace1 = temp;
    }
    return tplace;
}

//<因子> ::=ID|num|(<表达式>)
char * factor()
{
	char * fplace;
	fplace = (char *)malloc(MAXLENGTH);
    if(type == 10 || type == 20)//ID|num
    {
    	if(type == 10)
			sprintf(fplace, "%s", curword);
		else if(type == 20)
			sprintf(fplace, "%s", curword);
		sscaner();
    }
    else if (type == 26)//'('
    {
        sscaner();
        fplace = expression();
        if (type == 27)//')'
        	sscaner();
        else 
        { 
            printf("\nfactor(): lack of ')'\n");
            error();
        }
    }
    else { 
        printf("\nfator(): expression error\n");
        error();
    }
    return fplace;
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
	WORD *myword = (WORD *)malloc(sizeof(WORD));
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
			char *word_temp = (char *)malloc(20);
			sprintf(word_temp, "%g", sum);
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
