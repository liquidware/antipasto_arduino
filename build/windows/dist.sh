#!/bin/sh

REVISION=`head -c 6 ../../todo.txt`
 
# check to see if the version number in the app is correct
# so that mikkel doesn't kick my ass
VERSIONED=`cat ../../processing/app/Base.java | grep $REVISION`
if [ -z "$VERSIONED" ]
then
echo Fix the revision number in Base.java
  exit
fi

time1=`date +%s`

echo Creating distribution for revision $REVISION on `date`...
echo

##################################################
#Package the build
#<CML> renamed the package to Antipasto

#define the package variables
PACKAGING_BASE_DIR=/cygdrive/c/TEMP
PACKAGER_DIR=packager
PACKAGER="AdvancedInstaller.com"                #the packager
PACKAGE_DIST="Arduino"                          #The distribution
PACKAGE_NAME="Arduino"                          #the name of the App
PACKAGE_FOLDER=package                          #dest for our created package
PACKAGE_PROJ=package/$PACKAGE_NAME'_'$REVISION.aip   #packager project file
PACKAGE_DIST_DIR=$PACKAGE_FOLDER

##################################################
#Clean package
echo Cleaning...

rm -rf Arduino                                  # Remove existing packages

if test -d $PACKAGING_BASE_DIR/$PACKAGE_FOLDER  # Remove Temp directory
then
  echo Cleaning $PACKAGING_BASE_DIR/$PACKAGE_FOLDER ...
  rm -rf $PACKAGING_BASE_DIR/$PACKAGE_FOLDER
fi

if test -d $PACKAGING_BASE_DIR/$PACKAGER_DIR    # Remove packager
then
  echo Cleaning $PACKAGING_BASE_DIR/$PACKAGER_DIR ...
  rm -rf $PACKAGING_BASE_DIR/$PACKAGER_DIR
fi


##################################################
#Preprocessing
echo Preprocessing folders...
echo Copying work directory to $PACKAGE_NAME
cp -r work ./$PACKAGE_NAME                                       # Make a copy of the work dir
mkdir $PACKAGING_BASE_DIR/$PACKAGE_FOLDER                        #create package dir
mkdir $PACKAGING_BASE_DIR/$PACKAGE_DIST_DIR                      #create distribution dir
mv $PACKAGE_NAME $PACKAGING_BASE_DIR/$PACKAGE_DIST_DIR           #move directory
cp Antipasto-package.aip $PACKAGING_BASE_DIR/$PACKAGE_PROJ       #copy the packager project file

echo Extracting packager tool...
unzip -d $PACKAGING_BASE_DIR packager.zip

##################################################
# Go into the base directory because
# we have long file names in our package. Yuck.
pushd $PACKAGING_BASE_DIR

##################################################
# Let's pacakge already
./$PACKAGER_DIR/$PACKAGER /edit $PACKAGE_PROJ /AddFolder APPDIR $PACKAGE_FOLDER'\'$PACKAGE_DIST
./$PACKAGER_DIR/$PACKAGER /edit $PACKAGE_PROJ /SetVersion $REVISION
./$PACKAGER_DIR/$PACKAGER /edit $PACKAGE_PROJ /NewShortcut -name $PACKAGE_NAME -target 'APPDIR\'$PACKAGE_DIST'\'$PACKAGE_NAME'.exe' -dir 'ProgramMenuFolder\'$PACKAGE_DIST
./$PACKAGER_DIR/$PACKAGER /build $PACKAGE_PROJ

popd

#Copy the package from the temp directory
cp $PACKAGING_BASE_DIR/package/$PACKAGE_NAME'_'$REVISION.msi .
 
 
 
 
time2=`date +%s`
timediff=`expr $time2 - $time1`

echo 
echo Output files for $REVISION build:
echo $PACKAGE_NAME'_'$REVISION.msi
echo
echo Finished in `expr $timediff / 60` min `expr $timediff % 60` sec
echo Done.