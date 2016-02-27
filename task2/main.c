#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#define BUF 256
#define N 1<<20

struct person {
	int id;
	char name[BUF]; 
	char phone[BUF];
};

struct person people[N];
int used[N];

int is_name(char *s) {
	for(int i = 0; i < strlen(s); ++i)
		if(!isalpha(s[i]) )
			return 0;
	return 1;
}
int is_phone(char *s) {
	int k = 0, j = 0;
	int par0 = 0, par1 = 0;                       
	for(int i = 0; i < strlen(s); ++i) {
		if(isdigit(s[i]) || ( (s[i] == '-') && ( (i > 0 && s[i - 1] != '-') || (i == 0) ) ) || (s[i] == '(') || (s[i] == ')') || (s[i] == '+') )
			++k;
		if(s[i] == '(') 
			++par0;
		if(s[i] == ')')
			++par1;
		if(i != 0 && s[i] == '+')
			j = 1;
	}
	return (k == strlen(s) && par0 == par1 && par0 <= 1 && !j);
}

int is_id(char *s) {
	int k = 0;                        
	for(int i = 0; i < strlen(s); ++i)
		if(isdigit(s[i]))
			++k;
	return (k == strlen(s));	
}
		
char *lower(char *s) {
	char *res = (char *) malloc(strlen(s) * sizeof(char));
	for(int i = 0; i < strlen(s); ++i) {
		if(s[i] - 'A' >= 0 && s[i] - 'A' < 26)
			res[i] = s[i] - 'A' + 'a';
		else
			res[i] = s[i];
	}	
	return res;
}				

void find_by_name(char *name_part) {
	//printf("Found by name %s:\n", name_part);
	for(int i = 1; i < N; ++i) {
		if(strstr(lower(people[i].name), lower(name_part)))
			printf("%s is substring of %s\n", lower(name_part), lower(people[i].name));
		if(strstr(lower(people[i].name), lower(name_part)) && people[i].id != -1) 
			printf("%d %s %s\n", people[i].id, people[i].name, people[i].phone); 
	}
}

char *trim(char *phone) {
	char *res = (char *) malloc(strlen(phone) * sizeof(char));
	int cnt = 0;
	for(int i = 0; i < strlen(phone); ++i) 
		if(isdigit(phone[i]))
			res[cnt++] = phone[i];
	return res;
}


void find_by_phone(char *phone) {
	//printf("Found by phone %s:\n", phone);
	for(int i = 1; i < N; ++i) 
		if(!strcmp(people[i].phone, phone)  && people[i].id != -1) 
			printf("%d %s %s\n", people[i].id, people[i].name, people[i].phone);
}

void create(char *name, char *phone) {
	int uuid = 0; //least unused identifier
	int sz = 0;
	memset(used, 0, N);
	for(int i = 0; i < N; ++i) 
		if(people[i].id != -1) {
			used[people[i].id] = 1;
			++sz;
		}       /*
	for(int i = 0; i < N; ++i)
		if(!used[i]) {
			uuid = i;
			break;
		}             */
	//printf("%d %d\n", sz, uuid);
	people[sz].id = uuid;
	strcpy(people[sz].name, name);
	strcpy(people[sz].phone, phone);	
	for(int i = 0; i < N; ++i) {
		if(people[i].id != -1)
			printf("%d %s %s\n", people[i].id, people[i].name, people[i].phone);
	}
}

void delete(int id) {
	for(int i = 0; i < N; ++i) 
		if(people[i].id == id)
			people[i].id = -1;
}

void change(int id, char *name, char *phone, int t) {                            // 1 - name, 2 - phone
	for(int i = 0; i < N; ++i)
		if(people[i].id == id) {
			if (t == 1)
				strcpy(people[i].name, name);
			else
				strcpy(people[i].phone, phone);	

		}
}




int main(int argc, const char *argv[]) {
	FILE *fp = fopen(argv[1], "r+");
	assert(fp);
	char s[BUF];
	char cmd[BUF], name_part[BUF], name[BUF], phone[BUF], id[BUF], str[BUF];
	int id_int, cnt = 0;
	int i;
	for(i = 0; i < N; ++i) 
		people[i].id = -1;
	while(!feof(fp)) {
		fscanf(fp, "%d%s%s", &id_int, name, phone);
		//printf("%d %s %s", id_int, name, phone);
		people[cnt].id = id_int; strcpy(people[cnt].name, name); strcpy(people[cnt].phone, phone);	
		++cnt;
	}               
	for(;;) {
		scanf("%s", cmd);
		if(!strcmp(cmd, "find")) {
			scanf("%s", str);	
			if(is_phone(str))
				find_by_phone(str);
			else if(is_name(str)) {
				find_by_name(str);
			}	
		}                         
		else if(!strcmp(cmd, "create")) {
			scanf("%s%s", name, phone);
			while(!is_name(name) || !is_phone(phone)) {
				printf("Try again\n");
				scanf("%s%s", name, phone);	
			}
			create(name, phone);
		}
		else if(!strcmp(cmd, "delete")) {
			scanf("%s", id);
			while(!is_id(id)) {
				printf("Try again\n");
				scanf("%s", id);
			}
			delete(atoi(id));
		}
		else if(!strcmp(cmd, "change")) {
			scanf("%s%s", id, str);
			while(!is_id(id) || (!is_phone(str))) {
				printf("Try again\n");
				scanf("%s%s", id, str);
			}
			if(is_phone(str))
				change(atoi(id), str, str, 2);
			else 
				change(atoi(id), trim(str), trim(str), 1);
		}
	    else if(!strcmp(cmd, "exit")) {
	    	fclose(fp);
	    	FILE *fpw = fopen(argv[1], "w");
	    	for(i = 1; i < N; ++i) 
				if(people[i].id != -1) {
					//printf("%d %d %s %s\n", i, people[i].id, people[i].name, people[i].phone);
					fprintf(fpw, "%d %s %s\n", people[i].id, people[i].name, people[i].phone);
				}
			fclose(fpw);                   
			return 0;
	    }
		else
			printf("Try again\n");
			
	}
	fclose(fp);
	FILE *fpw = fopen(argv[1], "w");
	for(i = 1; i < N; ++i) 
		if(people[i].id != -1) {
			//printf("%d %d %s %s\n", i, people[i].id, people[i].name, people[i].phone);
			fprintf(fpw, "%d %s %s\n", people[i].id, people[i].name, people[i].phone);
		}
	fclose(fpw);                   
	return 0;
}
