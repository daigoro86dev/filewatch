#ifndef FILEWATCH_H
#define FILEWATCH_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define EVENT_BUF_LEN (1024 * (sizeof(struct inotify_event) + 16))

void watch_directory(const char *path);

#endif // FILEWATCH_H
