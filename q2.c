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

void proc_program(){
	if(strcmp(lookahead.tokenName,"main")==0){
		strcpy(st, "main");
		check();
		strcpy(st,"(");
		check();
		strcpy(st,")");
		check();
		strcpy(st,"{");
		check();
		//for next line
		nextline();
		//for next line finish
		declaration();
		statement_list();
		strcpy(st,"}");
		check();
	}else{
		pErr();
	}
}

void declaration(){
	if(strcmp(lookahead.tokenName,"int")==0){
		data_type();
		identifier_list();
		strcpy(st, ";\0");
		check();
		declaration();
	}else if(strcmp(lookahead.tokenName,"char")==0){
		data_type();
		identifier_list();
		strcpy(st, ";\0");
		check();
		declaration();
	}else{
		return;
	}
}

void data_type(){
	if(strcmp(lookahead.tokenName,"int")==0){
		strcpy(st, "int\0");
		check();
	}else if(strcmp(lookahead.tokenName,"char")==0){
		strcpy(st, "char\0");
		check();
	}else{
		pErr();
	}
}

void identifier_list(){
	if(strcmp(lookahead.attribute,"ID")==0){
		strcpy(st, lookahead.tokenName);
		check();
		identifier_list_1();
	}else{
		pErr();
	}
}

void identifier_list_1(){
	if(strcmp(lookahead.tokenName,",")==0){
		strcpy(st, ",\0");
		check();
		identifier_list();
	}else if(strcmp(lookahead.tokenName,"[")==0){
		strcpy(st, "[\0");
		check();
		if(strcmp(lookahead.attribute,"Number")==0){
			strcpy(st, lookahead.tokenName);
			check();
		}else{
			pErr();
		}
		strcpy(st, "]\0");
		check();
		identifier_list_2();
	}else{
		return;
	}
}

void identifier_list_2(){
	if(strcmp(lookahead.tokenName,",")==0){
		strcpy(st, ",\0");
		check();
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
	}else{
		return;
	}
}

void statement(){
	if(strcmp(lookahead.attribute,"ID")==0){
		assign_stat();
		strcpy(st, ";\0");
		check();
	}else if(strcmp(lookahead.tokenName,"if")==0){
		decision_stat();
	}else if(strcmp(lookahead.tokenName,"while")==0){
		looping_stat();
	}else if(strcmp(lookahead.tokenName,"for")==0){
		looping_stat();
	}else{
		pErr();
	}
}

void assign_stat(){
	if(strcmp(lookahead.attribute,"ID")==0){
		strcpy(st, lookahead.tokenName);
		check();
		strcpy(st, "=\0");
		check();
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
		strcpy(st, lookahead.tokenName);
		check();
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
		strcpy(st, lookahead.tokenName);
		check();
	}else if(strcmp(lookahead.attribute,"Number")==0){
		strcpy(st, lookahead.tokenName);
		check();
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
		strcpy(st, lookahead.tokenName);
		check();
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
		strcpy(st, lookahead.tokenName);
		check();
	}else{
		pErr();
	}
}

void decision_stat(){
	if(strcmp(lookahead.tokenName,"if")==0){
		strcpy(st, lookahead.tokenName);
		check();
		strcpy(st, "(\0");
		check();
		expn();
		strcpy(st, ")\0");
		check();
		strcpy(st, "{\0");
		check();
		statement_list();
		strcpy(st, "}\0");
		check();
		dprime();
	}else {
		pErr();
	}
}

void dprime(){
	if(strcmp(lookahead.tokenName,"else")==0){
		strcpy(st, lookahead.tokenName);
		check();
		strcpy(st, "{\0");
		check();
		statement_list();
		strcpy(st, "}\0");
		check();
	}else {
		return;
	}
}

void looping_stat(){
	if(strcmp(lookahead.tokenName,"while")==0){
		strcpy(st, lookahead.tokenName);
		check();
		strcpy(st, "(\0");
		check();
		expn();
		strcpy(st, ")\0");
		check();
		strcpy(st, "{\0");
		check();
		statement_list();
		strcpy(st, "}\0");
		check();
	}else if(strcmp(lookahead.tokenName,"for")==0){
		strcpy(st, lookahead.tokenName);
		check();
		strcpy(st, "(\0");
		check();
		assign_stat();
		strcpy(st, ";\0");
		check();
		expn();
		strcpy(st, ";\0");
		check();
		assign_stat();
		strcpy(st, ")\0");
		check();
		strcpy(st, "{\0");
		check();
		statement_list();
		strcpy(st, "}\0");
		check();
	}else{
		pErr();
	}
	//for next line
		nextline();
		//for next line finish
}



void check(){
	if(strcmp(lookahead.tokenName,st)==0){
		printf("%s ",st );
		lookahead=getNextToken();
	}else{
		pErr();
	}
}


void pErr(){
	printf(" <---\n");
	printf("%s was required\n",lookahead.attribute );
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
	if(strcmp(la.attribute, "END")==0){
		printf("\nSuccessful\n");
	}
	return 0;
}
