#include "wave.h"
#include "memory/library.include.h"
#include "stuff/enforce.h"
#include "defaultLogs/library.include.h"

#pragma pack(push, 4)

typedef struct
{
	char			szRIFF[4];
	unsigned long	ulRIFFSize;
	char			szWAVE[4];
} WAVEFILEHEADER;

typedef struct
{
	char			szChunkName[4];
	unsigned long	ulChunkSize;
} RIFFCHUNK;

typedef struct
{
	unsigned short	usFormatTag;
	unsigned short	usChannels;
	unsigned long	ulSamplesPerSec;
	unsigned long	ulAvgBytesPerSec;
	unsigned short	usBlockAlign;
	unsigned short	usBitsPerSample;
	unsigned short	usSize;
	unsigned short  usReserved;
	unsigned long	ulChannelMask;
	GUID            guidSubFormat;
} WAVEFMT;

#pragma pack(pop)

Wave::Wave()
{
	defaults();
}

Wave::~Wave()
{
	clear();
}

void Wave::clear()
{
	if(info.pData)
	{
		delete [] info.pData;
	}

	defaults();
}

void Wave::defaults()
{
	mem::memzero(info);
}

bool Wave::load(const char* filename)
{
	clear();

	WAVEFILEHEADER	waveFileHeader;
	RIFFCHUNK		riffChunk;
	WAVEFMT			waveFmt;
	
	// Open the wave file for reading

	FILE* file = fopen(filename, "rb");
	if (file)
	{
		// Read Wave file header
		fread(&waveFileHeader, 1, sizeof(WAVEFILEHEADER), file);
		if (!_strnicmp(waveFileHeader.szRIFF, "RIFF", 4) && !_strnicmp(waveFileHeader.szWAVE, "WAVE", 4))
		{
			while (fread(&riffChunk, 1, sizeof(RIFFCHUNK), file) == sizeof(RIFFCHUNK))
			{
				if (!_strnicmp(riffChunk.szChunkName, "fmt ", 4))
				{
					if (riffChunk.ulChunkSize <= sizeof(WAVEFMT))
					{
						fread(&waveFmt, 1, riffChunk.ulChunkSize, file);
					
						// Determine if this is a WAVEFORMATEX or WAVEFORMATEXTENSIBLE wave file
						if (waveFmt.usFormatTag == WAVE_FORMAT_PCM)
						{
							info.wfType = WF_EX;
							memcpy(&info.wfEXT.Format, &waveFmt, sizeof(PCMWAVEFORMAT));
						}
						else if (waveFmt.usFormatTag == WAVE_FORMAT_EXTENSIBLE)
						{
							info.wfType = WF_EXT;
							memcpy(&info.wfEXT, &waveFmt, sizeof(WAVEFORMATEXTENSIBLE));
						}
					}
					else
					{
						fseek(file, riffChunk.ulChunkSize, SEEK_CUR);
					}
				}
				else if (!_strnicmp(riffChunk.szChunkName, "data", 4))
				{
					info.ulDataSize = riffChunk.ulChunkSize;
					info.ulDataOffset = ftell(file);
					fseek(file, riffChunk.ulChunkSize, SEEK_CUR);
				}
				else
				{
					fseek(file, riffChunk.ulChunkSize, SEEK_CUR);
				}

				// Ensure that we are correctly aligned for next chunk
				if (riffChunk.ulChunkSize & 1)
					fseek(file, 1, SEEK_CUR);
			}

			if(info.ulDataSize && info.ulDataOffset && ((info.wfType == WF_EX) || (info.wfType == WF_EXT)))
			{
				// Allocate memory for sample data
				info.pData = new char[info.ulDataSize];

				// Seek to start of audio data
				fseek(file, info.ulDataOffset, SEEK_SET);

				// Read Sample Data
				if (fread(info.pData, 1, info.ulDataSize, file) == info.ulDataSize)
				{
					return true;	
				}
			}
			
			fclose(file);			
		}
	}
	else
	{
		LOG_ERROR(__FUNCTION__ << "() can't open file: " << filename);
	}

	return false;
}

