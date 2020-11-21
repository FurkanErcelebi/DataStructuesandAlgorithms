#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX 100000

typedef struct node
{
    char chr;
    int freq;
    struct node *left,*right,*next;
}Node;

Node *root;

//---List uzunlugu verme

int ListSize()
{
    int size = 0;
    Node *current = root;
	
    while (current != NULL)
    {
	current = current->next;
	size++;
    }
	return size;
}


//---Bas dugum olusturma

void CreateRoot(char letter)
{
	root=(Node *)malloc(sizeof(Node));
	if(root!=NULL)
	{
	    root->chr=letter;
	    root->freq=1;
	    root->left=NULL;
            root->right=NULL;
            root->next=NULL;
    	} 
        else 
	{
    	    printf("Yer ayrilacak alan kalmadi\n");
	    return ; 
	}
}

//---Dugum sayaclarini kintrol etme

bool freqControl(char str)
{
	Node *count=root;
	
	while(count!=NULL)
	{
		if(count->chr==str)
		{
		    count->freq++;
		    return true;
		}
		
            count = count->next;
	}
	
	return false;
}

//---Yeni bir dugum olsturma

Node *CreateNode(char chr1)
{
	Node *node=(Node*)malloc(sizeof(Node));
	
	if(node!=NULL)
	{
	    node->chr = chr1;
            node->freq=1;
	    node->next=NULL;
            node->right=NULL;
            node->left=NULL;
        }
        else 
	{
            printf("Yer ayrilacak alan kalmadi\n");
	    return node; 
	}
	
	return node; 
}

//---Yeni dugum ekleme

void InsertNode(char chr1)
{
	
	Node *node,*tmp;
	
	if(!(freqControl(chr1))) 
	{
	    node=CreateNode(chr1);
	    tmp=root;
	    root=node;
	    root->next=tmp;
	}
	
}

//---Listenin basi cikartilmsai

Node *ExtractNode()
{
    Node *extract,*swap; 
    extract = root;
    swap = root->next;
    extract->next = NULL;
    root = swap;
    return extract;
}

//---Agac gosterimi

void TreeTraversal(Node *root1,int shift)
{
    int r,l;
	
    if(root1)
    {
        printf("%c:%d\n",root1->chr,root1->freq);
		
	if (root1->right)
	{
            for (r = 0; r < shift-1; r++)
		        printf(" ");	
		
            printf("|");
	    printf("\n");
		
	    for (r = 0; r < shift; r++)
                printf(" ");
		   
	    printf("r-");	  
		    
	    TreeTraversal(root1->right,shift+2);
	} 
	    
	if (root1->left)
	{
	    for (l = 0; l < shift-1; l++)
		printf(" "); 	
			
	    printf("|");		
            printf("\n");
		
            for (l = 0; l < shift; l++)
	        printf(" ");	
		    
	    printf("l-");
		
            TreeTraversal(root1->left,shift+2);
        }
    }
}
  
//---Liste ve agci gosterme

void Traversal()
{
    Node *tree[ListSize()],*tm = root;
	int i=-1,size;
	printf("======Liknli Liste======\n");
    while ( tm != NULL)
    { 
		if (tm->chr == '\0') 
		{
			i++;
			tree[i] = tm;
		}
        if (tm->next != NULL) printf("%c:%d->\t",tm->chr,tm->freq);
        else printf("%c:%d\n\n",tm->chr,tm->freq);
        tm = tm->next;
    }
	size = i + 1;
	if (i >= 0)
	{
		printf("======Liste Icerisindeki agaclar======\n");
        for ( i = 0; i < size; i++)
	    {
	        TreeTraversal(tree[i],1);
		    printf("----------\n");
	    }
	}		
}

//---Insertion siralama

void InsertionSort()
{
    Node *current1, *current2, *tmp1, *tmp2;
    int min;
    current1 = root;
    while (current1)
    {
        current2 = current1->next;
        if(current2)
        { 
            if (current2->freq < current1->freq)
            {
                min = current2->freq;
                tmp1 = root;
                tmp2 = tmp1;
                while (tmp1 != current1 && tmp1->freq <= min)
                {
                    tmp2 = tmp1;
                    tmp1 = tmp1->next;
                }
                current1->next = current2->next;
                if (tmp1 == root)
                {
                    current2->next = tmp1;
                    root = current2;
                }
                else
                {  
                    tmp2->next = current2;
                    current2->next = tmp1;
                }
            }
            else current1 = current2;
        }
		else current1 = current2;    
        Traversal();
    }
}

//---Huffman agaci yapilimi

void MakeHuffmanTree()
{
	Node *swap1,*swap2,*total,*tmp,*next1,*previous,*previous1,*huffman; 
	huffman = root;
	do
	{
	    swap1 = ExtractNode();
	    Traversal();
		printf("1\n");
	    swap2 = ExtractNode();
	    Traversal();
		printf("2\n");
        total = CreateNode('\0');
	    total->freq = swap1->freq + swap2->freq;
	    total->left = swap1;
	    total->right = swap2;
        next1 = root;
	    while(next1 != NULL && next1->next != NULL && next1->freq <= total->freq)
	    {
			previous1 = next1;
		    next1 = next1->next;
	    }
		if (next1 == NULL) root = total;
	    else if(next1 == root)
	    {
			if (next1->freq > total->freq)
			{
                tmp = next1;
                root = total;
                root->next=tmp;
			}
			else root->next = total;	
	    }
	    else if (next1->next == NULL)
	    {
            if (next1->freq <= total->freq) next1->next = total;
			else
			{
				total->next = next1;
				previous1->next = total;
			}
	    }
		else
		{
            total->next = next1;
			previous1->next = total;
		}
		Traversal();
		printf("3\n");
		printf("\n============\n");
	}while (root->next);	
}
//---Ana algoritmayi calistirma

int main()
{
	int karar1;
	char karar;
	char str1[11] = "aaabbbcdee";
	char str2[74] = "ffffgggggggggghhhiijjjjjjjjjjjjjjjjkkkkkkkkklmmmmmmmmmmmmmmnnnnnnnnooooooo";
	char str3[MAX];
	printf("Ornek mi ,yoksa kendi dizginizimi girmek istiyorsunuz ?(ornek(o)/girdili dizgi(g))\n");
	scanf("%c",&karar);
	while(karar!='o' && karar!='g'){
    	printf("Yanlis karakter girdiz , lutfen bir daha deneyiniz\n");
    	scanf("%c",&karar);
	}
	if(karar == 'o'){
        printf("Hangi dizgiyi girmek istiyorsunuz: 1)%s ,2)%s\n",str1,str2);
		scanf("%d",&karar1);
        while(karar1 != 1  && karar1 != 2)
		{
    	    printf("Yanlis karakter girdiz , lutfen bir daha deneyiniz\n");
    	    scanf("%d",&karar1);
	    }
        if (karar1 == 1) strcpy(str3,str1);
		else strcpy(str3,str2);
	}
    else scanf("%s",str3);
	CreateRoot(str3[0]);
	for (int i = 1 ;'\0' != str3[i] && i < MAX ;i++){
		printf("%c\t",str3[i]);
        InsertNode(str3[i]);
	}
	printf("\n");
	Traversal();
	printf("\n======Insertion Siralama Yapilir======\n");
	InsertionSort();
	printf("\n=====Huffman Agaci Yapimi=======\n");
	printf("l:left , r:right\n");
	MakeHuffmanTree();
    return 0;
}
