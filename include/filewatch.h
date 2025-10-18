#ifndef FILEWATCH_H
#define FILEWATCH_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <dirent.h>
#include <signal.h>
#include <limits.h>
#include <sys/stat.h>
#include <limits.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#define EVENT_BUF_LEN (1024 * (sizeof(struct inotify_event) + 16))

typedef struct
{
    const char *path;
    bool recursive;
    bool verbose;
} Config;

void print_usage(const char *progname);
void parse_arguments(int argc, char *argv[], Config *config);
void watch_directory(const Config *config);

void add_watch_recursive(int fd, const char *path, bool verbose);
void handle_signal(int sig);

#endif // FILEWATCH_H
