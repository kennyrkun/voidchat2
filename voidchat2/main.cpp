#include "VoidChat.hpp"

int main()
{
	VoidChat chat;
	chat.Init();

	while (chat.window.isOpen()) // TODO: bool running
	{
		chat.HandleEvents();
		chat.Update();
		chat.Draw();
	}

	return 0;
}
