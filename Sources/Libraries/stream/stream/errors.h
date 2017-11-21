#pragma once

#include <stdexcept>
#include <exception>
#include <string>


namespace stream
{

struct stream_error : public std::runtime_error
{  
	stream_error(const char* err) : std::runtime_error(err){} 
	stream_error(const std::string& err) : std::runtime_error(err.c_str()){} 
	virtual ~stream_error() throw() {}
};

struct crc_check_failed : public stream_error
{
	crc_check_failed() : stream_error("crc check failed"){}
};

struct file_not_open : public stream_error
{
	file_not_open() : stream_error("file not open"){}
};

struct cant_open_file : public stream_error
{
	cant_open_file(const char* fileName) : stream_error( std::string("can't open file: ") + fileName ){}
};

struct cant_read_file : public stream_error
{
	cant_read_file(const char* fileName, std::size_t bytesToRead_, std::size_t bytesLeft_) : stream_error( std::string("can't open file: ") + fileName ), bytesToRead(bytesToRead_), bytesLeft(bytesLeft_){}
	std::size_t bytesToRead;
	std::size_t bytesLeft;
};

struct cant_write_file : public stream_error
{
	cant_write_file(const char* fileName, std::size_t bytesToWrite_) : stream_error( std::string("can't write to file: ") + fileName ), bytesToWrite(bytesToWrite_){}
	std::size_t bytesToWrite;
};

struct cursor_out_of_bound : public stream_error
{
	cursor_out_of_bound(std::size_t cursor_, std::size_t bound_) : stream_error("cursor is out of bound"), cursor(cursor_), bound(bound_){}
	std::size_t cursor;
	std::size_t bound;
};

struct incorrect_size_to_read : public stream_error
{
	incorrect_size_to_read(std::size_t size_) : stream_error("incorrect size to read"), size(size_){}
	std::size_t size;
};

struct incompatible_file_header : public stream_error
{
	incompatible_file_header() : stream_error("incompatible file header"){}
};


}
