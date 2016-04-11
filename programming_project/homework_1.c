#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFF_SIZE 256
#define TRUE 1
#define FALSE 0

int main()
{

	char *regx;
	FILE *fp ;
    int index, data;
    char* inp;
	
	int cnt = 0;
	for(cnt = 0; cnt < 5; cnt++)
	{
		printf("\n단어를 입력하세요 : ");
		fgets(regx, 30, stdin);
		int idx = 0;
		for(idx = 0; idx < 30; idx++){
			if(regx[idx] == '\n'){
				regx[idx] = '\0';
			}
		}

		fp = fopen("./sample.txt", "rw");
		inp = malloc(BUFF_SIZE+5);
		int num_line = 0;
		int prev_line = 0;
		int cnt_result = 0;
		while(fgets(inp,BUFF_SIZE,fp)){ // 문장단위
			num_line++;
			if(strlen(inp) != 1){ 
				char *ptr;
				int tmp;
				ptr = strtok(inp," ");
				
				while(ptr != NULL ){ // 단어단위
					int i = 0;
					int q_mark_idx = 0; //?
					int s_mark_idx = 0; //*
					int result = TRUE;
					int regx_idx = 0;
						
						for(i = 0; i < strlen(ptr); i++){
							if(regx[regx_idx] == '?'){
								regx_idx++;
								continue;
							}

							if(regx[regx_idx] == '*'){
								if((regx_idx+1) == strlen(regx)){ //last regx
									regx_idx++;
									break;
									//continue;
								}else{
									result = FALSE;
									if(regx[regx_idx+1] == ptr[i]){
										regx_idx++;
									}
								}
								
							}
						
							if(regx[regx_idx] != '?' && regx[regx_idx] != '*'){
								
								if(regx[regx_idx] == ptr[i]){
									result = TRUE;
									regx_idx++;
								}else{
									result = FALSE;
									break;
								}
							}
						}
						
						if(regx_idx > strlen(regx))
							regx_idx = regx;

						if(regx_idx != strlen(regx))
						{
							result = FALSE;
						}

						if(result){
							if(prev_line != num_line){
								printf("\nline#%d: %s",num_line,ptr);	
							}else{
								printf(" , %s",ptr);
							}

							prev_line = num_line;
							cnt_result++;
						}
	                ptr = strtok(NULL, " ");
	        	}
	            }
	        }
	    fclose(fp);
	    printf("\n\n총 %d 개의 %s 을 찾았습니다.\n",cnt_result,regx);
	}
}
