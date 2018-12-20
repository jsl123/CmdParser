/*
  CmdParser.cpp - Library to react on serial command line commands
  Created by Joerg Schmitz-Linneweber, December 18, 2018.
  Released into the public domain.
*/
#ifndef CmdParser_h
#define CmdParser_h

#include "Arduino.h"

typedef int (*parserCB)(int, int);
struct cmd_table_t {
  char *name;
  int parser_type;
  int command;
};

class CmdParser
{
  public:
    CmdParser(parserCB, struct cmd_table_t *,
	      boolean = true, boolean = true, boolean = true);
    void init(void);
    void loop(void);
    const String GetVersion(void);
    const enum {PARSER_END, PARSER_WORD, PARSER_WORD_INT} type;
  private:
    parserCB _parserCB;
    struct cmd_table_t *_cmd_table;
    const static int maxBufferLength = 31;
    char inBuffer[maxBufferLength + 1];
    boolean newData;
    boolean _verbose;
    boolean _prompt;
    boolean _echo;
    const String _version = "1.1";
};

#endif
