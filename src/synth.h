//
// Programmer:    cgoxopx <cgoxopx@qq.com>
// Creation Date: 2018.12.2
// Last Modified: 2018.12.2
// Filename:      vomidi/src/synth.h
// Website:       http://vo.midilib.com
// Syntax:        C++11
#ifndef VOMIDI_synth
#define VOMIDI_synth
#include "soundfont.h"
#include <map>
#include <exception>
namespace vomidi{
    class rateError : public std::exception {};
    class channelsError : public std::exception {};
    class synth{
        public:
            double kTone;
            synth(const char * path , soundfont * f , int nb);
            ~synth();
            int note2num(const std::string & note);
            bool write(int midinote , soundword * word , double vol , double time , double tempo);
            bool write(const std::string & midinote , const std::string & word , double vol , double time , double tempo);
            bool writeEmpty(int num);
        public:
            //inline functions
            inline static int getLenFromTime(int rate , double sec){
                return rate/sec;
            }
            inline void init(){
                this->pSoundTouch->setSampleRate(this->rate);
                this->pSoundTouch->setChannels(this->channel);
            }
        private:
            int numBits;
            std::map<std::string,int> tonemap;
            soundtouch::SoundTouch * pSoundTouch;
            
            soundfont * sf;
            WavOutFile *wavout;
            int rate;
            int channel;
            
            void buildToneMap();
            virtual void onWrite(const soundtouch::SAMPLETYPE *sampleBuffer , int len);
            virtual void outputInit(const char * path);
            virtual void outputClose();
    };
}
#endif
