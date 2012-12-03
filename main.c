#include <stdio.h>

void read_input() {
	printf(">> ");

	char input[256];
	fgets(input, sizeof(input), stdin);
	input[strlen(input) - 1] = '\0'; // remove new line
}

int main() {
	printf("\n");
	read_input();
	while(1) {
		read_input();
	}
	return 0;
}
