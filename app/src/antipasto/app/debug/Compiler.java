/* -*- mode: java; c-basic-offset: 2; indent-tabs-mode: nil -*- */

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
*/

package antipasto.app.debug;

import processing.app.*;
import processing.app.Base;
import processing.app.debug.MessageConsumer;
import processing.app.debug.RunnerException;
import processing.app.Preferences;
import processing.app.Sketch;
import processing.app.SketchCode;
import processing.core.*;

import java.io.*;
import java.util.*;
import java.util.zip.*;
import javax.swing.*;

import org.arduino.tools.AntRunner;

public class Compiler implements MessageConsumer {
  static final String BUGS_URL =
    "http://code.google.com/p/arduino/issues/list";
  static final String SUPER_BADNESS =
    "Compiler error, please submit this code to " + BUGS_URL;

  Sketch sketch;
  String buildPath;
  String primaryClassName;
  boolean verbose;

  RunnerException exception;

  AntRunner ant = new AntRunner();

  /**
   * Create a new compiler
   */
  public Compiler() {

  }

    /**
     * This function uploads the binary sketch to the board.
     *
     * @param buildPath
     *            The temporary build path
     * @param suggestedClassName
     *            The suggested temporary sketch name
     * @param target
     *            The target core to compile the sketch for.
     * @param sketchName
     *            The name of the current sketch.
     * @throws RunnerException
     */
    public boolean upload(String buildPath, String sketchName,
                          Target target) throws RunnerException {

        Map<String, String> boardPreferences = Base.getBoardPreferences();

        // Try to get the buildfile
        File buildFile = target.getBuildFile();

        if (buildFile != null) {
            if (!ant.isRunning()) {

                String uploadPort = (Base.isWindows() ? "\\\\.\\" : "")+
                                    Preferences.get("serial.port");
                String antTarget  =  "upload.sketch";
                String baudRate   =  boardPreferences.get("upload.speed");
                String protocol   =  boardPreferences.get("upload.protocol");
                String uploadCPU  =  boardPreferences.get("upload.mcu");
                String boardName  =  boardPreferences.get("name");

                if (Preferences.getBoolean("upload.verbose")) {
                    ant.setOutputVerbose();
                } else {
                    ant.setOutputQuiet();
                }

                // Run
                ant.run(buildFile.getPath(), antTarget, new String[] {
                            "build.dest", buildPath,
                            "sketch.name", sketchName.substring(0, sketchName.lastIndexOf('.')), //sketch name without extension
                            "upload.port", uploadPort,
                            "upload.avrdude.baud", baudRate,
                            "upload.avrdude.protocol", protocol,
                            "upload.avrdude.cpu", uploadCPU,
                            "board.name", boardName});

                // Wait to finish
                ant.waitForCompletion();

                //return the status
                return ant.getLastRunStatus();
            }
        } else {
            System.out.println("No buildFile found in " + target.getFolder());
            return false; //fail
        }
        return true; //ant is running
    }

