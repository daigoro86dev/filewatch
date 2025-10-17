#include "filewatch.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <directory_to_watch>\n", argv[0]);
        return EXIT_FAILURE;
    }

    watch_directory(argv[1]);
    return EXIT_SUCCESS;
}
