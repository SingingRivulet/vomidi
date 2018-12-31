//
// Programmer:    cgoxopx <cgoxopx@qq.com>
// Creation Date: 2018.12.2
// Last Modified: 2018.12.2
// Filename:      vomidi/src/parser.h
// Website:       http://vo.midilib.com
// Syntax:        C++11
#ifndef VOMIDI_parser
#define VOMIDI_parser
#include "synth.h"
#include <string>
#include <list>

namespace vomidi{
    class parser{
        public:
            struct syllable{
                std::string note,
                            word;
                double      tempo,
                            second,
                            vol;
                inline syllable(){
                    note.clear();
                    word.clear();
                    tempo=0;
                    second=0;
                    vol=1;
                }
                inline syllable(const syllable & s){
                    note=s.note;
                    word=s.word;
                    tempo=s.tempo;
                    second=s.second;
                    vol=s.vol;
                }
                inline const syllable & operator=(const syllable & s){
                    note=s.note;
                    word=s.word;
                    tempo=s.tempo;
                    second=s.second;
                    vol=s.vol;
                    return *this;
                }
            };
            std::list<syllable> track;
            double kTone;
            parser();
            ~parser();
            bool load(const char * path);
            bool save(const char * path);
            bool exportSynth(synth * s);
        private:
            void cpSetting2synth(synth * s);
            
    };
}
#endif
