#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <typeinfo>
#include <system_error>

//UNIX & POSIX
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define IP inet_addr("127.0.0.1")
#define PORT 8082
int main() {
	char buff[1024];
 	std::unique_ptr <sockaddr_in> self_addr(new sockaddr_in);
    std::unique_ptr <sockaddr_in> serv_addr(new sockaddr_in);
    self_addr->sin_family = AF_INET;
    self_addr->sin_port = 0;
    self_addr->sin_addr.s_addr = 0;
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port = htons(PORT);
    serv_addr->sin_addr.s_addr = inet_addr("127.0.0.1");
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if ( s == -1 )
        throw std::system_error(errno, std::generic_category());
    std::cout<<"socket success\n";
    int rc = bind(s, (sockaddr*) self_addr.get(), sizeof(sockaddr_in));
    if ( rc == -1 )
        throw std::system_error(errno, std::generic_category());
    std::cout<<"bind success\n";
    rc = connect(s, (sockaddr*) serv_addr.get(), sizeof(sockaddr_in));
    if ( rc == -1 )
        throw std::system_error(errno, std::generic_category());
    std::cout<<"connect success\n";
    std::cout<<"Отправка сообщения:\n";
    rc = send(s, "HELLO", 5, 0);
    if ( rc == -1 )
        throw std::system_error(errno, std::generic_category());
    std::cout<<"Сообщение отправлено HELLO\n";
    rc = recv(s, buff, sizeof buff, 0);
    if ( rc == -1 ){
        throw std::system_error(errno, std::generic_category());
}	
	std::cout<<"Server:"<<buff<<std::endl;
    buff[rc] = 0;
}
