#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 255

typedef struct adj{
    int addr;
    struct adj *next;
}Adj;

typedef struct node{
    char *query_str;
    int query_num;
    Adj *adjacent_list_head;
}Node;

typedef struct graph{
    int node_num;
    Node *node_list;
}Graph;

int findNode(Graph *one_graph,char *str,int len){
    Node *itr_node = one_graph->node_list;
    int i;
    
    for(i = 0;i < len;i++){
        if(strcmp(itr_node[i].query_str,str) == 0){
            return i;
        }
    }
    
    return -1;
}

Adj *findAdj(Node one_node,int index_adj){
    Adj *itr_adj = one_node.adjacent_list_head;
    
    while(itr_adj != NULL){
        if(itr_adj->addr == index_adj){
            return itr_adj;
        }
        itr_adj = itr_adj->next;
    }
    
    return NULL;
}

Adj *getLastAdj(Node one_node){
    Adj *itr_adj = one_node.adjacent_list_head;
    
    if(itr_adj != NULL){
        while(itr_adj->next != NULL){
            itr_adj = itr_adj->next;
        }
        return itr_adj;
    }
    
    return itr_adj;
    
}

void addNodeandAdjacent(Graph *one_graph,char *str1,char *str2,int num1,int num2,int *list_size){
    
    Adj *current_adj1,*current_adj2,*tmp_adj;
    int addr_node1,addr_node2;
    
    if((*list_size) == 0){
        one_graph->node_list = (Node *)malloc(one_graph->node_num * sizeof(Node));
        /*int i;
        for(i = 0;i < one_graph->node_num;i++){
        	one_graph->node_list[i] = NULL;
    	}*/
    }
    
    addr_node1 = findNode(one_graph,str1,*(list_size));
    if(addr_node1 == -1){
        ++(*list_size);
        addr_node1 = (*list_size) - 1;
        /*if( (one_graph->node_list[addr_node1].query_str = malloc(strlen(str1) + 1)) == NULL)
			return ;*/
		one_graph->node_list[addr_node1].query_str = malloc(strlen(str1) + 1);
        strcpy(one_graph->node_list[addr_node1].query_str,str1);
        one_graph->node_list[addr_node1].query_num = num1;
        one_graph->node_list[addr_node1].adjacent_list_head = NULL;
    }
    
    addr_node2 = findNode(one_graph,str2,*(list_size));
    if(addr_node2 == -1){
        ++(*list_size);
        addr_node2 = (*list_size) - 1;
        if( (one_graph->node_list[addr_node2].query_str = malloc(strlen(str2) + 1)) == NULL)
			return ;
        strcpy(one_graph->node_list[addr_node2].query_str,str2);
        one_graph->node_list[addr_node2].query_num = num2;
        one_graph->node_list[addr_node2].adjacent_list_head = NULL;
    }
    
    current_adj1 = findAdj(one_graph->node_list[addr_node1],addr_node2);
    if(current_adj1 == NULL){
        current_adj1 = (Adj*)malloc(sizeof(Adj));
        current_adj1->next = NULL;
        current_adj1->addr = addr_node2;
        tmp_adj = getLastAdj(one_graph->node_list[addr_node1]);
        if(tmp_adj == NULL){
            one_graph->node_list[addr_node1].adjacent_list_head = current_adj1;
        }
        else{
            tmp_adj->next = current_adj1; 
        }
    }

    current_adj2 = findAdj(one_graph->node_list[addr_node2],addr_node1);
    if(current_adj2 == NULL){
        current_adj2 = (Adj*)malloc(sizeof(Adj));
        current_adj2->next = NULL;
        current_adj2->addr = addr_node1;
        tmp_adj = getLastAdj(one_graph->node_list[addr_node2]);
        if(tmp_adj == NULL){
            one_graph->node_list[addr_node2].adjacent_list_head = current_adj2;
        }
        else{
            tmp_adj->next = current_adj2; 
        }
    }
    
}

