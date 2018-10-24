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
char * masDestinos;
rb_tree * tree = NULL;

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
			printf("   %s  --  %.3f minutos\n",l_item->data->key,(float)(l_item->data->delay/l_item->data->num_vols));
			l_item = l_item->next;
		}
	}
	else{
		printf("El aeropuerto %s no existe en arbol",(char *)IATA);
	}
}

void crearArbre(char * aeroports,char * dades){
	int num,i;
	list *l;
	list_data *l_data;
	list_item *l_item;
	FILE *fp;
	char str[100],str2[5000];
  	node_data *n_data;
	char *delay, *orig, *dest;

	fp = fopen("aeroports.csv","r");
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
	masDestinos = n_data->key;	
	fclose(fp);
	fp = fopen("dades.csv","r");
	if(fp==NULL){
		perror("Could not open file");
		exit(-1);
	}
	

	fgets(str2,5000,fp);

	while(fgets(str2,5000,fp)!=NULL){
		delay = getColumn(str2,15);
		orig = getColumn(str2,17);
		dest = getColumn(str2,18);
		
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
		if(find_node(tree,masDestinos)->list->num_items < n_data->list->num_items){
			masDestinos = n_data->key;
		}
		
	}
	
	free(delay);
	free(dest);
	free(orig);
	fclose(fp);
}

int main(int argc, char **argv)
{
    char str1[MAXLINE], str2[MAXLINE];
    int opcio;
    node_data *n_data;
    if (argc != 1)
        printf("Opcions de la linia de comandes ignorades\n");

    do {
        opcio = menu();
        printf("\n\n");


        switch (opcio) {
            case 1:
		if(tree != NULL){
			printf("alliberant arbre\n");
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
                printf("Introdueix el nom de fitxer en el qual es desara l'arbre: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;

		FILE *fp;
		fp = fopen(str1,"w+");
		n_data = malloc(sizeof(node_data));
		n_data = find_node(tree,masDestinos);
		printf("%s",(char*)n_data->key);
		fwrite(n_data->key,sizeof(n_data->key)+1,1,fp);
		fclose(fp);
                break;

            case 3:
                printf("Introdueix nom de fitxer que conte l'arbre: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;

		char buf[100];
		fread(buf,sizeof(n_data->key)+1,1,fp);
		printf("%s",(char*) buf);

                break;

            case 4:
                printf("Introdueix aeroport per cercar retard o polsa enter per saber l'aeroport amb mes destins: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;
		if(tree == NULL){
			printf("No hi ha arbre creat.");
		}
		else{
			if(strlen(str1) == 0){
				printf("\nAeropuerto con mas destinos\n");
				printf("Aeropuerto con mas destinos: %s, destinos %d\n",masDestinos,find_node(tree,masDestinos)->list->num_items);
			}
			else{
				printRetard(str1);
			}
		}
                break;

            case 5:


                break;

            default:
                printf("Opcio no valida\n");

        } 
    }
    while (opcio != 5);

    return 0;
}
