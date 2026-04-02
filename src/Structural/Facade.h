#pragma once

#include <iostream>
#include <string>

// Complex subsystems
class AudioCodecLoader
{
public:
	void loadCodec(const std::string &name);
};

class AudioMixer
{
public:
	void setVolume(int vol);
	void mix(const std::string &track);
};

class AudioOutputDevice
{
public:
	void open();
	void close();
	void play(const std::string &track);
};

// Facade
class GameAudioFacade
{
	AudioCodecLoader codec_;
	AudioMixer mixer_;
	AudioOutputDevice device_;

public:
	void playMusic(const std::string &track, int volume = 80);
	void stopMusic();
	void playSfx(const std::string &sfx);
};