void mergeTwoGraph(Graph *graph_one,Graph *graph_two,Graph *graph_three,int *size_of_graph){
	
	Node *current_node = graph_three->node_list;
	Node *tmp_node;
	Adj *tmp_adj,*current_adj;
	int i,j,size1,size2,extra,addr_node,addr_adj,end,addr_node1;
	
	for(i = 0;i < graph_two->node_num;i++){
		addr_node = findNode(graph_one,graph_two->node_list[i].query_str,graph_one->node_num);
		if(addr_node == -1){
			extra++;
		}
	}
	
	graph_three->node_num = graph_one->node_num + extra;
	graph_three->node_list = (Node *)malloc(graph_three->node_num * sizeof(Node));
	for(i = 0;i < graph_one->node_num;i++){
		graph_three->node_list[i] = graph_one->node_list[i];
	}
	
	size1 = graph_one->node_num;
	for(i = 0;i < graph_two->node_num;i++){
		addr_node = findNode(graph_three,graph_two->node_list[i].query_str,size1);
		
		if(addr_node == -1){
			
			graph_three->node_list[size1].query_str = (char*)malloc(strlen(graph_two->node_list[i].query_str) * sizeof(char) + 1);
			strcpy(graph_three->node_list[size1].query_str,graph_two->node_list[i].query_str);
			graph_three->node_list[size1].query_num = graph_two->node_list[i].query_num;
			graph_three->node_list[size1].adjacent_list_head = NULL;
			size1++;
			
			current_adj = graph_two->node_list[i].adjacent_list_head;
			
			/*while(current_adj != NULL){
				addr_node1 = findNode(graph_three,graph_two->node_list[current_adj->addr].query_str,size1);
				
				if(addr_node1 != -1){
			        Adj *current_adj1 = (Adj*)malloc(sizeof(Adj));
			        current_adj1->next = NULL;
			        current_adj1->addr = size1;
			        tmp_adj = getLastAdj(graph_three->node_list[addr_node1]);
			        if(tmp_adj == NULL){
			            graph_three->node_list[addr_node1].adjacent_list_head = current_adj1;
			        }
			        else{
			            tmp_adj->next = current_adj1; 
			        }
				}
				
				current_adj = current_adj->next;
			}*/
			
		}
	}
	
	for(i = 0;i < graph_three->node_num;i++){
		addr_node = findNode(graph_two,graph_three->node_list[i].query_str,graph_two->node_num);
		
		if(addr_node != -1){
			current_adj = graph_two->node_list[addr_node].adjacent_list_head;
			while(current_adj != NULL){
				
				addr_node1 = findNode(graph_three,graph_two->node_list[current_adj->addr].query_str,graph_three->node_num);
				
				Adj *current_adj1 = (Adj*)malloc(sizeof(Adj));
				current_adj1->next = NULL;
				current_adj1->addr = addr_node1;
				tmp_adj = getLastAdj(graph_three->node_list[i]);
				if(tmp_adj == NULL){
					graph_three->node_list[i].adjacent_list_head = current_adj1;
				}
				else{
				    tmp_adj->next = current_adj1; 
				}
				        
				current_adj = current_adj->next;
			}
			
		}
		
	}
	
	(*size_of_graph) = graph_three->node_num;
	
}

void printGraph(Graph *one_graph,int len){
    Node *current_node = one_graph->node_list;
    Adj *current_adj;
    int i;
    
    for(i = 0;i < len;i++){
        printf("%s(%d):",current_node[i].query_str,current_node[i].query_num);
        current_adj = current_node[i].adjacent_list_head;
        while(current_adj != NULL){
            printf("%s(%d) ",one_graph->node_list[current_adj->addr].query_str
                   ,one_graph->node_list[current_adj->addr].query_num);
            printf("-> ");
            current_adj = current_adj->next;
        }
        printf("|\n");
    }
    
}

void searchInGraph(Graph *graph,char *search){
	
	Graph *tmp_graph = graph;
	
	int addr = findNode(tmp_graph,search,tmp_graph->node_num);
	
	if(addr == -1){
		printf("\nAradiginiz grafta bulunamammistir!\n");
	}
	else{
		Node array[tmp_graph->node_num],tmp;
		int size = 1;
		
		tmp_graph->node_list[addr].query_num++;
		Adj *tmp_adj = tmp_graph->node_list[addr].adjacent_list_head;
		while(tmp_adj != NULL){
			array[size - 1] = tmp_graph->node_list[tmp_adj->addr];
			tmp_adj = tmp_adj->next;
			size++;
		}
		
		int i,j,max;
		for (i = 0; i < size - 1; i++) {
			max = i;
			for (j = i; j < size; j++) {
				if (array[j].query_num > array[max].query_num)
					max = j;
			}
			
			tmp = array[i];
			array[max] = tmp;
			array[i] = array[max];
		}
		
		if(size > 1){
			printf("\n--Aradiginiza yakın ençok aranan ilk üç string--\n");
			if(size > 2){
			printf("1. arama ve arama sayisi:%s(%d)\n",array[0].query_str,array[0].query_num);
			}
			if(size > 3){
			printf("2. arama ve arama sayisi:%s(%d)\n",array[1].query_str,array[1].query_num);
			}
			if(size > 4){
			printf("3. arama ve arama sayisi:%s(%d)\n",array[2].query_str,array[2].query_num);
			}
			
		}
		else{
			printf("\n--Aradiginiza yakın stringler buklunamamıstır--\n");
		}
		
	}
	
}


