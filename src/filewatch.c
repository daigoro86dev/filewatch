#include "filewatch.h"

void watch_directory(const char *path)
{
    int fd = inotify_init();
    if (fd < 0)
    {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    int wd = inotify_add_watch(fd, path, IN_CREATE | IN_DELETE | IN_MODIFY);
    if (wd == -1)
    {
        fprintf(stderr, "Error: Cannot watch '%s': %s\n", path, strerror(errno));
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Watching directory: %s\n", path);

    char buffer[EVENT_BUF_LEN];

    while (1)
    {
        ssize_t length = read(fd, buffer, EVENT_BUF_LEN);
        if (length < 0)
        {
            perror("read");
            break;
        }

        ssize_t i = 0;
        while (i < length)
        {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];

            printf("[event] ");

            if (event->mask & IN_CREATE)
                printf("Created: ");
            else if (event->mask & IN_DELETE)
                printf("Deleted: ");
            else if (event->mask & IN_MODIFY)
                printf("Modified: ");

            if (event->len)
                printf("%s\n", event->name);
            else
                printf("(unknown)\n");

            i += sizeof(struct inotify_event) + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}
