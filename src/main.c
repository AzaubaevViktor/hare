#include "hare.h"


int main(int argc, char *argv[], char *env[])
{
  char *str;
  char id;
  int i; int numFiles=0;
  FileNode *head = NULL;
  FileNode *iter;
  FILE *f = NULL;
  Context *cnt = (Context *)malloc(sizeof(Context));
  cnt->workFiles = (char **)malloc(sizeof(char *));
  if (argc == 1) {
  	return -1;
  }
  str = argv[1];
  if (*(str) == '-') {
  	id = *(str+1);
  	switch (id) {
        case 'a':
        if (argc < 4) break;
            for (i=3;i<argc;i++){
  				f = fopen(argv[i], "r");
  				if (f != NULL){
  					if (head == NULL){
  						head       = (FileNode*)malloc(sizeof(FileNode));
  						iter       = head;
  					} else {
  						iter->next = (FileNode*)malloc(sizeof(FileNode));
  						iter       = iter->next;
                    }
                    *(cnt->workFiles + numFiles) = argv[i];
                    numFiles++;
                    cnt->workFiles = (char **)realloc(cnt->workFiles, (numFiles + 1)*sizeof(char *));
                    iter->file     = f;
                    iter->next     = NULL;
                    iter->info     = (FileInfo *)malloc(sizeof(FileInfo));
  				}
  			}
  			break;
  		default:
  			break;
  		}
    *(cnt->workFiles + numFiles) = (char *)calloc(1, sizeof(char));//Как бы нулевой символ
  	}
  printf("%s\n%s\n", *(cnt->workFiles), *(cnt->workFiles + 1));
    /*if (head != NULL){
  		iter = head;

    }*/
  	return 0;
}

