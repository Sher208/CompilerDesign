#include<stdio.h>
#include<string.h>
#include<stdlib.h>


struct token{
	char tokenName[100];
	int row;
	int col;
	char attribute[100];
};


struct token to;
struct token rt;

char c;
FILE *fp;
int state=0;
int j=0;
int r=1;
int co=0;
char strBuff[100];
int k=0;
int sp=0;
int flag=0;
char special[] = ";(){},[]$':";
char spBuff[10];
char keywords[20][20] = {
	"if", "return","else","break","int","float"
	,"double","char","continue","void","for","switch","case", "default"
};

char dataTypeKey[20][20] = {
	"int","float","double","char","void"
};

struct token store(char t[], int rr, int cc, char att[]){
	strcpy(to.tokenName, t);
	to.row = r;
	to.col = co;
	strcpy(to.attribute, att);
	return to;
}

void removingCommentsDivide(){
	c = getc(fp);
	co++;
	if(c=='/'){
		c = getc(fp);
		co++;
		while(1){
			if(c=='\n'){
				r++;
				co=-1;
				break;
			}
			c = getc(fp);
		}
	}else if(c == '*'){
		c = getc(fp);
		while(1){
			if(c=='\n'){
				r++;
				co--;
			}
			c = getc(fp);
			if(c=='*'){
				c = getc(fp);
				if(c=='/'){
					break;
				}
			}
		}
	}else{
		rt = store("/",r,co,"AE");
		fseek(fp, -1, SEEK_CUR);
		c--;
	}
}

struct token literals(){
	int latnum = 0;
	c = getc(fp);
	co++;
	strBuff[k++]=c;
	c = getc(fp);
	latnum++;
	while(c!='"'){
		strBuff[k++] = c;
		c= getc(fp);
		latnum++;
	}
	strBuff[k]='\0';
	rt = store(strBuff,r,co,"LITERALS");
	co+=latnum;
	k=0;
	return rt;
}

void removingPreprocessors(){
	c=getc(fp);
	co++;
	while(c!='\n'){
		c=getc(fp);
	}
	r++;
	co=-1;
}

struct token keywordsIdentifier(){
	int idnum = 0;
	strBuff[k++] = c;
	c= getc(fp);
	idnum++;
	while((c>='0' && c<='9') || (c>='A' && c<='Z') || (c>='a' && c<='z')){
		strBuff[k++] = c;
		c = getc(fp);
		idnum++;
	}
	strBuff[k] = '\0';
	int len = sizeof(keywords) / sizeof(keywords[0]);
	for (int i = 0; i < len; ++i)
	{
		if(strcmp(keywords[i], strBuff)==0){
			rt = store(strBuff,r,co,"KEYWORD");
			flag = 1;
			break;
		}
	}
	if(flag==0){
		rt = store(strBuff,r,co,"ID");
	}
	flag=0;
	k=0;
	fseek( fp, -1, SEEK_CUR );
	co--;
	co += idnum;
	return rt;
}

struct token specialCharacters(){
	for(int i=0; i<11; i++){
		if(special[i]==c){
			spBuff[sp++]=c;
			spBuff[sp]='\0';
			rt = store(spBuff,r,co,"SP");
			sp=0;
			break;
		}
	}
	return rt;
}

struct token numbers(){
	int num=0;
	strBuff[k++] = c;
	c = getc(fp);
	num++;
	while(c>='0' && c<='9'){
		strBuff[k++] = c;
		c= getc(fp);
		co++;
	}
	strBuff[k] = '\0';
	k=0;
	rt = store(strBuff,r,co,"Number");
	fseek( fp, -1, SEEK_CUR );
	co--;
	co+= num;
	return rt;
}

