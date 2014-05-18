#pragma once

#include <box2d\Dynamics\b2WorldCallbacks.h>
#include <box2d\Dynamics\Contacts\b2Contact.h>
#include <map>

/**
*	A wrapper for a very basic box2d contact listener. Adds
*	all contacting shapes to a list. Will have huge processing overheads
*	when lots of collisions are occuring between lots of objects as each
*	time objects are checked for collision it performs an O(n) search where
*	n == number of colliding objects.
*/
class PhysicsContactListener :
	public b2ContactListener
{
public:
	PhysicsContactListener(){ };
	~PhysicsContactListener(void){ };

	/**
	*	The overriden virtual begin contact method. Adds the objects in
	*	collision to the list
	*/
	void BeginContact(b2Contact* contact){

		contactList.push_back(std::pair<b2Body*, b2Body*>(contact->GetFixtureA()->GetBody(), contact->GetFixtureB()->GetBody()));
	}

	/**
	*	The overriden virtual end contact method. Nothing is done.
	*/
	void EndContact(b2Contact* contact){

	}

	/**
	*	Clears the list of contacting shapes
	*/
	void ClearContacted(){
		contactList.clear();
	}

	/**
	*	Returns whether two shapes are contacting
	*/
	bool isContacting(b2Body* bodyOne, b2Body* bodyTwo){

		for (std::list<std::pair<b2Body*, b2Body*>>::const_iterator i = contactList.begin();
			i != contactList.end(); ++i){
				if (i->first == bodyOne){
					if (i->second == bodyTwo){
						return true;
					}
				}

				if (i->first == bodyTwo){
					if (i->second == bodyOne){
						return true;
					}
				}
		}

		return false;
	}

protected:

	//A list to keep track of the number of contact b2bodies
	std::list<std::pair<b2Body*, b2Body*>> contactList;
};

