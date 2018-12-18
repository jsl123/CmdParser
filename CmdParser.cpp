/*
  CmdParser.cpp - Library to react on serial command line commands
  Created by Joerg Schmitz-Linneweber, December 18, 2018.
  Released into the public domain.
*/

#include "Arduino.h"
#include "CmdParser.h"

CmdParser::CmdParser(parserCB pfunc, boolean prompt, boolean echo)
{
  _parserCB = pfunc;
  _prompt = prompt;
  _echo = echo;
}

CmdParser::init(boolean verbose)
{
  if (verbose) {
    Serial.println ("Simple command line parser (v1.0)");
  }
  if (_prompt) {
    Serial.print ("> ");
  }
  inBuffer[0] = inBuffer[maxBufferLength] = 0;
}

CmdParser::check(void) {
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
    if (0 != _parserCB(inBuffer, idx)) {
      Serial.println("Syntax error!");
    }
    newData = false;
    idx = 0;
    if (_prompt) {
      Serial.print ("> ");
    }
  }
}
