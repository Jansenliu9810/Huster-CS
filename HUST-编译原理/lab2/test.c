#include <stdio.h>
/* This is a chess board */
main(){
	int i,j;	//Two variables define the length and width of chess board
	for(i = 0; i<8; i++){
		for(j = 0; j<8; j++){
			if((i+j)%2 == 0)
				printf("%c%c",219,219);
			else
				printf(" ");
			printf("\n");
		}
	}
}