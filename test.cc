#include <SDL.h>
#include <SDL_audio.h>
#include <math.h>
#include <iostream>
#define FREQUENCY 44100
float phase=0;
long frame=0;
float delta=1.0f/FREQUENCY;
SDL_AudioSpec audioSpec;

extern "C" void mixeAudio(void *nichtVerwendet, Uint8 *stream, int laenge) {
  Sint16 v;
  Uint8 *a=(Uint8*)&v;
  Uint8 *b=a;
  b++;
  Uint8*p=stream;
  for(int i=0;i<laenge/2;i++) {
    phase=float(frame)/FREQUENCY;
    v=sin(phase*440*2*M_PI)*15000;

    *(p++)=*a;
    *(p++)=*b;
    frame++;
  }
}

void runAudio() {
  SDL_AudioSpec format;

  /* Format: 16 Bit, stereo, 22 KHz */
  format.freq = FREQUENCY;
  format.format = AUDIO_S16LSB;
  format.channels = 1;
  format.samples = 8*2048;        /* ein guter Wert für Spiele */
  format.callback = mixeAudio;
  format.userdata = NULL;

  /* das Audio-Gerät öffnen und das Abspielen beginnen */
  if ( SDL_OpenAudio(&format, &audioSpec) < 0 ) {
    fprintf(stderr, "Audio-Gerät konnte nicht geöffnet werden: %s\n", SDL_GetError());
    exit(1);
  }

  std::cout<<"OK"<<std::endl;
  //delta=1.0f/audioSpec.freq;
  std::cout<<"FREQ:"<<audioSpec.freq<<std::endl;

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
