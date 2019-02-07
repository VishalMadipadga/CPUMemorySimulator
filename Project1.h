#ifndef _PROJECT1_H
#define _PROJECT1_H
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <string.h>
using namespace std;

extern int pfds[2];
extern int ps[2];

//CPU class
class CPU
{
  
  //private variables
  private:
  int PC = 0;
  int IR = 0;
  int AC;
  int SP = 1000;
  int X;
  int Y;
  int instructions;
  int buffer;
  int writeFlag = 32767;
  int random;
  bool running = true;
  int port;
  int popVar;
  bool kernelMode = false;
  int instructionCounter = 0;

 //public functions
  public:
  CPU(int instructions);
  int requestMemory(int address);
  void push(int data);
  int pop();
  void instructionProcessing(int instruction); 
  void runCPU();
  void enter(int interruptHandler);
  void exit();

};

#endif

