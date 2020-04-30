#include<stdio.h>
#include<string.h>
#include "getNext.c"

struct token lookahead;
struct token lookback;
char st[100];

void check();
void proc_program();
void declaration();
void statement_list();
void identifier_list();
void identifier_list_1();
void identifier_list_2();
void data_type();
void statement();
void assign_stat();
void decision_stat();
void dprime_d();
void looping_stat();
void expn();
void simple_expn();
void eprime();
void relop();
void term();
void seprime();
void factor();
void seprime();
void tprime();
void addop();
void mulop();
void dprime();
void pErr();
void nextline();
void verify(char str[]);
void switch_stat();
void verifyAttribute();
void checkAttribute();
void case_stat();
void case_stat_d();

void proc_program(){
	if(strcmp(lookahead.tokenName,"main")==0){
		verify( "main");
		verify("(");
		verify(")");
		verify("{");
		//for next line
		nextline();
		//for next line finish
		declaration();
		statement_list();
		verify("}");
	}else{
		pErr();
	}
}

void declaration(){
	if(strcmp(lookahead.tokenName,"int")==0){
		data_type();
		identifier_list();
		verify( ";\0");
		declaration();
	}else if(strcmp(lookahead.tokenName,"char")==0){
		data_type();
		identifier_list();
		verify( ";\0");
		declaration();
	}else{
		return;
	}
}

void data_type(){
	if(strcmp(lookahead.tokenName,"int")==0){
		verify( "int\0");
	}else if(strcmp(lookahead.tokenName,"char")==0){
		verify( "char\0");
	}else{
		pErr();
	}
}

void identifier_list(){
	if(strcmp(lookahead.attribute,"ID")==0){
		verify( lookahead.tokenName);
		identifier_list_1();
	}else{
		pErr();
	}
}

void identifier_list_1(){
	if(strcmp(lookahead.tokenName,",")==0){
		verify( ",\0");
		identifier_list();
	}else if(strcmp(lookahead.tokenName,"[")==0){
		verify( "[\0");
		if(strcmp(lookahead.attribute,"Number")==0){
			verify( lookahead.tokenName);
		}else{
			pErr();
		}
		verify( "]\0");
		identifier_list_2();
	}else{
		return;
	}
}

void identifier_list_2(){
	if(strcmp(lookahead.tokenName,",")==0){
		verify( ",\0");
		identifier_list();
	}else{
		return;
	}
}

void statement_list(){
	if(strcmp(lookahead.attribute,"ID")==0){
		statement();
		statement_list();
	}else if(strcmp(lookahead.tokenName,"if")==0){
		//for next line
		nextline();
		//for next line finish
		statement();
		statement_list();
	}else if(strcmp(lookahead.tokenName,"while")==0){
		//for next line
		nextline();
		//for next line finish
		statement();
		statement_list();
	}else if(strcmp(lookahead.tokenName,"for")==0){
		//for next line
		nextline();
		//for next line finish
		statement();
		statement_list();
	}else if(strcmp(lookahead.tokenName,"switch")==0){
		statement();
		statement_list();
	}else{
		return;
	}
}

void statement(){
	if(strcmp(lookahead.attribute,"ID")==0){
		assign_stat();
		verify( ";\0");
	}else if(strcmp(lookahead.tokenName,"if")==0){
		decision_stat();
	}else if(strcmp(lookahead.tokenName,"while")==0){
		looping_stat();
	}else if(strcmp(lookahead.tokenName,"for")==0){
		looping_stat();
	}else if(strcmp(lookahead.tokenName,"switch")==0){
		switch_stat();
	}else{
		pErr();
	}
}

void assign_stat(){
	if(strcmp(lookahead.attribute,"ID")==0){
		verify( lookahead.tokenName);
		verify( "=\0");
		expn();
	}else{
		pErr();
	}
}

void expn(){
	if(strcmp(lookahead.attribute,"ID")==0){
		simple_expn();
		eprime();
	}else if(strcmp(lookahead.attribute,"Number")==0){
		simple_expn();
		eprime();
	}else{
		pErr();
	}
}

void simple_expn(){
	if(strcmp(lookahead.attribute,"ID")==0){
		term();
		seprime();
	}else if(strcmp(lookahead.attribute,"Number")==0){
		term();
		seprime();
	}else{
		pErr();
	}
}

void eprime(){
	if(strcmp(lookahead.attribute,"RE")==0){
		relop();
		simple_expn();
	}else{
		return;
	}
}

void relop(){
	if(strcmp(lookahead.attribute,"RE")==0){
		verify( lookahead.tokenName);
	}else{
		pErr();
	}
}

void term(){
	if(strcmp(lookahead.attribute,"ID")==0){
		factor();
		tprime();
	}else if(strcmp(lookahead.attribute,"Number")==0){
		factor();
		tprime();
	}else{
		pErr();
	}
}

