### Exercise 1. Creating your own ROOT class

We will compile a very simple class with few data members that will allow us to test the ROOT I/O features. First we will create the class source and header files.

We will create a new working directory for this exercise

```
mkdir dummy
cd dummy
```

The header file named `Dummy.h` will contain three data members, where one of them `fTempValue` will be ignored from the I/O streamer. The class will also define getter and setter methods, and it will prototype a `Print` method to print the values stored inside the class.

```
#include <iostream>
#include "TObject.h"
class Dummy : public TObject {
   private:
      Float_t   fX = 0;         //< x position in centimeters
      Float_t   fY = 0;         //< y position in centimeters
      Int_t     fTempValue = 10; //! temporary state value
   public:
      Dummy()             { fX = fY = -1; }
      void Print() const;
      void SetX(float x) { fX = x; }
      void SetY(float y) { fY = y; }
      void SetTempValue(int v) { fTempValue = v; }
      Float_t GetX(float x) { retunr fX; }
      Float_t GetY(float y) { return fY; }
      Int_t GetTempValue(int v) { return fTempValue; }

ClassDef(Dummy, 1)
};
```

Inside the source file named `Dummy.cxx` we include the header file and implement the `Print` method.

```
#include "Dummy.h"
ClassImp(Dummy);       
void Dummy::Print() const {                                             
        std::cout << "fX = " << fX << ", fY = " << fY << std::endl;
		std::cout << "The temporal value is : " << fTempValue << std::endl;} 
```

We should remark now the 3 elements that identify the complete ROOT-ification of our class. The class is inheriting from `TObject`, and the class implements the `ClassImp(Dummy)` and `ClassDef(Dummy, 1)` that extends the class with additional methods that must be defined by any ROOT class.

#### Exercise 2. Compiling your project

In order to create our own compiled library and executable we will build a project using the `cmake` tool. `cmake` is an scripting language allowing to define compilation rules and create advanced configuration environment for setting up our project.

We will create a new file inside named `CMakeLists.txt` and we will add few lines to setup our basic project.

The first thing we need to add to our project definition is its name and the minimum `cmake` version required. `cmake` is an active evolving project that implements new features as new versions come out, also some of the predefined `cmake` functions might become obsolete.

Add the following lines to the file to force the project to be configured with at least cmake 3.11, and name the project with an arbitrary name, such as `dummy`:

```
cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(dummy)
```

Then we need to make sure that our project setup will check that the ROOT libraries are installed and accessible in the system where we want to compile our project.

```
find_package(ROOT REQUIRED COMPONENTS Eve)
```

The following line will add new paths where our project header files should be found. In our case we will just add the root directory since our simple project will just have one single header file at the projects root.

```
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
```

On top of that, to profit from all the advantages of automatic schema evolution in ROOT we will need to generate a ROOT dictionary for our class, we can do this invoking a ROOT cmake function `ROOT_GENERATE_DICTIONARY`,

```
ROOT_GENERATE_DICTIONARY(DummyDict
   Dummy.h
   LINKDEF Dummy_LinkDef.h)
```

which needs that we create an additional file to let ROOT know which class elements we want to be added to the class I/O streamer. We must create the file `Dummy_LinkDef.h` with the following contents.

```
#ifdef __CINT__
#pragma link C++ class Dummy+;
#endif
```

Classes are compiled into libraries. A library is just a collection of compiled classes placed together. We then need to define inside our `CMakeLists.txt` that we want to generate a library where we add all the classes we want to be present in our library. In our case we just got one class `Dummy.cxx` so we just add one target to be compiled within the final library file `libDummy.so`.

```
add_library(Dummy SHARED Dummy.cxx)
target_link_libraries(Dummy PUBLIC ROOT::RIO ROOT::Net)
```

We then need to add any dependencies required to build the `Dummy` compiled class. This line will trigger the generation of the dictionary defined at `ROOT_GENERATE_DICTIONARY` 

```
target_sources(Dummy PRIVATE DummyDict.cxx)
```

We got already all the instructions required to compile our library (named `libDummy.so`, `libDummy.dylib` or `libDummy.dll` depending on the architecture of our OS) which will be ready to be integrated within ROOT.

Now we can now test the compilation of our project. First we use cmake to create the compilation environment.

```
mkdir build
cmake ..
```

On sucess, we will be able to compile our library using

```
make
```

### Exercise 3. Installing the library and testing 

Once compilation has suceeded, and before testing our new library, we must define few installation instructions that will help cmake to decide where the final compiled binaries should be placed. This can be done by adding the following lines to our `CMakeLists.txt`.

```
install (TARGETS Dummy Main
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib
         RUNTIME DESTINATION bin)
```

In addition to the targets that we asked `cmake` to generate, ROOT generates an additional `.pcm` file needs to be placed at the same location as the target library. Thus, we will also need to add the following line

```
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDummyDict_rdict.pcm DESTINATION lib)
```

