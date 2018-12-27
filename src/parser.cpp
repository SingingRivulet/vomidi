#include "parser.h"
#include <stdio.h>
#include <memory.h>
#include <sstream>
namespace vomidi{

parser::parser(){
    this->kTone=0;
}

parser::~parser(){
    
}

bool parser::save(const char * path){
    auto fp=fopen(path , "r");
    if(fp==NULL)
        return false;
    VOMIDIDebug("%s" , path);
    fprintf(fp , "@setKTone %lf\n" , this->kTone);
    
    double secondSum=0;
    
    for(auto it:this->track){
        fprintf(fp , "%s %s %lf %lf #time:%lf\n",
            it.note.c_str(),
            it.word.c_str(),
            it.second,
            it.tempo,
            secondSum
        );
        secondSum+=it.second;
    }
    
    fclose(fp);
    return true;
}

bool parser::load(const char * path){
    char bufs[2048];
    auto fp=fopen(path , "r");
    if(fp==NULL)
        return false;
    VOMIDIDebug("%s" , path);
    while(!feof(fp)){
        bzero(bufs , sizeof(bufs));
        fgets(bufs , sizeof(bufs) , fp);
        if(bufs[0]=='#' || bufs[0]=='\0'){
            continue;
        }else
        if(bufs[0]=='@'){
            std::istringstream iss(bufs+1);
            std::string event;
            int value;
            iss>>event;
            if(event=="setKTone"){
                iss>>(this->kTone);
            }
        }else{
            std::istringstream iss(bufs);
            syllable s;
            iss>>s.note;
            iss>>s.word;
            iss>>s.vol;
            iss>>s.tempo;
            iss>>s.second;
            this->track.push_back(s);
        }
    }
    
    fclose(fp);
    return true;
}

void parser::cpSetting2synth(synth * s){
    s->kTone=this->kTone;
}

bool parser::exportSynth(synth * s){
    if(s==NULL)
        return false;
    cpSetting2synth(s);
    for(auto it:this->track){
        s->write(
            it.note,
            it.word,
            it.vol,
            it.second,
            it.tempo
        );
    }
    return true;
}

}//namespace vomidi
