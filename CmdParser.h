/*
  CmdParser.cpp - Library to react on serial command line commands
  Created by Joerg Schmitz-Linneweber, December 18, 2018.
  Released into the public domain.
*/
#ifndef CmdParser_h
#define CmdParser_h

#include "Arduino.h"

typedef int (*parserCB)(int, int);
typedef struct {char *name, int parser_type, int command} cmd_table_t;

class CmdParser
{
  public:
    CmdParser(parserCB, boolean = true, boolean = true);
    void init(cmd_table_t cmd_table*, boolean);
    void loop(void);
    const String GetVersion(void);
    const enum {PARSER_WORD, PARSER_WORD_INT} type;
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