int main(void) {
    int node_list_size1 = 0;
    int node_list_size2 = 0;
    int node_list_size3 = 0;
	int node_size,adj_size,adj_count,node_count,i,j,len;
    char node_str[MAX],adj_str[MAX],*operation;
    
    Graph *graph1 = (Graph *)malloc(sizeof(Graph));
    graph1->node_num = 4;
    addNodeandAdjacent(graph1,"abc","def",3,4,&node_list_size1);
    addNodeandAdjacent(graph1,"abc","ghi",3,7,&node_list_size1);
    addNodeandAdjacent(graph1,"abc","jkl",3,8,&node_list_size1);
    printf("\n----------Arama Grafi giris kismi------------\n");
    
    printf("\n----------Birinci arama Graf verilerini giriniz------------\n");
    /*printf("Arama grafinda aranacak sorgu string dugum sayisi:");
    scanf("%d",&node_size);
    graph1->node_num = node_size;
    
    for(i = 0;i < graph1->node_num;i++){
    	
    	printf("\n----------\n");
	    printf("\n%d. sorgu stringi giriniz:",i + 1);
	    scanf("%s",&node_str);
	    printf("\n%d. sorgu stringin sorgulama sayisisni giriniz:",i + 1);
	    scanf("%d",&node_count);
	    printf("\n%d. sorgu stringine yakin aramalarin sayisini giriniz:",i + 1);
	    scanf("%d",&adj_size);
	   	
	   	printf("\n------\n");
		for(j = 0;j < adj_size;j++){
		    printf("\t\n%d. sorgu stringinin %d komsuyu giriniz:",i + 1,j + 1);
		    scanf("%s",&adj_str);
		    printf("\t\n%d. sorgu stringinin %d komsunun sorgulama sayisisni giriniz:",i + 1,j + 1);
			scanf("%d",&adj_count);
			
		    addNodeandAdjacent(graph1,node_str,adj_str,node_count,adj_count,&node_list_size1);
		}	
	}*/
	
    printGraph(graph1,node_list_size1);
	
    Graph *graph2 = (Graph *)malloc(sizeof(Graph));
    graph2->node_num = 5;
    addNodeandAdjacent(graph2,"mnp","opr",4,5,&node_list_size2);
    addNodeandAdjacent(graph2,"mnp","ghi",4,7,&node_list_size2);
    addNodeandAdjacent(graph2,"mnp","stu",4,2,&node_list_size2);
    addNodeandAdjacent(graph2,"mnp","def",4,8,&node_list_size2);
	printf("\n----------İkinci arama Graf verilerini giriniz------------\n");
    /*printf("Arama grafinda aranacak sorgu string dugum sayisi:");
    scanf("%d",&node_size);
    graph2->node_num = node_size;
    
    for(i = 0;i < graph2->node_num;i++){
    	
    	printf("\n----------\n");
	    printf("\n%d. sorgu stringi giriniz:",i + 1);
	    scanf("%s",&node_str);
	    printf("\n%d. sorgu stringin sorgulama sayisisni giriniz:",i + 1);
	    scanf("%d",&node_count);
	    printf("\n%d. sorgu stringine yakin aramalarin sayisini giriniz:",i + 1);
	    scanf("%d",&adj_size);
	   	
	   	printf("\n------\n");
		for(j = 0;j < adj_size;j++){
		    printf("\n%d. sorgu stringinin %d komsuyu giriniz:",i + 1,j + 1);
		    scanf("%s",&adj_str);
		    printf("\n%d. sorgu stringinin %d komsunun sorgulama sayisisni giriniz:",i + 1,j + 1);
			scanf("%d",&adj_count);
			
		    addNodeandAdjacent(graph2,node_str,adj_str,node_count,adj_count,&node_list_size2);
		}	
	}*/
	
    printGraph(graph2,node_list_size2);
    
	Graph *graph3 = (Graph *)malloc(sizeof(Graph));
    printf("\n----------İki Arama Grafi birlesim kismi------------\n");
    mergeTwoGraph(graph1,graph2,graph3,&node_list_size3);
    printf("\n----------Birleşik Graf alt liste halinde düğümler ve okla gözterilen bağlantılı aramalar------------\n");
    printGraph(graph3,node_list_size3);
    
    printf("\n----------İki Arama Grafi arama kismi------------\n");
    
    int end = 0;
    operation = (char *)malloc(MAX * sizeof(char));
    do{
    	printf("\nAradiginiz stringi giriniz:");
        memset(operation,0,strlen(operation));
        if(fgets(operation, MAX, stdin) == NULL){
            printf("Error reading from input\n");
	    	return 1;
        }
        
        len = strlen(operation);
        operation[len - 1] = '\0';
        if(strcmp(operation,"-1") == 0){
        	end = 1;
		}
        else{
        	searchInGraph(graph3,operation);
		}
    	
	}while(!end);
	
	
    return 0;
}
