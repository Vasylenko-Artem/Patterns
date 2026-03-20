#include "Structural/Facade.h"

// AudioCodecLoader
void AudioCodecLoader::loadCodec(const std::string &name)
{
	std::cout << "    [CodecLoader] Loading codec: " << name << "\n";
}

// AudioMixer
void AudioMixer::setVolume(int vol)
{
	std::cout << "    [Mixer] Volume set to " << vol << "\n";
}

void AudioMixer::mix(const std::string &track)
{
	std::cout << "    [Mixer] Mixing: " << track << "\n";
}

// AudioOutputDevice
void AudioOutputDevice::open()
{
	std::cout << "    [OutputDevice] Device opened\n";
}

void AudioOutputDevice::close()
{
	std::cout << "    [OutputDevice] Device closed\n";
}

void AudioOutputDevice::play(const std::string &track)
{
	std::cout << "    [OutputDevice] Playing: " << track << "\n";
}

// Facade
void GameAudioFacade::playMusic(const std::string &track, int volume)
{
	std::cout << "  [AudioFacade] playMusic(\"" << track << "\")\n";

	codec_.loadCodec("OGG");
	device_.open();
	mixer_.setVolume(volume);
	mixer_.mix(track);
	device_.play(track);
}

void GameAudioFacade::stopMusic()
{
	std::cout << "  [AudioFacade] stopMusic()\n";
	device_.close();
}

void GameAudioFacade::playSfx(const std::string &sfx)
{
	std::cout << "  [AudioFacade] playSfx(\"" << sfx << "\")\n";

	codec_.loadCodec("WAV");
	device_.play(sfx);
}
