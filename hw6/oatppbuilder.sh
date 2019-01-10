clang++ -march=armv8-a -target aarch64-linux-gnu \
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
-static -o initrd/bin/run_app

chmod +x initrd/bin/run_app
