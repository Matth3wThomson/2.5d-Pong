#pragma once
//#include <SFML/Window/Event.hpp>

#include "EventListener.h"
#include <map>

using std::multimap;

typedef multimap<string, EventListener*>::iterator EventIterator;

/**
* An abstract class to represent an event Emitter
*/
class EventEmitter
{
public:

	/**
	*	Call to register for an event from this object
	*/
	void RegisterForEvent(const string& Event, EventListener &l){
		listeners.insert(std::make_pair(Event, &l));
	}

	/**
	*	Call to emit an event to all registered listeners
	*/
	void EmitEvent(const string& Event){
		std::pair<EventIterator, EventIterator> ii 
			= listeners.equal_range(Event); //Get iterators for type 'event'

		//Iterate over them
		for (EventIterator i = ii.first; i != ii.second; ++i){
			i->second->ReceiveEvent(Event);
		}
	}

protected:
	/**
	*	All registered listeners
	*/
	multimap<string, EventListener*> listeners;
};

