#include <string>
#include <vector>

class CodeMaker{
    public:

    void add_line(std::string line);
    void begin_block();
    void end_block();

    std::string render();
    
    int bId = 0;
    std::string tab = "    ";
    std::vector<std::string> lines;
};