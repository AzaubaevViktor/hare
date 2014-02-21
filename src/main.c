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
                    char *nameFile = argv[i];
                    *(cnt->workFiles+numFiles) = nameFile;
                    numFiles++;
                    cnt->workFiles = realloc(cnt->workFiles, (numFiles+1)*sizeof(char *));
                    iter->file     = f;
                    iter->next     = NULL;
                    iter->info     = (FileInfo *)malloc(sizeof(FileInfo));
  				}
  			}
  			break;
  		default:
  			break;
  		}
  	}
    if (head != NULL){
  		iter = head;
        printf("%s\n", *(cnt->workFiles + 1));
    }
  	return 0;
}

