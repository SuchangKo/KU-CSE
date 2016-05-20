#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFF_SIZE 256
#define LIST_COUNT 26 // A-Z + Others
#define ASCII 1
#define BINARY 2


// strcasecmp -> 대소문자 구분 안함
// strcmp -> 대소문자 구분


typedef struct _node
{
	char *string;
	struct _node* next;
}node;

typedef node* nptr;

typedef struct _list
{
	int count;
	nptr head;
}list;

char *scat(char *s,char *t)
{
	char *p=malloc(strlen(s)+strlen(t)+1);    /* 3: you will have to reserve memory to hold the copy. */
	memset(p,'\0',strlen(s)+strlen(t)+1);
	int ptr =0, temp = 0;                   /* 4 initialise some helpers */

	while(s[temp]!='\0'){                  /* 5. use the temp to "walk" over string 1 */
		p[ptr++] = s[temp++];
	}
	temp=0;
	while(t[temp]!='\0'){                   /* and string two */
		p[ptr++]=t[temp++];
	}
	
	return p;
}


void init(list* lptr){
	lptr->count = 0;
	lptr->head = NULL;
}

void insert(list* lptr[], char* value){
	char index = toupper(value[0]) - 'A';
	
	nptr new_nptr = (node*)malloc(sizeof(node));
	new_nptr->string = (char*)malloc(strlen(value+1));
	strcpy(new_nptr->string,value);
	new_nptr->next = lptr[index]->head;
	lptr[index]->head = new_nptr;
	lptr[index]->count++;

	free(value);
}

int search(list *lptr[], char* value, int debug){
	char index = toupper(value[0]) - 'A';
	int isexist = 0;
	if(lptr[index]->count > 0 ){
		int j;
		nptr tmp_nptr = lptr[index]->head;
		for(j = 0; j < lptr[index]->count; j++){
			if(strcasecmp(tmp_nptr->string,value) == 0){
				isexist = 1;
				break;
			}
			tmp_nptr = tmp_nptr->next;
		}

		if(debug == 0)
			return isexist;

		if(isexist == 1){
			printf("-> 단어장에 있습니다.\n");
		}else{
			printf("-> 단어장에 없습니다.\n");
		}
	}else{
		if(debug == 1)
			printf("단어가 없습니다.\n");
	}
	return isexist;
}

void delete(list *lptr[], char* value){
	char index = toupper(value[0]) - 'A';
	int isexist = 0;
	int level = 0;
	if(lptr[index]->count > 0 ){
		int j;
		nptr tmp_nptr = lptr[index]->head;
		nptr prev_nptr;
		for(j = 0; j < lptr[index]->count; j++){
			if(strcasecmp(tmp_nptr->string,value) == 0){
				free(tmp_nptr->string);
				isexist = 1;
				level = 1;
				lptr[index]->count--;
				if(j == 0){
					lptr[index]->head = tmp_nptr->next;
				}else{
					prev_nptr->next = tmp_nptr->next;
				}
				break;
			}
			prev_nptr = tmp_nptr;
			tmp_nptr = tmp_nptr->next;
		}
		if(isexist == 1){
			printf("-> 삭제되었습니다.\n");
		}else{
			printf("-> 단어장에 없습니다.\n");
		}
	}else{
		printf("단어가 없습니다.\n");
	}
}

void save(list *lptr[], int save_type){
	FILE *fp;
	int i;
	if(save_type == BINARY){
  		if ((fp = fopen("voc.txt", "wb")) == NULL ) {
    		fputs("파일 열기 에러!", stderr);
    		exit(1);
  		}
  		for(i = 0; i < LIST_COUNT; i++){
			int j;
			nptr tmp_nptr = lptr[i]->head;
			for(j = 0; j < lptr[i]->count; j++){
				int k;
				for(k = 0; k < strlen(tmp_nptr->string); k++){
					if(fputc(tmp_nptr->string[k],fp) == EOF){
						fputs("파일 쓰기 에러!", stderr);
      					exit(2);
					}
				}
				if(fputc('\0',fp) == EOF){
						fputs("파일 쓰기 에러!", stderr);
      					exit(2);
					}
				tmp_nptr = tmp_nptr->next;
			}
		}
		fclose(fp); // 파일 닫기
	}else if(save_type == ASCII){
		if ((fp = fopen("voc_ascii.txt", "w")) == NULL ) {
    		fputs("파일 열기 에러!", stderr);
    		exit(1);
  		}
  		for(i = 0; i < LIST_COUNT; i++){
			int j;
			nptr tmp_nptr = lptr[i]->head;
			for(j = 0; j < lptr[i]->count; j++){
				int k;
				for(k = 0; k < strlen(tmp_nptr->string); k++){
					if(fputc(tmp_nptr->string[k],fp) == EOF){
						fputs("파일 쓰기 에러!", stderr);
      					exit(2);
					}
				}
				if(fputc(' ',fp) == EOF){
					fputs("파일 쓰기 에러!", stderr);
      				exit(2);
				}
				tmp_nptr = tmp_nptr->next;
			}
			if(fputc('\r',fp) == EOF){
				fputs("파일 쓰기 에러!", stderr);
      			exit(2);
			}
			if(fputc('\n',fp) == EOF){
				fputs("파일 쓰기 에러!", stderr);
      			exit(2);
			}
		}
		fclose(fp); // 파일 닫기
	}else{
		printf("잘못된 Type 입니다.\n");
	}
}

