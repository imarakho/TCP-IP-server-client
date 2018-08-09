#include "Server.hpp"

Server::Server(boost::asio::io_service &io_service, char *host , short port):
    io_service_(io_service),
    acceptor_(io_service, tcp::endpoint(boost::asio::ip::address::from_string(host), port))
    {
        Session *new_session = new Session(io_service_);
        acceptor_.async_accept(new_session->socket(),
        boost::bind(&Server::handle_accept,this,new_session,
        boost::asio::placeholders::error));
        id = 1;
    }
void    Server::handle_accept(Session *new_session, 
        const boost::system::error_code &error)
{
    if(!error)
    {
        new_session->start(id);
        id++;
        new_session = new Session(io_service_);
        acceptor_.async_accept(new_session->socket(),
        boost::bind(&Server::handle_accept,this,new_session,
        boost::asio::placeholders::error));
    }
    else 
        delete new_session;
}
