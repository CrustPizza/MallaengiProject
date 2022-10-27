#include "LibraryPCH.h"
#include "SoundManager.h"

SoundManager::SoundManager() :
	mSystem(nullptr), mChannel(nullptr),
	mBgmChannel(nullptr),
	mSound(nullptr), buffer(50), mBGMVolume(0.5f), mSFXVolume(0.5f)
{

}

SoundManager::~SoundManager()
{
	for (unsigned int i = 0; i < buffer; i++)
	{
		if (mChannel != nullptr && mChannel[i])
		{
			mChannel[i]->stop();
		}
		if (mBgmChannel != nullptr && mBgmChannel[i])
		{
			mBgmChannel[i]->stop();
		}
		if (mSound != nullptr && mSound[i])
		{
			mSound[i]->release();
		}
	}

	delete(mBgmChannel);
	delete(mChannel);
	delete(mSound);

	if (system != nullptr)
	{
		mSystem->release();
		mSystem->close();
	}
	sounds.clear();
}

void SoundManager::Update()
{
	mSystem->update();
}

void SoundManager::Initialize()
{
	System_Create(&mSystem);							// �ý���(����̽�)����

	mSystem->init(buffer, FMOD_INIT_NORMAL, 0);		// ���� ���ҽ� �� ä�� ����

	mSound = new Sound * [buffer];					// 15�� ������ ����Ű�� ������ �迭 �����Ҵ�
	mChannel = new Channel * [buffer];				// 15�� ä���� ����Ű�� ������ �迭 �����Ҵ�
	mBgmChannel = new Channel * [buffer];

	memset(mSound, 0, sizeof(Sound*) * (buffer));
	memset(mChannel, 0, sizeof(Channel*) * (buffer));
	memset(mBgmChannel, 0, sizeof(Channel*) * (buffer));

	//memset �Լ��� �޸��� ����(��)�� ���ϴ� ũ�⸸ŭ Ư�� ������ ������ �� �ִ� �Լ�
	//�Լ� ���� void* memset(void* ptr, int value, size_t num);
	//mSoundTable = new SoundTable();
}

void SoundManager::LoadJsonAddSound()
{
	SoundTable mSoundTable;
	mSoundTable.DeserializeFromFile("Resources\\Json\\SoundTable.json");

	int count = 0;

	for (list<SoundTable>::const_iterator itr = mSoundList.begin();
		itr != mSoundList.end(); itr++, count++)
	{
		if (itr->Loop())
		{
			mSystem->createSound(itr->FileRoot().c_str(),
				FMOD_LOOP_NORMAL, nullptr,
				&mSound[count]);
		}
		else if (!itr->Loop())
		{
			mSystem->createSound(itr->FileRoot().c_str(),
				FMOD_LOOP_OFF, nullptr,
				&mSound[count]);
		}
	}
}

void SoundManager::Play(int num)
{
	int count = 0;

	for (list<SoundTable>::const_iterator itr = mSoundList.begin();
		itr != mSoundList.end(); itr++, count++)
	{
		if (itr->Id() == num)
		{
			if (itr->IsBgm())
			{
				mSystem->playSound
				(
					mSound[count],
					nullptr, false,
					&mBgmChannel[count]
				);

				mBgmChannel[count]->setVolume(mBGMVolume);

				break;
			}
			else if (!itr->IsBgm())
			{
				mSystem->playSound
				(
					mSound[count],
					nullptr, false,
					&mChannel[count]
				);
				mChannel[count]->setVolume(mSFXVolume);

				break;
			}
		}
	}
}

void SoundManager::Stop(int num)
{
	int count = 0;

	for (list<SoundTable>::const_iterator itr = mSoundList.begin();
		itr != mSoundList.end(); itr++, count++)
	{
		if (itr->Id() == num)
		{
			if (itr->IsBgm())
				mBgmChannel[count]->stop();
			else
				mChannel[count]->stop();

			break;
		}
	}
}

void SoundManager::Pause(int num)
{
	int count = 0;

	for (list<SoundTable>::const_iterator itr = mSoundList.begin();
		itr != mSoundList.end(); itr++, count++)
	{
		if (itr->Id() == num)
		{
			if (itr->IsBgm())
				mBgmChannel[count]->setPaused(true);
			else
				mChannel[count]->setPaused(true);

			break;
		}
	}
}

