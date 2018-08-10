#include "Parser.hpp"

Parser::Parser(char *dt): data(dt)
{

}

void Parser::output_bits(char b)
{
    if(b <= 10)
        std::cout << "\033[0;31m";
    else
        std::cout << "\033[0;32m";
    for (int i = 0; i < 8; i++)
    {
        if (b & 0x80)
            std::cout << "1";
        else
            std::cout << "0";
        b = b << 1;
    }
    std::cout << " " << "\033[0m";
}

bool    Parser::output(std::string &resp)
{
    std::cout << "\033[0m" << "Data in bits: ";
    bool ret = true;
    for(int i = 0;i < data.length();i++)
    {
        if(data[i] == -1)
        {
         std::cout << "NULL: " <<  "\033[0;31m" << "00000000 " << "\033[0m";
         ret = false;
         continue ;
        }
        else if(isprint(data[i]))
            std::cout << data[i] << ": ";
        else
             std::cout << "no prnt: ";
        output_bits(data[i]);
    }
    std::cout << std::endl;
    return ret;
}
