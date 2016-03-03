#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

char *read_str(void) {
    int c;
    char *res = malloc(sizeof(char));
    int cnt = 0, sz = 1;
    do {
        c = getchar();
        res[cnt++] = (char) c;
        if(cnt + 1 > sz) {
            res = (char *) realloc(res, 2 * sz * sizeof(char));
            sz *= 2;
        }
    } while(c != ' ' && c != '\n');
    res[cnt - 1] = '\0';
    return res;
}

int main(int argc, const char* argv[]) {
	FILE *fp = fopen(argv[1], "r+");
	char str[256];
	printf("%s", argv[1]);
	while(!feof(fp)) {
		//fgets(str, 100, fp);
		if(fscanf(fp, "%s", str) != EOF)
			printf("%s\n", str);
	}
	char *str1 = read_str();
	printf("%s", str1);
	char full[512], first[512];
	strcpy(full, "Roma Golchin");
	strcpy(first, "Roma");
	//if(strstr(full, first))
	//	printf("Y\n");
	
	return 0;
}