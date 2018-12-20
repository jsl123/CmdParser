/*
  CmdParser.cpp - Library to react on serial command line commands
  Created by Joerg Schmitz-Linneweber, December 18, 2018.
  Released into the public domain.
*/

#include "Arduino.h"
#include "CmdParser.h"

CmdParser::CmdParser(parserCB pfunc, struct cmd_table_t *cmd_table,
		     boolean verbose, boolean prompt, boolean echo)
{
  _parserCB = pfunc;
  _cmd_table = cmd_table;
  _verbose = verbose;
  _prompt = prompt;
  _echo = echo;
}

void CmdParser::init(void)
{
  if (_verbose) {
    Serial.print ("Simple command line parser (v");
    Serial.print (_version);
    Serial.println (")");
  }
  if (_prompt) {
    Serial.print ("> ");
  }
  inBuffer[0] = inBuffer[maxBufferLength] = 0;
}

void CmdParser::loop(void) {
  byte rc, read;
  static boolean newData = false;
  static byte idx = 0;

  read = 0;
  while (read < maxBufferLength && Serial.available() > 0) {
    rc = Serial.read();
    read++;
    if (! newData) {
      if (rc == '\n' || rc == '\r' ) {
        inBuffer[idx] = 0;
	if (idx > 0) {
	  newData = true;
	}
	break;
      }
      else {
        if (idx < maxBufferLength) {
	  inBuffer[idx++] = rc;
        }
      }
    }
  }

  if (newData) {
    if (_echo) {
      Serial.println (inBuffer);
    }
    else {
      Serial.println ("");
    }

    // Lookup 'idx' bytes in 'inBuffer' in '_cmd_table'
    int integer1;
    struct cmd_table_t *pc = _cmd_table;

    for (pc = _cmd_table; pc->name && strlen (pc->name) > 0; pc++) {
      integer1 = 0;

      if (0 == strncmp(inBuffer, pc->name,
		       min(strlen(inBuffer), strlen (pc->name)))) {
	switch (pc->parser_type) {
	case PARSER_WORD:
	  l = strlen (inBuffer);
	  break;
	  
	case PARSER_WORD_INT:
	  l = strlen (pc->name) + 1;
	  if (strlen (inBuffer) < (l+1) || inBuffer[l-1] != ' ') {
	    if (_verbose) {
	      Serial.println("Parameter error");
	    }
	    continue;
	  }
	  else {
	    integer1 = atoi (inBuffer + l);
	  }
	  break;
	  
	case PARSER_END: // fall-through
	default:
	  if (_verbose) {
	    Serial.println("Command not found or implemented");
	  }
	  continue;
	}
	
	if (0 != _parserCB(pc->command, integer1)) {
	  if (_verbose) {
	    Serial.println("Command failed");
	  }
	}
      }
    }
    newData = false;
    idx = 0;
    if (_prompt) {
      Serial.print ("> ");
    }
  }
}

const String CmdParser::GetVersion(void) {
  return _version;
}
