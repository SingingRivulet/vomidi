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
                            second;
                inline syllable(){
                    note.clear();
                    word.clear();
                    tempo=0;
                    second=0;
                }
                inline syllable(const syllable & s){
                    note=s.note;
                    word=s.word;
                    tempo=s.tempo;
                    second=s.second;
                }
                inline const syllable & operator=(const syllable & s){
                    note=s.note;
                    word=s.word;
                    tempo=s.tempo;
                    second=s.second;
                    return *this;
                }
            };
            std::list<syllable> track;
            double kTone;
            parser();
            ~parser();
            bool load(const char * path);
            bool loadMidi(const char * path);
            bool save(const char * path);
            bool exportSynth(synth * s);
        private:
            void cpSetting2synth(synth * s);
            
    };
}
#endif
