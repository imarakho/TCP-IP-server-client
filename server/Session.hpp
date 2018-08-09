#pragma once
#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <strings.h>

using boost::asio::ip::tcp;

class Session
{
    private:
            tcp::socket socket_;
            enum {max_length = 1024 };
            char data_[max_length];
            size_t id;
            time_t req_time;
            time_t res_time;
            int block_number;
    public:
        Session(boost::asio::io_service &io_service);
        tcp::socket &socket();
        void    handle_read(const boost::system::error_code &error, size_t bytes_transferred);
        void    handle_write(const boost::system::error_code &error);
        void start(size_t id = 1);
};
