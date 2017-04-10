#include <stdio.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include "netsh.h"

int start_serve(void) {
        int sock_d;
        int client_sock;
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
        
        return 0;
}
