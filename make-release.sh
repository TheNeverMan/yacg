#!/bin/bash

if(($#!=1));
then
	echo "No Version Has Been Provided!";
	exit;
fi

echo "Version $1";
echo "Copying files..."

mkdir -p ../yacg-releases/windows-$1
mkdir -p ../yacg-releases/linux-$1

cp -r ./assets/ ../yacg-releases/windows-$1/
cp -r ./assets/ ../yacg-releases/linux-$1/

cp -r ./maps/ ../yacg-releases/windows-$1/
cp -r ./maps/ ../yacg-releases/linux-$1/

mkdir -p ../yacg-releases/windows-$1/saves
mkdir -p ../yacg-releases/linux-$1/saves

cp -r ./yacg.exe ../yacg-releases/windows-$1/
cp -r ./yacg ../yacg-releases/linux-$1/

cp -r ./font-install.bat ../yacg-releases/windows-$1/
cp -r ./font-install.bat ../yacg-releases/linux-$1/

echo "Creating installers..."
cd ../yacg-releases
./create-installers.sh $1
