#include "net/Server.h"

#include <iostream>
#include <string>
#include <cstdio>
#include <cerrno>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

Server::Server(int port)
    : port_(port)
{
}

int Server::start()
{
    // 1. 创建服务器 Socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1)
    {
        perror("socket");
        return 1;
    }

    // 2. 设置端口地址复用
    int opt = 1;

    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        perror("setsockopt");
        close(server_fd);
        return 1;
    }

    // 3. 设置服务器地址
    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // 4. 绑定 IP 和端口
    if (bind(server_fd,
             reinterpret_cast<sockaddr*>(&addr),
             sizeof(addr)) == -1)
    {
        perror("bind");
        close(server_fd);
        return 1;
    }

    // 5. 开始监听
    if (listen(server_fd, 128) == -1)
    {
        perror("listen");
        close(server_fd);
        return 1;
    }

    std::cout << "HTTP Server 1.0\n";
    std::cout << "监听地址: 127.0.0.1:"
              << port_ << std::endl;

    // 6. 持续接收客户端连接
    while (true)
    {
        std::cout << "等待客户端连接..."
                  << std::endl;

        int client_fd = accept(
            server_fd, nullptr, nullptr
        );

        if (client_fd == -1)
        {
            if (errno == EINTR)
                continue;

            perror("accept");
            break;
        }

        std::cout << "客户端连接成功，fd: "
                  << client_fd << std::endl;

        // 7. 接收客户端数据
        char buffer[4096];

        ssize_t n;

        do
        {
            n = recv(client_fd, buffer,
                     sizeof(buffer), 0);
        }
        while (n == -1 && errno == EINTR);

        if (n > 0)
        {
            std::cout << "收到数据: ";
            std::cout.write(buffer, n);
            std::cout << std::endl;

            // 8. 返回测试消息
            std::string response =
                "Hello from C++ Server\n";

            size_t sent = 0;

            while (sent < response.size())
            {
                ssize_t count = send(
                    client_fd,
                    response.data() + sent,
                    response.size() - sent,
                    MSG_NOSIGNAL
                );

                if (count > 0)
                {
                    sent += count;
                }
                else if (count == -1 && errno == EINTR)
                {
                    continue;
                }
                else
                {
                    std::cerr << "发送失败\n";
                    break;
                }
            }
        }
        else if (n == 0)
        {
            std::cout << "客户端结束发送\n";
        }
        else
        {
            perror("recv");
        }

        // 9. 关闭本次客户端连接
        close(client_fd);

        std::cout << "客户端连接已关闭\n";
    }

    close(server_fd);

    return 1;
}
