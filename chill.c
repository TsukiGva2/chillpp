/*
 * File:	chill.c
 * Author:	TsukiGva -> tsukigva@gmail.com
 * Description:	CHILL programming language
 * Note: i'm the WORST programmer in earth
 * so dont judge me for my code, just fix it.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* we need a header file, the next 100+ lines are a mess!
 * i dont care.
 */

#define MAXL 700

// ANSI COLORS!
#define RED "\033[31;1m"
#define GRN "\033[32;1m"
#define YLW "fixme "
#define BLU "\033[34;1m"
#define PNK "fixme too"

// return values
#define COMMENT 2
#define STOPPED 3
#define SKIPPED 4
#define OPERROR 5

// we need a pretty code!
#define TEMPINT int
#define COUNTER int

int nR 	        = 0;
int used        = 0;
int nused       = 0;
char *keyw[7]   = {"yell","say","maybe","?NUM","skip","repeat","paint"};
char *aritm[11] = {"is","add","sub","mul","pow","div","num","mod","store-in","load-from","ask"};
char *comp[4]   = {"eqs",">","<","not"};
char *colors[6]	= {"red","green","yellow","blue","pink",""};

// pow_(5,2) = 5*5 = 25
int pow_(int base,int exp){
	long long result = 1;
	while(exp != 0){
		result *= base;
		--exp;
	}
	return(result);
}

int cvrt(char cvrtd){
	int byt;
	byt = (int)cvrtd - 48 * (cvrtd <= '9' && cvrtd >= '0');
	return(byt);
}

char *sbst(char *source,int spos,int epos){
	char *sub;
	char str[MAXL];
	sub = str;
	spos += 1;
	epos += 1;
	int len = epos - spos;
	int c = 0;
	while(c < len){
		sub[c] = source[spos+c-1];
		c++;
	}
	sub[c] = '\0';
	return(sub);
}

int lookfor_vn(char **vnames,char *wha){
	int w_len = strlen(wha);
	int len = 0;
	int match = 0;
	for(int t = 0;t < nused;t++){
		len = strlen(vnames[t]);
		for(int d = 0;d < len;d++){
			for(int n = 0;n < w_len;n++){
				if(vnames[t][d] != wha[n]){
					continue;
				}
				match += 1;
				if(match == w_len){
					return(t);
				}
			}
		}
	}
	return(-1);
}

int lookfor_var(char **varnames,char *what){
	int w_len = strlen(what);
	int len = 0;
	int match = 0;
	for(int t = 0;t < used;t++){
		len = strlen(varnames[t]);
		for(int d = 0;d < len;d++){
			for(int n = 0;n < w_len;n++){
				if(varnames[t][d] != what[n]){
					continue;
				}
				match += 1;
				if(match == w_len){
					return(t);
				}
			}
		}
	}
	return(-1);
}

int lookfor(char *what,char *where){
	int len = strlen(where);
	int s_len = strlen(what);
	int match = 0;
	for(int i = 0;i <= len;i++){
		for(int d = 0;d <= s_len;d++){
			if(where[i + d] != what[d]){
				break;
			}
			match += 1;
			if(match == s_len){
				return(i);
			}
		}
	}
	return(len);
}

void upperc(char *d,int count){
	for(int l = 0;l < count;l++){
		d[l] -= 32 * (d[l] >= 'a' && d[l] <= 'z');
	}
}

