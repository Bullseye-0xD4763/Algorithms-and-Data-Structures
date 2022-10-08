 g#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define Max 100

typedef struct pQueue {
	int valores [Max];
	int tamanho;
} PriorityQueue;



//Parte 1 - Min heaps -------------------------------
#define pai(i) ((i-1)/2)
#define right(i) 2*i+2
#define left(i) 2*i+1


//aux
void swap(int h[], int ix, int iy){
    int temp = h[ix];
    h[ix] = h[iy];
    h[iy] = temp;
}

//é menor que o pai ? Sim -> Torca | Não -> Nada
void bubbleUp(int i, int h[]){
	while (i > 0 && (h[i] < h[pai(i)])){
		swap(h, i, pai(i));
		i = pai(i);
	}
}

//é maior que os filhos ? Sim -> troca com filho mais pequeno | Não -> Nada
void bubbleDown(int i, int h[], int N){
	int m;
	while (h[left(i)] < N){
		m = h[left(i)];

		if (h[right(i)] < N && h[right(i)] < h[left(i)])
			m = h[right(i)];

		if(h[m] > h[i]) break;
		swap(h,i,m);
		i = m;
	}
}


//Exercicio 4
void empty(PriorityQueue *q){
	q->tamanho = 0;
}

int isEmpty (PriorityQueue *q){
	return (q->tamanho == 0);
}

int add (int x, PriorityQueue *q){
	if (q->tamanho == Max) return 1;
	q->valores[q->tamanho] = x;
	bubbleUp(x, q->valores);
	q->tamanho++;
	return 0;
}

int remover (PriorityQueue *q, int *rem){
	if (isEmpty(q)) return 1;
	*rem = q->valores[0];
	q->tamanho--;
	q->valores[0] = q->valores[q->tamanho-1];
	bubbleDown(0,q->valores,q->tamanho);
	return 0;
}




//AUX--------------------------------------------------
void printQueue(PriorityQueue *q){
	putchar('\n');putchar('['); putchar(' ');
	for (int i = 0; i < q->tamanho; i++)
		printf("%d ,",q->valores[i]);
	putchar(']');
}

void teste(PriorityQueue *q){
    add(1,q);
    add(2,q);
    add(3,q);
}
//------------------------------------------------------

//Exercicio 5
void heapify_topdown(int v[], int N){
    for (int i = 0; i < N; i++){
        bubbleUp(i,v);
    }
}

void heapify_bottomup(int v[], int N){
    for (int i = N-1; i > -1; i--){
        bubbleDown(i,v,N);
    }   
}

//Exercicio 6
void ordenaHeap(int h[], int N){
    for(int i = 0; i < N; i++){
        swap(h, 0, N-i-1);
        bubbleDown(0,h,N);
    }
}



int heap_pop(int h[], int N){
    swap(h, 0, N-1);
    bubbleDown(0,h,N-1);
    return h[N-1];
}

//Execicio 7
int* sequencia(int v[], int N, int k){
    int *aux = malloc( k * sizeof(int));
    
    for (int i = 0; i < k; i++) aux[i] = v[i];
    
    heapify_topdown(aux,k);
    
    for(int i = k; i < N; i++){
        if(v[i] > aux[0]){
            heap_pop(aux, k);
            aux[k-1] = v[i];
            bubbleUp(k-1, aux);
        }
    }
    return aux;
}



//função de hash---------------------------------------------------------------
unsigned hash(char *str){
	unsigned hash = 5381;
	int c;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	
	return hash;
}


#define Size 15
	typedef struct nodo {
	char *chave; int ocorr;
	struct nodo *prox;
} Nodo, *THash [Size];


void initEmpty (THash t){
	int i;
	for (i = 0; i<Size; i++) t[i] = NULL;
}

int add_hash (char *s, THash t){
	int h = hash(s)%Size;
	Nodo *aux = t[h];

	while (aux && strcmp(s, aux->chave)) aux = aux->prox;

	if (aux) aux->ocorr++;
	else{
		//aux = malloc(1*sizeof (struct (Nodo)));
		if (!aux) return 1;
		aux->chave = strdup(s);
		aux->ocorr = 1;
		t[h] = aux;		
	}
	return 0;
}



int lookup (char *s, THash t){
	int h = hash(s)%Size;
	Nodo *aux = t[h];

	while (aux && strcmp(s,aux->chave)) aux = aux->prox;

	if(aux) return aux->ocorr;
	else    return 0;
}

int remove_hash (char *s, THash t){
	int h = hash(s)%Size;
	Nodo *aux = t[h], *ant = NULL;

	while (aux && strcmp(s,aux->chave)){
		ant = aux;
		aux = aux->prox;
	}

	if (aux){
		if (aux->ocorr > 1) aux->ocorr--;
		else t[h] = aux->prox;
		free(aux);
	}else return 1;

	return 0;
}	

//-----------------Open addressing -----------------------------------------
#define Free 0
#define Used 1
#define Del 2

typedef struct bucket {
	int status; // Free | Used | Del
	char *chave; int ocorr;
} THashOpen [Size];


int where(char *s, THashOpen t){
	int h = hash(s)%Size;
	return h;
}
void initEmpty_open (THashOpen t){
	int i;
	for (i  = 0; i < Size; i++){
		t[i].status = 0;
		t[i].chave = NULL;
		t[i].ocorr = 0;
	}
}

void add_open(char *s, THashOpen t){
    int ind = where(s,t);
    if(lookup2(s,t)){ 
        t[ind].status = Used;
        t[ind].ocorr++;
    }
    else if(t[ind].status == Free){
        t[ind].status = Used;
        t[ind].ocorr = 1;
        t[ind].chave = s;
    }

}
int lookup_open(char *s, THashOpen t){
    int i = where(s);
    if(t[i]->chave && !strcmp(t[i]->chave,s)) return t[i]->ocorr;
    else return 0;
}
int remove_open(char *s, THashOpen t){
    if(lookup_open(s,t)){
        int ind = where(s,t);
        int oc = --t[ind]->ocorr;
        if(!oc) t[ind]->status = Free;
    }
}
	
int garb_collection(THashOpen t){
    for (int i = 0; i < Size; i++){
        if(t[i]->status==Del){ 
            remove2(t[i].chave,t);
        } 
    }
}

int main(int argc, char const *argv[]){
   /* PriorityQueue pq = (PriorityQueue){.valores = {10,15,11,16,22}, .tamanho = 5};
    printQueue(&pq);
    teste(&pq);
    printQueue(&pq);
    int x = 11;
    remover(&pq,&x);    
    printQueue(&pq);*/
	/*THash t;
	initEmpty(&t);
	add_hash("nigga",&t); add_hash("mayra andrade",&t); add_hash("dababy",&t);add_hash("dababy",&t);
	printf("\n-----------------------\n");
	for (int i = 0; i < Size; i++){
		printf("Chave -> %s | ocorr -> %d |\n" ,&(t[i]->chave),&(t[i]->ocorr));
	}*/
	THashOpen t;
    initEmpty_open(t);
    add_open("boing",t);
    add_open("a tua prima",t);
    add_open("a tua prima",t);
    add_open("a tua prima",t);
    add_open("toppppppp",t);
    garb_collection(t);
    for(int i=0;i<Size;i++)
        if(t[i].status == Used)
            printf("%s %d %d\n", t[i].chave, t[i].ocorr, t[i].status);


	return 0;
}
