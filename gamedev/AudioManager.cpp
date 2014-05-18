#include "AudioManager.h"

AudioManager::AudioManager(void){
}

AudioManager::~AudioManager(void)
{
	//Delete all sounds
	for (map<string, sf::Sound*>::iterator i = soundEffects.begin(); i != soundEffects.end(); ++i){
		i->second->stop();
		delete i->second;
	}

	//Delete all songs
	for (map<string, sf::Music*>::iterator i = songs.begin(); i != songs.end(); ++i){
		i->second->stop();
		delete i->second;
	}

	//Delete all buffers
	for (list<sf::SoundBuffer*>::iterator i = buffers.begin(); i != buffers.end(); ++i){
		delete *i;
	}
}

sf::Music* AudioManager::AddSong(const string& filename){
	//Check not already loaded
	sf::Music* music = AudioManager::GetSong(filename);

	if (music != NULL){
		return music;
	}

	music = new sf::Music();

	if (music->openFromFile((AUDIO_PATH + filename).c_str())) songs.insert(std::pair<string, sf::Music*>(filename, music));

	return music;
}

sf::Music* AudioManager::GetSong(const string& filename){
	map<string, sf::Music*>::iterator i = songs.find(filename);

	//If found, return it!
	if (i != songs.end()){
		return i->second;
	}

	//Else return null flag
	return NULL;
}

sf::Sound* AudioManager::AddSound(const string& filename){
	sf::Sound* s = GetSound(filename);

	if (s != NULL){
		return s;
	}

	s = new sf::Sound();
	sf::SoundBuffer* buffer = new sf::SoundBuffer();

	if (!buffer->loadFromFile((AUDIO_PATH + filename).c_str())){
		delete s;
		delete buffer;
		return NULL;
	}

	buffers.push_back(buffer);
	s->setBuffer(*buffer);

	soundEffects.insert(std::pair<string, sf::Sound*>(filename, s));

	return s;
}



sf::Sound* AudioManager::GetSound(const string& filename){
	map<string, sf::Sound*>::iterator i = soundEffects.find(filename);

	//If found, return it!
	if (i != soundEffects.end()){
		return i->second;
	}

	//Else return null flag
	return NULL;
}

void AudioManager::PSound(const string& filename){
	sf::Sound* sound = GetSound(filename);

	if (sound != NULL){
		sound->play();
	}
}

void AudioManager::TPlaySong(const string& filename){
	sf::Music* music = GetSong(filename);

	if (music != NULL){
		if (music->getStatus() == sf::Music::Playing){ music->pause(); }
		else { music->play(); }
	}
}

void AudioManager::SetLoop(const string& song, bool loop){
	sf::Music* music = GetSong(song);

	if (music != NULL){
		music->setLoop(loop);
	}
}

void AudioManager::StopSong(const string& filename){
	sf::Music* music = GetSong(filename);

	if (music != NULL){
		if (music->getStatus() != sf::Music::Stopped) music->stop();
	}
}

bool AudioManager::GetPlaying(const string& song){
	sf::Music* music = GetSong(song);

	if (music != NULL){
		if (music->getStatus() != sf::Music::Playing) return false;
	}

	return true;
}



