#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct knr
{
    int cnt;
    struct knr *next;
} NODE;

NODE *list;

//---Yeni bir dugum olsturma

NODE *CreateNode(int chr1)
{
    NODE *node = (NODE *)malloc(sizeof(NODE));
    if (node != NULL)
    {
        node->cnt = chr1;
        node->next = NULL;
    }
    else
    {
        printf("Yer ayrilacak alan kalmadi\n");
        return node;
    }
    return node;
}

//---Yeni dugum ekleme(Derin bakilmasi lazim)

void InsertNode(int chr1)
{
    NODE *node, *tmp;
    printf("dugum eklendi\n");
    node = CreateNode(chr1);
    tmp = list;
    list = node;
    list->next = tmp;
}

int ListSize()
{
    int size = 0;
	NODE *current = list;
    while (current != NULL)
	{
		current = current->next;
		size++;
	}
	return size;
}

void Traversal()
{
    NODE *tm = list;
    while ( tm != NULL)
    {
        if (tm->next != NULL) printf("%d->\t",tm->cnt);
        else printf("%d\n\n",tm->cnt);
        tm = tm->next;
    }
}

NODE *PreviousNode(NODE *node)
{
    NODE *previous = list;
    while (previous->next != node && previous->next != NULL)
    {
        previous = previous->next;
    }
    if (previous->next == NULL)
    {
        printf("Dugum listete yok\n");
    }
    return previous;
}

void swapNode(NODE *node1, NODE *node2)
{
    NODE *pre1 = list, *pre2 = NULL, *curr1, *curr2;
    pre2 = list;
    pre1 = PreviousNode(node1);
    pre2 = PreviousNode(node2);
    curr1 = node1->next;
    curr2 = node2->next;
    if (node1 == list) 
    {
        list = node2;
    }   
    if (curr1 == node2 && pre2 == node1)
    {
        if (pre1->next != NULL)
        {
            pre1->next = node2;
        }
        node2->next = node1;
        node1->next = curr2;
    }
    else
    {
        if (pre1->next != NULL)
        {
            pre1->next = node2;
        }
        node2->next = curr1;
        if (pre2->next != NULL)
        {
            pre2->next = node1;
        }
        node1->next = curr2;
    }
}

//printf("\n");
void InsertionSort1()
{
    NODE *current1, *current2, *tmp1, *tmp2;
    int min,j = 0;
    current1 = list;
    while (current1 != NULL && j<10)
    {
        j++;
        printf("curr1:%d\n",current1->cnt);
        current2 = current1->next;
        if(current2 != NULL)
        { 
            if (current2->cnt < current1->cnt)
            {
                printf("curr1:%d<->curr2:%d\n",current1->cnt,current2->cnt);
                min = current2->cnt;
                tmp1 = list;
                tmp2 = tmp1;
                printf("+%d\n\n",tmp1->cnt);
                while (tmp1 != current1 && tmp1->cnt <= min)
                {
                    tmp2 = tmp1;
                    tmp1 = tmp1->next;
                    printf("*%d\n\n",tmp1->cnt);
                }
                current1->next = current2->next;
                if (tmp1 == list)
                {
                    current2->next = tmp1;
                    list = current2;
                }
                else
                {  
                    tmp2->next = current2;
                    current2->next = tmp1;
                }
            }
            else current1 = current2;
        }    
        Traversal();
    }
}

void InsertionSort2()
{
    NODE *ls,*fu,*current,*trailing,*tmp;
    ls = list;
    int i = 0;
    while (ls->next)
    {
        fu = ls->next;
        //printf("ls->cnt :%d, fu->cnt :%d\n",ls->cnt,fu->cnt);
        if(fu->cnt < list->cnt)
        {
            printf("true\n");
            ls->next = fu->next;
            fu->next = list;
            list = fu;
            Traversal();
        }
        else
        {
            printf("false\n");
            trailing = list;
            for (current = trailing->next; fu->cnt > current->cnt; current = current->next)
            {
                //printf("trailing->cnt :%d\n",trailing->cnt);
                trailing = current;  
                Traversal();                 
            }  
            if (fu == current) ls = fu;    
            else
            {
                ls->next = fu->next;
                fu->next = current;
                trailing->next = fu;
            }
            printf("xxxxxxxxxx\n");
            Traversal();
        }
    } 
}

