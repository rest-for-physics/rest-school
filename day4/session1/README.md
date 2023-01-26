You will need to move to the directory day4/session2 to match the proposed commands with the relative paths given in the examples.

### Exercise 1. Creating a dummy library with a metadata and a process

We are now going to create a dummy library that will contain a metadata and a process. As shown in the slides, this library will be at the same level as REST official libraries.

First we create a new directory inside the framework libraries, and put a CMake file in it. You might search for inspiration or even reuse any `CMakeLists.txt` from an existing library.

```
cd framework/source/libraries
mkdir coffee
cd coffee
```

#### Exercise 1.1 Creating the CMakeLists.txt

We create a new file `CMakeLists.txt` with the following contents:

```
set(LibraryVersion "1.0")
add_definitions(-DLIBRARY_VERSION="${LibraryVersion}")

COMPILELIB("")

file(GLOB_RECURSE MAC "${CMAKE_CURRENT_SOURCE_DIR}/examples/*")
INSTALL(FILES ${MAC} DESTINATION ./examples/wimp)

ADD_LIBRARY_TEST()
```

Most of those definitions are mandatory in a REST-for-Physics library, where we have predefined all the necessary compilation instructions inside `COMPILELIB` and `ADD_LIBRARY_TEST` for integration with the framework standards.

**NOTE:** The library version is a value that it is registered at any metadata class in your new library, and it needs to be defined inside the `Initialize` method of your new class.

On top of that we have added an additional `examples` directory for adding a dummy configuration file.

#### Exercise 1.2 Creating the source metadata and process classes

We have created two framework macros that will assist you with the generation of an skeleton metadata and process class, and that it will provide guidelines on documentation. There are few code comments that need to be reviewed and removed afterwards!

Just generate now them using those macros:

```
restRootMacros
root [0] REST_MakeMetadata("TRestCoffee");
root [1] REST_MakeProcess("TRestCoffeeProcess");
```

This will create the source files `.cxx` and header files `.h` in the root directory. Simply now create the source/header directory structure and move the files there.

```
mkdir src
mv *.cxx src
mkdir inc
mv *.h inc
```

Our coffe library is ready for compilation!

**NOTE:** The `REST_MakeProcess` macro allows two additional arguments that allow you to define the input and output event types. However, our library does not define yet an event type, and thus it will just be a non specialed `TRestEvent`. Therefore, it will be able to connect at any point of the event data processing chain in between any library processes.

#### Exercise 1.3 Compiling our new library

Now go to the framework build directory, and check the new library is recognized by the framework.

```
cd ~/framework/build
cmake ..
```

You should see the new library between the list of libraries to be compiled:

```
REST will be installed in: /home/jgalan/rest-framework/install
===========================================================
System is: Linux
Packages: restG4 
Libraries: RestFramework;RestAxion;RestCoffee;RestConnectors;RestDetector;RestGeant4;RestLegacy;RestRaw;RestTrack;RestWimp
```

You might even disable all libraries and enable only your own library, this will make compilation faster while developing your library.

```
cmake -DREST_ALL_LIBS=OFF -DRESTLIB_COFFEE=ON ..
```

Still for completing the exercise you will need the rawlib and detectorlib compiled. So just add them now.

```
cmake -DRESTLIB_DETECTOR=ON -DRESTLIB_RAW=ON ..
```

Now compile and install the libraries

```
make -j install
```

and test everything is recognized as expected inside a `restRoot` session.

#### Exercise 1.4 Modifying our new metadata and process


### Final exercise. Exam for getting an official REST-for-Physics certificate!

Suppose now we are going to establish a set of analysis schema for a new experiment. 
To simplify, we assume the detector only contains one readout module with one readout channel. Like an HPGe detector. The daq system triggers
at each event, and records one single waveform(TRestRawSignal) within 512 sampling window. There could be multiple pulses inside one window.

The analysis for such detector needs: 
* A new event class containing energy(s), rise time(s), triggered time(s), pulse FWHM(s) and pulse width 1/10 height(s).
* A new process to convert TRestRawSignalEvent to our new event type, and extract observables like baseline noise level, first trigger position, etc.

For the exercise we suggest you to write new c++ codes of you own. Don't use the existing methods like `GetPointsOverThreshold()` from TRestRawSignal.

Here are the examples:




