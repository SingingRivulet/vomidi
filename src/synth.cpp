#include "synth.h"
#include <memory.h>
#define BUFF_SIZE           6720
namespace vomidi{

synth::synth(const char * path , soundfont * f , int nb){
    this->buildToneMap();
    this->kTone=0;
    this->pSoundTouch = new soundtouch::SoundTouch;
    
    this->numBits=nb;
    this->sf=f;
    this->rate=f->rate;
    this->channel=f->channel;
    
    this->outputInit(path);
}

synth::~synth(){
    delete this->pSoundTouch;
    this->outputClose();
}

void synth::buildToneMap(){
    static const char * keys[]={"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
    char buf[16];
    for(int i=0;i<=10;i++){
         for(int j=0;j<12;j++){
            bzero(buf,16);
            snprintf(buf,16,"%s%d",keys[j],i);
            this->tonemap[buf]=i*12+j-82;
         }
    }
}

int synth::note2num(const std::string & note){
    auto it=this->tonemap.find(note);
    if(it==this->tonemap.end())
        return 0;
    else
        return it->second;
}

bool synth::write(const std::string & midinote , const std::string & word , double time , double tempo){
    if(midinote=="0" || midinote.empty() || word.empty())
        return this->write(0 , NULL , time , tempo);
    
    int nid=this->note2num(midinote);
    auto wd=this->sf->seek(word);
    
    if(wd==NULL)
        return this->write(0 , NULL , time , tempo);
    
    return this->write(nid , wd , time , tempo);
}

bool synth::writeEmpty(int num){
    soundtouch::SAMPLETYPE sampleBuffer[BUFF_SIZE];
    int sum=num;
    bzero(sampleBuffer , sizeof(sampleBuffer));
    int framelen=BUFF_SIZE/this->channel;
    
    this->pSoundTouch->setTempo(1);
    this->pSoundTouch->putSamples(sampleBuffer , framelen);
    this->pSoundTouch->receiveSamples(sampleBuffer,BUFF_SIZE);
    //this->pSoundTouch->setChannels(this->channel);
    
    int flc=framelen * this->channel;
    while(1){
        if(sum > framelen){
            this->onWrite(sampleBuffer , flc);
            sum-=framelen;
        }else{
            this->onWrite(sampleBuffer , sum * this->channel);
            return true;
        }
    }
}

bool synth::write(int tone , soundword * word , double time , double tempo){
    int sum=this->getLenFromTime(this->rate , time);
    
    if(tone<=0 || word==NULL){
    
        return this->writeEmpty(sum);
    
    }else{
    
        soundtouch::SAMPLETYPE sampleBuffer[BUFF_SIZE];
        
        this->pSoundTouch->setPitchSemiTones(tone + word->kTone + this->kTone);
        this->pSoundTouch->setTempo(tempo);
        
        int nChannels = (int)word->getNumChannels();
        
        if(this->rate!=word->getSampleRate()){
            throw rateError();
            return false;
        }
        
        if(this->channel!=nChannels){
            throw channelsError();
            return false;
        }
        
        //this->pSoundTouch->setChannels(this->channel);
        
        bzero(sampleBuffer , sizeof(sampleBuffer));
        while(1){
            if(word->eof()){
                this->writeEmpty(sum);
                break;
            }
            int num = word->read(sampleBuffer, BUFF_SIZE);
            int nSamples = num / nChannels;
            int buffSizeSamples = BUFF_SIZE / nChannels;
            this->pSoundTouch->putSamples(sampleBuffer, nSamples);
            while(1){
                nSamples = this->pSoundTouch->receiveSamples(sampleBuffer,buffSizeSamples);
                if(nSamples <= 0)
                    break;
                if(sum>nSamples){
                    this->onWrite(sampleBuffer, nSamples * nChannels);
                    sum-=nSamples;
                }else{
                    this->onWrite(sampleBuffer, sum * nChannels);
                    goto loopend;
                }
            }
        }
        loopend:
        word->rewind();
        return true;
    }
}

void synth::outputInit(const char * path){
    this->wavout=new WavOutFile(path,this->rate,this->numBits,this->channel);
}

void synth::onWrite(const soundtouch::SAMPLETYPE *sampleBuffer , int len){
    if(this->wavout)
        this->wavout->write(sampleBuffer , len);
}

void synth::outputClose(){
    if(this->wavout)
        delete this->wavout;
}

}//namespace vomidi