//---En buyuk heapi olusturma

/*void MinHeap(int dizi[],int i, int n)
{
	printf("----MinHeap ile siralandi----\n");
	TreeDisplay(dizi,n);
    int l = 2*i-1;
	int r = (2*i);
	int i1 = i-1;
	printf("i:%d,l:%d,r:%d,n:%d\n",i-1,l,r,n);
	int smallest = 0,tmp = 0;
    if(l < n && dizi[l] < dizi[i1] ) smallest = l;
	else smallest = i1;
    if(r < n && dizi[r] < dizi[smallest]) smallest = r;
    if(smallest != i1){
        tmp = dizi[i1];
        dizi[i1] = dizi[smallest];
		dizi[smallest] = tmp;
		MinHeap(dizi,smallest+1,n);
	}
	else printf("----MinHeap sonu----\n");
}
//---Insertion sort iles siralama

void InsertionSort(int dizi[],int n)
{
    int i,j,m,tmp;
	for ( i = 0; i < n-1; i++)
	{
		if (dizi[i+1] < dizi[i])
		{
			tmp = dizi[i+1];
			j = i;
            do	
			{
				dizi[j+1] = dizi[j];
				--j;
			}while (j >= 0 && dizi[j] > tmp );
			j++;
			dizi[j] = tmp;
		}
		for (m = 0; m < 10;m++)
	        printf("%d\t",dizi[m]);
	    printf("\n\n");
	}
}*/

//---Heap'i verilen deger uzerinden degismesi kontrolu

/*void HeapIncreaseKey(int dizi[],int i ,int key)
{
    int tmp = 0,i1 = i-1;
    if (key <= dizi[i1]){
        dizi[i1] = key;
		printf("HeapIncreaseKey calisiyor\n");
		while ( i1 >= 0 && dizi[i1/2] > dizi[i-1])
		{
			printf("HeapIncreaseKey calisiyor,%d\n",i-1);
			tmp = dizi[i1];
			dizi[i1] = dizi[i1/2];
			dizi[i1/2] = tmp;
			i1 = i1/2;
		}
	}
	else printf("New key is samller than current key\n");
}

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

//---Heapten deger cikarma

int HeapExtractMax(int dizi[] ,int n)
{
	int max=0;
    if (n >= 1){
        max = dizi[0];
		dizi[0] = dizi[n-1];
		n = n - 1;
		MinHeap(dizi,1,n);
	}
    else printf("Heap underflow\n");
	return max;
}

//---Maksimum degerini heap'e ekleme

void MaxHeapInsert(int dizi[] ,int key ,int n)
{
    printf("MaxHeapInsert calisiyor\n");
    dizi[n] = MAX;
	HeapIncreaseKey(dizi,n + 1,key);
}
*/

int main()
{
    NODE *tmp;
    int i;
    char c = '/';
    list = (NODE *)malloc(sizeof(NODE));
    if (list != NULL)
    {
        list->next = NULL;
        list->cnt = 3;
    }
    else
        return 1;
    /*for (i = 0; i < 5; i++)
    {
        InsertNode(i + 1);
    }*/
    InsertNode(4);
    InsertNode(10);
    InsertNode(1);
    InsertNode(2);
    InsertNode(0);
    InsertNode(5);
    InsertNode(7);
    InsertNode(11);
    InsertNode(9);
    Traversal();
    printf("\n");
    //swapNode(list->next->next, list->next->next->next);
    //Traversal();
    //printf("\n");
    printf("====Insertion sort====\n");
    //InsertionSort2();
    InsertionSort1();
    //printf("%d\n",c);
    return 0;
}