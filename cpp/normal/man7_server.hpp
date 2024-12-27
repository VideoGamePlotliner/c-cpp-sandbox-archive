// Adapted from the "File server.c" section of https://www.man7.org/linux/man-pages/man7/unix.7.html

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include "man7_connection.hpp"

int man7_server_main(void)
{
    int down_flag = 0;
    int ret;
    int connection_socket;
    int data_socket;
    int result;
    ssize_t r, w;
    struct sockaddr_un name;
    char buffer[BUFFER_SIZE];

    /* Create local socket. */

    connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (connection_socket == -1)
    {
        perror("socket");
        unlink(SOCKET_NAME);
        return 1;
    }

    /*
     * For portability clear the whole structure, since some
     * implementations have additional (nonstandard) fields in
     * the structure.
     */

    memset(&name, 0, sizeof(name));

    /* Bind socket to socket name. */

    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    ret = bind(connection_socket, (const struct sockaddr *)&name,
               sizeof(name));
    if (ret == -1)
    {
        perror("bind");
        close(connection_socket);
        unlink(SOCKET_NAME);
        return 2;
    }

    /*
     * Prepare for accepting connections. The backlog size is set
     * to 20. So while one request is being processed other requests
     * can be waiting.
     */

    ret = listen(connection_socket, 20);
    if (ret == -1)
    {
        perror("listen");
        close(connection_socket);
        unlink(SOCKET_NAME);
        return 3;
    }

    /* This is the main loop for handling connections. */

    for (;;)
    {

        /* Wait for incoming connection. */

        data_socket = accept(connection_socket, NULL, NULL);
        if (data_socket == -1)
        {
            perror("accept");
            close(connection_socket);
            unlink(SOCKET_NAME);
            return 4;
        }

        result = 0;
        for (;;)
        {

            /* Wait for next data packet. */

            r = read(data_socket, buffer, sizeof(buffer));
            if (r == -1)
            {
                perror("read");
                close(data_socket);
                close(connection_socket);
                unlink(SOCKET_NAME);
                return 5;
            }

            /* Ensure buffer is 0-terminated. */

            buffer[sizeof(buffer) - 1] = 0;

            /* Handle commands. */

            if (!strncmp(buffer, "DOWN", sizeof(buffer)))
            {
                down_flag = 1;
                continue;
            }

            if (!strncmp(buffer, "END", sizeof(buffer)))
            {
                break;
            }

            if (down_flag)
            {
                continue;
            }

            /* Add received summand. */

            result += atoi(buffer);
        }

        /* Send result. */

        sprintf(buffer, "%d", result);
        w = write(data_socket, buffer, sizeof(buffer));
        if (w == -1)
        {
            perror("write");
            close(data_socket);
            close(connection_socket);
            unlink(SOCKET_NAME);
            return 6;
        }

        /* Close socket. */

        close(data_socket);

        /* Quit on DOWN command. */

        if (down_flag)
        {
            break;
        }
    }

    close(connection_socket);

    /* Unlink the socket. */

    unlink(SOCKET_NAME);

    return 0;
}
