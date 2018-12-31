//
// Programmer:    cgoxopx <cgoxopx@qq.com>
// Creation Date: 2018.12.2
// Last Modified: 2018.12.2
// Filename:      vomidi/src/vomidi-synth.cpp
// Website:       http://vo.midilib.com
// Syntax:        C++11
#include "parser.h"
#include <stdio.h>

int main(int argc , const char ** argv){
    if(argc<4){
        printf("%s [sound font path] [notation path] [output path]\n" , argv[0]);
        return 1;
    }
    vomidi::parser      ps;
    vomidi::soundfont   sf(argv[1]);
    vomidi::synth       syn(argv[3] , &sf , 16);
    if(ps.load(argv[2])){
        ps.exportSynth(&syn);
    }else{
        VOMIDIDebug("load notation fail:%s",argv[2]);
    }
    return 0;
}