void factor(){
	if(strcmp(lookahead.attribute,"ID")==0){
		verify(lookahead.tokenName);
	}else if(strcmp(lookahead.attribute,"Number")==0){
		verify(lookahead.tokenName);
	}else{
		pErr();
	}
}

void seprime(){
	if(strcmp(lookahead.attribute,"AE")==0){
		addop();
		term();
		seprime();
	}else{
		return;
	}
}

void addop(){
	if(strcmp(lookahead.attribute,"AE")==0){
		verify( lookahead.tokenName);
	}else{
		pErr();
	}
}

void tprime(){
	if(strcmp(lookahead.attribute,"AEM")==0){
		mulop();
		factor();
		tprime();
	}else{
		return;
	}
}

void mulop(){
	if(strcmp(lookahead.attribute,"AEM")==0){
		verify( lookahead.tokenName);
	}else{
		pErr();
	}
}

void decision_stat(){
	if(strcmp(lookahead.tokenName,"if")==0){
		verify( lookahead.tokenName);
		verify( "(\0");
		expn();
		verify( ")\0");
		verify( "{\0");
		statement_list();
		verify( "}\0");
		dprime();
	}else {
		pErr();
	}
}

void dprime(){
	if(strcmp(lookahead.tokenName,"else")==0){
		verify(lookahead.tokenName);
		dprime_d();
	}else {
		return;
	}
}

void dprime_d(){
	if(strcmp(lookahead.tokenName,"{\0")==0){
		verify(lookahead.tokenName);
		statement_list();
		verify( "}\0");
	}else if(strcmp(lookahead.tokenName,"if")==0){
		decision_stat();
	}else{
		pErr();
	}
}

void looping_stat(){
	if(strcmp(lookahead.tokenName,"while")==0){
		verify( lookahead.tokenName);
		verify( "(\0");
		expn();
		verify( ")\0");
		verify( "{\0");
		statement_list();
		verify( "}\0");
		//for next line
		nextline();
		//for next line finish
	}else if(strcmp(lookahead.tokenName,"for")==0){
		verify( lookahead.tokenName);
		verify( "(\0");
		assign_stat();
		verify( ";\0");
		expn();
		verify( ";\0");
		assign_stat();
		verify( ")\0");
		verify( "{\0");
		statement_list();
		verify( "}\0");
		//for next line
		nextline();
		//for next line finish
	}else{
		pErr();
	}
}

void switch_stat(){
	if(strcmp(lookahead.tokenName,"switch")==0){
		verify( lookahead.tokenName);
		verify( "(\0");
		verifyAttribute("ID");
		verify( ")\0");
		verify( "{\0");
		case_stat();
		verify( "}\0");
		//for next line
		nextline();
		//for next line finish
	}else{
		pErr();
	}
}

void case_stat(){
	if(strcmp(lookahead.tokenName,"case")==0){
		verify( lookahead.tokenName);
		case_stat_d();
	}else if(strcmp(lookahead.tokenName,"default")==0){
		verify( lookahead.tokenName);
		verify( ":\0");
		statement_list();
	}else{
		return;
	}

}

void case_stat_d(){
	if(strcmp(lookahead.attribute,"Number")==0){
		verifyAttribute( lookahead.attribute);
		verify(":\0");
		statement_list();
		case_stat();
	}else if(strcmp(lookahead.tokenName,"'")==0){
		verify( lookahead.tokenName);
		verifyAttribute( "ID");
		verify(":\0");
		statement_list();
		verify("'\0");
	}else{
		pErr();
	}
}

char errorString[100];

void verify(char str[]){
	strcpy(st, str);
	strcpy(errorString, str);
	check();
}

void verifyAttribute(char str[]){
	strcpy(st, str);
	strcpy(errorString, str);
	checkAttribute();
}

void check(){
	if(strcmp(lookahead.tokenName,st)==0){
		printf("%s ",st);
		lookahead=getNextToken();
	}else{
		pErr();
	}
}

void checkAttribute(){
	if(strcmp(lookahead.attribute,st)==0){
		printf("%s ",lookahead.tokenName);
		lookahead=getNextToken();
	}else{
		pErr();
	}
}

void pErr(){
	printf(" <---\n");
	printf("---------------------------------\n");
	printf("%s was required\n",errorString);
	printf("Error at Row %d and Column %d \n",lookahead.row, lookahead.col);
	printf("---------------------------------\n");
	exit(0);
}

void nextline(){
	printf("\n");
}

int main()
{
	fp = fopen("testing.c", "r");
	if(fp == NULL){
		printf("Not Found\n");
		exit(0);
	}
	lookahead = getNextToken();
	lookahead = getNextToken();
	proc_program();
	if(strcmp(lookahead.attribute, "END")==0){
		printf("\n----------------------------------\n");
		printf("Successful\n");
		printf("------------------------------------\n");
	}
	return 0;
}
