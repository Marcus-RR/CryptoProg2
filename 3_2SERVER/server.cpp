#include <iomanip>
#include <typeinfo>
#include <system_error>

//UNIX & POSIX
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <memory.h>
#include <iostream>
#define IP inet_addr("127.0.0.1")
#define PORT 8082
int main()
{
    //адресная структура для клиента

    struct sockaddr_in server;
    struct sockaddr_in client;

    //172.16.40.1
    //создание сокета
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        throw std::system_error(errno, std::generic_category(), "socket create error");
    memset(&server, 0, sizeof(server));
    memset(&client, 0, sizeof(client));
    /*
        //заполнение адресной структуры для клиента
        client.sin_family = AF_INET;
        client.sin_addr.s_addr = IP;
        client.sin_port = htons(PORT);
    */
    //заполнение адресной структуры для сервера
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = IP;

    //привязка адреса к сокету
    int rc = bind(sock, (const struct sockaddr *)&server,sizeof(server));
    if (rc==-1)
        throw std::system_error(errno, std::generic_category(), "bind error");
    //установка сокета в режим ожидания
    rc = listen(sock, 3);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "listen error");
    socklen_t len = sizeof (sockaddr_in);
    //обмен сообщениями
    while(1) {
        int work_sock = accept(sock, (struct sockaddr *)&client, &len);
        if (work_sock == -1)
            throw std::system_error(errno, std::generic_category(), "accept error");
        std::string ip_addr(inet_ntoa(client.sin_addr));
        std::clog << "log: Соединение установлено с " << ip_addr <<std::endl;
        char buf[256];
        std::clog << "Чтение"<<std::endl;
        rc = recv(work_sock, buf, 5, 0);
        if (rc==-1)
            throw std::system_error(errno, std::generic_category(), "recv error");
		std::clog<<"Client: "<<buf<<std::endl;
        rc = send(work_sock, buf, sizeof(buf), 0);
        if (rc==-1)
            throw std::system_error(errno, std::generic_category(), "send error");
        std::clog << "Message has sent"<<std::endl;
        std::clog << "log: Соединение закрыто c " << ip_addr <<std::endl;
        close(work_sock);
    }
    return 0;
}
