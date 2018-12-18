/*
  CmdParser.cpp - Library to react on serial command line commands
  Created by Joerg Schmitz-Linneweber, December 18, 2018.
  Released into the public domain.
*/

#include "Arduino.h"
#include "CmdParser.h"

CmdParser::CmdParser(parserCB pfunc)
{
  _parserCB = pfunc;
}

CmdParser::init(void)
{
  Serial.begin (9600);
  while (!Serial) {
    ;
  }
  Serial.println ("Simple command line parser (v1.0)");
  Serial.print ("> ");
  inBuffer[0] = inBuffer[maxBufferLength] = 0;
}

CmdParser::check(void) {
  byte rc;
  static boolean newData = false;
  static byte idx = 0;

  while (Serial.available() > 0) {
    rc = Serial.read();
    if (! newData) {
      if (rc != '\n') {
        inBuffer[idx++] = rc;
        if (idx >= maxBufferLength) {
          idx--;
        }
      }
      else {
        inBuffer[idx] = 0;
        idx = 0;
        newData = true;
	break;
      }
    }
  }

  if (newData) {
    Serial.println (inBuffer);
    if (0 != _parserCB(inBuffer)) {
      Serial.println("Syntax error!");
    }
    newData = false;
    Serial.print ("> ");
  }
}
