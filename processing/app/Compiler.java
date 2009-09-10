/* -*- mode: jde; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
  Compiler - default compiler class that connects to avr-gcc
  Part of the Arduino project - http://www.arduino.cc/

  Modified by Christopher Ladden
              David A. Mellis

  Copyright (c) 2004-05 Hernando Barragan

  Processing version
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

  $Id: Compiler.java 520 2008-10-26 14:02:15Z mellis $
*/

package processing.app;

import java.io.*;
import java.util.*;
import java.util.zip.*;
import javax.swing.*;

import org.arduino.tools.AntRunner;

public class Compiler implements MessageConsumer {
  static final String BUGS_URL =
    "https://developer.berlios.de/bugs/?group_id=3590";
  static final String SUPER_BADNESS =
    "Compiler error, please submit this code to " + BUGS_URL;

  Sketch sketch;
  String buildPath;
  RunnerException exception;

  AntRunner ant;
  Editor editor;

  /**
   *
   * @param ant
   * A reference to an AntRunner. The AntRunner is used to run the compilation.
   *
   * @param editor
   * A reference to an Editor.
   */
  public Compiler(AntRunner ant, Editor editor) {

	this.ant = ant;
	this.editor = editor;
  }


  /**
   *
   * @param target
   * The target core to find the buildfile in.
   * @return
   * Returns the build file found in the specified core.
   * If the file is not found, it returns null.
   */
  public static File getBuildFile(Target target) {
	 File buildFile = new File(target.path + File.separator + "build.xml");

	 if (!buildFile.exists()) {
		 buildFile = null;
	 }

	 return buildFile;
  }

    /**
     * This function compiles the sketch.
     *
     * @param sketch
     *            The current sketch.
     * @param buildPath
     *            The temporary build path
     * @param target
     *            The core to compile the sketch for.
     * @throws RunnerException
     */
    public boolean compile(Sketch sketch, String buildPath, Target target)
    throws RunnerException {

        // Try to get the buildfile
        File buildFile = getBuildFile(target);

        if (buildFile != null) {
            if (!ant.isRunning()) {

                String antTarget = Preferences.get("boards." +
                                                   Preferences.get("board") +
                                                   ".targets.sketch.verify");

                // Configure
                if (Preferences.getBoolean("build.verbose")) {
                    ant.setOutputVerbose();
                } else {
                    ant.setOutputQuiet();
                }

                // Run
                ant.run(buildFile.toString(),antTarget, new String[] {
                            "build.dest",  buildPath,
                            "sketch.name", sketch.name});

                // Wait to finish
                ant.waitForCompletion();

                //Return the result
                return ant.getLastRunStatus();
            }
        } else {
            System.out.println("No buildFile found in " + target.getPath());
            return false; //fail
        }
        return true; //ant is running
    }

  boolean firstErrorFound;
  boolean secondErrorFound;

