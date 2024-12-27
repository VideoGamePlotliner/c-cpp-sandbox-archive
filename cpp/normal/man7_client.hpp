// Adapted from the "File client.c" section of https://www.man7.org/linux/man-pages/man7/unix.7.html

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include "man7_connection.hpp"

// https://www.man7.org/linux/man-pages/man2/wait.2.html
#include <sys/wait.h>

int man7_client_main(int argc, const char *argv[], int num_tries_left = 5)
{
    int ret;
    int data_socket;
    ssize_t r, w;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    /* Create local socket. */

    data_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (data_socket == -1)
    {
        perror("socket");
        return 1;
    }

    /*
     * For portability clear the whole structure, since some
     * implementations have additional (nonstandard) fields in
     * the structure.
     */

    memset(&addr, 0, sizeof(addr));

    /* Connect socket to socket address. */

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    errno = 0;
    ret = connect(data_socket, (const struct sockaddr *)&addr,
                  sizeof(addr));
    if (ret == -1)
    {
        int errnum = errno;

        // https://www.man7.org/linux/man-pages/man3/strerror.3.html
#ifdef _GNU_SOURCE
        fprintf(stderr, "The server is down. Error number is %d. Error name is \"%s\". Error desc is \"%s\"\n", errnum, strerrorname_np(errnum), strerrordesc_np(errnum));
#else
        fprintf(stderr, "The server is down. Error number is %d. Error string is \"%s\".\n", errnum, strerror(errnum));
#endif // _GNU_SOURCE

        close(data_socket);

        num_tries_left--;

        if (num_tries_left > 0)
        {
            fprintf(stderr, "Whoops. Failed to connect. Waiting...\n");
            sleep(1);
            fprintf(stderr, "Let's try connecting again (%d %s left).\n", num_tries_left, num_tries_left == 1 ? "try" : "tries");
            return man7_client_main(argc, argv, num_tries_left);
        }
        else
        {
            fprintf(stderr, "Whoops. Can't try connecting again.\n");
            return 2;
        }
    }

    /* Send arguments. */

    for (int i = 1; i < argc; ++i)
    {
        w = write(data_socket, argv[i], strlen(argv[i]) + 1);
        if (w == -1)
        {
            perror("write");
            close(data_socket);
            break;
        }
    }

    /* Request result. */

    strcpy(buffer, "END");
    w = write(data_socket, buffer, strlen(buffer) + 1);
    if (w == -1)
    {
        perror("write");
        close(data_socket);
        return 3;
    }

    /* Receive result. */

    r = read(data_socket, buffer, sizeof(buffer));
    if (r == -1)
    {
        perror("read");
        close(data_socket);
        return 4;
    }

    /* Ensure buffer is 0-terminated. */

    buffer[sizeof(buffer) - 1] = 0;

    printf("Result = %s\n", buffer);

    /* Close socket. */

    close(data_socket);

    return 0;
}

// https://www.man7.org/linux/man-pages/man2/fork.2.html
// https://www.man7.org/linux/man-pages/man2/wait.2.html
int man7_client_main_fork(int argc, const char *argv[])
{
    errno = 0;
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 20;
    }
    else if (pid == 0)
    {
        // Child
        exit(man7_client_main(argc, argv));
    }
    else
    {
        // Parent
        int status = 0;
        errno = 0;
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid");
            return 21;
        }
        else if (WIFEXITED(status))
        {
            return WEXITSTATUS(status);
        }
        else
        {
            return 22;
        }
    }
}