ALuint determineFormat(const WAVEFILEINFO& info)
{
	if (info.wfType == WF_EX)
	{
		if (info.wfEXT.Format.nChannels == 1)
		{
			switch (info.wfEXT.Format.wBitsPerSample)
			{
			case 4:	return AL_FORMAT_MONO_IMA4;
			case 8:	return AL_FORMAT_MONO8;
			case 16:return AL_FORMAT_MONO16;
			}
		}
		else if (info.wfEXT.Format.nChannels == 2)
		{
			switch (info.wfEXT.Format.wBitsPerSample)
			{
			case 4:	return AL_FORMAT_STEREO_IMA4;
			case 8:	return AL_FORMAT_STEREO8;
			case 16:return AL_FORMAT_STEREO16;
			}
		}
		else if ((info.wfEXT.Format.nChannels == 4) && (info.wfEXT.Format.wBitsPerSample == 16))
			return AL_FORMAT_QUAD16;
	}
	else if (info.wfType == WF_EXT)
	{
		if ((info.wfEXT.Format.nChannels == 1) && ((info.wfEXT.dwChannelMask == SPEAKER_FRONT_CENTER) || (info.wfEXT.dwChannelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT)) || (info.wfEXT.dwChannelMask == 0)))
		{
			switch (info.wfEXT.Format.wBitsPerSample)
			{
			case 4:	return AL_FORMAT_MONO_IMA4;
			case 8: return AL_FORMAT_MONO8;
			case 16:return AL_FORMAT_MONO16;
			}
		}
		else if ((info.wfEXT.Format.nChannels == 2) && (info.wfEXT.dwChannelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT)))
		{
			switch (info.wfEXT.Format.wBitsPerSample)
			{
			case 4:	return AL_FORMAT_STEREO_IMA4;
			case 8:	return AL_FORMAT_STEREO8;
			case 16:return AL_FORMAT_STEREO16;
			}
		}
		else if ((info.wfEXT.Format.nChannels == 2) && (info.wfEXT.Format.wBitsPerSample == 16) && (info.wfEXT.dwChannelMask == (SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT)))
			return AL_FORMAT_REAR16;
		else if ((info.wfEXT.Format.nChannels == 4) && (info.wfEXT.Format.wBitsPerSample == 16) && (info.wfEXT.dwChannelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT)))
			return AL_FORMAT_QUAD16;
		else if ((info.wfEXT.Format.nChannels == 6) && (info.wfEXT.Format.wBitsPerSample == 16) && (info.wfEXT.dwChannelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_FRONT_CENTER|SPEAKER_LOW_FREQUENCY|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT)))
			return AL_FORMAT_51CHN16;
		else if ((info.wfEXT.Format.nChannels == 7) && (info.wfEXT.Format.wBitsPerSample == 16) && (info.wfEXT.dwChannelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_FRONT_CENTER|SPEAKER_LOW_FREQUENCY|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT|SPEAKER_BACK_CENTER)))
			return AL_FORMAT_61CHN16;
		else if ((info.wfEXT.Format.nChannels == 8) && (info.wfEXT.Format.wBitsPerSample == 16) && (info.wfEXT.dwChannelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_FRONT_CENTER|SPEAKER_LOW_FREQUENCY|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT|SPEAKER_SIDE_LEFT|SPEAKER_SIDE_RIGHT)))
			return AL_FORMAT_71CHN16;
	}
}

bool bufferWAVE(openal::buffer& buffer, const Wave& wave)
{
	buffer.init();

	ALuint format = determineFormat(wave.info);
	alBufferData(buffer.id, format, wave.info.pData, wave.info.ulDataSize, wave.info.wfEXT.Format.nSamplesPerSec);
	return true;
}

bool bufferWAVE(const char* filename, openal::buffer& buffer, Wave& wave)
{
	return wave.load(filename) ? bufferWAVE(buffer, wave) : false;
}

bool bufferWAVE(const char* filename, openal::buffer& buffer)
{
	Wave wave;
	return bufferWAVE(filename, buffer, wave);
}

stream::ostream& operator<<(stream::ostream& os, const WAVEFILEINFO& wave)
{
	os << wave.wfType << wave.wfEXT << wave.ulDataSize;

	if(wave.ulDataSize > 0 && wave.pData)
	{
		os.write(wave.pData, wave.ulDataSize);
	}

	return os;
}

stream::istream& operator>>(stream::istream& is, WAVEFILEINFO& wave)
{
	is >> wave.wfType >> wave.wfEXT >> wave.ulDataSize;

	if(wave.ulDataSize > 0)
	{
		wave.pData = new char[wave.ulDataSize];

		is.read(wave.pData, wave.ulDataSize);
	}

	return is;
}

stream::ostream& operator<<(stream::ostream& os, const Wave& wave)
{
	return os << wave.info;
}

stream::istream& operator>>(stream::istream& is, Wave& wave)
{
	return is >> wave.info;
}