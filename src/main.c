#include <stdio.h>

int main(int argc, char *argv[], char *env[])
{
  char *str;
  char id;
  if (argc == 1) {
  	return -1;
  }
  str = argv[1];
  if (*(str) == '-') {
  	id = *(str+1);
  	switch (id) {
  		case 'a':
  			printf("YES\n");
  			break;
  		default:
  			printf("NO\n");
  			break;
  	}
  } else {
  	printf("NO\n");
  }
  return 0;
}