struct token getn(){
		if(c==EOF){
			return store("END",0,0,"END");
			exit(0);
		}
		switch(state){
			case 0:
				c = getc(fp);
				co++;
				if(c=='>'){
					state = 1;
				}else if(c=='='){
					state = 2;
				}else if(c=='+'){
					state = 3;
				}else if(c=='-'){
					state = 4;
				}else if(c=='*'){
					state = 5;
				}else if(c=='/'){
					state = 6;
				}else if(c=='"'){
					state = 7;
				}else if(c=='#'){
					state = 8;
				}else if(c=='<'){
					state = 9;
				}else if(c=='|'){
					state = 10;
				}else if(c=='&'){
					state = 11;
				}else if(c=='_' || (c>='A' && c<='Z') || (c>='a' && c<='z')){
					state = 12;
				}else if(c>='0' && c<='9'){
					state = 13;
				}else if(c=='\n'){
					r++;
					co=-1;
				}else{
					rt = specialCharacters();
				}
				break;
			case 1: 
				c = getc(fp);
				co++;
				if(c=='='){
					rt = store(">=",r,co,"RE");
				}else{
					rt = store(">",r,co,"RE");
					fseek(fp, -1, SEEK_CUR);
					c--;
				}
				state=0;
				break;
			case 2: 
				c = getc(fp);
				co++;
				if(c=='='){
					rt = store("==",r,co,"RE");
				}else{
					rt = store("=",r,co,"AE");
					fseek(fp, -1, SEEK_CUR);
					c--;
				}
				state=0;
				break;
			case 3: 
				c = getc(fp);
				co++;
				if(c=='+'){
					rt = store("++",r,co,"RE");
				}else if(c=='='){
					rt = store("+=",r,co,"RE");
				}else{
					rt = store("+",r,co,"AE");
					fseek(fp, -1, SEEK_CUR);
					c--;
				}
				state=0;
				break;
			case 4: 
				c = getc(fp);
				co++;
				if(c=='-'){
					rt = store("--",r,co,"RE");
				}else if(c=='='){
					rt = store("-=",r,co,"RE");
				}else{
					rt = store("-",r,co,"AE");
					fseek(fp, -1, SEEK_CUR);
					c--;
				}
				state=0;
				break;
			case 5: 
				c = getc(fp);
				co++;
				if(c=='='){
					rt = store("*=",r,co,"RE");
				}else{
					rt = store("*",r,co,"AEM");
					fseek(fp, -1, SEEK_CUR);
					c--;
				}
				state=0;
				break;
			case 6: 
				removingCommentsDivide();
				state=0;
				break;
			case 7:
				literals();
				state=0;
				break;
			case 8:
				removingPreprocessors();
				state=0;
				break;
			case 9: 
				c = getc(fp);
				co++;
				if(c=='='){
					rt = store("<=",r,co,"RE");
				}else{
					rt = store("<",r,co,"RE");
					fseek(fp, -1, SEEK_CUR);
					c--;
				}
				state=0;
				break;
			case 10: 
				c = getc(fp);
				co++;
				if(c=='|'){
					rt = store("||",r,co,"LO");
				}else{
					rt = store("|",r,co,"LO");
					fseek(fp, -1, SEEK_CUR);
					c--;
				}
				state=0;
				break;
			case 11: 
				c = getc(fp);
				co++;
				if(c=='&'){
					rt = store("&&",r,co,"LO");
				}else{
					rt = store("&",r,co,"LO");
					fseek(fp, -1, SEEK_CUR);
					c--;
				}
				state=0;
				break;
			case 12: 
				rt= keywordsIdentifier();
				state=0;
				break;
				return rt;
			case 13: 
				rt= numbers();
				state=0;
				break;	
		}
	return rt;
}

struct token la;
struct token prev;
struct token firstCase;

struct token getUniqueToken(){
	la = getn();
	if(strcmp(la.tokenName, prev.tokenName)==0 && la.row == prev.row && la.col == prev.col){
		return store("XX",0,0,"XX");
	}else{
		prev = la;
		return la;
	}
}

struct token getNextToken(){
	la = getUniqueToken();
	while(strcmp(la.tokenName, "XX")==0){
		la = getUniqueToken();
		// if(strcmp(la.attribute, "END") == 0){
		// 	exit(1);
		// }
	}
	return la;
}

// struct token get;
// int main()
// {
// 	fp = fopen("testing.c", "r");
// 	if(fp == NULL){
// 		printf("Not Found\n");
// 		exit(0);
// 	}
// 	while(1){
// 		get = getNextToken();
// 		printf("%s-",get.tokenName );
// 		printf("%d-",get.row );
// 		printf("%d-",get.col );
// 		printf("%s\n",get.attribute );
// 	}
// 	return 0;
// }



// int main(){
// 	array = (struct arrayToken*) malloc(max * sizeof(struct arrayToken*));
// 	init_array();
// 	fp = fopen("test.c", "r");
// 	if(fp == NULL){
// 		printf("Not Found\n");
// 		exit(0);
// 	}
// 	getNextToken();
// 	display();
// 	getSymbolTable();
// 	displayT();

// }