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

set(deps "")
COMPILELIB(deps)

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

### Exercise 5 Modify the code and have some change!

It's time to add real changes to our code. We start by making the process printing something on the screen. 
Insert the code inside `TRestCoffeeProcess::ProcessEvent()` method like following:

```c++
TRestEvent* TRestCoffeeProcess::ProcessEvent(TRestEvent * evInput) {
    fAnaEvent = (TRestRawSignalEvent*)evInput;
    
    std::cout << "Event ID: " << fAnaEvent->GetID()
              << ", Number Of Signals: " << fAnaEvent->GetNumberOfSignals() << std::endl;

    return fAnaEvent;
}
```

Then back to the build directory, type `make install` to rebuild the project(in principle only the modified process needs to be compiled). 

In the terminal of rest-school, you can directly re-call the previous `reatManager` command. You will be able to see various messages shown on the screen.


### Exercise 6 Add data members in metadata

We shall now make `TRestCoffee` a real *coffee*. We start by adding related data members.

```c++
class TRestCoffee : public TRestMetadata {
private:
    Double_t fCupSize = 0;  //
    Double_t fInitialWater = 0;  //
    Double_t fInitialSugar = 0;  //

    void Initialize() override;
public:
    Double_t GetCupSize() { return fCupSize;}
    Double_t GetInitialWater() { return fInitialWater; }
    Double_t GetInitialSugar() { return fInitialSugar; }
    
    ...
};
```

We also implement its `PrintMetadata()` method:

```c++
void TRestCoffee::PrintMetadata() {
    TRestMetadata::PrintMetadata();

    RESTMetadata << " - Cup Size (kg): " << fCupSize << RESTendl;
    RESTMetadata << " - Initial Water amount(kg): " << fInitialWater << RESTendl;
    RESTMetadata << " - Initial Sugar amount(kg): " << fInitialSugar << RESTendl;
}
```

These data members are correlated with rml parameters with certain naming convention. We update the rml like:

```xml
<TRestCoffee name="Coffee" >
    <parameter name="cupSize" value="300g"/>
    <parameter name="initialWater" value="100g"/>
    <parameter name="initialSugar" value="0g"/>
</TRestCoffee>
```

### Exercise 7 Access metadata from the process

Our `TRestCoffeeProcess` is going to make a perfert coffee by adding water and sugar into it. Include the header first:

```c++
#include "TRestCoffee.h"
```

Define the data members, including max water, amount of water/sugar to add, and current amount of water/sugar inside the cup:

```c++
class TRestCoffeeProcess : public TRestEventProcess {
private:
    TRestRawSignalEvent* fAnaEvent;  //!
    void Initialize() override;
    Bool_t fSuccessfull = false;
    Double_t fMaxWater = 0;  
    Double_t fWaterToAdd = 0;   
    Double_t fSugarToAdd = 0;    
    Double_t fWater = 0;     //!
    Double_t fSugar = 0;     //!
public:
...
};
```

**Note:** Since the amount of water/sugar is kind of temporary variable during execution, there is no need to keep them as *metadata*. 
We can add `//!` annotation to prevent them being stored.

Before starting, we need to know the current status of our coffee. Write in its `InitProcess()` method:

```c++
TRestCoffee* coffee = GetMetadata<TRestCoffee>();
fMaxWater = coffee->GetCupSize();
fWater = coffee->GetInitialWater();
fSugar = coffee->GetInitialSugar();
```

Now, write in the `ProcessEvent()` method to make a coffee! Assume we add 1 gram of water and 0.02 grams of sugar each time. 
When it comes to the best mixture(1%), we stop the process. If the Water 


```c++
TRestEvent* TRestCoffeeProcess::ProcessEvent(TRestEvent* evInput) {
    fAnaEvent = (TRestRawSignalEvent*)evInput;

    if (fWater >= fMaxWater) {
        std::cout << "Water is full!" << std::endl;
    } else if (!fSuccessfull) {
        fWater += fWaterToAdd;
        fSugar += fSugarToAdd;

        if (fWater / fSugar < 100) {
            std::cout << "Made a perfect cup of coffee!" << std::endl;
            fSuccessfull = true;
        }
    }

    return fAnaEvent;
}
```

We can recompile the code now. Also don't forget to modify the rml:

```xml
<addProcess type="TRestCoffeeProcess" name="cfp" value="ON" verboseLevel="silent" waterToAdd="1g" sugarToAdd="0.02g"/>
```

Run the process and you can see it is successful.

### Exercise 8 Generate observables from the process

We can also record the water/sugar ratio at each step. Just add simply one line to the `ProcessEvent()` method:

```c++
SetObservableValue("WaterToSugarRatio", fWater / fSugar);
```

Recompile the code and you will see the difference.






