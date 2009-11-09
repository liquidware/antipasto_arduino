/* -*- mode: jde; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
  Part of the Processing project - http://processing.org

  Copyright (c) 2004-05 Ben Fry and Casey Reas
  Copyright (c) 2001-04 Massachusetts Institute of Technology

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

package processing.app;

import processing.core.*;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.lang.reflect.*;
import java.util.*;

import com.oroinc.text.regex.*;


/**
 * Runs a compiled java applet, whether as an external application
 * or internally as an applet object in a window.
 */
public class Runner implements MessageConsumer {

  PApplet applet;
  RunnerException exception;
  Window window;
  PrintStream leechErr;

  Editor editor;
  Sketch sketch;

  boolean newMessage;
  int messageLineCount;
  boolean foundMessageSource;

  Process process;
  SystemOutSiphon processInput;
  OutputStream processOutput;
  MessageSiphon processError;


  public Runner(Sketch sketch, Editor editor) {
    this.sketch = sketch;
    this.editor = editor;
  }


  public void start(Point windowLocation) throws RunnerException {

  }


  public void startPresenting() throws Exception {
    Vector params = new Vector();

    params.add("java");

    String options = Preferences.get("run.options");
  }


  public void startExternalRuntime(Point windowLocation) throws Exception {

  }


  public void startInternal(Point windowLocation) throws Exception {
 
  }


  public void stop() {

  }


  public void close() {

  }


  // made synchronized for rev 87
  synchronized public void message(String s) {

    // always shove out the mesage, since it might not fall under
    // the same setup as we're expecting
    System.err.print(s);
    System.err.flush();

    // exit here because otherwise the exception name
    // may be titled with a blank string
    if (s.trim().length() == 0) return;

    // annoying, because it seems as though the terminators
    // aren't being sent properly

    if (newMessage) {
      exception = new RunnerException(s);  // type of java ex
      exception.hideStackTrace = true;
      newMessage = false;
      foundMessageSource = false;
      messageLineCount = 0;

    } else {
      messageLineCount++;

      if (!foundMessageSource) {
        int afterAt = s.indexOf("at") + 3;
        if (afterAt == 2) {  // means indexOf was -1
          return;
        }
        s = s.substring(afterAt + 1);

        int startParen = s.indexOf('(');
        String pkgClassFxn = null;
        int codeIndex = -1;
        int lineNumber = -1;

        if (startParen == -1) {
          pkgClassFxn = s;

        } else {
          pkgClassFxn = s.substring(0, startParen);
          String fileAndLine = s.substring(startParen + 1);
          int stopParen = fileAndLine.indexOf(')');
          fileAndLine = fileAndLine.substring(0, stopParen);
          int colonIndex = fileAndLine.indexOf(':');
          if (colonIndex != -1) {
            String filename = fileAndLine.substring(0, colonIndex);
            lineNumber =
              Integer.parseInt(fileAndLine.substring(colonIndex + 1)) - 1;

            for (int i = 0; i < sketch.codeCount; i++) {
              SketchCode code = sketch.code[i];
              if (((code.preprocName == null) &&
                   (lineNumber >= code.preprocOffset)) ||
                  ((code.preprocName != null) &&
                   code.preprocName.equals(filename))) {
                codeIndex = i;
              }
            }

            if (codeIndex != -1) {
              //System.out.println("got line num " + lineNumber);
              // in case this was a tab that got embedded into the main .java
              lineNumber -= sketch.code[codeIndex].preprocOffset;

              // lineNumber is 1-indexed, but editor wants zero-indexed
              // getMessage() will be what's shown in the editor
              exception =
                new RunnerException(exception.getMessage(),
                                    codeIndex, lineNumber, -1);
              exception.hideStackTrace = true;
              foundMessageSource = true;
            }
          }
        }
        editor.error(exception);

      } else if (messageLineCount > 10) {  // 5 -> 10 for 0088
        // this means the class name may not be mentioned
        // in the stack trace.. this is just a general purpose
        // error, but needs to make it through anyway.
        // so if five lines have gone past, might as well signal
        messageLineCount = -100;
        exception = new RunnerException(exception.getMessage());
        exception.hideStackTrace = true;
        editor.error(exception);

      } else {
      }
    }
  }


  //////////////////////////////////////////////////////////////


  /**
   * Siphons from an InputStream of System.out (from a Process)
   * and sends it to the real System.out.
   */
  class SystemOutSiphon implements Runnable {
    InputStream input;
    Thread thread;

    public SystemOutSiphon(InputStream input) {
      this.input = input;

      thread = new Thread(this);
      // unless this is set to min, it seems to hork the app
      // since it's in charge of stuffing the editor console with strings
      // maybe it's time to get rid of/fix that friggin console
      // ...disabled for 0075, with 0074's fix for code folder hanging
      // this only seems to make the console unresponsive
      thread.start();
    }

    public void run() {
      byte boofer[] = new byte[256];

      while (Thread.currentThread() == thread) {
        try {
          // can't use a buffered reader here because incremental
          // print statements are interesting too.. causes some
          // disparity with how System.err gets spewed, oh well.
          int count = input.read(boofer, 0, boofer.length);
          if (count == -1) {
            thread = null;

          } else {
            System.out.print(new String(boofer, 0, count));
          }

        } catch (IOException e) {
          thread = null;

        } catch (Exception e) {
          thread = null;
        }
      }
    }
  }
}
