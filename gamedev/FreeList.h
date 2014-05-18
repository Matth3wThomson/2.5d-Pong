#pragma once

#include <queue>
using std::queue;

/**
* A generic class that handles a "free list".
* Useful for any object where multiple instances
* are required which are regularly offscreen.
*/
template<class T>
class FreeList
{
public:
	FreeList(unsigned int startCount = 0){
		for (unsigned int i = 0; i < startCout; ++i){
			contents.push(new T());
		}
	};
	~FreeList(void){ ClearList(); };

	T* GetNewObject(){
		if (contents.empty()){
			return new T();
		}

		T* object = contents.front();
		contents.pop();

		return object;
	}

	void DiscardObject(T* object){
		contents.push(object);
	}

	void ClearList(){
		while (!contents.empty()){
			T* object = contents.front();
			contents.pop();
			delete object;
		}
	}

protected:
	queue<T*> contents;

}