//
// Programmer:    cgoxopx <cgoxopx@qq.com>
// Creation Date: 2019.1.1
// Last Modified: 2019.1.1
// Filename:      vomidi/src/mixer.h
// Website:       http://vo.midilib.com
// Syntax:        C++11
#ifndef VOMIDI_mixer
#define VOMIDI_mixer
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "synth.h"
namespace vomidi{
    
    class mixer{
        public:
            double f;
            WavOutFile * wavout;
            std::vector<std::pair<WavInFile * , double> > resource;
            mixer();
            ~mixer();
            void mix(const std::vector<std::pair<soundtouch::SAMPLETYPE,double> > & res , soundtouch::SAMPLETYPE & out);
            bool mixAll();
            virtual void writeOut(const soundtouch::SAMPLETYPE * out , int len);
    };
    
    class wavMixer:public mixer{
        public:
            int channel;
            int rate;
            bool add(const char * path , double v);
            bool setOutput(const char * path);
            wavMixer();
            ~wavMixer();
    };
}
#endif

