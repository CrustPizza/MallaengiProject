#pragma once
#include "TemplateSingleton.h"
#include "Products.h"
#include "SoundTable.h"

#include "..\\inc\\fmod.hpp"
#pragma comment(lib, "..\\lib\\fmod_vc.lib")

#include <map>

using std::map;

using namespace FMOD;

class SoundManager : public TemplateSingleton<SoundManager>
{
private:
	System* mSystem;
	Sound** mSound;							// Resource �ڿ�
	Channel** mChannel;						// ��� ���� ä��
	Channel** mBgmChannel;

	//SoundTable* mSoundTable;
	std::list<SoundTable> mSoundList;

	unsigned int buffer;					// ����
	float mBGMVolume;						// ����
	float mSFXVolume;						// ����

	std::map<int, Sound*>sounds;			// ���� ����
	std::map<int, Sound**>::iterator iter;    


public:
	SoundManager();
	~SoundManager();

	void Update();
	void Initialize();
	void LoadJsonAddSound();
	void Play(int num );
	void Stop(int num);
	void Pause(int num);
	void Resume(int num);

	bool Playing(int num);
	int	 GetCurrentPlayBGM();
	bool Paused(int num);

	void Volume(int num, float volume);

	float GetCurrentPlayBGMVolume();
	float GetVolume(int num);

	float GetBGMVolume() { return mBGMVolume; }
	void SetBGMVolume(float volume);
	float GetSFXVolume() { return mSFXVolume; }
	void SetSFXVolume(float volume);

	std::list<SoundTable>& GetSoundList() { return mSoundList; }
};