#include <stdio.h>
#include <string.h>

int main() {
	char a[100] = "ab cd df ww w w wrr f";
	char * x = strtok(a," ");
	printf("%u\n",x);

	char * y = x;
	char * z = y;
	x = strtok(NULL," ");
	printf("%s\n",x);
	printf("%s\n",y);
	printf("%s\n",z);

	printf("%u\n",x);
	printf("%u\n",y);
	printf("%u\n",z);
x = strtok(NULL," ");
	printf("\n%u\n",x);
	x = strtok(NULL," ");
	printf("\n%u\n",x);



	return 0;
}
