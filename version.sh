#!/bin/bash

# Full version has format "A.B.C.D", where A and B are the major and
# minor.  When built, the build number adds an additional two
# integers, "E.F".  C, D, E, and F are maintained as 0 here in the
# repository and are set by release engineering when making an
# official build.

option=$1

# Edit major/minor version here
VERSION_MAJOR_MINOR=1.2

# Release engineering will overwrite this entire file when it does a
# build, do we don't even provide a way to pass in new C, D, E, or F
# values on the command line/environment.
CD_VALUE="0.0"
EF_VALUE="0.0"

VERSION="${VERSION_MAJOR_MINOR}.${CD_VALUE}"
FULL_VERSION="${VERSION}-${EF_VALUE}"

case "$option" in
	--full)
		echo $FULL_VERSION
		;;
	--version)
		echo $VERSION
		;;
	--build-id)
		echo $EF_VALUE
		;;
	*)
		echo "Usage: $0 {--full|version|build-id}"
		exit 1
esac
