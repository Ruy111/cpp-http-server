#ifndef SERVER_H
#define SERVER_H

class Server
{
private:
    int port_;

public:
    explicit Server(int port);

    void start();
};

#endif
