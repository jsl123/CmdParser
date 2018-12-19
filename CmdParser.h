/*
  CmdParser.cpp - Library to react on serial command line commands
  Created by Joerg Schmitz-Linneweber, December 18, 2018.
  Released into the public domain.
*/
#ifndef CmdParser_h
#define CmdParser_h

#include "Arduino.h"

typedef int (*parserCB)(char *, int);

class CmdParser
{
  public:
    CmdParser(parserCB, boolean = true, boolean = true);
    void init(boolean);
    void loop(void);
    const String version(void);
  private:
    parserCB _parserCB;
    const static int maxBufferLength = 31;
    char inBuffer[maxBufferLength + 1];
    boolean newData;
    boolean _prompt;
    boolean _echo;
    const String _version = "1.1";
};

#endif
