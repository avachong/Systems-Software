#include <stdio.h>
#include <stdlib.h>

FILE *file;
char currChar;
int isPrintable(char c){
	if((c >= 32 && c <= 126)||(c == 9)){
		return 1;
	}
	else return 0;
}

int isPrintable(char c);

int main(int argc, char** argv){
    //open file
	file = fopen(argv[1], "rb");
    //check if null
	if(file == NULL){
		printf("File is NULL\n");
		exit(0);
	}
    //find printable chars
	int len =0;
	while(fread(&currChar, sizeof(char), 1, file)){
		if(isPrintable(currChar)){
			len ++;
		}
		else{
			if(len >= 4){
                //seek for length*length of char 
				fseek(file, -(len +1)*sizeof(char), SEEK_CUR);
				for(int i = 0;i <= len ; i++){
					if((fread(&currChar, sizeof(char), 1, file))&&isPrintable(currChar)){
						printf("%c",currChar);
					}
				}
				printf("\n");
				len = 0;
			}
			else{
				len = 0;
			}
		}
	}
}
