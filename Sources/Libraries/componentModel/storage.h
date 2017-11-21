#pragma once

#include "config.h"
#include "memoryManager.h"

//
//
//
struct ComponentsMemoryBlock
{
	~ComponentsMemoryBlock() 
	{ 
		mem::deallocate(memBlock);
	}

	template<class Component>
	Component* allocateMemory()
	{
		std::size_t shift = memBlock.len;
		mem::allocate(memBlock, memBlock.len + sizeof(Component));
		Component* m = (Component*) ((char*) memBlock.data + shift);
		return new (m) Component();
	}

	void allocateMemory(std::size_t size)
	{
		mem::allocate(memBlock, size);
	}

	mem::mem_desc memBlock;
};