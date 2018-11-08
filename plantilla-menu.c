/**
 *
 * Practica 3 
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "red-black-tree.h"
#include "linked-list.h"
#define MAXLINE      200
#define MAGIC_NUMBER 0x0133C8F9

/**
 * 
 *  Menu
 * 
 */
rb_tree * tree = NULL;
int num;

/*
char* getColumn(char *str, int columna){
    int count = 0,i,j=0;
    char *valor = calloc(4,sizeof(char));
    for(i = 0; i < strlen(str);i++){
		if(count == columna-1){
		    while(str[i] != ','){	
				valor[j] = str[i];
				j++;
				i++;
		    }
	    }
	    if(str[i]==','){
		    count++;
        }
    }
	return valor;
}
*/
char* getColumn(char *str, int columna,char *cosa){
    int count = 0,i,j=0;
    for(i = 0; i < strlen(str);i++){
		if(count == columna-1){
		    while(str[i] != ','){	
				cosa[j] = str[i];
				j++;
				i++;
		    }
	    }
	    if(str[i]==','){
		    count++;
        }
    }
	return cosa;
}

int menu() 
{
    char str[5];
    int opcio;

    printf("\n\nMenu\n\n");
    printf(" 1 - Creacio de l'arbre\n");
    printf(" 2 - Emmagatzemar arbre a disc\n");
    printf(" 3 - Llegir arbre de disc\n");
    printf(" 4 - Consultar informacio de l'arbre\n");
    printf(" 5 - Sortir\n\n");
    printf("   Escull opcio: ");

    fgets(str, 5, stdin);
    opcio = atoi(str); 

    return opcio;
}

void printRetard(char * IATA){
	node_data *n_data;
	list_item *l_item;
	n_data = find_node(tree,IATA);
	if(n_data != NULL){
		l_item = n_data->list->first;
		printf("Media de retardos para %s\n",IATA);
		printf("Retardos para el aeropuerto: %s\n",IATA);
		while(l_item){
			printf("   %s  --  %.3f minutos\n",(char*)l_item->data->key,l_item->data->delay/l_item->data->num_vols);
			l_item = l_item->next;
		}
	}
	else{
		printf("El aeropuerto %s no existe en arbol",(char *)IATA);
	}
}

void crearArbre(char * aeroports,char * dades){
	int i;
	list *l;
	list_data *l_data;
	list_item *l_item;
	FILE *fp;
	char str[100],str2[5000];
  	node_data *n_data;
	char *delay, *orig, *dest;
/*
	delay = malloc(sizeof(char)*4);
	orig = malloc(sizeof(char)*4);
	dest = malloc(sizeof(char)*4);
*/
	delay = calloc(4,sizeof(char));
	orig = calloc(4,sizeof(char));
	dest = calloc(4,sizeof(char));
		
	fp = fopen(aeroports,"r");
	if(fp == NULL){
		perror("Could not open file");
		exit(-1);
	}
	if(fgets(str,100,fp) != NULL){
		num = atoi(str);
		tree = malloc(sizeof(rb_tree));
		init_tree(tree);
		for(i = 0;i<num;i++){
			fgets(str,100,fp);
			n_data = malloc(sizeof(node_data));
			n_data->key = malloc(sizeof(char)*(strlen(str)+1));
			str[strlen(str)-1] = '\0';
			strcpy(n_data->key,str);
			insert_node(tree,n_data);
			l = (list *)malloc(sizeof(list));
			n_data->list = l;
			init_list(n_data->list);
		}
	}
	fclose(fp);
	fp = fopen(dades,"r");
	if(fp==NULL){
		perror("Could not open file");
		exit(-1);
	}
	
	fgets(str2,5000,fp);

	while(fgets(str2,5000,fp)!=NULL){

		getColumn(str2,15,delay);
		getColumn(str2,17,orig);
		getColumn(str2,18,dest);
		
		n_data = find_node(tree,orig);
		l_data = find_list(n_data->list, dest);
		if(l_data == NULL){
			l_data = malloc(sizeof(list_data));
			l_data->key = dest;
			if(strcmp(delay,"NA") ==0){
				l_data->delay = 0;
			}
			else{	
				l_data->delay = atof(delay);
			}
			l_data->num_vols = 1;
			insert_list(n_data->list,l_data);
		}
		else{
			if(strcmp(delay,"NA") !=0){
				l_data->delay += atof(delay);	
			}	
			l_data->num_vols += 1;	
		}
		
	}
	free(delay);
	free(dest);
	free(orig);
	printf("Arbre creat\n");
	fclose(fp);
}
void carregarArbre(FILE *fp){
	char buffer[100];
	int i,j1,num_nodes,num_destins,maxDest = 0,magic;
	float x;
	char *aux;
	node_data *n_data;
	list_item *l_item;
	list_data *l_data;
	list *l;

	tree = malloc(sizeof(rb_tree));
	init_tree(tree);
	fread(&magic,sizeof(int),1,fp);
	int realMagic = MAGIC_NUMBER;
	if(55 != MAGIC_NUMBER){
		perror("Error en el nombre Magic");
		exit(-1);
	}
	fread(&num,sizeof(int),1,fp);
	for(num_nodes=0;num_nodes<num;num_nodes++){

		fread(buffer, 3, 1, fp);
		buffer[3] = '\0';       
		aux = (char*)buffer;
		fread(&num_destins, sizeof(int), 1, fp); 
		n_data = malloc(sizeof(node_data));
		n_data->key = malloc(sizeof(char)*(strlen(aux)+1));
		strcpy(n_data->key,aux);
		l = (list *)malloc(sizeof(list));
		n_data->list = l;
		init_list(n_data->list);
		insert_node(tree,n_data);
		for(i=0;i<num_destins;i++){
			fread(buffer,3,1,fp);
			buffer[3] = '\0';    
			l_data = malloc(sizeof(list_data));
			aux = malloc(sizeof(char)*4);
			strcpy(aux,buffer);
			l_data->key = aux;
			fread(&j1,sizeof(int),1,fp);
			l_data->num_vols = j1;
			fread(&x,sizeof(float),1,fp);
			l_data->delay = x;
			insert_list(n_data->list,l_data);
		}
	}
	printf("Arbre creat\n");
}
void guardarArbre(node *child,FILE *fp)
{
    if (child->right != NIL)
	guardarArbre(child->right,fp);

    if (child->left != NIL)
	guardarArbre(child->left,fp);

    fwrite(child->data->key, strlen(child->data->key), 1, fp);
    int num_destins = child->data->list->num_items;
    fwrite(&num_destins,sizeof(int),1,fp);
    if(num_destins > 0){
	list_item *l_item = child->data->list->first;
	int a;
	float f;
	while(l_item){
		fwrite(l_item->data->key,strlen(l_item->data->key),1,fp);
		a = l_item->data->num_vols;
		fwrite(&a,sizeof(int),1,fp);
		f = l_item->data->delay;
		fwrite(&f,sizeof(float),1,fp);
		l_item = l_item->next;
	}

    }
}