int dothings(char *ln,int lnum,int *ign,int *kgoin,char **varnames,char **values,char **nvnames,int *nvals){
	if(lnum == 0){
		printf(" \b");
	}
	if(lookfor("nop",sbst(ln,0,5)) != strlen(sbst(ln,0,5))){
		return(COMMENT);
	}
	if(*ign > 0){
		*ign -= 1;
		return(SKIPPED);
	}
	if(*kgoin != 0){
		*kgoin = 0;
		return(STOPPED);
	}
	char *slice = sbst(ln,0,lookfor(";",ln));
	char *newl = "\n";
	for(int i = 7;i > -1;i--){
		if(lookfor(keyw[i],ln) != strlen(ln)){
			if(keyw[i] == keyw[5]){
				printf("%s",sbst(ln,lookfor(keyw[5],ln) + strlen(keyw[5]) + 1,lookfor(";",ln)));
				return(0);
			}
			else if(keyw[i] == keyw[6]){
				for(int x = 0;x < 5;x++){
					if(lookfor(colors[x],ln) != strlen(ln)){
						switch(x){
							case 0:
								printf("%s",RED);
							break;
							case 1:
								printf("%s",GRN);
							break;
							case 2:
								printf("%s",YLW);
							break;
							case 5:
								printf("%s",YLW);
							break;
							case 3:
								printf("%s",BLU);
							break;
							case 4:
								printf("%s",PNK);
							break;
						}
					}
				}
			}
			else if(keyw[i] == keyw[1]){
				if(lookfor("/",ln) != strlen(ln) && lookfor("%",ln) != strlen(ln)){
					int name = lookfor_var(varnames,sbst(ln,lookfor("/",ln) + 1,lookfor("%",ln)));
					if(name != -1){
						printf("%s%s%s\n",sbst(ln,lookfor(keyw[1],ln) + strlen(keyw[1]) + 1,lookfor("/",ln)),values[name],sbst(ln,lookfor("%",ln) + 1,lookfor(";",ln)));
						return(0);
					} else {
						printf("%s\n",sbst(slice,lookfor(keyw[1],slice) + strlen(keyw[1]) + 1,strlen(slice)));
						return(0);
					}
				} else if(lookfor("!",ln) != strlen(ln) && lookfor("%",ln) != strlen(ln)){
					int name = lookfor_vn(nvnames,sbst(ln,lookfor("!",ln) + 1,lookfor("%",ln)));
					if(name != -1){
						printf("%s%d%s\n",sbst(ln,lookfor(keyw[1],ln) + strlen(keyw[1]) + 1,lookfor("!",ln)),nvals[name],sbst(ln,lookfor("%",ln) + 1,lookfor(";",ln)));
						return(0);
					} else {
						printf("%s\n",sbst(slice,lookfor(keyw[1],slice) + strlen(keyw[1]) + 1,strlen(slice)));
						return(0);
					}
				} else if(lookfor("%NUM%",ln) != strlen(ln)) {
					printf("%s%d%s\n",sbst(ln,lookfor(keyw[1],ln) + strlen(keyw[1]) + 1,lookfor("%NUM%",ln)),nR,sbst(ln,lookfor("%NUM%",ln) + strlen("%NUM%"),lookfor(";",ln)));
					return(0);
				} else {
					printf("%s\n",sbst(slice,lookfor(keyw[1],slice) + strlen(keyw[1]) + 1,strlen(slice)));
					return(0);
				}
			}
			else if(keyw[i] == keyw[0]){
				char *tbprintd = sbst(slice,lookfor(keyw[0],slice) + strlen(keyw[0]) + 1,strlen(slice));
				upperc(tbprintd,strlen(tbprintd));
				printf("%s\n",tbprintd);
			}
			else if(keyw[i] == keyw[2]){
				int ranum;
				ranum = (rand() % 2);
				*kgoin = ranum;
			}
			else if(keyw[i] == keyw[4]){
				char *ignoredLines = sbst(ln,lookfor(keyw[4],ln) + strlen(keyw[4]) + 1,lookfor(";",ln));
				int conv[strlen(ignoredLines)];
				for(int r = 0;r < strlen(ignoredLines);r++){
					conv[r] = cvrt(ignoredLines[r]);
				}
				int arrlen = sizeof(conv) / 4;
				COUNTER cR = 0;
				for(int w = 0;w < arrlen;w++){
					if(arrlen - (w + 1) != 0){
						cR += conv[w] * pow_(10,arrlen - (w + 1));
					}
					else {
						cR += conv[w];
					}
				}
				*ign = cR;
			}
			else if(keyw[i] == keyw[3]){
				char *sub;
				int operator = -1;
				for(int v = 0;v < 4;v++){
					if(lookfor(comp[v],ln) != strlen(ln)){
						char *sub = sbst(ln,lookfor(keyw[3],ln) + strlen(keyw[3]) + 1,lookfor(comp[v],ln) - 1);
						operator = v;
						break;
					}
				}
				if(operator == -1){
					return(OPERROR);
				}
				int conv[strlen(sub)];
				for(int e = 0;e < strlen(sub);e++){
					conv[e] = cvrt(sub[e]);
				}
				int arrlen = sizeof(conv) / 4;
				TEMPINT Comp_nR = 0;
				for(int h = 0;h < arrlen;h++){
					if(arrlen - (h + 1) != 0){
						Comp_nR += conv[h] * pow_(10,arrlen - (h + 1));
					}
					else {
						Comp_nR += conv[h];
					}
				}
				switch(operator){
					case 0:
						if(Comp_nR == nR){
							
						} else {
							*kgoin = 1;
						}
					break;
					case 1:
						if(Comp_nR > nR){
							
						} else {
							*kgoin = 1;
						}
					break;
					case 2:
						if(Comp_nR < nR){
							
						} else {
							*kgoin = 1;
						}
					break;
					case 3:
						if(Comp_nR != nR){
							
						} else {
							*kgoin = 1;
						}
					break;
				}
			}
		}
	}
	for(int l = 11;l > -1;l--){
		if(lookfor(aritm[l],ln) != strlen(ln)){
			if(aritm[l] == aritm[0]){
				if(used >= used + 100){
					char *more = realloc(varnames,used + 100 * sizeof(char[MAXL]));
					*varnames = more;
					more = realloc(values,used + 100 * sizeof(char[MAXL]));
					*values = more;
				}
				int name = lookfor_var(varnames,sbst(ln,0,lookfor(aritm[0],ln)));
				/* NUNCA FAÇA SUBSTRING EM UMA SUBSTRING */
				if(name != -1){
					strcpy(values[name],sbst(ln,lookfor(aritm[0],ln) + strlen(aritm[0]) + 1,lookfor(";",ln)));
				}
				else {
					varnames[used] = malloc(MAXL);
					strcpy(varnames[used],sbst(ln,0,lookfor(aritm[0],ln) - 1));
					values[used] = malloc(MAXL);
					strcpy(values[used],sbst(ln,lookfor(aritm[0],ln) + strlen(aritm[0]) + 1,lookfor(";",ln)));
					used += 1;
				}
			}
			else if(aritm[l] == aritm[10]){
				scanf("%d",&nR);
			}
			else if(aritm[l] == aritm[6]){
				int index = lookfor_var(varnames,sbst(ln,lookfor("num",ln) + 4,lookfor(";",ln)));
				if(index != -1){
					int conv[strlen(values[index])];
					for(int b = 0;b < strlen(values[index]);b++){
						conv[b] = cvrt(values[index][b]);
					}
					int arrlen = sizeof(conv) / 4;
					nR = 0;
					for(int s = 0;s < arrlen;s++){
						if(arrlen - (s + 1) != 0){
							nR += conv[s] * pow_(10,arrlen - (s + 1));
						}
						else{
							nR += conv[s];
						}
					}
				}
			}
			else if(aritm[l] == aritm[8]){
				if(nused > nused + 95){
					int *imore = realloc(nvals,nused + 100 * sizeof(int*));
					*nvals = *imore;
					char *more = realloc(nvnames,nused + 100 * sizeof(char[MAXL]));
					*nvnames = more;
				}
				nvnames[nused] = malloc(MAXL);
				strcpy(nvnames[nused],sbst(ln,lookfor(aritm[8],ln) + strlen(aritm[8]) + 1,lookfor(";",ln)));
				nvals[nused] = nR;
				nused += 1;
			}
			else if(aritm[l] == aritm[9]){
				int inDx = lookfor_vn(nvnames,sbst(ln,lookfor("!",ln) + 1,lookfor("%",ln)));
				if(inDx != -1){
					nR = nvals[inDx];
				}
			}
			else if(l < 8 && l != 6){
				char *subs = sbst(ln,lookfor(aritm[l],ln) + strlen(aritm[l]) + 1,lookfor(";",ln));
				int snR = 0;
				if(lookfor("!",ln) != strlen(ln) && lookfor("%",ln) != strlen(ln)){
					int inDx = lookfor_vn(nvnames,sbst(ln,lookfor("!",ln) + 1,lookfor("%",ln)));
					if(inDx != -1){
						snR = nvals[inDx];
					} else {
						return(OPERROR);
					}
				} else {
					int conv[strlen(subs)];
					for(int m = 0;m < strlen(subs);m++){
						conv[m] = cvrt(subs[m]);
					}	
					int arrlen = sizeof(conv) / 4;
					for(int h = 0;h < arrlen;h++){
						if(arrlen - (h + 1) != 0){
							snR += conv[h] * pow_(10,arrlen - (h + 1));
						}
						else {
							snR += conv[h];
						}
					}
				}
				switch(l){
					case 1:
						nR += snR;
					break;
					case 2:
						nR -= snR;
					break;
					case 3:
						nR *= snR;
					break;
					case 4:
						nR = pow_(nR,snR);
					break;
					case 5:
						if(nR > 0 && snR > 0){
							nR = nR / snR;
						}
					break;
					case 7:
						if(nR > 0 && snR > 0){
							nR = nR % snR;
						}
					break;
				}
			}
		}
	} 
exit:
	return(0);
}