When we modify our cmake files, or when we want to change some of the cmake definitions (such as `CMAKE_INSTALL_PREFIX`) we will need to re-run cmake. We execute `cmake` now specifying the directory where we want the binaries to be installed.

```
cmake -DCMAKE_INSTALL_PREFIX=../install ..
```

And add to the `make` command the `install` argument so that after the compilation it proceeds to copy the files into te final location.

```
make install
```

If the library and binary were not installed at a system location, such as `/usr/local/bin` and `/usr/local/lib`, we will need to let the system know that a new location for libraries and executables is available to the users. We need to update two environment variables, `PATH` and `LD_LIBRARY_PATH` that allow to define where binary executables and libraries are located, respectively.

We need to execute the following commands in our bash shell.

```
export PATH=$PATH:/path/to/our/installed/binary/dummy/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/our/installed/library/dummy/lib
```

where you need to substitute for the proper path where the executable and library are found inside the library.

**NOTE**: Make sure you add the `$PATH` and `$LD_LIBRARY_PATH` to the new definition so that previous paths are also included in the new defintion, and are not overwritten.

Those exported variables will be only valid at your present terminal session, and you will need to add them to your `.bashrc`, `.bash_profile` or similar in order to make them permanent.

Finally, we are ready to test our class inside ROOT!

```
root
root [0] gSystem->Load("libDummy.so");
root [1] Dummy dummy;
root [2] dummy.SetX(TMath::Exp(1));
root [3] dummy.SetY(TMath::Power(TMath::Pi()) )
root [4] dummy.SetTempValue( 137 )
root [5] dummy.Print()
root [6] TFile *f = TFile::Open("output_shell_V1.root");
root [7] dummy.Write("SavedDummy")
root [9] f->Close()
```

### Exercise 4. Adding an executable to your project.

Optionally we can also add an executable program, that we will name `MainDummy`, and that makes use of the class found at the Dummy library.

```
add_executable(MainDummy MainDummy.cxx)
target_link_libraries(MainDummy Dummy)
```

We can write an executable inspired in our latest tests in the ROOT interactive shell.

```
#include "TFile.h"
#include "Dummy.h"

int main()
{
    Dummy myClass;

    std::cout << "Just created an instance of the class" << std::endl;
    myClass.Print();

    myClass.SetX(25);
    myClass.SetY(30);

    myClass.SetY(30);

    std::cout << "The class data members have been updated" << std::endl;
    myClass.Print();

    std::cout << "Saving to disk on output.root" << std::endl;
    TFile *f = TFile::Open("output_exec_V1.root", "RECREATE");
    myClass.Write("SavedData");

    f->Close();
}
```

We re-launch `cmake` and `make` compilation 

```
cmake ..
make -j install
```

and check that the binary is working properly.

```
MainDummy
```

which should execute as soon as the binary can be found by the `PATH` system variable, and the library can be found at `LD_LIBRARY_PATH`.

### Exercise 5. Reading back the saved file contents

You may now try to recover the contents saved on the files generated by the ROOT shell commands and the executable.

```
root
root [0] gSystem->Load("libDummy.so");
root [1] TFile *f1 = TFile::Open("output_shell_V1.root");
root [2] Dummy *d1 = (Dummy *) f1->Get("SavedData");
root [3] d1->Print();
root [4] TFile *f2 = TFile::Open("output_shell_V1.root");
root [5] Dummy *d2 = (Dummy *) f1->Get("SavedData");
root [6] d2->Print();
```

What is the value of the `fTempValue` recovered? Why?

### Exercise 6. Updating the data members and testing the ROOT automatic schema evolution

We will now test the schema evolution, i.e. we will modify the data members from our class, and try to read back the contents of the file that was generated with a previous version.

Inside the header `Dummy.h` we write now a new member just after the other data members,

```
Float_t   fZ = 0;
```

and re-compile the library from the build directory we execute. 

```
cmake ..
make -j install
```

Now when we try to read the old file with the new class version we should get a warning.

```
root [0] gSystem->Load("libDummy.so")
root [1] TFile *f = TFile::Open("output_shell_V1.root")
Warning in <TStreamerInfo::BuildCheck>: 
   The StreamerInfo of class Dummy read from file output.root
   has the same version (=1) as the active class but a different checksum.
   You should update the version to ClassDef(Dummy,2).
   Do not try to write objects with the current class definition,
   the files will not be readable.

Warning in <TStreamerInfo::CompareContent>: The following data member of
the in-memory layout version 1 of class 'Dummy' is missing from 
the on-file layout version 1:
   float fZ; //
```

ROOT is preventing you that it identified changes on the new library with respect to the data members found on the ROOT file written to disk. This means that if you write now using the new library definition, you will have files stamped with the same class version but with different data members!

You must update the class version using the `ClassDef` arguments. Simply update the corresponding `Dummy.h` header line to its version 2,

```
ClassDef( Dummy, 2)
```

recompile, and check that reading the old file does not produce a warning anymore!
