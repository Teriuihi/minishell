#include <dirent.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    DIR *f = opendir(argv[1]);
    printf("%s is f\n", f);
    while (1){}  
}
