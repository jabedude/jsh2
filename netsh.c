#include <stdio.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include "netsh.h"

int main(void) {
        int sock_d;
        int client_sock;
        int cmd_sz;
        char cmd_buf[1024];
        struct sockaddr_in serv;

        if ((sock_d = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                fprintf(stderr, "jsh: error creating socket\n");
                return -1;
        }

        serv.sin_family = AF_INET;
        serv.sin_addr.s_addr = INADDR_ANY;
        serv.sin_port = htons(8080);

        /* bind() here */
        if ((bind(sock_d, (struct sockaddr *) &serv, sizeof(serv))) == -1) {
                fprintf(stderr, "jsh: error binding to socket\n");
                return -1;
        }

        listen(sock_d, 3);

        if ((client_sock = accept(sock_d, NULL, NULL)) == -1) {
                fprintf(stderr, "jsh: error accepting connection\n");
                return -1;
        }

        dup2(client_sock, 0);
        dup2(client_sock, 1);
        dup2(client_sock, 2);

        /*
        while ((cmd_sz = read(client_sock, cmd_buf, 1024)) > 0) {
                cmd_buf[cmd_sz] = '\0';
                printf("cmd is: %s", cmd_buf);
        }
        */
        execv("./bin/jsh", NULL);
        
        return 0;
}
