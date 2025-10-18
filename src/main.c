#include "filewatch.h"

int main(int argc, char *argv[])
{
    Config config = {0};

    parse_arguments(argc, argv, &config);

    if (config.path == NULL)
    {
        fprintf(stderr, "Error: You must specify a directory to watch.\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    watch_directory(&config);
    return EXIT_SUCCESS;
}
