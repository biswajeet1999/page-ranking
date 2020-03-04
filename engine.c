#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>

#define MAX 500

/* uses LINEAR SEARCH to search links */
int findoutboundlinks(char *data,char **files,int count)
{
    int low = 0;
    int high = count-1;
    int i;
    for(i=low;i<=high;i++){
        if(strstr(data,files[i]) != NULL)
            return i;   /* INDEX found */
    }
    return -1;  /* INDEX not found */
}


int main()
{
    int count = 0;
    int i;           /* ITERATOR */
    int countlink;
    int index;       /* store index of links */
    char **files;    /* Char pointer array */
    FILE *graphdata; /* write graph */
    FILE *f;         /* Point to each and every html file */
    struct dirent *de=NULL;
    DIR *dir = opendir("./source");

    if(dir == NULL){
        printf("Directory \"source\" not found\n");
        exit(0);
    }
    /* read total html file present */
    while(de = readdir(dir))
    {
        if(strstr(de->d_name,".html") != NULL)
           count++;
    }
    closedir(dir);

    files = (char **)malloc(count*sizeof(char *));

    dir = opendir("./source");
    printf("Finding html files...\n\n");
    for(i = 0;(de = readdir(dir))!=NULL;){
        if(strstr(de->d_name,".html")!=NULL){
            files[i] = (char *)malloc((strlen(de->d_name)+1)*sizeof(char));
            strcpy(files[i],(char *)de->d_name);
            printf("%s\n",de->d_name);
            i++;
        }
    }
    closedir(dir);
    printf("\n\nSearching complete\n");
    /* reading folder and getting html files process complete */

    graphdata = fopen("GraphData.txt","w");

    if(!graphdata){
        printf("Error opening file \"GraphData.txt\"");
        exit(0);
    }

    fprintf(graphdata,"%d\n",count);

    for(i=0;i<count;i++)
    {
        countlink = 0;
        char data[MAX]={'\0'};  /* read data from html files */

        char temp[50]={'\0'};
        strcat(temp,"source/");
        strcat(temp,files[i]);
        f = fopen(temp,"r");
        if(!f){
            printf("Error opening file %s",files[i]);
            exit(0);
        }
        fprintf(graphdata,"%d %s-",i,files[i]); /* "-" is used for tokenization purpose */

        while(!feof(f)){
            fscanf(f,"%s",data);
            if((strstr(data,"href"))!=NULL)
                countlink++;
        }
        fprintf(graphdata,"%d ",countlink);

        rewind(f);

        while(!feof(f))
        {
            fgets(data,MAX,f);
            index = findoutboundlinks(data,files,count);
            if(index != -1)
                 fprintf(graphdata,"%d ",index); /* func return the link index and stored in file by fprintf */
        }


        fprintf(graphdata,"\n"); /* print new line in text file */
        fclose(f);
    }
    fclose(graphdata);
}
