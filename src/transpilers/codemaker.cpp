#include "codemaker.h"

#include <stdexcept>

void CodeMaker::begin_block()
{
    bId++;
}

void CodeMaker::end_block()
{
    bId--;
}

void CodeMaker::add_line(std::string line)
{
    std::string indent = "";
    for(int i = 0; i < bId; i++)
    {
        indent += tab;
    }
    lines.push_back(indent + line);
}

std::string CodeMaker::render()
{
    if(bId != 0)
    {
        throw std::runtime_error("mismatching block error!");
    }

    std::string result = "";
    for(auto line : lines)
    {
        result += line + "\n";
    }
    return result;
}