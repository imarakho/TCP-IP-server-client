#include "Server.hpp"

int     main(int argc, char **argv)
{
    boost::asio::io_service io_service;

    if(argc ==3)
    {
        Server s(io_service, argv[1],std::atoi(argv[2]));
        io_service.run();
    }
    else
        std::cout << "Usage: client <host> <port>" << std::endl;
    return 0;
}