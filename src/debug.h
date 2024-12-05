#include <Arduino.h>
void debug();
void debug(String msg, int freq);
void debug(String msg, double value, int freq);
void printStruct(struct target,String name, int freq = 500);