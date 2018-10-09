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
		printf("%d\n",cnt);
		cnt++;
	}
	++content;
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

	/*
	for(i=0;i<num;i++){
		printf("%s\n",ptr[i]);
	}
	*/

	free(ptr);
	fclose(fp);

	fp = fopen("test_getColumna.csv","r");
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

	find_node(tree,"YUM")->list = l;

	char *test;
	test = malloc(sizeof(char)*20);

	fgets(str,500,fp);

	printf("%s\n",str);

	test = getColumn(3,str);

	printf(".................");
	printf("\n%s\n",test);
	printf(".................\n");

	memcpy(test,&test[0],1);

	printf(".................");
	printf("\n%s\n",test);
	printf(".................\n");

	free(l);
	fclose(fp);

	return 0;
}