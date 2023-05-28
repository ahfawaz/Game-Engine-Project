#pragma once
#include "framework.h"

constexpr int MGBYTE = 1000000;
constexpr int MAX_MEM_SIZE = 10 * MGBYTE;


class MemoryManager
{
private:
	using mem_size_type = int;
	struct MemoryHole
	{
		mem_size_type data_size;
		void* data;
	};

	uint8_t* prgm_mem;
	std::vector<MemoryHole> mem_holes_list;
	uint8_t* next_mem_spot;
	mem_size_type memory_usage; //does not includes memory holes;

public:
	MemoryManager();
	~MemoryManager();
	MemoryManager(MemoryManager& _cpy);
	MemoryManager& operator=(MemoryManager& _assign);

	bool Initialize();
	void ClearMemory();
	bool IsMemory(); // Checks to see if memory is being used;
	void* AddData(void* _data, mem_size_type _data_size);
	bool DeleteData(void* _data_addr);
};