int main(int argc, char *argv[]){
	srand(time(NULL));
	char *(*varnames) = malloc(used + 100 * sizeof(char[MAXL]));
	char *(*numvarnames) = malloc(nused + 100 * sizeof(char[MAXL]));
	char *(*values) = malloc(used + 100 * sizeof(char[MAXL]));
	int (*nvalues) = malloc(nused + 100 * sizeof(int));
	varnames[0] = malloc(MAXL);
	strcpy(varnames[0],"haoifhuifgadgf");	
	values[0] = malloc(MAXL);
	strcpy(values[0],"100");
	used += 1;
	if(argc > 1){
		if((strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--file") == 0) && argc > 2){
			char  line[MAXL];
			FILE *filePtr;
			char  l_line[MAXL];
			int   linum   = 0;
			int   kgoin   = 0;
			int   ignore  = 0;
			int   savec   = 0;
			if((filePtr = fopen(argv[2], "r")) == NULL){
				printf("\nError opening file\n");
				return(1);
			}
			while(fgets(line, MAXL, filePtr) != NULL){
				dothings(line,linum,&ignore,&kgoin,varnames,values,numvarnames,nvalues);
				linum += 1;
			}
			fclose(filePtr);
			free(varnames);
			free(values);
			free(numvarnames);
			free(nvalues);
		}
		else if(strcmp("-v",argv[1]) == 0){
			printf("CHILL programming language, -alpha- v0.2\n");
		}
	}
	return(0);
}
