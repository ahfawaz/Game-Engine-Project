#include "MainSystemHeaders.h"

MessageSystem::MessageSystem()
{
	this->mem_manager_ref = nullptr;
}

MessageSystem::~MessageSystem()
{

}

bool MessageSystem::Initialize(MemoryManager* _mem_ref)
{
	bool success = true;
	
	if (_mem_ref != nullptr)
	{
		this->mem_manager_ref = _mem_ref;
	}
	else success = false;

	
		
	return success;
}

void MessageSystem::sendMessage(Message _message)
{
	this->messageQueue.push(_message);
}

int MessageSystem::processMessages()
{
	int return_msg = 1;
	while (!this->messageQueue.empty())
	{
		Message proc_msg = this->messageQueue.front();
		this->messageQueue.pop();
		switch (proc_msg.type)
		{
		case QUIT:
			return_msg = 0;
			break;
		case LOAD_OBJECT:
			break;
		case RENDER_OBJECT:
			break;
		case PLAYER_MOVE:
			break;
		case NPC_MOVE:
			break;
		default:
			break;
		}
	}

	return return_msg;
}