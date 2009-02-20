#!/bin/sh

REVISION=`head -c 5 ../../todo.txt`
 
# check to see if the version number in the app is correct
# so that mikkel doesn't kick my ass
VERSIONED=`cat ../../processing/app/Base.java | grep $REVISION`
if [ -z "$VERSIONED" ]
then
echo Fix the revision number in Base.java
  exit
fi
 
./make.sh
 
echo Creating P5 distribution for revision $REVISION...
echo
 
# remove any old boogers
rm -rf arduino
rm -rf arduino-* 
 
# use 'shared' files as starting point
cp -r ../shared arduino
mkdir arduino
#cp -r work/lib/targets arduino/lib
 
# new style examples thing ala reas
#cd arduino
#mkdir examples
#unzip -d examples -q dist/examples.zip
#rm dist/examples.zip
#rm -rf dist
#cd ..
 
mv arduino/dist/examples arduino/examples
rm -rf arduino/dist
 
# extract reference
cd arduino
unzip reference.zip
rm reference.zip
cd ..
 
# add java (jre) files
unzip -q -d arduino jre.zip
 
# copy stuff from work/
cp work/readme.txt arduino
cp -r work/hardware arduino
cp -r work/drivers arduino
#cp -r work/examples arduino
 
# directories used by the app
#mkdir arduino/lib/build
 
# grab pde.jar and export from the working dir
cp work/lib/pde.jar arduino/lib/
cp work/java/lib/rt.jar arduino/lib/
#cp work/lib/core.jar arduino/lib/
#cp -r work/lib/export arduino/lib/
#rm -rf arduino/lib/export/CVS
 
# get jikes and depedencies
#gunzip < dist/jikes.gz > arduino/jikes.exe
cp dist/jikes.exe arduino/
chmod +x arduino/jikes.exe
 
#cp dist/ICE_JNIRegistry.dll arduino/
#chmod +x arduino/ICE_JNIRegistry.dll
#cp work/rxtxSerial.dll arduino/
#chmod +x arduino/rxtxSerial.dll
cp work/*.dll arduino
chmod +x arduino/*.dll
 
# get platform-specific goodies from the dist dir
cp launcher/Arduino.exe arduino/
cp dist/run.bat arduino/
chmod +x arduino/run.bat
 
# convert notes.txt to windows LFs
# the 2> is because the app is a little chatty
unix2dos arduino/revisions.txt 2> /dev/null
unix2dos arduino/lib/preferences.txt 2> /dev/null
unix2dos arduino/lib/keywords.txt 2> /dev/null
rm -f arduino/*.bak
rm -f arduino/lib/*.bak
 
# remove boogers
find arduino -name "*~" -exec rm -f {} ';'
find arduino -name ".DS_Store" -exec rm -f {} ';'
find arduino -name "._*" -exec rm -f {} ';'
find arduino -name "Thumbs.db" -exec rm -f {} ';'
 
# chmod +x the crew
find arduino -name "*.dll" -exec chmod +x {} ';'
find arduino -name "*.exe" -exec chmod +x {} ';'
find arduino -name "*.html" -exec chmod +x {} ';'
 
# clean out the svn entries
find arduino -name ".svn" -exec rm -rf {} ';' 2> /dev/null
 
# zip it all up for release
echo Packaging standard release...
echo

##################################### 
#Package the build
#<CML> renamed the package to Antipasto

echo Extracting packager tool...
unzip packager.zip

#define the package
PACKAGER="AdvancedInstaller.com"                #the packager
PACKAGE_DIST="Arduino"                          #The distribution
PACKAGE_NAME="Arduino"                          #the name of the App
PACKAGE_FOLDER=package                          #dest for our created package
PACKAGE_PROJ=$PACKAGE_FOLDER/$PACKAGE_NAME'_'$REVISION.aip   #packager project file
PACKAGE_DIST_DIR=$PACKAGE_FOLDER/$PACKAGE_DIST

#cleanup package directory
if test -d package
then
  rm -rf package/*
else
 mkdir $PACKAGE_FOLDER
fi

#build the package
mv arduino $PACKAGE_NAME                      #rename
mkdir $PACKAGE_DIST_DIR                       #create distribution dir
mv $PACKAGE_NAME $PACKAGE_DIST_DIR            #move directory
cp Antipasto-package.aip $PACKAGE_PROJ        #copy the packager project file

./packager/$PACKAGER /edit $PACKAGE_PROJ /AddFolder ProgramFilesFolder $PACKAGE_FOLDER'\'$PACKAGE_DIST
./packager/$PACKAGER /edit $PACKAGE_PROJ /SetVersion $REVISION
./packager/$PACKAGER /edit $PACKAGE_PROJ /NewShortcut -name $PACKAGE_NAME -target 'ProgramFilesFolder\'$PACKAGE_DIST'\'$PACKAGE_NAME'\'$PACKAGE_NAME'.exe' -dir 'ProgramMenuFolder\'$PACKAGE_DIST
./packager/$PACKAGER /build $PACKAGE_PROJ

echo Cleaning packager
rm -rf packager
 
echo Cleaning work directory
rm -rf work
 
echo Done.