  /**
   * Part of the MessageConsumer interface, this is called
   * whenever a piece (usually a line) of error message is spewed
   * out from the compiler. The errors are parsed for their contents
   * and line number, which is then reported back to Editor.
   */
  public void message(String s) {
    // This receives messages as full lines, so a newline needs
    // to be added as they're printed to the console.
    //System.err.print(s);

    // ignore cautions
    if (s.indexOf("warning") != -1) return;

    // jikes always uses a forward slash character as its separator,
    // so replace any platform-specific separator characters before
    // attemping to compare
    //
    //String buildPathSubst = buildPath.replace(File.separatorChar, '/') + "/";
    String buildPathSubst = buildPath.replace(File.separatorChar,File.separatorChar) + File.separatorChar;

    String partialTempPath = null;
    int partialStartIndex = -1; //s.indexOf(partialTempPath);
    int fileIndex = -1;  // use this to build a better exception

    // iterate through the project files to see who's causing the trouble
    for (int i = 0; i < sketch.codeCount; i++) {
      if (sketch.code[i].preprocName == null) continue;

      partialTempPath = buildPathSubst + sketch.code[i].preprocName;
      partialStartIndex = s.indexOf(partialTempPath);
      if (partialStartIndex != -1) {
        fileIndex = i;
        //System.out.println("fileIndex is " + fileIndex);
        break;
      }
    }
    //+ className + ".java";

    // if the partial temp path appears in the error message...
    //
    //int partialStartIndex = s.indexOf(partialTempPath);
    if (partialStartIndex != -1) {

      // skip past the path and parse the int after the first colon
      //
      String s1 = s.substring(partialStartIndex +
                              partialTempPath.length() + 1);
      //System.out.println(s1);
      int colon = s1.indexOf(':');

      if (s1.indexOf("In function") != -1 || colon == -1) {
        System.err.print(s1);
        //firstErrorFound = true;
        return;
      }

      int lineNumber;
      try {
        lineNumber = Integer.parseInt(s1.substring(0, colon));
      } catch (NumberFormatException e) {
        System.err.print(s1);
        return;
      }

      // the "1" corresponds to the amount of lines written to the main code
      // file by PdePreprocessor's writeHeader() routine before prototypes
      if (fileIndex == 0)
        lineNumber -= 1;

      //System.out.println("pde / line number: " + lineNumber);

      if (fileIndex == 0) {  // main class, figure out which tab
        for (int i = 1; i < sketch.codeCount; i++) {
          if (sketch.code[i].flavor == Sketch.PDE) {
            //System.out.println("preprocOffset "+ sketch.code[i].preprocOffset);
            if (sketch.code[i].preprocOffset < lineNumber) {
              fileIndex = i;
              //System.out.println("i'm thinkin file " + i);
            }
          }
        }
        if (fileIndex != 0) {  // if found another culprit
          lineNumber -= sketch.code[fileIndex].preprocOffset;
          //System.out.println("i'm sayin line " + lineNumber);
        }
      }

      //String s2 = s1.substring(colon + 2);
      int err = s1.indexOf(":");
      if (err != -1) {

        // if the first error has already been found, then this must be
        // (at least) the second error found
        if (firstErrorFound) {
          secondErrorFound = true;
          return;
        }

        // if executing at this point, this is *at least* the first error
        firstErrorFound = true;

        err += ":".length();
        String description = s1.substring(err);
        description = description.trim();
        System.err.print(description);

        exception = new RunnerException(description, fileIndex, lineNumber-1, -1);

      } else {
        System.err.println("i suck: " + s);
      }

    } else {
      // this isn't the start of an error line, so don't attempt to parse
      // a line number out of it.

      // if the second error hasn't been discovered yet, these lines
      // are probably associated with the first error message,
      // which is already in the status bar, and are likely to be
      // of interest to the user, so spit them to the console.
      //
      if (!secondErrorFound) {
        System.err.println(s);
      }
    }
  }

  static String bootClassPath;

  static public String calcBootClassPath() {
    if (bootClassPath == null) {
      String additional = "";
      if (Base.isMacOS()) {
        additional =
          contentsToClassPath(new File("/System/Library/Java/Extensions/"));
      }
      bootClassPath =  System.getProperty("sun.boot.class.path") + additional;
    }
    return bootClassPath;
  }


  ///


  /**
   * Return the path for a folder, with appended paths to
   * any .jar or .zip files inside that folder.
   * This will prepend a colon (or whatever the path separator is)
   * so that it can be directly appended to another path string.
   *
   * This will always add the root folder as well, and doesn't bother
   * checking to see if there are any .class files in the folder or
   * within a subfolder.
   */
  static public String contentsToClassPath(File folder) {
    if (folder == null) return "";

    StringBuffer abuffer = new StringBuffer();
    String sep = System.getProperty("path.separator");

    try {
      // add the folder itself in case any unzipped files
      String path = folder.getCanonicalPath();
      abuffer.append(sep);
      abuffer.append(path);

      if (!path.endsWith(File.separator)) {
        path += File.separator;
      }
      //System.out.println("path is " + path);

      String list[] = folder.list();
      for (int i = 0; i < list.length; i++) {
        if (list[i].toLowerCase().endsWith(".o") ||
            list[i].toLowerCase().endsWith(".a")) {
          abuffer.append(sep);
          abuffer.append(path);
          abuffer.append(list[i]);
        }
      }
    } catch (IOException e) {
      e.printStackTrace();  // this would be odd
    }
    //System.out.println("included path is " + abuffer.toString());
    //packageListFromClassPath(abuffer.toString());  // WHY?
    return abuffer.toString();
  }
}
