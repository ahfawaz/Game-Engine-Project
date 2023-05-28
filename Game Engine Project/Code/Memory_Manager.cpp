#include "Memory_Manager.h"



MemoryManager::MemoryManager()
{
	prgm_mem = nullptr;
	next_mem_spot = nullptr;
	memory_usage = NULL;
}

MemoryManager::~MemoryManager()
{
	this->ClearMemory();
}

MemoryManager::MemoryManager(MemoryManager& _cpy)
{
	*this = _cpy;
}

MemoryManager& MemoryManager::operator=(MemoryManager& _assign)
{
	if (&_assign != nullptr)
	{
		if (this->prgm_mem != nullptr)
		{
			this->ClearMemory();
		}
		this->prgm_mem = _assign.prgm_mem;
		this->mem_holes_list = _assign.mem_holes_list;
		this->next_mem_spot = _assign.next_mem_spot;
		this->memory_usage = _assign.memory_usage;
	}
	return *this;
}

bool MemoryManager::Initialize()
{
	//Create the mem_size;
	prgm_mem = new uint8_t[MAX_MEM_SIZE];
	next_mem_spot = prgm_mem;
	SecureZeroMemory(prgm_mem, MAX_MEM_SIZE);

	if (prgm_mem == nullptr)
		return false;
	return true;
}

void MemoryManager::ClearMemory()
{
	if (prgm_mem != nullptr)
	{
		delete prgm_mem;
		prgm_mem = nullptr;
	}

	mem_holes_list.clear();
	memory_usage = NULL;
	next_mem_spot = nullptr;
}

bool MemoryManager::IsMemory()
{
	if (this->prgm_mem != nullptr && this->memory_usage && this->next_mem_spot != nullptr)
		return true;
	return false;
}

void* MemoryManager::AddData(void* _data, mem_size_type _data_size)
{
	void* data_location = nullptr;
	mem_size_type cpy_size = _data_size + sizeof(mem_size_type);

	if (this->IsMemory() && MAX_MEM_SIZE >= (memory_usage + cpy_size))
	{
		data_location = this->next_mem_spot;
		memcpy_s(data_location, sizeof(mem_size_type), &_data_size, sizeof(mem_size_type)); //First copy data size
		this->next_mem_spot += sizeof(mem_size_type); //Move pointer over to copy data.
		memcpy_s(next_mem_spot, _data_size, _data, _data_size); //copy over the object data

		this->memory_usage += cpy_size; //increase the data usage.
		this->next_mem_spot += _data_size; //move pointer to the next available spot.
	}

	return data_location;
}

bool MemoryManager::DeleteData(void* _data_addr)
{
	bool successful = false;
	mem_size_type mem_size = NULL;
	uint8_t* mem_loc = static_cast<uint8_t*>(_data_addr);

	if (this->IsMemory())
	{
		//Get data size
		memcpy_s(&mem_size, sizeof(mem_size_type), _data_addr, sizeof(mem_size_type));

		//Making sure we're not looking an empty spot
		if (mem_size)
		{
			MemoryHole mem_hole = { mem_size, _data_addr };
			SecureZeroMemory(mem_loc, mem_size + sizeof(mem_size_type));
			this->mem_holes_list.push_back(mem_hole);
			memory_usage -= (sizeof(mem_size_type) + mem_size);
			successful = true;
		}
	}

	return successful;
}
