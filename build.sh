
# argument new
# rm -rf build bin && mkdir build

cd build
cmake ..
cmake --build .
make

cd ..

./bin/c_graphics_learn-/c_graphics_learn