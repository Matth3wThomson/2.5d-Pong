#pragma once

#include <string>
#include <SFML/Window/Event.hpp>

using std::string;

/**
* An abstract class to represent an event listener
*/
class EventListener
{
public:
	/**
	*	The virtual method to be overriden to act accordingly when
	*	an event is recieved
	*/
	virtual void ReceiveEvent(const string& Event) = 0;
};

