#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

typedef struct
{
    int sock;
    struct sockaddr address;
    int addr_len;
} connection_t;

void *process(void *ptr)
{
    char *buffer, buffer2[500], check[500];
    int len, retVal, lenSent, retval;
    connection_t *conn;
    long addr = 0;
    char *messageSuccess = "Data Backedup Successfully";
    char *messageFail = "failed to backup";
    FILE *backupPtr;

    if (!ptr)
        pthread_exit(0);
    conn = (connection_t *)ptr;
    addr = (long)((struct sockaddr_in *)&conn->address)->sin_addr.s_addr;
    /* read length of message */
    read(conn->sock, &len, sizeof(int));
    if (len > 0)
    {
        buffer = (char *)malloc((len + 1) * sizeof(char));
        buffer[len] = 0;
        printf("%d.%d.%d.%d: Connected\n",
               (int)((addr)&0xff),
               (int)((addr >> 8) & 0xff),
               (int)((addr >> 16) & 0xff),
               (int)((addr >> 24) & 0xff));

        read(conn->sock, buffer, len);

        printf("%d.%d.%d.%d: Intiated a backup request\n",
               (int)((addr)&0xff),
               (int)((addr >> 8) & 0xff),
               (int)((addr >> 16) & 0xff),
               (int)((addr >> 24) & 0xff));
        retval = system("./searchBackupFiles.sh");
        backupPtr = fopen("backupCheck.txt", "r");

        fgets(buffer2, 500, backupPtr);
        sscanf(buffer2, "%s", check);
        fclose(backupPtr);

        if (retVal == 0 && strcmp(check, "Found") == 0)
        {

            printf("%d.%d.%d.%d: Data backedup Successfully\n",
                   (int)((addr)&0xff),
                   (int)((addr >> 8) & 0xff),
                   (int)((addr >> 16) & 0xff),
                   (int)((addr >> 24) & 0xff));
            free(buffer);

            lenSent = strlen(check);
            write(conn->sock, &lenSent, sizeof(int));
            write(conn->sock, check, lenSent);
        }
        else
        {
            printf("%d.%d.%d.%d: failed to backup\n",
                   (int)((addr)&0xff),
                   (int)((addr >> 8) & 0xff),
                   (int)((addr >> 16) & 0xff),
                   (int)((addr >> 24) & 0xff));
            free(buffer);

            lenSent = strlen(messageFail);
            write(conn->sock, &lenSent, sizeof(int));
            write(conn->sock, messageFail, lenSent);
        }
    }

    printf("%d.%d.%d.%d: Disconnected\n",
           (int)((addr)&0xff),
           (int)((addr >> 8) & 0xff),
           (int)((addr >> 16) & 0xff),
           (int)((addr >> 24) & 0xff));
    close(conn->sock);
    free(conn);
    fflush(stdout);
    pthread_exit(0);
}

int main(int argc, char **argv)
{
    int sock = -1;
    struct sockaddr_in address;
    int port = 5678;
    connection_t *connection;
    pthread_t thread;

    /* create socket */
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock <= 0)
    {
        fprintf(stderr, "%s: error: cannot create socket\n", argv[0]);
        return -3;
    }

    /* bind socket to port */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(sock, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) < 0)
    {
        fprintf(stderr, "%s: error: cannot bind socket to port %d\n", argv[0], port);
        return -4;
    }

    /* listen on port */
    if (listen(sock, 5) < 0)
    {
        fprintf(stderr, "%s: error: cannot listen on port\n", argv[0]);
        return -5;
    }

    printf("%s: ready and listening\n", argv[0]);

    while (1)
    {
        /* accept incoming connections */
        connection = (connection_t *)malloc(sizeof(connection_t));
        connection->sock = accept(sock, &connection->address, &connection->addr_len);
        if (connection->sock <= 0)
        {
            free(connection);
        }
        else
        {
            /* start a new thread but do not wait for it */
            pthread_create(&thread, 0, process, (void *)connection);
            pthread_detach(thread);
        }
    }

    return 0;
}