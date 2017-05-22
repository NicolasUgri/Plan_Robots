g++ -c `pkg-config --cflags playerc++`  createMap.cc `pkg-config --libs playerc++`
g++ -c `pkg-config --cflags playerc++`  brushfireMap.cc `pkg-config --libs playerc++`
g++ -c `pkg-config --cflags playerc++`  distanceMap.cc `pkg-config --libs playerc++`
g++ -c `pkg-config --cflags playerc++`  potentialMap.cc `pkg-config --libs playerc++`
g++ -o runBrushfire `pkg-config --cflags playerc++` runBrushfire.cc createMap.o brushfireMap.o distanceMap.o potentialMap.o `pkg-config --libs playerc++`
