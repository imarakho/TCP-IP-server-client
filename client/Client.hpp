#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

#define DATA_SIZE 1000

using boost::asio::deadline_timer;
using boost::asio::ip::tcp;

class client
{
    public:
        client(boost::asio::io_service& io_service);
        void start(tcp::resolver::iterator endpoint_iter);
        void stop();
    private:
        std::string req;
        bool stopped_;
        tcp::socket socket_;
        boost::asio::streambuf input_buffer_;
        boost::asio::deadline_timer deadline_;
        boost::asio::deadline_timer heartbeat_timer_;
        void start_connect(tcp::resolver::iterator endpoint_iter);
        void handle_connect(const boost::system::error_code& ec,
      tcp::resolver::iterator endpoint_iter);
        void start_read();
        void handle_read(const boost::system::error_code& ec);
        std::string   generate_data(int len);
        void start_write();
        void handle_write(const boost::system::error_code& ec);
        void check_deadline();
};