    /**
     * This function produces the binary sketch size after compilation.
     *
     * @param buildPath
     *            The temporary build path
     * @param suggestedClassName
     *            The suggested temporary sketch name
     * @param corePath
     *            The core path to compile the sketch for.
     * @param sketchName
     *            The name of the current sketch.
     * @throws RunnerException
     */
    public boolean size(String buildPath, String suggestedClassName,
                        Target target, String sketchName) throws RunnerException {

        // Check for a successful compile run
        if (ant.getLastRunStatus() == true) {

            // Configure
            String buildFile = target.getFolder() + File.separator + "build.xml";
            if (Preferences.getBoolean("build.verbose")) {
                ant.setOutputVerbose();
            } else {
                ant.setOutputQuiet();
            }

            // Run
            ant.run(buildFile, "size.all", new String[] {
                        "build.dest", buildPath,
                        "sketch.name", sketchName.substring(0, sketchName.lastIndexOf('.'))});

            // Wait to finish
            ant.waitForCompletion();

        }

        //return the status
        return ant.getLastRunStatus();
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

        Map<String, String> boardPreferences = Base.getBoardPreferences();

        //Error checking
        if ((sketch.getName() == null) || (buildPath == null) || (target.getName() == null)) {
            System.err.println("Error: compile parameters " +
                               "sketch=" + sketch +
                               "buildPath=" + buildPath +
                               "target=" + target.getName());
            return false;
        }

        // Try to get the buildfile
        File buildFile = target.getBuildFile();

        if (buildFile != null) {
            if (!ant.isRunning()) {

                String antTarget = "build.all";
                String boardMCU  = boardPreferences.get("build.mcu");
                String maxSize   = boardPreferences.get("upload.maximum_size");
                String boardName = boardPreferences.get("name");

                // Configure
                if (Preferences.getBoolean("build.verbose")) {
                    ant.setOutputVerbose();
                } else {
                    ant.setOutputQuiet();
                }

                // Run
                ant.run(buildFile.getPath(),antTarget, new String[] {
                            "build.dest",  buildPath,
                            "sketch.name", sketch.getName(),
                            "board.cpu", boardMCU,
                            "sketch.maxsize", maxSize,
                            "board.name", boardName});

                // Wait to finish
                ant.waitForCompletion();

                //Return the result
                return ant.getLastRunStatus();
            }
        } else {
            System.out.println("No buildFile found in " + target.getFolder());
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

    // ignore this line; the real error is on the next one
    if (s.indexOf("In file included from") != -1) return;

    // jikes always uses a forward slash character as its separator,
    // so replace any platform-specific separator characters before
    // attemping to compare
    //
    //String buildPathSubst = buildPath.replace(File.separatorChar, '/') + "/";
    String buildPathSubst =
      buildPath.replace(File.separatorChar,File.separatorChar) +
      File.separatorChar;

    String partialTempPath = null;
    int partialStartIndex = -1; //s.indexOf(partialTempPath);
    int fileIndex = -1;  // use this to build a better exception

    // check the main sketch file first.
    partialTempPath = buildPathSubst + primaryClassName;
    partialStartIndex = s.indexOf(partialTempPath);

    if (partialStartIndex != -1) {
      fileIndex = 0;
    } else {
      // wasn't there, check the other (non-pde) files in the sketch.
    // iterate through the project files to see who's causing the trouble
      for (int i = 0; i < sketch.getCodeCount(); i++) {
        if (sketch.getCode(i).isExtension("pde")) continue;

        partialTempPath = buildPathSubst + sketch.getCode(i).getFileName();
        //System.out.println(partialTempPath);
      partialStartIndex = s.indexOf(partialTempPath);
      if (partialStartIndex != -1) {
        fileIndex = i;
        //System.out.println("fileIndex is " + fileIndex);
        break;
      }
    }
    //+ className + ".java";
    }

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

      //System.out.println("pde / line number: " + lineNumber);

      if (fileIndex == 0) {  // main class, figure out which tab
        for (int i = 1; i < sketch.getCodeCount(); i++) {
          if (sketch.getCode(i).isExtension("pde")) {
            //System.out.println("preprocOffset "+ sketch.getCode(i).getPreprocOffset());
            if (sketch.getCode(i).getPreprocOffset() < lineNumber) {
              fileIndex = i;
              //System.out.println("i'm thinkin file " + i);
            }
          }
        }
        // XXX: DAM: if the lineNumber is less than sketch.getCode(0).getPreprocOffset()
        // we shouldn't subtract anything from it, as the error is above the
        // location where the function prototypes and #include "WProgram.h"
        // were inserted.
        lineNumber -= sketch.getCode(fileIndex).getPreprocOffset();
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

        //System.out.println("description = " + description);
        //System.out.println("creating exception " + exception);
        exception = new RunnerException(description, fileIndex, lineNumber-1, -1, false);

        // NOTE!! major change here, this exception will be queued
        // here to be thrown by the compile() function
        //editor.error(exception);

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



  /////////////////////////////////////////////////////////////////////////////

  static private void createFolder(File folder) throws RunnerException {
    if (folder.isDirectory()) return;
    if (!folder.mkdir())
      throw new RunnerException("Couldn't create: " + folder);
  }

  /**
   * Given a folder, return a list of the header files in that folder (but
   * not the header files in its sub-folders, as those should be included from
   * within the header files at the top-level).
   */
  static public String[] headerListFromIncludePath(String path) {
    FilenameFilter onlyHFiles = new FilenameFilter() {
      public boolean accept(File dir, String name) {
        return name.endsWith(".h");
      }
    };

    return (new File(path)).list(onlyHFiles);
  }

  static public ArrayList<File> findFilesInPath(String path, String extension,
                                                boolean recurse) {
    return findFilesInFolder(new File(path), extension, recurse);
  }

  static public ArrayList<File> findFilesInFolder(File folder, String extension,
                                                  boolean recurse) {
    ArrayList<File> files = new ArrayList<File>();

    if (folder.listFiles() == null) return files;

    for (File file : folder.listFiles()) {
      if (file.getName().equals(".") || file.getName().equals("..")) continue;

      if (file.getName().endsWith("." + extension))
        files.add(file);

      if (recurse && file.isDirectory()) {
        files.addAll(findFilesInFolder(file, extension, true));
      }
    }

    return files;
  }

  /**
   * Given a folder, return a list of absolute paths to all jar or zip files
   * inside that folder, separated by pathSeparatorChar.
   *
   * This will prepend a colon (or whatever the path separator is)
   * so that it can be directly appended to another path string.
   *
   * As of 0136, this will no longer add the root folder as well.
   *
   * This function doesn't bother checking to see if there are any .class
   * files in the folder or within a subfolder.
   */
  static public String contentsToClassPath(File folder) {
    if (folder == null) return "";

    StringBuffer abuffer = new StringBuffer();
    String sep = System.getProperty("path.separator");

    try {
      String path = folder.getCanonicalPath();

//    disabled as of 0136
      // add the folder itself in case any unzipped files
//      abuffer.append(sep);
//      abuffer.append(path);
//
      // When getting the name of this folder, make sure it has a slash
      // after it, so that the names of sub-items can be added.
      if (!path.endsWith(File.separator)) {
        path += File.separator;
      }

      String list[] = folder.list();
      for (int i = 0; i < list.length; i++) {
        // Skip . and ._ files. Prior to 0125p3, .jar files that had
        // OS X AppleDouble files associated would cause trouble.
        if (list[i].startsWith(".")) continue;

        if (list[i].toLowerCase().endsWith(".jar") ||
            list[i].toLowerCase().endsWith(".zip")) {
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


  /**
   * A classpath, separated by the path separator, will contain
   * a series of .jar/.zip files or directories containing .class
   * files, or containing subdirectories that have .class files.
   *
   * @param path the input classpath
   * @return array of possible package names
   */
  static public String[] packageListFromClassPath(String path) {
    Hashtable table = new Hashtable();
    String pieces[] =
      PApplet.split(path, File.pathSeparatorChar);

    for (int i = 0; i < pieces.length; i++) {
      //System.out.println("checking piece '" + pieces[i] + "'");
      if (pieces[i].length() == 0) continue;

      if (pieces[i].toLowerCase().endsWith(".jar") ||
          pieces[i].toLowerCase().endsWith(".zip")) {
        //System.out.println("checking " + pieces[i]);
        packageListFromZip(pieces[i], table);

      } else {  // it's another type of file or directory
        File dir = new File(pieces[i]);
        if (dir.exists() && dir.isDirectory()) {
          packageListFromFolder(dir, null, table);
          //importCount = magicImportsRecursive(dir, null,
          //                                  table);
                                              //imports, importCount);
        }
      }
    }
    int tableCount = table.size();
    String output[] = new String[tableCount];
    int index = 0;
    Enumeration e = table.keys();
    while (e.hasMoreElements()) {
      output[index++] = ((String) e.nextElement()).replace('/', '.');
    }
    //System.arraycopy(imports, 0, output, 0, importCount);
    //PApplet.printarr(output);
    return output;
  }


  static private void packageListFromZip(String filename, Hashtable table) {
    try {
      ZipFile file = new ZipFile(filename);
      Enumeration entries = file.entries();
      while (entries.hasMoreElements()) {
        ZipEntry entry = (ZipEntry) entries.nextElement();

        if (!entry.isDirectory()) {
          String name = entry.getName();

          if (name.endsWith(".class")) {
            int slash = name.lastIndexOf('/');
            if (slash == -1) continue;

            String pname = name.substring(0, slash);
            if (table.get(pname) == null) {
              table.put(pname, new Object());
            }
          }
        }
      }
    } catch (IOException e) {
      System.err.println("Ignoring " + filename + " (" + e.getMessage() + ")");
      //e.printStackTrace();
    }
  }


  /**
   * Make list of package names by traversing a directory hierarchy.
   * Each time a class is found in a folder, add its containing set
   * of folders to the package list. If another folder is found,
   * walk down into that folder and continue.
   */
  static private void packageListFromFolder(File dir, String sofar,
                                            Hashtable table) {
                                          //String imports[],
                                          //int importCount) {
    //System.err.println("checking dir '" + dir + "'");
    boolean foundClass = false;
    String files[] = dir.list();

    for (int i = 0; i < files.length; i++) {
      if (files[i].equals(".") || files[i].equals("..")) continue;

      File sub = new File(dir, files[i]);
      if (sub.isDirectory()) {
        String nowfar =
          (sofar == null) ? files[i] : (sofar + "." + files[i]);
        packageListFromFolder(sub, nowfar, table);
        //System.out.println(nowfar);
        //imports[importCount++] = nowfar;
        //importCount = magicImportsRecursive(sub, nowfar,
        //                                  imports, importCount);
      } else if (!foundClass) {  // if no classes found in this folder yet
        if (files[i].endsWith(".class")) {
          //System.out.println("unique class: " + files[i] + " for " + sofar);
          table.put(sofar, new Object());
          foundClass = true;
        }
      }
    }
  }

    /**
     * Clean the project using ANT
     * Runs the clean.all on the current board's target
     */
    public boolean clean(Target target) {

        // Try to get the buildfile
        File buildFile = target.getBuildFile();

        if (buildFile != null) {
            if (!ant.isRunning()) {

                String antTarget = "clean.all";

                //Configure
                if (Preferences.getBoolean("build.verbose")) {
                    ant.setOutputVerbose();
                } else {
                    ant.setOutputQuiet();
                }

                try {
                    // Run
                    ant.run(target.getBuildFile().getPath(), antTarget, new String[] {
                                "build.dest",  Base.getBuildFolder().getAbsolutePath(),
                            });

                    // Wait to finish
                    ant.waitForCompletion();
                } catch (Exception e) {
                    //fail quietly
                }
            }
        } else {
            System.out.println("No buildFile found in " + target.getFolder());
            return false; //fail
        }

        return ant.getLastRunStatus();
    }

   /**
   *
   *  Uploads the the bootloader using the ANT buildfile targets
   *  found in the core.
   *
   * @param target
   *  The core target
   *
   * @return boolean
   *  Returns true if the upload was successful
   */
    public boolean burnBootloader() {

        Target target = Base.getCurrentTarget();
        // Try to get the buildfile
        File buildFile = target.getBuildFile();

        if (buildFile != null) {
            if (!ant.isRunning()) {

                String antTarget = "upload.bootloader";
                String uploadPort = (Base.isWindows() ? "\\\\.\\" : "") +
                                    Preferences.get("serial.port");

                //Configure
                if (Preferences.getBoolean("upload.verbose")) {
                    ant.setOutputVerbose();
                } else {
                    ant.setOutputQuiet();
                }

                // Run
                ant.run(buildFile.getPath(), antTarget, new String[] {
                            "build.dest",  Base.getBuildFolder().getAbsolutePath(),
                            "upload.port", uploadPort,
                        });

                // Wait to finish
                ant.waitForCompletion();
            }
        } else {
            System.out.println("No buildFile found in " + target.getFolder());
            return false; //fail
        }

        return ant.getLastRunStatus();
    }

    /**
     *  If the ant compiler is running,
     *  then return true
     */
    public boolean isRunning() {
        return ant.isRunning();
    }
}
