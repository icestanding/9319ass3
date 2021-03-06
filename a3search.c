#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

// Basically use the BM Horspool algorithm

//Declaration
// working for the straight edition
void Bad_match_table(size_t *a, const char * pattern);
float BMH(char *adds,const char * pattern);
int compare(char *a, char *b);

typedef struct node {
    char *adds;  //remember the name of file
    float num;  //recorde the number of it occur
    struct node * next;
} Node;

typedef struct list {
    Node * head;
} Dictionary;

Node * CreateNode(int num, char * adds) {
    Node *a;
    a = (Node *)malloc(sizeof(Node));
    a->num = num;
    a->adds = adds;
    a->next = NULL;
    return a;
}


Dictionary *CreateList() {
    Dictionary *p = (Dictionary *)malloc(sizeof(Dictionary));
    p->head = NULL;
    return p;
}

void AddNode(Dictionary *p, Node *node) {
    if(p->head == NULL){
        p->head = node;
        return;
    }
    else {
        Node *vector = p->head;
        while (vector->next!=NULL){
            vector = vector->next;
        }
        vector->next = node;
        return;
    }

    // below is insertion sort

}
// delete behind node and return next

Node * DeleteNode(Node * front, Node * behind, Dictionary *a) {
    if (behind->next == NULL) {
        if(front == NULL) {
            return NULL;
        }
        front->next = NULL;
        return front->next;
    }
    else {
        Node * tmp = behind->next;

        if (front == NULL) {
            a->head = tmp;
            return tmp;
        }
        else {
            front->next = tmp;
            return tmp;
        }
    }
}

// adds is the file address
float BMH(char *adds,const char * pattern) {
    size_t *a = (size_t *)malloc(256*sizeof(size_t));
    int i = 0;
    size_t size = strlen(pattern);


    for (i = 0; i < 256; i++) {
        a[i] = size;
    }


    Bad_match_table(a, pattern);

    //BMH Search
    FILE *fp = fopen(adds, "r");

    //initial readline arguments
    int bytes_read;
    size_t nbytes = 1000;
    char *my_string = NULL;
    int tmp = 0;
    float result = 0;

    int length = strlen(pattern);
    while (1)
    {
        //bit operation
        // my_string = (char *) malloc (nbytes + 1);
        bytes_read = getline (&my_string, &nbytes, fp);
        int pos = 0;
        if (bytes_read == -1)
        {
            break;
        }
        else
        {
            while ((bytes_read - pos) >= length) {
                i = length - 1;
                while (tolower(my_string[pos + i]) == tolower(pattern[i])) {
                    if (i == 0) {
                        result += 1;
                        pos += 1;
                        break;
                    }
                    i = i - 1;
                }
                pos = pos + a[tolower(my_string[pos + length - 1])];
            }

        }


    }
    if (a)
    {
        free(a);
    }

    fclose(fp);
    return result;

}

void Bad_match_table(size_t *a, const char * pattern) {
    int i = 0;
    size_t  length = strlen(pattern);
    size_t  value = 0; // to compute the bad match table for BMH
    for (i = 0; i < length - 1; i++) {
        a[tolower(pattern[i])] = length - i - 1;
    }
}


void InsertionSort(Dictionary *p, Node *node) {
    if (p->head == NULL)
    {
        p->head = node;
        return;
    }
    Node *n = p->head;
    Node *tmp = NULL; // store tmp pointer

    if (n->num < node->num) {
        node->next = n;
        p->head = node;
        return;
    }
    if (n->num == node->num) {
        int k = compare(n->adds, node->adds);
        if (k == 0) {
            node->next = n;
            p->head = node;
            return;
        }
        if (k == 1) {
            if(n->next!=NULL) {
                tmp = n;
                n = n->next;
            }
            else {
                n->next = node;
                return;
            }
        }
    }

    while(n->next != NULL)
    {
        if (n->num > node->num )
        {
            tmp = n;
            n = n->next;
        }
        else if (n->num == node->num)
        {
            int k = compare(n->adds, node->adds);
            if (k == 1) {
                tmp = n;
                n = n->next;
            }
            if (k == 0) {
                tmp->next = node;
                node->next = n;
                return;
            }
        }
        else {
            tmp->next = node;
            node->next = n;
            return;
        }
    }
    if(n->num < node->num) {
        tmp->next = node;
        node->next = n;
    }
    else if (n->num == node->num)
    {
        int k = compare(n->adds, node->adds);
        if (k == 1) {
            n->next = node;
        }
        if (k == 0) {
            tmp->next = node;
            node->next = n;
        }
    }
    else {
        n->next = node;
    }
    return;
}