void recursion(rb_tree *t,node *child,char *res){
	
	if(child->right != NIL){
		recursion(t,child->right,res);
	}
	if(child->left != NIL){
		recursion(t,child->left,res);
	}
	if(child->data->list->num_items > find_node(t,res)->list->num_items){
		strcpy(res,child->data->key);
	}
}

char *massDestinos(rb_tree *tree){
	node *n = tree->root;
	char *r;
	r = malloc(sizeof(char)*4);
	strcpy(r,n->data->key);
	recursion(tree,n,r);
	return r;
}

int main(int argc, char **argv)
{
    char str1[MAXLINE], str2[MAXLINE];
    int opcio;
    FILE *fp;

    if (argc != 1)
        printf("Opcions de la linia de comandes ignorades\n");

    do {
        opcio = menu();
        printf("\n\n");


        switch (opcio) {
            case 1:
				if(tree != NULL){
					printf("Alliberant arbre\n\n");
					delete_tree(tree);
				}
                printf("Introdueix fitxer que conte llistat d'aeroports: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;

                printf("Introdueix fitxer de dades: ");
                fgets(str2, MAXLINE, stdin);
                str2[strlen(str2)-1]=0;
				crearArbre(str1,str2);
		
                break;

            case 2:
				if(tree == NULL){
					printf("Primer has de crear un arbre\n");
				}
				else{
			        printf("Introdueix el nom de fitxer en el qual es desara l'arbre: ");
			        fgets(str1, MAXLINE, stdin);
			        str1[strlen(str1)-1]=0;

			
		    		fp = fopen(str1,"w");
		    		int magic = MAGIC_NUMBER;
					fwrite(&magic,sizeof(int),1,fp);
		    		fwrite(&num,sizeof(int),1,fp);
					guardarArbre(tree->root,fp);
	    			printf("Arbre guardat\n");
		    		fclose(fp);
				}
                break;

            case 3:
                printf("Introdueix nom de fitxer que conte l'arbre: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;
				fp = fopen(str1,"r");
				if(tree != NULL){
					delete_tree(tree);
					printf("Alliberant arbre\n");
				}
				carregarArbre(fp);
				fclose(fp);
                break;

            case 4:
				if(tree == NULL){
					printf("No hi ha arbre creat.\n");
				}
				else{
			        printf("Introdueix aeroport per cercar retard o polsa enter per saber l'aeroport amb mes destins: ");
			        fgets(str1, MAXLINE, stdin);
			        str1[strlen(str1)-1]=0;
					if(strlen(str1) == 0){
						char *md;
						md = massDestinos(tree);
						printf("\nAeropuerto con mas destinos %s.\n",md);
						printf("\nCon un total de %d vuelos.\n",find_node(tree,md)->list->num_items);
						free(md);
					}
					else{
						printRetard(str1);
					}
				}
                break;

            case 5:
            	/*if(tree != NULL){
            		delete_tree(tree);
            	}*/
                break;

            default:
                printf("Opcio no valida\n");

        } 
    }
    while (opcio != 5);

    return 0;
}