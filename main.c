#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define BUF 256
#define INF 1<<30
#define N 1<<20

struct person {
    int id;
    char *name;
    char *phone;
    struct person *next;
};

struct person *Person(int id, char *name, char *phone, struct person *next) {
    struct person *res = malloc(sizeof(struct person));
    res->id = id;
    res->name = (char *) malloc(strlen(name) * sizeof(char));
    strcpy(res->name, name);
    res->phone = (char *) malloc(strlen(phone) * sizeof(char));
    strcpy(res->phone, phone);
    res->next = next;
    return res;
}

void Person_d(struct person *p) {
    free(p->name);
    free(p->phone);
    free(p->next);
    free(p);
}

void person_out(struct person p) {
    printf("%d %s %s", p.id, p.name, p.phone);
}

int lst_size;
struct person *first, *last;
int i;

int is_name(char *s) {
    for (i = 0; i < strlen(s); ++i)
        if (!isalpha(s[i]))
            return 0;
    return 1;
}

int is_phone(char *s) {
    int k = 0, j = 0;
    int par0 = 0, par1 = 0;
    for (i = 0; i < strlen(s); ++i) {
        if (isdigit(s[i]) || ((s[i] == '-') && ((i > 0 && s[i - 1] != '-') || (i == 0))) || (s[i] == '(') ||
            (s[i] == ')') || (s[i] == '+'))
            ++k;
        if (s[i] == '(')
            ++par0;
        if (s[i] == ')')
            ++par1;
        if (i != 0 && s[i] == '+')
            j = 1;
    }
    return (k == strlen(s) && par0 == par1 && par0 <= 1 && !j);
}

int is_id(char *s) {
    int k = 0;
    for (i = 0; i < strlen(s); ++i)
        if (isdigit(s[i]))
            ++k;
    return (k == strlen(s));
}

char *lower(char *s) {
    char *res = (char *) malloc(strlen(s) * sizeof(char));
    for (i = 0; i < strlen(s); ++i) {
        res[i] = s[i] - 'A' >= 0 && s[i] - 'A' < 26 ? s[i] - 'A' + 'a' : s[i];
    }
    return res;
}

void find_by_name(char *name_part) {
    printf("Found by name %s:\n", name_part);
    struct person cur = *(first->next);
    for(i = 0; i < lst_size; ++i) {
        if(strstr(cur.name, name_part))
            person_out(cur);
        cur = *cur.next;
    }
}

char *trim(char *phone) {
    char *res = (char *) malloc(strlen(phone) * sizeof(char));
    int cnt = 0;
    for (i = 0; i < strlen(phone); ++i)
        if (isdigit(phone[i]))
            res[cnt++] = phone[i];
    return res;
}


void find_by_phone(char *phone) {
    printf("Found by phone%s: \n", phone);
    struct person cur = *(first->next);
    for(i = 0; i < lst_size; ++i) {
        if(!strcmp(phone, cur.phone))
            person_out(cur);
        cur = *cur.next;
    }
}

int used[N];

void create(char *name, char *phone) {
    struct person cur = *(first->next);
    for(i = 0; i < lst_size; ++i) {
        used[cur.id] = 1;
        cur = *cur.next;
    }
    int uuid = 0;                                                                  //least unused identifier
    while(used[uuid])
        ++uuid;
    struct person *tmp = cur.next;
    struct person *new_el = Person(uuid, name, phone, NULL);
    cur.next = new_el;
    new_el->next = tmp;
    lst_size++;
}

void delete(int id) {
    struct person cur = *(first->next);
    for(i = 0; i < lst_size; ++i) {
        if(id == (*cur.next).id) {
            cur.next = (*cur.next).next;
            Person_d(&cur);
        }
        cur = *cur.next;
    }
    lst_size--;
}

void change(int id, char *name, char *phone, int t) {                            // 1 - name, 2 - phone
    struct person cur = *(first->next);
    for(i = 0; i < lst_size; ++i) {
        if(cur.id == id) {
            if (t == 1)
                cur.name = name;
            else
                cur.phone = phone;
        }
        cur = *cur.next;
    }
}

char *read_str(int t, FILE *fp) {                                                  // read string of not fixed length
    int c;                                                                         // t == 0: stdin, 1: file
    char *res = malloc(sizeof(char));
    int cnt = 0, sz = 1;
    do {
        c = t ? getc(fp) : getchar();
        res[cnt++] = (char) c;
        if(cnt + 1 > sz) {
            res = (char *) realloc(res, 2 * sz * sizeof(char));
            sz *= 2;
        }
    } while(c != ' ' && c != '\n');
    res[cnt - 1] = '\0';
    return res;
}

void init(FILE *fp) {
    assert(fp);
    char *name, *phone;
    int id;
    char s[1], t[1];
    strcpy(s, ""); strcpy(t,"");
    first = Person(INF, s, t, NULL);
    struct person cur = *first;
    lst_size = 0;
    while(!feof(fp)) {
        if(fscanf(fp, "%d", &id) != EOF) {
            name = read_str(1, fp);
            phone = read_str(1, fp);
            struct person *elem = Person(id, name, phone, NULL);
            cur.next = elem;
            cur = *elem;
            lst_size++;
        }
    }
    last = Person(INF, s, t, NULL);
    cur.next = last;
}

void output(const char *file_name) {
    FILE *fp = fopen(file_name, "w");
    assert(fp);
    struct person cur = *(first->next);
    for(i = 0; i < lst_size; ++i) {
        fprintf(fp, "%d %s %s\n", cur.id, cur.name, cur.phone);
        cur = *cur.next;
    }
    fclose(fp);
}


int main(int argc, const char *argv[]) {
    FILE *fp = fopen(argv[1], "r+");
    init(fp);
    char cmd[BUF], id[BUF];
    for (; ;) {
        scanf("%s", cmd);
        if (!strcmp(cmd, "find")) {
            char *str = read_str(0, NULL);
            while(!is_name(str) && !is_phone(str)) {
                perror("Try again: ");
                str = read_str(0, NULL);
            }
            if (is_phone(str))
                find_by_phone(str);
            else
                find_by_name(str);

        }
        else if (!strcmp(cmd, "create")) {
            char *name = read_str(0, NULL);
            char *phone = read_str(0, NULL);
            while (!is_name(name) || !is_phone(phone)) {
                perror("Try again: ");
                name = read_str(0, NULL);
                phone = read_str(0, NULL);
            }
            create(name, phone);
        }
        else if (!strcmp(cmd, "delete")) {
            scanf("%s", id);
            while (!is_id(id)) {
                perror("Try again: ");
                scanf("%s", id);
            }
            delete(atoi(id));
        }
        else if (!strcmp(cmd, "change")) {
            scanf("%s", id);
            char *str = read_str(0, NULL);
            while (!is_id(id) || (!is_phone(str))) {
                perror("Try again: ");
                scanf("%s", id);
                str = read_str(0, NULL);
            }
            if (is_phone(str))
                change(atoi(id), str, str, 2);
            else
                change(atoi(id), trim(str), trim(str), 1);
        }
        else if (!strcmp(cmd, "exit")) {
            output(argv[1]);
            return 0;
        }
        else
            perror("Try again: ");

    }
    output(argv[1]);
    return 0;
}