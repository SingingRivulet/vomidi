#ifndef VOMIDI_soundfont
#define VOMIDI_soundfont

//lib:soundTouch
#include "WavFile.h"
#include "debug.h"
#include <SoundTouch.h>
#include <BPMDetect.h>
#include <STTypes.h>

#include <string>
#include <map>
namespace vomidi{
    class soundword{
        public:
            WavInFile * wavin;
            double kTone;
            soundword(const char * path);
            ~soundword();
            inline int read(soundtouch::SAMPLETYPE *sampleBuffer , int len){
                return wavin->read(sampleBuffer, len);
            }
            inline void rewind(){
                wavin->rewind();
            }
            inline unsigned int getSampleRate(){
                wavin->getSampleRate();
            }
            inline int getNumChannels(){
                wavin->getNumChannels();
            }
            inline int eof(){
                return wavin->eof();
            }
    };
    class soundfont{
        public:
            std::map<std::string,soundword*> fonts;
            int channel;
            int rate;
            soundfont(const char * path);
            ~soundfont();
            soundword * seek(const std::string & name);
    };
}
#endif
