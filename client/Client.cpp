#include "Client.hpp"

  client::client(boost::asio::io_service& io_service)
    : stopped_(false),
      socket_(io_service),
      deadline_(io_service),
      heartbeat_timer_(io_service)
    {
        req = generate_data(1000);
    }

  void client::start(tcp::resolver::iterator endpoint_iter)
  {
    start_connect(endpoint_iter);
    deadline_.async_wait(boost::bind(&client::check_deadline, this));
  }

  void client::stop()
  {
    stopped_ = true;
    socket_.close();
    deadline_.cancel();
    heartbeat_timer_.cancel();
  }

  void client::start_connect(tcp::resolver::iterator endpoint_iter)
  {
    if (endpoint_iter != tcp::resolver::iterator())
    {
      std::cout << "Trying " << endpoint_iter->endpoint() << "...\n";
      deadline_.expires_from_now(boost::posix_time::seconds(60));
      socket_.async_connect(endpoint_iter->endpoint(),
          boost::bind(&client::handle_connect,
            this, _1, endpoint_iter));
    }
    else
      stop();
  }

  void client::handle_connect(const boost::system::error_code& ec,
      tcp::resolver::iterator endpoint_iter)
  {
    if (stopped_)
      return;

    if (!socket_.is_open())
    {
      std::cout << "Connect timed out\n";
      start_connect(++endpoint_iter);
    }
    else if (ec)
    {
      std::cout << "Connect error: " << ec.message() << "\n";
      socket_.close();
      start_connect(++endpoint_iter);
    }
    else
    {
      std::cout << "Connected to " << endpoint_iter->endpoint() << "\n";
      start_read();
      start_write();
    }
  }

  void client::start_read()
  {
    deadline_.expires_from_now(boost::posix_time::seconds(30));
    boost::asio::async_read_until(socket_, input_buffer_, '\n',
        boost::bind(&client::handle_read, this, _1));
  }

  void client::handle_read(const boost::system::error_code& ec)
  {
    if (stopped_)
      return;
    if (!ec)
    {
      std::string line;
      std::istream is(&input_buffer_);
      std::getline(is, line);
      if (!line.empty())
      {
        std::cout << "\033[1;32m" << "Received: " << line << "\033[0m" << "\n";
      }
      start_read();
    }
    else
    {
      std::cout << "Error on receive: " << ec.message() << "\n";
      stop();
    }
  }
std::string   client::generate_data(int len)
{
        srand(time(0));
        char *data = (char *)malloc(len * sizeof(char) + 1);
         static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) 
        data[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    data[len] = '\0';
    std::string ret = data;
    return ret;

}

  void client::start_write()
  {
    if (stopped_ || req.empty())
      return;
    boost::asio::async_write(socket_, boost::asio::buffer(req.c_str(), 10),
        boost::bind(&client::handle_write, this, _1));
    req.erase(0, 10);
    
  }

  void client::handle_write(const boost::system::error_code& ec)
  {
      std::string req = generate_data(DATA_SIZE);
    if (stopped_)
      return;

    if (!ec)
    {
      heartbeat_timer_.expires_from_now(boost::posix_time::seconds(1));
      heartbeat_timer_.async_wait(boost::bind(&client::start_write, this));
    }
    else
    {
      std::cout << "Error on heartbeat: " << ec.message() << "\n";

      stop();
    }
  }

  void client::check_deadline()
  {
    if (stopped_)
      return;
    if (deadline_.expires_at() <=  boost::asio::deadline_timer::traits_type::now())
    {
      socket_.close();
      deadline_.expires_at(boost::posix_time::pos_infin);
    }
    deadline_.async_wait(boost::bind(&client::check_deadline, this));
  }
