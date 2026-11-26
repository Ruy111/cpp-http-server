#include "net/Server.h"

int main()
{
    Server server(8080);

    return server.start();
}
