#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE 500

void decodeMessage(char *msg1,char *msg2){
	
	int n = strlen(msg1);
	int m = strlen(msg2);
	int i,j,k;
	char stack[LINE];
	char *temp;
	int head;
	
	//Empty stack
	for(i = 0;i < LINE;i++ ){
		stack[i] = 0;
	}
	head = -1;
	
	for(i = 0;i < n;i++){
		if(msg1[i] > 47 && msg1[i] < 58){
			k = msg1[i] - '0';
			if(head == -1){
				printf("Gerginden fazla silme sayýsý verildi(%d)\n",k);
				return ;
			}
			else{
				//Poping from stack
				temp = &stack[head - k + 1];
				*temp = '\0';
				head -= k;	
			}
			/*for(j = 0;j < k;j++){
				if(head == -1){
					printf("Gerginden fazla silme sayýsý verildi(%d)\n",k);
					return ;
				}
				else{
					//Poping from stack
					stack[head] = 0;
					head--;	
				}
			}*/
		}
		else{
			//Pushing to stack
			head++;
			stack[head] = msg1[i];
		}
		
		
	}
	
	strcpy(msg1,stack);
	
	
	//Empty stack
	for(i = 0;i < head;i++ ){
		stack[i] = 0;
	}
	head = -1;
	
	for(i = 0;i < m;i++){
		
		if(msg2[i] > 47 && msg2[i] < 58){
			k = msg2[i] - '0';
			if(head == -1){
				printf("Gerginden fazla silme sayisi verildi(%d)\n",k);
				return ;
			}
			else{
				//Poping from stack
			    temp = &stack[head - k + 1];
			    *temp = '\0';
			    head -= k;
			}
			/*for(j = 0;j < k;j++){
				if(head == -1){
					printf("Gerginden fazla silme sayisi verildi(%d)\n",k);
					return ;
				}
				else{
					//Poping from stack
					stack[head] = 0;
					head--;	
				}
			}*/
		}
		else{
			//Pushing to stack
			head++;
			stack[head] = msg2[i];
		}
		
	}
	
	strcpy(msg2,stack);
	
	if(strcmp(msg1,msg2) == 0){
		printf("Bu iki casus iletisime gecebilir\n");
	}
	else{
		printf("Bu iki casus iletisime gecemez\n");
	}
}


int main(){
	char msg1[LINE];
	char msg2[LINE];
	printf("\nKod1:");
	scanf("%s",msg1);
	printf("\nKod2:");
	scanf("%s",msg2);
	
	decodeMessage(msg1,msg2);
	
	return 0;
}
