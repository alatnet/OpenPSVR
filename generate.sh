#!/bin/bash
set -eE
###########################################################################
# Author: Stephen O'Hair                                                                    
# Purpose: Generates the openpsvr driver make project files for use on linux.
###########################################################################

# Initialise
PROJECT_ROOT=$PWD

#Function loads configuration properties and generates project files
main(){
	# Generate the project files for PSMoveService
	generateProjectFiles || return $?

	# Exit batch script successfully
    echo -e "\E[1;32mBUILD SUCCESS\E[;0m";
	exit 0
}

#Function generates project files for the configured ide
generateProjectFiles(){
	if [ ! -d "$PROJECT_ROOT/generated" ]; then
		mkdir $PROJECT_ROOT/generated
	fi
	echo "Generating OpenPSVR Project files..."
	echo "Running cmake in $PROJECT_ROOT"
	cd $PROJECT_ROOT/generated
	cmake .. -G "Unix Makefiles"
}

#Function to handle errors
function handleError() {
   error_msg="GENERATE FAILED"
   echo -e "\E[1;31m$error_msg\E[;0m";
   exit 1;
}


# this will trap any errors or commands with non-zero exit status
# by calling function catch_errors()
trap handleError ERR;

# entry point of script
main
