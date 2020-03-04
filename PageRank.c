/*
*  Random Surfer Model Algorithm: PR(A)= (1-d)+d(PR(T1)/C(T1)+...+PR(Tn)/C(Tn))
*  Implemented by Google
*  Rank value rages from 0-10
*  Damping/Teleportation factor ranges from 0-1
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 50
#define MAX_ITERATION 100
#define DAMPING_FACTOR 0.85

typedef struct edge
{
    int targetvertex;
}Edge;

typedef struct vertex
{
    int vertexno;
    int totedges;
    char *vertexname;
    Edge *edges;
}Vertex;

typedef struct graph
{
    int totvertex;
    Vertex *vertices;
}Graph;

Graph *creategraph()
{


    char get[MAX],*str_tok;
    int i,j;        /* Iterator */
    int getdata;    /* Collect data from File */
    FILE *graphdata = fopen("GraphData.txt","r");

    if(!graphdata){
        printf("Error opening file \"GraphData.txt\"");
        exit(0);
    }
    /*create graph*/
    Graph *g = (Graph *)malloc(sizeof(Graph));
    fscanf(graphdata,"%[^\n]s",get);
    g->totvertex = atoi(get);
    g->vertices = (Vertex *)malloc(sizeof(Vertex)*(g->totvertex));
    fgetc(graphdata);   /* It will remove last new line character */
    for(i=0;i<g->totvertex;i++)
    {

        fscanf(graphdata,"%[^\n]s",get);

        /* Read Vertex no */
        fgetc(graphdata);
        str_tok = strtok(get," ");
        g->vertices[i].vertexno = atoi(str_tok);

        /* Read Vertex name */
        str_tok = strtok(NULL,"-");
        g->vertices[i].vertexname = (char *)malloc((strlen(str_tok)+1)*sizeof(char));
        strcpy(g->vertices[i].vertexname,str_tok);

        /* Read no of outbound links */
        str_tok = strtok(NULL," ");
        g->vertices[i].totedges = atoi(str_tok);
        g->vertices[i].edges = (Edge *)malloc((g->vertices[i].totedges)*sizeof(Edge));

        /* Read outbound links one by one */
        for(j=0;j<(g->vertices[i].totedges);j++)
        {
            str_tok = strtok(NULL," ");
            g->vertices[i].edges[j].targetvertex = atoi(str_tok);

        }

    }

    fclose(graphdata);
    return g;
}

void displaygraph(Graph *g)
{
    int i,j;
    printf("\nGraph: \n");
    for(i=0;i<(g->totvertex);i++)
    {
        for(j=0;j<(g->vertices[i].totedges);j++)
        {
            printf("%30s -----------> %s\n",g->vertices[i].vertexname,g->vertices[g->vertices[i].edges[j].targetvertex].vertexname);
        }
    }
    printf("\n\n");
}

void random_surfer_algorithm(Graph *g)
{
    int i,j,k,loop;                       /* Iterator */
    double rank[g->totvertex];     /* hold rank of all Vertex */
    FILE *PR = fopen("PageRank.txt","w");
    if(!PR){
        printf("Error opening file \"PageRank.txt\"");
        exit(0);
    }
    /* Initialization of page rank & check for DANGLING link */
    for(i=0;i<g->totvertex;i++)
    {
        if(g->vertices[i].totedges == 0){
            rank[i] = 0.0;   /* Dangling link */
        }else{
            rank[i] = 1.0;
        }
        fprintf(PR,"%30s:  ",g->vertices[i].vertexname);
        fprintf(PR,"%lf\n",rank[i]);
    }
    fprintf(PR,"\n");
    /* Initialization complete */
    for(loop = 1;loop <= MAX_ITERATION; loop++)
    {
        fprintf(PR,"LOOP %d: \n",loop);
        fprintf(PR,"----------------------------------------------------------------------------------------------------------------------------------\n");
        for(i = 0;i<g->totvertex;i++)
        {
            if(rank[i] != 0)     /* If it is not dangling link */
            {
                rank[i] = 0;
                for(j = (i+1)%g->totvertex;j != i;)        /* Traverse each vertex */
                {
                    for(k = 0;k < g->vertices[j].totedges;k++)  /* Traverse each edge */
                    {
                        if(g->vertices[j].edges[k].targetvertex == i)
                        {
                            rank[i] = rank[i]+((double)rank[j]/(double)g->vertices[j].totedges); /* calculate PR(T1)/C(T1)+...+PR(Tn)/C(Tn) */
                            break;
                        }
                    }
                    j = (j+1)%g->totvertex;
                }
                rank[i] =  ( 1 - DAMPING_FACTOR ) + ( DAMPING_FACTOR * rank[i] );
                fprintf(PR,"%30s:  ",g->vertices[i].vertexname);
                fprintf(PR,"%lf  \n",rank[i]);
            }
        }
        fprintf(PR,"\n\n");
    }

    /* Display Rank */
    printf("\nPage Rank:\n");
    for(i=0;i<g->totvertex;i++)
    {
        printf("%30s: %lf\n",g->vertices[i].vertexname,rank[i]);
    }

    fclose(PR);
}



int main()
{
    system("engine");
    Graph *g = creategraph();
    displaygraph(g);
    random_surfer_algorithm(g);
}
