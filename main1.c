#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#define BUF 256

int len10(int n) {
	if(n)
		return floor(log10(n)) + 1;
	else
		return 1;
}

int is_phone(char *s) {
	int k = 0;                        
	for(int i = 0; i < strlen(s); ++i)
		if(isdigit(s[i]) || (s[i] == '(') || (s[i] == ')') || (s[i] == '-'))
			++k;
	return (k == strlen(s));
}

int is_id(char *s) {
	int k = 0;                        
	for(int i = 0; i < strlen(s); ++i)
		if(isdigit(s[i]))
			++k;
	return (k == strlen(s));	
}
		

void find_by_name(char *name_part, FILE *fp) {
	assert(fp);
	char id[BUF], name[BUF], phone[BUF];
	fseek(fp, 0, SEEK_SET);
	int r = 0;
	while(!feof(fp)) {
		++r;
		fseek(fp, 1, SEEK_CUR);
	}
	printf("%d", r);
	while(!feof(fp)) {
		fscanf(fp, "%s%s%s", id, name, phone);
		for(int i = 0; i < strlen(name); ++i)
			printf("%c", name[i]);
		if(strstr(name, name_part)) {         
			printf("Y");
			printf("%s %s %s\n", id, name, phone);
		}			
	}
}

char *trim(char *phone) {
	char *res;
	int cnt = 0;
	for(int i = 0; i < strlen(phone); ++i) 
		if(isdigit(phone[i]))
			res[cnt++] = phone[i];
	return res;
}


void find_by_phone(char *phone, FILE *fp) {
	char entry[3][BUF];
	while(!feof(fp)) {
		for(int i = 0; i < 3; ++i) 
			fscanf(fp, "%s", entry[i]);
		if(strstr(entry[2], phone)) {
			for(int i = 0; i < 3; ++i)
				printf("%s ", entry[i]);
			printf("\n");
		}			
	}	
}

void create(char *name, char *phone, FILE *fp) {
	int id, cur = 0;
	char str[BUF], str1[BUF];
	/*while(!feof(fp)) { 
		fscanf(fp, "%d", &id);
		if (id == cur)
			++cur;			
		fscanf(fp, "%s%s", str, str1);
	}             */
	fprintf(fp, "%d %s %s\n", cur, name, phone);
	printf("%d %s %s\n", cur, name, phone);
	fclose(fp);
}

void delete(int id, FILE *fp) {
	int x;
	char name[BUF], phone[BUF];
	while(!feof(fp)) { 
		fscanf(fp, "%d", &x);
		fscanf(fp, "%s%s", name, phone);
		if(x == id)
			break;
	}
	fseek(fp, -1, SEEK_CUR);
	for(int i = strlen(name) + strlen(phone) + len10(id) - 1; i >= 0; --i) {
		fputc(' ', fp);	
		if(i)
			fseek(fp, -2, SEEK_CUR);
	}
}

void change(int id, char *name, char *phone, FILE *fp, int t) {                            // 1 - name, 2 - phone
	int x, i, pos;
	char name_old[BUF], phone_old[BUF], str[BUF];
	FILE *tmp = tmpfile();
	if(!tmp) {
		printf("Failed creating temporary file\n");
		return;
	}
	else {
		while(!feof(fp)) {
			fscanf(fp, "%d%s%s", &x, name_old, phone_old);
			if(x == id) 
				break;
			fprintf(tmp, "%d %s %s", x, name_old, phone_old);
		}
		if(t == 1)
			fprintf(tmp, "%d %s %s", x, name, phone_old);
		else
			fprintf(tmp, "%d %s %s", x, name_old, phone);
		while(!feof(fp)) {
			fscanf(fp, "%d%s%s", &x, name_old, phone_old);
			fprintf(tmp, "%d %s %s", x, name_old, phone_old);
		}
		fseek(fp, 0, SEEK_SET);
		fseek(tmp, 0, SEEK_SET);
		while(!feof(tmp)) {
			fscanf(tmp, "%s", str);
			fprintf(fp, "%s", str);
		}
	}
}




int main(int argc, const char *argv[]) {
	FILE *fp = fopen(argv[1], "r+");
	//FILE *fp = fopen(argv[1], "w+");
	assert(fp);
	//assert(fp);
	char cmd[BUF], name_part[BUF], name[BUF], phone[BUF], id[BUF], str[BUF];
    for(;;) {
		scanf("%s", cmd);
		if(!strcmp(cmd, "find")) {
			scanf("%s", str);	
			if(is_phone(str))
				find_by_phone(str, fp);
			else {
				printf("Y");
				find_by_name(str, fp);
			}	
		}                         
		else if(!strcmp(cmd, "create")) {
			scanf("%s%s", name, phone);
			while(!is_phone(phone)) {
				printf("Try again\n");
				scanf("%s%s", name, phone);	
			}
			fprintf(fp, "%s %s\n", name, phone);
			//create(name, phone, fp);
			fp = fopen(argv[1], "r+");
		}
		else if(!strcmp(cmd, "delete")) {
			scanf("%s", id);
			while(!is_id(id)) {
				scanf("%s", id);
				printf("Try again\n");
			}
			delete(atoi(id), fp);
		}
		else if(!strcmp(cmd, "change")) {
			scanf("%s%s", id, str);
			while(!is_id(id) || (!is_phone(str))) {
				scanf("%s%s", id, str);
				printf("Try again\n");
			}
			if(is_phone(str))
				change(atoi(id), str, str, fp, 2);
			else 
				change(atoi(id), trim(str), trim(str), fp, 1);
		}
	    else if(!strcmp(cmd, "exit"))
			return 0;
		else
			printf("Try again\n");
			
	}
	fclose(fp);
	//fclose(fp);
	return 0;
}