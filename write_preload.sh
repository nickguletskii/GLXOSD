#! /bin/sh
OUT_FILE=$1/glxosd
echo "#! /bin/sh" > ${OUT_FILE}
echo "INJ_PATH="$2 >> ${OUT_FILE}
echo "LD_LIBRARY_PATH=\$INJ_PATH/lib/glxosd:\$LD_LIBRARY_PATH LD_PRELOAD=libglxosd-elfhacks.so:libglxosd-glinject.so:libglxosd.so:libglxosd-ftgl.so:\$LD_PRELOAD \$@" >> ${OUT_FILE}
chmod +x ${OUT_FILE}
