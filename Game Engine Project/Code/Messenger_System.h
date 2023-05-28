#pragma once
#include "framework.h"

//Use preprocess classes for other systems for logic. This will reduce compile times.
class MemoryManager;

enum MessageTypes
{
	QUIT,
	LOAD_OBJECT,
	RENDER_OBJECT,
	PLAYER_MOVE,
	NPC_MOVE,
};

struct Message
{
	MessageTypes type;
	void* msg_data = nullptr;
};

class MessageSystem
{
public:
	MessageSystem();
	~MessageSystem();
	
	
	bool Initialize(MemoryManager* _mem_ref);

	void sendMessage(Message _message);
	int processMessages();

private:
	MessageSystem(MessageSystem& _cpy) { *this = _cpy; }
	MessageSystem& operator=(MessageSystem& _assign) { return *this; }

	//All data rec'd will only be references and doesn't require to be cleaned up. This system should not be allocating memory.
	std::queue<Message> messageQueue;
	MemoryManager* mem_manager_ref;
};