char* subString (const char* input, int offset, int len, char* dest)
{
  int input_len = strlen (input);

  if (offset + len > input_len)
  {
     return NULL;
  }

  strncpy (dest, input + offset, len);
  dest[len] = '\0';

  return dest;
}

char* onlyalpha(char* s){
	
	int i;
	int idx_first = -1;
	int idx_last = -1;
	int length;
	for(i = 0 ; i < strlen(s); i++){
		if(idx_first == -1){
			if(isalpha(s[i])){
				idx_first = i;
			}
		}
		if(isalpha(s[i])){
			idx_last = i;
		}
	}
	length = idx_last - idx_first + 1;

	char *dest = (char*)malloc(length);
  	subString(s,idx_first,idx_last-idx_first+1,dest);
	return dest;
}

int main(){
	FILE *fp;
	FILE *fpb; //binary File Pointer
	char *inp;
	

	list *mylist[LIST_COUNT];
	int i;

	for(i = 0; i < LIST_COUNT; i++){
		mylist[i] = (list*)malloc(sizeof(list));
		init(mylist[i]);
	}

	fp = fopen("./sample.txt", "r");
	inp = malloc(BUFF_SIZE + 5);

	while (fgets(inp, BUFF_SIZE, fp)) { // 문장단위
		if (strlen(inp) != 1) {
                char *ptr;
                int tmp;
                ptr = strtok(inp, " ");
            while (ptr != NULL) { // 단어단위
            	char *tmp = onlyalpha(ptr);

            	if(strlen(tmp) > 0){
            		if(search(mylist,tmp,0) == 0)
            			insert(mylist,tmp);
            	}
            	ptr = strtok(NULL, " ");
            }
        }
	}

	free(inp);

	int ch;
	char *tmp_str = "";
	char *empty_str = "";
	// test.bin 이라는 이진파일을 읽기 전용으로 열기
	if ( (fpb = fopen("./voc.txt", "rb")) == NULL ) {
		fputs("파일 열기 에러!", stderr);
		exit(1);
	}

  	// 이진 파일, 1바이트씩 읽어, 헥사로 출력
  	while ( (ch = fgetc(fpb)) != EOF ) {
  		if(ch != '\0'){
		    char cToStr[2];
		    cToStr[0] = ch;
		    cToStr[1] = '\0';
		    tmp_str = scat(tmp_str,cToStr);
  		}else{
  			if(search(mylist,tmp_str,0) == 0){
            	insert(mylist,tmp_str);
            	free(tmp_str);
	    	}
            tmp_str = strcpy(tmp_str,empty_str);
  		}
  	}


  	fclose(fpb); // 파일 닫기
  

	while(1){
		
		//Show All Dict.
		/*
		printf("======================\n");
		for(i = 0; i < LIST_COUNT; i++){
			if(i == 26)
				printf("The Others : ");
			else
				printf("%c : ",'A'+i);

			int j;
			nptr tmp_nptr = mylist[i]->head;
			for(j = 0; j < mylist[i]->count; j++){
				printf("%s,",tmp_nptr->string);
				tmp_nptr = tmp_nptr->next;
			}
			printf("\n");
		}
		printf("======================\n");
		*/

		printf("\n기능을 선택하세요\n");
		printf("[1] Search [2] Insert\n");
		printf("[3] Delete [4] Quit\n");
		printf("[5] store in ASCII\n");
		printf("[6] store in binary\n");
		int cmd;
		scanf("%d",&cmd);
		char *cmd_str;
		switch(cmd){
			case 1:{
				printf("찾을 단어를 입력하세요 : ");
				scanf("%s",&cmd_str);
				search(mylist,&cmd_str,1);
				break;
			}
			case 2:{
				printf("삽입할 단어를 입력하세요 : ");
				scanf("%s",&cmd_str);
				if(search(mylist,&cmd_str,0))
					insert(mylist,&cmd_str);	
				break;
			}
			case 3:{
				printf("삭제할 단어를 입력하세요 : ");
				scanf("%s",&cmd_str);
				delete(mylist,&cmd_str);
				break;
			}
			case 4:{
				printf("프로그램을 종료합니다.\n");
				return 0;
				break;
			}
			case 5:{
				printf("ASCII로 저장합니다.\n");
				save(mylist,ASCII);
				
				break;
			}
			case 6:{
				printf("Binary로 저장합니다.\n");
				save(mylist,BINARY);
				break;
			}
			default:{
				printf("다시 입력해주세요\n");
				break;
			}
		}

	}
	

	return 0;
}