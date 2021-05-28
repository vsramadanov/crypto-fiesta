#include<iostream>
#include<fstream>
#include<algorithm>
#include<iterator>

#include<errno.h>
#include<cstring>

#include "utils/prefix_tree.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
        return -1;

    std::ifstream src(argv[1]);
    if (!src)
    {
        std::cerr << "Unable to open file " << argv[1] << " : " << std::strerror(errno) << std::endl;
        return -2;
    }

    std::string input;
    utils::prefix_tree ptree;
    while(true)
    {
        src >> input;
        if(src.eof())
            break;
        ptree.insert(input);
    }
    std::cout << "Got " << ptree.size() << " words\n";
    std::cin >> input;

    auto res = ptree.match(input);
    std::cout << "Mached " << res.size() << " words\n";
    std::copy(res.begin(), res.end(), std::ostream_iterator<std::string>(std::cout, "\n"));

    return 0;
}