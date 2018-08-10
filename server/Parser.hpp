#pragma once
#include <string>
#include <iostream>
#include <cctype>

class Parser
{
    public:
        Parser(char *dt);
        bool    output(std::string &resp);
    private:
        void    output_bits(char b);
        std::string data;
};