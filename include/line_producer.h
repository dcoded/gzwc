#ifndef INCLUDE_LINE_PRODUCER_H
#define INCLUDE_LINE_PRODUCER_H

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include <dcoady/thread.h>
#include <dcoady/circular_fifo.h>

using boost::iostreams::input;
using boost::iostreams::filtering_stream;

class line_producer : public dcoady::thread {
public:
    line_producer(std::istream& istream, dcoady::circular_fifo<std::string, 128>& buffer);
    ~line_producer();

    bool eof() const;

    virtual void* run();
    void stop();
private:
    filtering_stream<input> decompressor;
    bool is_running = true;

    dcoady::circular_fifo<std::string, 128>* buffer;
};


line_producer::line_producer(std::istream& istream,
    dcoady::circular_fifo<std::string, 128>& buffer)
: buffer(&buffer)
{
    decompressor.push(boost::iostreams::gzip_decompressor());
    decompressor.push(istream);
}

line_producer::~line_producer()
{
    is_running = false;
}

bool line_producer::eof() const
{
    return is_running;
}

void line_producer::stop()
{
    is_running = false;
}

void* line_producer::run()
{
    while(true)
    {
        std::string line;
        if(!buffer->full() && getline(decompressor, line))
        {
            buffer->enqueue(line);
        }
    }

    return nullptr;
}

#endif