#pragma once
#include "Session.hpp"

class Server
{
    private:
        size_t id;
        boost::asio::io_service &io_service_;
        tcp::acceptor acceptor_;
    public:
        Server(boost::asio::io_service &io_service, char *host ,short port);
        void    handle_accept(Session *new_session, 
        const boost::system::error_code &error);
};
