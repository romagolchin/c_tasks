#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char* argr[]) {
	int n, left, right, top, bottom;
        scanf("%d", &n);
        int** t = (int**) malloc(sizeof(int*) * n);
        int** cur = t;
        int it = 0;
        if(!cur)
            return -1;
        while(it < n) {
            int* cur1 = *cur = (int*) malloc(sizeof(int) * n);
            if(!cur1)
                return -1;
            ++cur;
            ++it;
        }
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j)
                t[i][j] = (i + 1) * (j + 1);
        }
	for(;;){
	        scanf("%d", &left);
	        if(left) {
	            scanf("%d%d%d", &top, &right, &bottom);
	            for(int j = top - 1; j < bottom; ++j) {
	                for(int i = left - 1; i < right; ++i)
	                    printf("%d ", t[j][i]);
	                printf("\n");
	            }
	        }
	        else {
	            free(t);
	            return 0;
        	}
	}
	free(t);
	return 0;
}
