# In Memory File Cache Design

An arbitrary implementation of the LRU cache with write, read and dirty cache update all in constant time.

## Dependencies

This code depends on the `gtests`. Simple installation instruction can be found [here](https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/).

This code also uses cmake to build. 

## Build and run

To Build, go to the source folder and create a build directory:
```
cd <SOURCE_FOLDER>
mkdir build
```
Then use cmake to configure and build:
```
cmake ..
cmake --build .
```
The executable is produced with the name `app` and takes exactly 4 extra arguments(as stated in the assignment). To finally run, you'll need all the necessary files as stated in the assignment:
```
./app <cache_size> <readers> <writers> <items>
```

## Code

The code basically has 3 major files:

* Cppcache.hpp : This is the main LRU cache file which holds the templated class of CPPcache all the  source code is inside a single header file because, the template definitions need to be availaible at runtime. 

* fileIO.cpp : This file contains methods which help in abstracting the IO operation to/from files on disk. This took me a loooong time, turns out there is no efficient way to insert a particular line of an arbitrary size into a file at any index, without corrupting it's original data. Well actually there might be, but it seemed too complex to implement. 

* main.cpp : The main function and the driver code, it takes in all the arguments, and spawns multiple readers and writers, which read and write from disk/cache.

I've used RAII lock_gaurd for any modification operation on disk/cache which could've been concurrent and not thread safe, since these are better than just mutexes/locks.

The complexity is constant time for each operation, even the dirty cache update, but due to locks(which were necessary due to threads), the performance might be hindered a bit.

## Tests

I didn't have a lot of time remaining to finish up a lot of unit tests, sorry about that. 
I do have some basic unit tests, I tried to configure more cases but not enough time.
To run the tests after the cmake build, just run:
```
make test
```
If you want to run test individually, then you can also go to the `tests` folder inside the `build` folder and run:
```
./app_cppcache_test
```
or for the fileIO tests : 
```
./app_fileio_test 
```

#### That's all folks
