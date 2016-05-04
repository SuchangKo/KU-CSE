#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFF_SIZE 256
#define LIST_COUNT 27 // A-Z + Others


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

void init(list* lptr){
	lptr->count = 0;
	lptr->head = NULL;
}

void insert(list* lptr[], char* value){
	char index = toupper(value[0]) - 'A';
	if(lptr[index]->count >= 5)
		index = 26;
	
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

		if(lptr[index]->count == 5 && lptr[26]->count > 0){
			tmp_nptr = lptr[26]->head;
			for(j = 0; j < lptr[26]->count; j++){
				if(strcasecmp(tmp_nptr->string,value) == 0){
					isexist = 1;
					break;
				}
				tmp_nptr = tmp_nptr->next;
			}	
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

		if(lptr[index]->count == 5 && lptr[26]->count > 0){
			tmp_nptr = lptr[26]->head;
			for(j = 0; j < lptr[26]->count; j++){
				if(strcasecmp(tmp_nptr->string,value) == 0){
					free(tmp_nptr->string);
					isexist = 1;
					level = 2;
					lptr[26]->count--;
					if(j == 0){
						lptr[26]->head = tmp_nptr->next;
					}else{
						prev_nptr->next = tmp_nptr->next;
					}
					break;
				}
				prev_nptr = tmp_nptr;
				tmp_nptr = tmp_nptr->next;
			}	
		}

		//Compact
		if(lptr[index]->count == 4 && level == 1){
			tmp_nptr = lptr[26]->head;
			for(j = 0; j < lptr[26]->count; j++){
				if(toupper(tmp_nptr->string[0]) == 'A'+index ){
					char *tmp_str = (char*)malloc(strlen(tmp_nptr->string)+1);
					strcpy(tmp_str,tmp_nptr->string);
					free(tmp_nptr->string);
					lptr[26]->count--;
					if(j == 0){
						lptr[26]->head = tmp_nptr->next;
					}else{
						prev_nptr->next = tmp_nptr->next;
					}
					insert(lptr,tmp_str);
					break;
				}
				prev_nptr = tmp_nptr;
				tmp_nptr = tmp_nptr->next;
			}
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


	
	while(1){
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
			default:{
				printf("다시 입력해주세요\n");
				break;
			}
		}

	}
	

	return 0;
}