#include <SDL.h>
#include <SDL_audio.h>
#include <math.h>
#include <iostream>
#define FREQUENCY 44100
float phase=0;
long frame=0;
SDL_AudioSpec audioSpec;


class Master {
  long frame;
  float volume;
  float frequency;
  float tone;

  public:
  Master(float freq) {
    frame=0;
    frequency=freq;
    volume=14000;
    tone=440;
  }
  Uint16 nextSample() {

    return sin((float(frame++)/frequency)*tone*2*M_PI)*volume;
  }
};

static Master *master=0;
static Sint16 word;
static Uint8 *firstByte=(Uint8*)&word;
static Uint8 *secondByte=firstByte+1;
extern "C" void mixeAudio(void *nichtVerwendet, Uint8 *stream, int laenge) {
  if(master) {

    Uint8*p=stream;
    for(int i=0;i<laenge/2;i++) {
      word=master->nextSample();

      *(p++)=*firstByte;
      *(p++)=*secondByte;
      frame++;
    }
  }
  else {
    std::cout<<"No master defined"<<std::endl;
  }
}

void runAudio() {
  SDL_AudioSpec format;
  master=new Master(FREQUENCY);
  /* Format: 16 Bit, stereo, 22 KHz */
  format.freq = FREQUENCY;
  format.format = AUDIO_S16LSB;
  format.channels = 1;
  format.samples = 512;        /* ein guter Wert für Spiele */
  format.callback = mixeAudio;
  format.userdata = NULL;

  /* das Audio-Gerät öffnen und das Abspielen beginnen */
  if ( SDL_OpenAudio(&format, &audioSpec) < 0 ) {
    fprintf(stderr, "Audio-Gerät konnte nicht geöffnet werden: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_PauseAudio(0);

  SDL_Delay(2000);

  SDL_CloseAudio();
}
int main() {
  SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
  //  SDL_Delay(1000);
  runAudio();

  return 0;
}
