#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

#if !defined(COMPILER)
    #define COMPILER "gcc"
#endif

int main(int argc, char** argv) {
    printf("\nTesting examples:\n");

    printf("Compiler: "COMPILER"\n\n");

    chdir("tests");

    DIR* dir = opendir(".");

    struct dirent* de;

    while((de = readdir(dir))) {
        if(de->d_type != DT_DIR || de->d_namlen < 3 || de->d_name[0] == '.') continue;

        printf("it ");
        char* name = de->d_name;
        char ch;
        while((ch = *name++)) {
            if(ch >= 'A' && ch <= 'Z') {
                putchar(' ');
                putchar(ch + ('a' - 'A'));
            } else {
                putchar(ch);
            }
        }
        printf(" ... ");

        chdir(de->d_name);

        system(COMPILER" -o ./run.gen -Wall -Wextra -std=c99 -pedantic main.c");

        if(system("./run.gen > actual.gen.txt") < 0) {
            printf("Failed to execute test\n");

            closedir(dir);

            return 1;
        }

        system("diff actual.gen.txt expected.txt > diff.gen.txt");

        FILE* diff = fopen("diff.gen.txt", "r");

        fseek(diff, 0, SEEK_END);

        fpos_t size;

        fgetpos(diff, &size);

        fclose(diff);

        if(size > 0) {
            printf(
                "Actual result doesn't match expected one.\n"
                "See diff in 'tests/examples/%s/diff.gen.txt' and actual result in 'tests/examples/%s/actual.gen.txt'\n", 
                de->d_name, de->d_name
            );

            closedir(dir);

            return 1;
        }
        
        system("rm -f run.gen actual.gen.txt diff.gen.txt");

        chdir("..");

        printf("DONE\n");
    }

    closedir(dir);

    printf("\nFINISHED\n");

    return 0;
}