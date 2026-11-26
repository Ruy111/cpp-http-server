#include "net/Server.h"

#include <iostream>

Server::Server(int port)
    : port_(port)
{
}

void Server::start()
{
    std::cout << "HTTP Server 1.0\n";

    std::cout << "配置端口: "
              << port_ << "\n";

    std::cout << "服务器模块初始化完成\n";
}
