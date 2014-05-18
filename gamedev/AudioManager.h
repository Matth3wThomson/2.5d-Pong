#pragma once
#include "singleton.h"
#include <map>
#include <list>
#include <string>
#include <SFML/Audio.hpp>

#define AUDIO_PATH "Resources\\Audio\\"

using std::string;
using std::map;
using std::pair;
using std::list;

/**
* A singleton that manages the audio backend of sfml in an easy way to use.
*/
class AudioManager :
	public Singleton<AudioManager>
{
public:

	friend class Singleton<AudioManager>;

	/**
	* Loads the song into main memory
	*/
	sf::Music* AddSong(const string& filename);

	/**
	* Retreives the song from main memory
	*/
	sf::Music* GetSong(const string& filename);

	/**
	* Adds a sound into main memory (These should be short sound clips.)
	*/
	sf::Sound* AddSound(const string& filename);

	/**
	* Gets a sound from main memory.
	*/
	sf::Sound* GetSound(const string& filename);

	/**
	* Directly plays a sound by name
	*/
	void PSound(const string& filename);

	/**
	* Directly toggles the playing of a song by name
	*/
	void TPlaySong(const string& filename);

	/**
	* Directly stops the song playing by name
	*/
	void StopSong(const string& filename);

	/**
	* Sets looping of a music track by filename
	*/
	void SetLoop(const string& song, bool loop = true);

	bool GetPlaying(const string& song);

protected:
	AudioManager(void);
	~AudioManager(void);

	//A map of all strings to their sounds and songs
	map<string, sf::Sound*> soundEffects;
	map<string, sf::Music*> songs;

	//list of all buffers used in the system.
	list<sf::SoundBuffer*> buffers;
};

