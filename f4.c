#include <stdio.h>
#include <stdlib.h>

#define NV 5

typedef struct aresta {
	int dest; int custo;
	struct aresta *prox;
} *LAdj, *GrafoL [NV];

typedef int GrafoM [NV][NV];



//-------------------------Auxiliares-----------------------------------
void imprimeL(GrafoL g){//V+A
	int i;
	LAdj aux;

	for(i = 0; i < NV; i++){
		printf("%d -> ",i);
		aux = g[i];
		while (aux){
			printf("%d " ,aux->dest);
			aux = aux->prox;
		}
		printf("\n");
	}
}

void imprimeM(GrafoM g){//V²
	int i,j;
	for(i = 0; i < NV; i++){
		printf("%d -> ",i);
		for(j = 0; j < NV; j++){
			if(g[i][j]) printf("%d ", j);
		}
		printf("\n");
	}
}

//-----------------AUX Travessias--------------------------------
int DF (GrafoL g, int or, int v[], int p[], int l[]){
	int i;
	for (i=0; i<NV; i++) {
		v[i]=0;
		p[i] = -1;
		l[i] = -1;
	}
	p[or] = -1; l[or] = 0;
	return DFRec (g,or,v,p,l);
}
int DFRec (GrafoL g, int or, int v[], int p[], int l[]){
	int i; LAdj a;
	i = 1;
	v[or] = -1;

	for (a=g[or]; a!=NULL; a=a->prox){
		if (!v[a->dest]){
			p[a->dest] = or;
			l[a->dest] = 1+l[or];
			i += DFRec(g,a->dest,v,p,l);
		}
	}
	v[or] = 1;
	return i;
}


int BF (GrafoL g, int or, int v[], int p[], int l[]){
	int i, x; LAdj a;
	int q[NV], front, end;
	
	for (i=0; i<NV; i++){
		v[i]=0;
		p[i] = -1;
		l[i] = -1;
	}
	front = end = 0;
	q[end++] = or; //enqueue
	v[or] = 1; p[or]=-1; l[or]=0;
	i = 1;

	while (front != end){
		x = q[front++]; //dequeue
		for (a=g[x]; a!=NULL; a=a->prox)
			if (!v[a->dest]){
				i++;
				v[a->dest]=1;
				p[a->dest]=x;
				l[a->dest]=1+l[x];
				q[end++]=a->dest; //enqueue
			}
	}
	return i;
}
//----------Exercício------------------------------------
//1-
void fromMat (GrafoM in, GrafoL out){//V²
	LAdj aux; int o,d;
	
	for (o = 0; o < NV; o++){
		out[o] = NULL;
		for (d = NV-1; d >= 0; d--){
			if (in[o][d] != 0){
				aux = malloc(sizeof (struct aresta));
				aux->dest = d;
				aux->custo = 1;
				aux->prox = out[o];
				out[o] = aux;
			}
		}
	}
}

//V+A
void inverte (GrafoL in, GrafoL out){
	int o;
	LAdj d, aux;

	for (o = 0; o < NV; o++) out[o] = NULL;
	for (o = 0; o < NV; o++){
		d = in[o];
		while(d){
			aux = malloc(sizeof (struct aresta));
			aux->dest = o;
			aux->custo = 1;
			aux->prox = out[d->dest];
			out[d->dest] = aux;
			d = d->prox;
		}
	}
}

//3 V+A
int inDegree (GrafoL g){
	int o,m, ind[NV]; 
	LAdj d;

	for(o = 0; o < NV; o++) ind[o] = 0;
	for(o = 0; o < NV; o++){
		d = g[o];
		while (d){
			ind[d->dest]++;
			d = d->prox;
		}
	}
	m = 0;
	for (o = 0; o < NV; o++)
		if (ind[o] > m) m = ind[o];

	return m;
}

//4
int colorOK (GrafoL g, int cor[]){
    //pior caso V+A
	//melhor caso 1
	int i;

	for (i = 0; i < NV; i++){
		LAdj aux = g[i];
		while (aux){
			if (cor[aux->dest] == cor[i]) return 0;
			aux = aux->prox;
		}
	}
	return 1;
}

//5

int homomorfOK (GrafoL g, GrafoL h, int f[]){
	int i;
	LAdj aux, aux2;

    for(i = 0; i < NV; i++){
        while (aux && aux2){
            if (f[aux->dest] != f[aux2->dest]) return -1;
            aux = aux->prox;
            aux2 = aux2->prox;
        }
    }
    return 0;
}


//------------------Travessias------------------------------
int maisLonga (GrafoL g, int or, int p[]){
    int v[NV],l[NV],pai[NV],i,j = 0;
    int m = BF(g,or,v,pai,l);
    for(i = 0; i < NV; i++){
        if (l[i] > l[j]) j = i;
    }
    i = j;
    p[l[j]] = j;
    for(int aux = l[j] - 1 ; pai[j] != -1; aux--){
        p[aux] = pai[j];
        j = pai[j];
    }
    return i;
}

int componentes (GrafoL g, int c[]){
    int v[NV],l[NV],pai[NV],count, o = 0;
    BF(g,o,c,pai,l);
    count = 1;
    while (o < NV) {
        for (o = 0; o < NV && c[o] != 0; o++)
            ;
        if (o < NV) {
            BF(g,o,v,pai,l);
            count++;
            for(int i = 0; i < NV;i++){
                if (v[i]) c[i] = count;
            }
        }
    }
    return count;
}

int ordTop (GrafoL g, int ord[]){
    int vis[NV], pai[NV], l[NV],or = 0,count = 1,bk;
    LAdj aux;
    BF(g,or,vis,pai,l);
    for(int i = 0; i < NV ;i++){
        if (pai[i] == -1) {
            ord[0] = i;
            pai[i] = -2;
        }
    }
    for(int m = 1; m < NV; m++){
        bk = 1;
        for(int n = 0;n < NV && bk ;n++){
            if ((pai[pai[n]]) == -2){
                pai[n] = -2;
                ord[m] = n;
                bk = 0;
            }
        }
    }
    return 0;
}

///---------------------Main----------------------
int main(int argc, char const *argv[]){
	GrafoM go = { {0,0,0,0,0},
				  {0,0,0,1,0},
				  {1,0,0,0,0},
				  {0,0,1,0,0},
				  {0,1,1,0,0}};

    GrafoM gno = {{0,1,1,0,0},
				  {1,0,1,0,0},
				  {1,1,0,0,0},
				  {0,0,0,0,1},
				  {0,0,0,1,0}};
				  				


	GrafoL i,n;
	//imprimeM(go);
	//putchar('\n');
	int cor1[NV] = ,cor2[NV]= 
	fromMat(gno,i);		
	imprimeL(i);
	//printf("\nDegree: %d\n",inDegree(i));
	//inverte(i,n);
	//imprimeL(n);
	return 0;
} 