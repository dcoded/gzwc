#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>

using boost::iostreams::input;
using boost::iostreams::filtering_stream;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: ./" << argv[0] << "<filename>\n";
        exit(0);
    }

    const char* filename = argv[1];

    std::ifstream file;
    file.exceptions(std::ios::failbit | std::ios::badbit);
    file.open(filename, std::ios_base::in | std::ios_base::binary);

    filtering_stream<input> decompressor;
    decompressor.push(boost::iostreams::gzip_decompressor());
    decompressor.push(file);

    std::istream_iterator<char> it(decompressor >> std::noskipws), end;

    int i = 1;
    for(; it != end; it++)
    {
        if (*it == '\n') i++;
        std::cout << "\rLines in file: " << i << "          ";
    }
    std::cout << "\n";

}