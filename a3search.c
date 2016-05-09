#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <ctype.h>
//Declaration
void Bad_match_table(size_t *a, char * pattern);
int BMH(char *adds, char * pattern);

int main() {
    DIR *dir;
    struct dirent *ent;
    int i = 0;
    if ((dir = opendir ("/Users/chenyu/index")) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            char adds[100] = "/Users/chenyu/index/";
            i += 1;
            if (i<=2) {
                continue;
            }
            strcat(adds, ent->d_name);
            int result = BMH(adds, "invest");
            if(result == 1) {
                printf("%s\n",adds);
            }
//            printf("%s\n",adds);

        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        return EXIT_FAILURE;
    }

    return 0;
}

// adds is the file address
int BMH(char *adds, char * pattern) {
    size_t a[128];
    int i = 0;
    size_t size = strlen(pattern);
    for (i = 0; i < 128; i++) {
        a[i] = size;
    }
    Bad_match_table(a, pattern);
    //BMH Search
    FILE *fp = fopen(adds, "r");
    fseek(fp, size - 1 ,SEEK_SET);
    char ch =' ';
    while(1) {
        if (feof(fp)) {
            return 0;
        }
        for (i = size - 1; i >= 0; i--) {
            ch = fgetc(fp);
            if (pattern[i] != tolower(ch)) {
                fseek(fp, a[ch] - 1, SEEK_CUR);
                if(fgetc(fp) == EOF){
                    return 0;
                }
                fseek(fp, -1, SEEK_CUR);
                break;
            }
            if(i == 0) {
                return 1;
            }
            fseek(fp, -2, SEEK_CUR);
        }

    }

}

void Bad_match_table(size_t *a, char * pattern) {
    int i = 0;
    size_t  length = strlen(pattern);
    size_t  value = 0; // to compute the bad match table for BMH
    for (i = 0; i < length; i++) {
        value = length - i - 1;
        if (value == 0) {
            a[pattern[i]] = length;
            continue;
        }
        a[pattern[i]] = value;
    }
}