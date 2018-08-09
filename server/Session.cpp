#include "Session.hpp"

Session::Session(boost::asio::io_service &io_service) : socket_(io_service)
{
}

tcp::socket &Session::socket()
{
    return socket_;
}

void Session::start(size_t id)
{
    socket_.non_blocking(true);
    this->id = id;
    block_number = 1;
    req_time = 0;
    res_time = 0;
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
    boost::bind(&Session::handle_read,
    this,
    boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred));
}

void    Session::handle_read(const boost::system::error_code &error, size_t bytes_transferred)
{
    if(!error)
    {
        block_number == 0 ? 0 : time(&res_time);
        std::cout << "\033[1;32m" << "--------------------------|" << "\033[0m" << std::endl;
        std::cout << "\033[0;36m" << "Client id:" << this->id  << std::endl;
        std::cout << "Timeout time:" << res_time - req_time << std::endl;
        std::cout << "Bytes:" << bytes_transferred << std::endl;
        data_[bytes_transferred] = '\0';
        std::cout << "Block_number:" << block_number << std::endl;
        std::cout << "Data:" << data_ << std::endl;
        std::cout << "\033[1;32m" << "--------------------------|" << std::endl;
        block_number++;
        std::string resp = "Client_id " + std::to_string(this->id) + ":Message with len of " + std::to_string(bytes_transferred) + " bytes was received.\n";
        boost::asio::async_write(socket_,boost::asio::buffer(resp.c_str(), resp.length()),
        boost::bind(&Session::handle_write,this,boost::asio::placeholders::error));
    }
    else
    {
        delete this;
    }
}

void    Session::handle_write(const boost::system::error_code &error)
{
    if(!error)
    {
        block_number == 0 ? 0 : time(&req_time);
        socket_.async_read_some(
            boost::asio::buffer(data_, max_length),
            boost::bind(&Session::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        delete this;
    }
}
