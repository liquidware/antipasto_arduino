#!/bin/sh


### -- SETUP WORK DIR -------------------------------------------

if test -d work
then
  BUILD_PREPROC=false
else
  echo Setting up directories to build arduino...
  BUILD_PREPROC=true
  cp -r ../shared work
  rm -f work/.DS_Store 
  #cp ../../lib/*.dll work
  cp dist/*.dll work
  cp dist/run.bat work
  chmod 755 work/run.bat
  cp ../../readme.txt work
  unix2dos work/readme.txt
  
  # needs to make the dir because of packaging goofiness
  mkdir -p work/classes/arduino/app/syntax
  mkdir -p work/classes/arduino/app/tools

  echo Extracting reference...
  cd work
  unzip reference.zip
  # necessary for launching reference from shell/command prompt
  # which is done internally to view reference
  #chmod +x reference/*.html
  # needed by 'help' menu
  #chmod +x reference/environment/*.html
  # chmod -R +x *.html doesn't seem to work
  rm reference.zip
  cd ..

  echo Extracting enormous JRE...
  unzip -q  -d work jre.zip
  # cygwin requires this because of unknown weirdness
  # it was not formerly this anal retentive
  cd work/java/bin/
  #chmod +x *.exe *.dll 
  #chmod +x client/*.dll
  cd ../../..

  mkdir work/lib/build
  #mkdir work/classes

  echo Compiling arduino.exe
  cd launcher
  make && cp arduino.exe ../work/
  cd ..

  cp dist/ICE_JNIRegistry.dll work/

  cp -r ../../hardware work/
  cp dist/bootloader/*.* work/hardware/bootloaders/atmega8
  mkdir work/drivers
  cp -r dist/drivers/* work/drivers/

# Extracting Compilers
#  echo Extracting Java Compiler ...
#  unzip -q  -d work/hardware/tools jre.zip

  echo Extracting AVR Compiler...
  unzip -q  -d work/hardware dist/avr_tools.zip

  echo Extracting JRuby Compiler...
  rm work/jruby.zip
  unzip -q  -d work/hardware/tools ../shared/jruby.zip  
  

  # take care of the examples
  cp -r ../shared/dist/examples work/

  #clean the remaining dist directory away 
  rm -rf work/dist

  # chmod +x the crew
  find work -name "*.dll" -exec chmod +x {} ';'
  find work -name "*.exe" -exec chmod +x {} ';'
  find work -name "*.html" -exec chmod +x {} ';'

fi

cd ../..

### -- BUILD PDE ------------------------------------------------

cd processing/app

LIBDIR="..\\..\\build\\windows\\work\\lib"

LIBRARIES="$LIBDIR\\junit.jar;\
$LIBDIR\\sax2.jar;\
$LIBDIR\\filterbuilder.jar;\
$LIBDIR\\htmllexer.jar;\
$LIBDIR\\htmlparser.jar;\
$LIBDIR\\thumbelina.jar;\
$LIBDIR\\RXTXcomm.jar;\
$LIBDIR\\mrj.jar;\
$LIBDIR\\antlr.jar;\
$LIBDIR\\oro.jar;\
$LIBDIR\\registry.jar;\
$LIBDIR\\rt.jar"

../../build/windows/work/java/bin/java \
                                        -classpath "..\\..\\build\\windows\\work\\java\\lib\\tools.jar" com.sun.tools.javac.Main \
                                        -deprecation \
                                        -Xlint:unchecked \
                                        -source 1.6 \
                                        -target 1.6 \
                                        -classpath "$LIBRARIES;" \
                                        -d ..\\..\\build\\windows\\work\\classes ../core/*.java *.java preproc/*.java syntax/*.java tools/*.java preproc/*.java  ../../antipasto/Util/*.java ../../antipasto/*.java ../../antipasto/Interfaces/*.java  ../../antipasto/GUI/GadgetListView/*.java ../../antipasto/GUI/GadgetListView/GadgetPanelEvents/*.java ../../antipasto/GUI/GadgetListView/AddGadgetActions/*.java ../../antipasto/ModuleRules/*.java ../../antipasto/GUI/ImageListView/*.java




cd ../../build/windows/work/classes
rm -f ../lib/pde.jar
zip -0rq ../lib/pde.jar .

# back to build/windows
cd ../..



echo
echo Done.

