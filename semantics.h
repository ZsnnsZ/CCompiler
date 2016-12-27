int statement();
char * expression();
char * term();
char * factor();
void sscaner();
void error();
void anlyse_syntax();
void program();
int block();
int statement_strand();
int assign_statement();
int if_statement();
void condition(int *etc, int *efc);
int operator();
int do_statement();
#define MAXLENGTH 20
typedef struct QUATERNION
{
	char op[MAXLENGTH];
	char arg1[MAXLENGTH];
	char arg2[MAXLENGTH];
	char res[MAXLENGTH];	
}QUAD;
void emit(char *op, char *arg1, char *arg2, char *res);
char * newtemp();
void printQuad();
void printQuad2();
int merge(int p1, int p2);
void backpatch(int p, int t);
