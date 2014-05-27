/* CS 32 MP 2
 * Dijkstra's Algorithm
 * Lou Merlenette B. Somoson
 * 2009-06984
 */

#include<stdio.h>
#include<stdlib.h>
#define INFINITY 999999
#define MAX_NODES 100

typedef struct node_t{
	int vertex;
	int cost;
	struct node_t *next;
}node_t;

typedef struct graph_t{
	node_t *list[MAX_NODES];
	int pred[MAX_NODES];
	int dist[MAX_NODES];
	int numOfVertices;
}graph_t;

typedef struct pq_t{
	int heap[MAX_NODES];
	int index[MAX_NODES];
	int key[MAX_NODES];
	int sizePQ;
}pq_t;

void dijkstra(graph_t *, int);
void initPQ(graph_t *, pq_t *, int);
void extractMin(pq_t *, int *);
void heapify(pq_t *, int);
void decreaseKey(pq_t *, int, int);
void displayPath(graph_t *, int, int);

int main(int argc, char *argv[]){
	int i, j, k, h;
	char ch, str[5];
	graph_t graph;
	node_t *lptr;
	if(argc!=2){
		printf("Usage: %s <input filename>\n", argv[0]);
		exit(1);
	}
	FILE *fp = fopen(argv[1], "r");
	if(fp==NULL){
		printf("File not found.\n");
		exit(1);
	}

	//Checking the number of vertices
	graph.numOfVertices=0;
	while((ch=fgetc(fp))!='\n'){
		if(ch==' ')
			graph.numOfVertices++;
	}
	graph.numOfVertices=graph.numOfVertices+1;
	
	//Setting the values of the list to NULL
	for(int h=1; h<=graph.numOfVertices; h++){
		graph.list[h]=NULL;
	}

	int matrix[graph.numOfVertices][graph.numOfVertices];
	
	//Reading from the text file
	i=1, j=1;
	rewind(fp);
	while(!feof(fp)){
		ch = fgetc(fp);
		if(ch!=' ' && ch!='\n'){
			str[k]=ch;
			k++;
		}else{
			if(str[0]=='x'){
				matrix[i][j]=INFINITY;
			}else{
				matrix[i][j]=atoi(str);
			}
			for(k=0; str[k]!='\0'; k++){
				str[k]='\0';
			}
			k=0;
			if(ch==' '){
				j++;
			}else if(ch=='\n'){
				i++;
				j=1;
			}
		}
	}	
	fclose(fp);

	matrix[graph.numOfVertices][graph.numOfVertices]=0;
		
	//Constructing the list
	for(i=1; i<=graph.numOfVertices; i++){
		for(j=1; j<=graph.numOfVertices; j++){
			if(matrix[i][j]!=0 && matrix[i][j]!=INFINITY){
				if(graph.list[i]==NULL){
					graph.list[i]=(node_t *)malloc(sizeof(node_t));
					graph.list[i]->vertex=j;
					graph.list[i]->cost=matrix[i][j];
					graph.list[i]->next=NULL;
				}else{
					lptr=graph.list[i];
					while(lptr->next!=NULL){
						lptr=lptr->next;
					}
					lptr->next=(node_t *)malloc(sizeof(node_t));
					lptr=lptr->next;
					lptr->vertex=j;
					lptr->cost=matrix[i][j];
					lptr->next=NULL;
				}
			}
		}
	}
	
	//Iterated dijkstra's
	for(i=1; i<=graph.numOfVertices; i++){
		dijkstra(&graph, i);
	}	
	return 0;
}

void dijkstra(graph_t *graph, int s){
	pq_t pq;
	node_t *aptr;
	int newval, u, v, i;
	initPQ(graph, &pq, s);

	//Find shortest paths from s to every other vertex in V
	while(pq.sizePQ!=0){
		extractMin(&pq, &u);
		if(pq.key[u]==INFINITY){
			return;
		}
		aptr=graph->list[u];
		while(aptr!=NULL){
			v=aptr->vertex;
			newval=pq.key[u]+aptr->cost;
			if(pq.key[v]>newval){
				graph->pred[v]=u;
				decreaseKey(&pq, v, newval);
			}
			aptr=aptr->next;
		}
	}
	graph->dist=pq.key;

	//Display path
	for(i=1; i<=graph->numOfVertices; i++){
		if(i!=s)
			displayPath(graph, s, i);
	}
}

void initPQ(graph_t *graph, pq_t *pq, int s){
	int i=1, v;
	for(v=1; v<=graph->numOfVertices; v++){
		if(v==s){
			pq->heap[1]=s;
			pq->index[s]=1;
			pq->key[s]=0;
		}else{
			i++;
			pq->heap[i]=v;
			pq->index[v]=i;
			pq->key[v]=INFINITY;
		}
	}
	pq->sizePQ = graph->numOfVertices;
}

void extractMin(pq_t *pq, int *j){
	if(pq->sizePQ!=0){
		*j=pq->heap[1];
		pq->heap[1]=pq->heap[pq->sizePQ];
		pq->index[pq->heap[1]]=1;
		pq->sizePQ--;
		heapify(pq, 1);
	}
}
	
void heapify(pq_t *pq, int r){
	int k, l, i=r, j;
	k=pq->key[pq->heap[r]];
	l=pq->heap[r];
	j=2*i;
	while(j<=pq->sizePQ){
		if(j < pq->sizePQ && pq->key[pq->heap[j+1]] < pq->key[pq->heap[j]]){
			j++;
		}
		if(pq->key[pq->heap[j]] < k){
			pq->heap[i]=pq->heap[j];
			pq->index[pq->heap[j]]=i;
			i=j;
			j=2*i;
		}else
			return;
	}
	pq->heap[i]=l;
	pq->index[l]=i;
}

void decreaseKey(pq_t *pq, int l, int newkey){
	int i, j;
	pq->key[l]=newkey;
	i=pq->index[l];
	j=i/2;
	while(i>1 && pq->key[pq->heap[j]] > newkey){
		pq->heap[i]=pq->heap[j];
		pq->index[pq->heap[j]]=i;
		i=j;
		j=i/2;
	}
	pq->heap[i]=l;
	pq->index[l]=i;
}

void displayPath(graph_t *graph, int s, int v){
	int path[graph->numOfVertices];
	int len=1, i=v, a;
	path[len]=v;
	printf("%d TO %d: ", s, v);
	while(i!=s){
		if(graph->pred[i]==0){
			printf("No path found.\n");
			return;
		}else{
			i=graph->pred[i];
			len++;
			path[len]=i;
		}
	}
	for(a=len; a>=1; a--){
		printf("%d ", path[a]);
		if(a!=1){
			printf("-> ");
		}
	}
	printf("(cost = %d)\n", graph->dist[v]);
}