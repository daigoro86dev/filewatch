#include "filewatch.h"

static volatile sig_atomic_t keep_running = 1;

void handle_signal(int sig)
{
    (void)sig; // unused
    keep_running = 0;
    printf("\n[info] Caught SIGINT, shutting down...\n");
}

void print_usage(const char *progname)
{
    printf("Usage: %s [OPTIONS] <directory>\n", progname);
    printf("\nOptions:\n");
    printf("  -r, --recursive   Watch directories recursively\n");
    printf("  -v, --verbose     Enable verbose output\n");
    printf("  -h, --help        Show this help message\n");
}

void parse_arguments(int argc, char *argv[], Config *config)
{
    static struct option long_options[] = {
        {"recursive", no_argument, 0, 'r'},
        {"verbose", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}};

    int opt;
    while ((opt = getopt_long(argc, argv, "rvh", long_options, NULL)) != -1)
    {
        switch (opt)
        {
        case 'r':
            config->recursive = true;
            break;
        case 'v':
            config->verbose = true;
            break;
        case 'h':
            print_usage(argv[0]);
            exit(EXIT_SUCCESS);
        default:
            print_usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (optind < argc)
    {
        config->path = argv[optind];
    }
}

void add_watch_recursive(int fd, const char *path, bool verbose)
{
    int wd = inotify_add_watch(fd, path, IN_CREATE | IN_DELETE | IN_MODIFY | IN_ISDIR);
    if (wd == -1)
    {
        fprintf(stderr, "Error: Cannot watch '%s': %s\n", path, strerror(errno));
        return;
    }

    if (verbose)
        printf("[watch] Added: %s\n", path);

    DIR *dir = opendir(path);
    if (!dir)
        return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char subpath[PATH_MAX];
        snprintf(subpath, sizeof(subpath), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(subpath, &st) == 0 && S_ISDIR(st.st_mode))
        {
            add_watch_recursive(fd, subpath, verbose);
        }
    }

    closedir(dir);
}

void watch_directory(const Config *config)
{
    signal(SIGINT, handle_signal);

    int fd = inotify_init1(IN_NONBLOCK);
    if (fd < 0)
    {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    printf("Watching: %s%s\n", config->path, config->recursive ? " (recursive)" : "");

    if (config->recursive)
        add_watch_recursive(fd, config->path, config->verbose);
    else
        inotify_add_watch(fd, config->path, IN_CREATE | IN_DELETE | IN_MODIFY);

    char buffer[EVENT_BUF_LEN];

    while (keep_running)
    {
        ssize_t length = read(fd, buffer, EVENT_BUF_LEN);
        if (length < 0)
        {
            if (errno == EAGAIN)
            {
                usleep(100000); // sleep briefly if no events
                continue;
            }
            perror("read");
            break;
        }

        ssize_t i = 0;
        while (i < length)
        {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];

            if (config->verbose)
                printf("[mask=%08x] ", event->mask);

            if (event->mask & IN_CREATE)
            {
                printf("Created: ");
                if ((event->mask & IN_ISDIR) && config->recursive)
                {
                    // Add new subdirectory to watch
                    char newpath[PATH_MAX];
                    snprintf(newpath, sizeof(newpath), "%s/%s", config->path, event->name);
                    add_watch_recursive(fd, newpath, config->verbose);
                }
            }
            else if (event->mask & IN_DELETE)
            {
                printf("Deleted: ");
            }
            else if (event->mask & IN_MODIFY)
            {
                printf("Modified: ");
            }

            if (event->len)
                printf("%s\n", event->name);
            else
                printf("(unknown)\n");

            i += sizeof(struct inotify_event) + event->len;
        }
    }

    printf("[info] Cleaning up...\n");
    close(fd);
}
