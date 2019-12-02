#include "VoidChat.hpp"

int main()
{
	VoidChat chat;

	while (chat.isRunning()) // TODO: bool running
	{
		chat.HandleEvents();
		chat.Update();
		chat.Draw();
	}

	return 0;
}
