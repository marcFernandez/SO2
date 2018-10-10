#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "red-black-tree.h"
#include "linked-list.h"

char *getColumn(int c, char *content){
	int cnt = 0;
	while(cnt < c){
		content = strchr(++content, ',');
		cnt++;
	}
	++content;

	int w = 0;
	char res[5000];
	while(content[w]!=','){
		res[w] = content[w];
		w++;
	}
	content = res;
	return content;
}

int main(int argc, char **argv){

	int num,i;

	rb_tree *tree;
  	node_data *n_data;

  	list *l;
	list_item *l_item;
	list_data *l_data;

	FILE *fp;
	char str[100];
	char str2[5000];
	void **ptr;

	fp = fopen("aeroports.csv","r");
	if(fp == NULL){
		perror("Error opening file");
		return(-1);
	}

	if(fgets(str,100,fp) != NULL){
		num = atoi(str);
		ptr = malloc(sizeof(char*)*num);
		for(i = 0;i<num;i++){
			fgets(str,100,fp);
			ptr[i] = malloc(sizeof(char)*(strlen(str)+1));
			str[strlen(str)-1] = '\0';
			strcpy(ptr[i],str);
		}
	}

	/* Allocate memory for tree */
	tree = malloc(sizeof(rb_tree));
	/* Initialize the tree */
	init_tree(tree);

	printf("Insertem els nodes a l'arbre:\n");
	for(i=0;i<num;i++){
		printf("Afegim '%s' a l'arbre\n",(char *)ptr[i]);
		/* Allocate memory for data */
		n_data = malloc(sizeof(node_data));

		n_data->key = (char *)ptr[i];
		insert_node(tree,n_data);
	}
	printf("\n");

	free(ptr);
	fclose(fp);

	fp = fopen("dades.csv","r");
	if(fp==NULL){
		perror("Error opening file");
		return -1;
	}

	l = (list *)malloc(sizeof(list));
	init_list(l);

	/****** CODI PER AFEGIR ELEMENT A LA LLISTA ******/
	l_data = malloc(sizeof(list_data));
	l_data->key = 55;//key
	insert_list(l,l_data);
	/*************************************************/

	/****** CODI PER AFEGIR LLISTA AL NODE ***********/
	find_node(tree,"YUM")->list = l;
	/*************************************************/

	fgets(str2,5000,fp);

	/****** CODI PER OBTENIR LA COLUMNA **************/
	char *delay,delarray[200];
	memset(delarray, '\0', sizeof(delarray));
	char *orig,*origarray;
	char *dest,*destarray;
	delay = malloc(sizeof(char)*200);
	orig = malloc(sizeof(char)*200);
	dest = malloc(sizeof(char)*200);
	fgets(str2,5000,fp);
	printf("%s\n",str2);
	strcpy(getColumn(14,str2),delarray);
	printf("%s\n",delarray);
	strcpy(getColumn(16,str2),orig);
	strcpy(getColumn(17,str2),dest);
	/*************************************************/

	printf(".................");
	printf("\nDe l'aeroport %s a l'aeroport %s hi ha un delay de %ss.\n",orig,dest,delay);
	printf(".................\n");

	free(l);
	fclose(fp);

	return 0;
}