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

    int i = 1;
    for(std::string line; getline(decompressor, line); i++)
    {
        std::cout << "\rLines in file: " << i << "          ";
    }
}