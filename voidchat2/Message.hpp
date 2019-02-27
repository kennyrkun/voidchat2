#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>

class Message
{
public:
	Message();
	~Message();

	int authorID;
	int size;

	std::string messageContent;
};

#endif // !MESSAGE_HPP
