#include "hare.h"


int main(int argc, char *argv[], char *env[])
{
  char *str;
  char id;
  int i;
  FileNode *head = NULL;
  FileNode *iter;
  FILE *f = NULL;
  if (argc == 1) {
  	return -1;
  }
  str = argv[1];
  if (*(str) == '-') {
  	id = *(str+1);
  	switch (id) {
  		case 'a':
  			i = 2;
  			for (i=2;i<argc;i++){
  				f = fopen(argv[i], "r");
  				if (f != NULL){
  					if (head == NULL){
  						head       = (FileNode*)malloc(sizeof(FileNode));
  						iter       = head;
  					} else {
  						iter->next = (FileNode*)malloc(sizeof(FileNode));
  						iter       = iter->next;
  					}
  					iter->file     = f;
  					iter->fileName = argv[i];
  					iter->next     = NULL;
  					iter->nameLen  = 0;//Сделаю позже
  				}
  			}
  			break;
  		default:
  			break;
  		}
  	}
  	if (head != NULL){
  		iter = head;
  		printf("%s\n", iter->fileName);
  	}
  	return 0;
}

