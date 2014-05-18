#pragma once
#include "Singleton.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "AudioManager.h"
#include "MeshManager.h"

/**
* This class is used to abstract the other managers into a single 
* resource manager to make it easier for the developer. The other managers
* can still be accessed to make end game more streamlined if so desired.
*/
class ResourcesManager : public Singleton<ResourcesManager>
{
public:
	friend class Singleton<ResourcesManager>;

	//Textures
	inline unsigned int GetTexture(const string &filename){ return TextureManager::Instance().GetTexture(filename); };
	inline unsigned int AddTexture(const string &filename){ return TextureManager::Instance().AddTexture(filename); };

	//Shaders
	inline Shader* GetShader(const string &filename){ return ShaderManager::Instance().GetShader(filename); };
	inline Shader* AddShader(const string &filename, const string &vert, const string &frag, const string &tcs = "", const string &tes = "", const string & geom = ""){
		return ShaderManager::Instance().AddShader(filename, vert, frag, tcs, tes, geom);	
	};

	//Audio
	sf::Music* AddSong(const string& filename){ return AudioManager::Instance().AddSong(filename); };
	sf::Music* GetSong(const string& filename){ return AudioManager::Instance().GetSong(filename); };
	sf::Sound* AddSound(const string& filename){ return AudioManager::Instance().AddSound(filename); };
	sf::Sound* GetSound(const string& filename){ return AudioManager::Instance().GetSound(filename); };
	void PSound(const string& filename){ return AudioManager::Instance().PSound(filename); };
	void TPlaySong(const string& filename){ return AudioManager::Instance().TPlaySong(filename); };
	void StopSong(const string& filename){ return AudioManager::Instance().StopSong(filename); };
	void SetLoop(const string& song, bool loop = true){ return AudioManager::Instance().SetLoop(song, loop); };
	bool GetPlaying(const string& song){ return AudioManager::Instance().GetPlaying(song); };

	//Meshes
	Mesh* GetMesh(const string& filename){ return MeshManager::Instance().GetMesh(filename); };
	Mesh* AddMesh(const string& filename){ return MeshManager::Instance().AddMesh(filename); };

	ResourcesManager(void){
		TextureManager::Create();
		ShaderManager::Create();
		AudioManager::Create();
		MeshManager::Create();
	};

	~ResourcesManager(void){
		TextureManager::Destroy();
		ShaderManager::Destroy();
		AudioManager::Destroy();
		MeshManager::Destroy();
	};
};

