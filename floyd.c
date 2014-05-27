/* CS 32 MP 2
 * Floyd's Algorithm
 * Lou Merlenette B. Somoson
 * 2009-06984
 */

#include<stdio.h>
#include<stdlib.h>
#define INFINITY 9999999
#define MAX_NODES 30

typedef struct graph_t{
	int dist[MAX_NODES][MAX_NODES];
	int pred[MAX_NODES][MAX_NODES];
	int numOfVertices;
}graph_t;

void displayPath(graph_t *, int, int);

int
main(int argc, char *argv[]){
	graph_t graph;
	int i, j, k, dijk=0;
	char ch, str[6];
	if(argc!=2){
		printf("Usage: %s <input filename>\n", argv[0]);
		exit(1);
	}
	FILE *fp = fopen(argv[1], "r");
	if(fp==NULL){
		printf("File not found.\n");
		exit(1);
	}
	
	graph.numOfVertices=0;
	//Checking the number of vertices
	while((ch=fgetc(fp))!='\n'){
		if(ch==' ')
			graph.numOfVertices++;
	}
	graph.numOfVertices=graph.numOfVertices+1;

	//Reading from the text file
	i=1, j=1;
	rewind(fp);
	while(!feof(fp)){
		ch = fgetc(fp);
		if(ch!=' ' && ch!='\n'){
			str[k]=ch;
			k++;
		}else{
			if(str[0]!='x'){
				graph.dist[i][j]=atoi(str);
			}else{
				graph.dist[i][j]=INFINITY;
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

	//Initializations
	for(i=1; i<=graph.numOfVertices; i++){
		for(j=1; j<=graph.numOfVertices; j++){
			if(j==i || graph.dist[i][j]==INFINITY){
				graph.pred[i][j]=0;
			}else{
				graph.pred[i][j]=i;
			}
		}
	}
	
	//Floyd's Algorithm
	for(k=1; k<=graph.numOfVertices; k++){
		for(i=1; i<=graph.numOfVertices; i++){
			for(j=1; j<=graph.numOfVertices; j++){
				dijk = graph.dist[i][k] + graph.dist[k][j];
				if(dijk < graph.dist[i][j]){
					graph.dist[i][j] = dijk;
					graph.pred[i][j] = graph.pred[k][j];
				}
			}
		}
	}
		
	//Display path
	for(i=1; i<=graph.numOfVertices; i++){
		for(j=1; j<=graph.numOfVertices; j++){
			if(i!=j)
				displayPath(&graph, i, j);
		}
	}

	return 0;
}

void displayPath(graph_t *graph, int i, int j){
	int path[graph->numOfVertices], len=1, k, a;
	path[len]=j;
	k=j;
	printf("%d TO %d: ", i, j);
	while(k!=i){
		if(graph->pred[i][k]==0){
			printf("No path found.\n");
			break;
		}else{
			k=graph->pred[i][k];
			len++;
			path[len]=k;
		}
	}
	for(a=len; a>=1; a--){
		printf("%d ", path[a]);
		if(a!=1){
			printf("-> ");
		}
	}
	printf("(cost = %d)\n", graph->dist[i][j]);
}