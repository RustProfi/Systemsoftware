BASEDIR=$(dirname "$0")
cd $BASEDIR/hw6

clang++  -std=gnu++11 \
-pthread \
`find "./lib/oatpp/" -type f -name *.cpp` \
`find "./src/" -type f -name *.cpp` \
-I "./lib" \
-I "./src" \
-D OATPP_USE_TARGET \
-D OATPP_TARGET_APP \
-D OATPP_DISABLE_ENV_OBJECT_COUNTERS \
-O2 \
-Wall \
-o initrd/bin/complexserver

chmod +x initrd/bin/complexserver

cd ..
