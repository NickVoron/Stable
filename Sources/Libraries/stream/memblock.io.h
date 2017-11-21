#include "memory/library.include.h"

namespace stream
{

inline ostream& operator <<(ostream& stream, const mem::mem_desc& block)
{
	stream << block.len;
	stream.write(block.data, block.len);
	return stream;
}

inline istream& operator >>(istream& stream, mem::mem_desc& block)
{
	mem::deallocate(block);

	stream >> block.len;
	mem::allocate(block, block.len);
	stream.read(block.data, block.len);
	return stream;
}

}