#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

void do_service(int client_fd)
{
        char buf[1024] = {0};
        while(1){
            memset(buf, 0, sizeof(buf));
            int ret = read(client_fd, buf, sizeof(buf)-1);
            if(ret == 0){
                printf("client quit...\n");
                break;
            }
            buf[strlen(buf)] = '\0';
            printf("%s", buf);
            write(client_fd, buf, ret);
        }
}

int main(int argc, char* argv[])
{
    if(argc != 3){
        printf("Usage: %s, [IP], [port]\n", argv[0]);
        return 1;
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("socket");
        return 2;
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr.s_addr = inet_addr(argv[1]);

    if(bind(sock, (struct sockaddr*)&server, sizeof(server)) < 0){
        perror("bind");
        return 3;
    }

    if(listen(sock, 10) < 0){
        perror("listen");
        return 4;
    }

    pid_t pid;
    while(1){
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int client_fd;
        if((client_fd = accept(sock, (struct sockaddr*)&client, &len)) < 0){
            perror("accept");
            return 5;
        }

        if((pid = fork()) < 0){
            perror("fork");
            return 6;
        }else if(pid == 0){//child
            close(sock);
            printf("client[ip:%s,port:%d]\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
            do_service(client_fd);
            exit(EXIT_SUCCESS);
        }else{
            close(client_fd);
        }
    }
    return 0;
}