void SoundManager::Resume(int num)
{
	int count = 0;

	for (list<SoundTable>::const_iterator itr = mSoundList.begin();
		itr != mSoundList.end(); itr++, count++)
	{
		if (itr->Id() == num)
		{
			if (itr->IsBgm())
				mBgmChannel[count]->setPaused(false);
			else
				mChannel[count]->setPaused(false);

			break;
		}
	}
}

bool SoundManager::Playing(int num)
{
	int count = 0;
	bool bPlay = false;

	for (list<SoundTable>::const_iterator itr = mSoundList.begin();
		itr != mSoundList.end(); itr++, count++)
	{
		if (itr->Id() == num)
		{
			if (itr->IsBgm())
				mBgmChannel[count]->isPlaying(&bPlay);
			else
				mChannel[count]->isPlaying(&bPlay);

			break;
		}
	}
	return bPlay;
}

//���� bgm �����ö�
int SoundManager::GetCurrentPlayBGM()
{
	int count = 0;
	bool bPlay = false;

	for (list<SoundTable>::const_iterator itr = mSoundList.begin();
		itr != mSoundList.end() && count < buffer; itr++, count++)
	{
		if (mBgmChannel[count] == nullptr)
			continue;

		mBgmChannel[count]->isPlaying(&bPlay);

		if (bPlay == true)
		{
			return itr->Id();
		}
	}

	return -1;
}

bool SoundManager::Paused(int num)
{
	bool bPause = false;
	int count = 0;

	for (list<SoundTable>::const_iterator itr = mSoundList.begin();
		itr != mSoundList.end(); itr++, count++)
	{
		if (itr->Id() == num)
		{
			if (itr->IsBgm())
				mBgmChannel[count]->getPaused(&bPause);
			else
				mChannel[count]->getPaused(&bPause);

			break;
		}
	}
	return bPause;
}

void SoundManager::Volume(int num, float volume)
{
	int count = 0;

	for (list<SoundTable>::const_iterator itr = mSoundList.begin();
		itr != mSoundList.end(); itr++, count++)
	{
		if (itr->Id() == num)
		{
			if (itr->IsBgm())
				mBgmChannel[count]->setVolume(volume);
			else
				mChannel[count]->setVolume(volume);

			break;
		}
	}
}

float SoundManager::GetCurrentPlayBGMVolume()
{
	int count = 0;
	bool bPlay = false;

	for (list<SoundTable>::const_iterator itr = mSoundList.begin();
		itr != mSoundList.end() && count < buffer;  itr++, count++)
	{
		mBgmChannel[count]->isPlaying(&bPlay);

		if (bPlay == true)
		{
			float ret;

			mBgmChannel[count]->getVolume(&ret);

			return ret;
		}
	}

	return -1;
}

float SoundManager::GetVolume(int num)
{
	int count = 0;
	bool bPlay = false;

	for (list<SoundTable>::const_iterator itr = mSoundList.begin();
		itr != mSoundList.end(); itr++, count++)
	{
		if (itr->Id() == num)
		{
			float ret;

			if (itr->IsBgm())
			{
				mBgmChannel[count]->getVolume(&ret);
			}
			else
			{
				mChannel[count]->getVolume(&ret);
			}

			return ret;
		}
	}

	return -1;
}

void SoundManager::SetBGMVolume(float volume)
{
	int count = 0;
	mBGMVolume = volume;

	for (list<SoundTable>::const_iterator itr = mSoundList.begin();
		itr != mSoundList.end() && count < buffer; itr++, count++)
	{
		if (itr->IsBgm())
		{
			mBgmChannel[count]->setVolume(volume);
		}
	}
}

void SoundManager::SetSFXVolume(float volume)
{
	int count = 0;
	mSFXVolume = volume;

	for (list<SoundTable>::const_iterator itr = mSoundList.begin();
		itr != mSoundList.end() && count < buffer; itr++, count++)
	{
		if (itr->IsBgm() != true)
		{
			mChannel[count]->setVolume(mSFXVolume);
		}
	}
}