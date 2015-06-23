#!/bin/sh -e

AUTO_HELL_FILES="TODO ChangeLog AUTHORS README NEWS INSTALL"

touch $AUTO_HELL_FILES

AUTORECONF=${AUTORECONF:-autoreconf}

${AUTORECONF} -f -i
( cd filters/SVGFilter ; ${AUTORECONF} -f -i )

rm -f $AUTO_HELL_FILES
