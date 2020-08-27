#include "../header.h"

int main(){
	char buff[100];
	fgets(buff, 100, stdin);
	fprintf(stdout, "%s", buff);
	return 0;
}
