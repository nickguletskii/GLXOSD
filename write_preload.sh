#! /bin/sh
OUT_FILE="glxosd"
echo "#! /bin/sh" > ${OUT_FILE}
echo "INJ_PATH="$PWD >> ${OUT_FILE}
echo "LD_LIBRARY_PATH=\$INJ_PATH/bin:\$LD_LIBRARY_PATH LD_PRELOAD=libelfhacks.so:libglinject.so:libglxosd.so:libftgl.so:\$LD_PRELOAD \$@" >> ${OUT_FILE}
chmod +x ${OUT_FILE}