// compute average and sort;
void Average(Dictionary *a, Dictionary *b, int num) {
    if(a->head == NULL) {
        return;
    }
    else {
        Node * node = a->head;
        while (node->next != NULL) {
            node->num = node->num/num;
            Node * tmp = node;
            node = node->next;
            tmp->next = NULL;
            InsertionSort(b, tmp);
        }
        node->num = node->num/num;
        InsertionSort(b, node);
        return;
    }
}

// compare two file name, determine the lexical order.
int compare(char *a, char *b) {
    int la = strlen(a);
    int lb = strlen(b);
    int i = 0;
    int tmpa = 0;
    int tmpb = 0;
    while (tmpa == tmpb) {
        (i < la)?(tmpa = a[i]):(tmpa = 0);
        (i < lb)?(tmpa = b[i]):(tmpb = 0);
        i++;
    }
    if(tmpa > tmpb) {
        return 1;
    }
    else {
        return 0;
    }
}





int main(int argc, char const *argv[])
{
// adds is argv[1]
// if argv[2] is '-s' argv[3] is the size, argv[4]...argv[n] could be the pattern
// else argv[2]...argv[n] could be the pattern

    char *mode = "-s";
    int num_pattern;
    int j = 0;
    int i = 0;

    DIR *dir = opendir (argv[1]);
    struct dirent *ent;
    Dictionary *a = CreateList();

// testing code
//    char adds[] = "/Users/chenyu/Desktop/cnm.txt";
//    int result = BMH(adds, "conclusions");
//    printf("%d",result);

    char *slash = "/";
    char *nothing = "..";
    char *nothing2 = ".";
    int flag = 0;

    (strcmp(mode, argv[3]) == 0)?(num_pattern = argc - 5):(num_pattern = argc - 3);
    (strcmp(mode, argv[3]) == 0)?(flag = 1):(flag = 0);

    // below is the search part
    for (j = 0; j < num_pattern; ++j) {
        if (j == 0) {
            while ((ent = readdir(dir)) != NULL) {

                if (strcmp(ent->d_name, nothing) == 0)
                {
                    continue;
                }
                if (strcmp(ent->d_name, nothing2) == 0)
                {
                    continue;
                }
                char *adds = (char *) malloc(150);
                strcpy(adds, argv[1]);
                strcat(adds, slash);
                strcat(adds, ent->d_name);
                float result = 0;

                (flag == 1)? (result = BMH(adds, argv[5 + j])):(result=BMH(adds, argv[3 + j]));

                if (result == 0) {
                    if (adds)
                    {
                        free(adds);
                    }
                    continue;
                }
                char *addsname = (char *) malloc(strlen(ent->d_name) + 1);
                stpcpy(addsname, ent->d_name);
                AddNode(a, CreateNode(result, addsname));
                if (adds)
                {
                    free(adds);
                }

            }
        }
        else {

            Node *node = a->head;
            Node *front = NULL;
            //check if the frequency is empty
            if (node == NULL) {
                break;
            }
            if (node->next == NULL) {
                char *adds = (char *) malloc(150);
                strcpy(adds, argv[1]);
                strcat(adds, slash);
                strcat(adds, node->adds);

                float result = 0;
                (flag == 1)? (result = BMH(adds, argv[5 + j])):(result=BMH(adds, argv[3 + j]));
                if (result == 0) {
                    a->head = NULL;
                    break;
                }
                else {
                    a->head->num += result;
                    break;
                }

            }

            while (node->next != NULL) {

                char *adds = (char *) malloc(150);
                strcpy(adds, argv[1]);
                strcat(adds, slash);
                strcat(adds, node->adds);
                float result = 0;
                (flag == 1)? (result = BMH(adds, argv[5 + j])):(result=BMH(adds, argv[3 + j]));
                if(result == 0) {
                    node = DeleteNode(front, node, a);
                    if(node == NULL) {
                        break;
                    }
                }
                else {
                    node->num += result;
                    front = node;
                    node = node->next;
                }
            }
            if(node != NULL) {
                char *adds = (char *) malloc(150);
                strcpy(adds, argv[1]);
                strcat(adds, slash);
                float result = 0;
                strcat(adds, node->adds);
                (flag == 1)? (result = BMH(adds, argv[5 + j])):(result=BMH(adds, argv[3 + j]));
                if (result == 0) {
                    DeleteNode(front, node, a);

                }
                else {
                    node->num += result;
                }
            }

        }
    }
    Dictionary *b = CreateList();
    Average(a, b,num_pattern);
///* print all the files and directories within directory */
    Node * node = b->head;
    if(node != NULL) {
        while (node->next != NULL) {
            printf("%s\n", node->adds);
            node = node->next;
        }
        printf("%s\n", node->adds);
    }
//
    closedir (dir);
    return 0;
}