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

### Exercise 2 Creating the source metadata and process classes

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

### Exercise 3 Compiling our new library

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


### Exercise 4 Tryout the newly added metadata and process

Now it's time to add our new metadata and process to the process chain. You will need day3's rml file as a starting point.

Copy it with:

```
cp ../../day3/session1/cast.rml testCoffee.rml
```

Then open the file with vim, insert:

```xml
<TRestCoffee name="Coffee" >
    <parameter name="CupSize" value="2" />
</TRestCoffee>
```

under `TRestRun` section, and insert:

```xml
<addProcess type="TRestCoffeeProcess" name="cfp" value="ON" verboseLevel="silent"/>
```

after `TRestRawMultiFEMINOSToSignalProcess`. Now you will have the new process chain modified. Run the new process by:

```
restManager --c testCoffee.rml --f ../../data/R10513_Calibration_15min_Vm_340_Vd_113_Pr_1.4_Gain_0x1_Shape_0xD_Clock_0x02-000.aqs
```

Nothing will change during execution. But when you open the output file with restRoot, you can see the two new metadata objects contained.

```
root [1] .ls
TFile**         data/R10513_00000_BasicRaw_Calibration_15min_nkx_2.3.15.root
 TFile*         data/R10513_00000_BasicRaw_Calibration_15min_nkx_2.3.15.root
  OBJ: TTree    EventTree       REST Event Tree : 0 at: 0x7fffe5d91890
  OBJ: TRestAnalysisTree        AnalysisTree    REST Process Analysis Tree : 0 at: 0x7fffe62f6640
  OBJ: TH1D     daqChannelActivityRaw   daqChannelActivityRaw : 0 at: 0x7fffe3c9d270
  KEY: TTree    EventTree;1     REST Event Tree
  KEY: TRestAnalysisTree        AnalysisTree;1  REST Process Analysis Tree
  KEY: TH1D     daqChannelActivityRaw;1 daqChannelActivityRaw
  KEY: TRestDetector    detector;1      Default TRestDetector
  KEY: TRestCoffee      Coffee;1        Default TRestCoffee
  KEY: TRestRun CAST-Ar;1       CAST Ar data taking
  KEY: TRestProcessRunner       RawSignals;1    Raw processing and analysis
  KEY: TRestRawMultiFEMINOSToSignalProcess      virtualDAQ;1    Default TRestRawMultiFEMINOSToSignalProcess
  KEY: TRestCoffeeProcess       cfp;1   Default TRestCoffeeProcess
  KEY: TRestRawSignalChannelActivityProcess     rawChActivity;1 Default TRestRawSignalChannelActivityProcess
  KEY: TRestRawVetoAnalysisProcess      veto;1  Default TRestRawVetoAnalysisProcess
  KEY: TRestRawSignalAnalysisProcess    sAna;1
  KEY: TRestEventRateAnalysisProcess    rateAna;1       Default TRestEventRateAnalysisProcess
```

### Exercise 5 Modifying the code and have some change!

We can add some code to 







