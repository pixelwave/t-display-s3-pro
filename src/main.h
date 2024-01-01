#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus

extern "C"
{

#endif

//declare functions here
  void toggleRelay1();
  void toggleRelay2();
  void toggleRelay3();
  void toggleRelay4();
  void brightness(int value);
  void color(int angle);

#ifdef __cplusplus

}

#endif

#endif