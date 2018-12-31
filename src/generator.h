//
// Programmer:    cgoxopx <cgoxopx@qq.com>
// Creation Date: 2018.12.2
// Last Modified: 2018.12.2
// Filename:      vomidi/src/generator.h
// Website:       http://vo.midilib.com
// Syntax:        C++11
#ifndef VOMIDI_generator
#define VOMIDI_generator
#include "MidiFile.h"
#include "Options.h"
#include "debug.h"
#include <stdio.h>
#include <list>
#include <string>
namespace vomidi{
    class generator{
        public:
            std::string word;
            generator(const char * path);
            ~generator();
            void pushVoice(int note,int vol,double second);
            void pushEmpty(double second);
            bool pushAtTime(int note,int vol,double begin,double second);
            void noteid2str(int note , char * str ,int len);
        private:
            double time;
            FILE * fp;
    };
    class generators{
        public:
            generators(const char * output , FILE * config);
            ~generators();
            void pushNotation(int note,int vol,double begin,double second);
        private:
            std::list<generator*> gens;
            std::string pathPre;
            int chnum;
            FILE * cfp;
    };
    bool midiLoader(const char * infile , const char * outfile);
}
#endif
