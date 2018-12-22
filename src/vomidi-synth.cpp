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
        printf("load notation fail\n");
    }
    return 0;
}
