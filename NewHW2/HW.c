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
        addr_node1 = *(list_size) - 1;
        if( (one_graph->node_list[addr_node1].query_str = malloc(strlen(str1) + 1)) == NULL)
			return ;
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
	graph_three = graph_one;
	
	Node *current_node = graph_three->node_list;
	Node *tmp_node;
	Adj *tmp_adj;
	int i,j,size1,size2,extra,addr_node,addr_adj,end;
	
	for(i = 0;i < graph_one->node_num;i++){
		addr_node = findNode(graph_two,graph_one->node_list[i].query_str,graph_two->node_num);
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
	for(i = 0;i < graph_three->node_num;i++){
		addr_node = findNode(graph_two,graph_three->node_list[i].query_str,graph_two->node_num);
		
		if(addr_node != -1){
			addr_adj = findAdj(graph_three->node_list[i],graph_two->node_list[addr_node].query_str);
			
			if(addr_adj == -1){	
			    graph_three->node_list[size1] = graph_two->node_list[addr_node];
		        Adj *current_adj = (Adj*)malloc(sizeof(Adj));
		        current_adj->next = NULL;
		        current_adj->addr = size1;
		        tmp_adj = getLastAdj(graph_three->node_list[size1]);
		        if(tmp_adj == NULL){
		            graph_one->node_list[addr_node1].adjacent_list_head = current_adj1;
		        }
		        else{
		            tmp_adj->next = current_adj1; 
		        }
		        size1++;
			}
		}
	}
	
	size2 = graph_one->node_num;
	while(size1 < graph_three->node_num){
		addr_node = findNode(graph_two,graph_three->node_list[size2].query_str,graph_two->node_num);
		tmp_adj = graph_two->node_list[addr_node];
		
		while(tmp_adj != NULL){
			addr_node = findNode(graph_three,graph_two->node_list[tmp_adj->addr].query_str,graph_three->node_num);
			
			if(addr_node == -1){
		    	addNodeandAdjacent(graph_three
									,graph_three->node_list[size2].query_str
									,graph_two->node_list[tmp_adj->addr].query_str
									,graph_three->node_list[size2].query_num
									,graph_two->node_list[tmp_adj->addr].query_num
									,size1);
			}
		}
		
		size2++;
	}
	
	(*size_of_graph) = size;
	
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
	
	Graph *tmp = node_list_size3;
	
	int addr = findNode(tmp,search,tmp->node_num);
	
	if(addr == -1){
		printf("\nAradiğiniz grafta bulunamammistir!\n")
	}
	else{
		Node array[tmp->node_num];
		int size = 1;
		Adj *tmp_adj = tmp->node_list[addr];
		while(tmp_adj != NULL){
			array[size - 1] = tmp->node_list[tmp_adj->addr];
		}
		
		
	}
	
}


int main(void) {
    int node_list_size1 = 0;
    int node_list_size2 = 0;
    int node_list_size3 = 0;
	int node_size,adj_size,adj_count,node_count,i,j;
    char node_str[MAX],adj_str[MAX];
    
    Graph *graph1 = (Graph *)malloc(sizeof(Graph));
    graph1->node_num = 4;
    addNodeandAdjacent(graph1,"abc","def",3,4,&node_list_size1);
    addNodeandAdjacent(graph1,"abc","ghi",7,3,&node_list_size1);
    addNodeandAdjacent(graph1,"abc","jkl",8,3,&node_list_size1);
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
    graph1->node_num = 5;
    addNodeandAdjacent(graph1,"mnp","opr",node_count,adj_count,&node_list_size2);
    addNodeandAdjacent(graph1,"mnp","ghi",node_count,adj_count,&node_list_size2);
    addNodeandAdjacent(graph1,"mnp","stu",node_count,adj_count,&node_list_size2);
    addNodeandAdjacent(graph1,"mnp","def",node_count,adj_count,&node_list_size2);
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
    mergeTwoGraph(graph1,graph2,graph3,node_list_size3);
    printf("\n----------Birleşik Graf alt liste halinde düğümler ve okla gözterilen bağlantılı aramalar------------\n");
    printGraph(graph3,node_list_size3);
    
    /*printf("\n----------İki Arama Grafi arama kismi------------\n");
    
    int end
    do{
    	
        memset(operation,0,strlen(operation));
        if(fgets(operation, LINE, stdin) == NULL){
            printf("Error reading from input\n");
	    	return 1;
        }
        if(operation = -1)
        
        searchInGraph(graph3,operation);
    	
	}while();*/
	
	
    return 0;
}
