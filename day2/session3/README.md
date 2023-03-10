You will need to move to the directory `day2/session3` to match the proposed commands with the relative paths given in the examples.

As you have learnt during the corresponding session lecture, we can use the REST-for-Physics libraries in different environments, including the `restRoot` ROOT based interactive shell, `C-macros` executed by the ROOT interface interpreter, C++ compiled executables, or `python3` scripts.

In this session we will learn how to access REST-for-Physics generated files using `restRoot` prompt(which is plain C++) or `python3`. The methods calls are similar. And the choice of which language to use is up to you. During the exercises you may chose to write the commands inside a file and execute the file, or write them directly inside the interactive session. If you use `restRoot` to execute commands as a file, you may need to add additional function definitions.

### Required python headers

When we write our python script we need to import the ROOT libraries, and any REST library we used to generate or process our files. This is done automatically by the `import REST` command inside python.

In order to use REST libraries in python the best option is to import both ROOT and REST libraries using the `import` python command:

```python
#!/usr/bin/python3

import ROOT
import REST
```

### Exercise 1. Opening a ROOT file processed with REST

The main class for event I/O and metadata access in REST is [TRestRun](https://sultan.unizar.es/rest/classTRestRun.html). TRestRun defines few helper methods that centralize the access to the event data, the analysis tree and the metadata objects stored inside the file. `TRestRun` can be used to open a ROOT file generated with REST, and access the data in a coherent way.

The following example shows how to create a `TRestRun` instance , named *rn* in this script, print the run metadata information, and get the number of event entries stored inside the file.

python:
```python
# We open a ROOT file generated by REST
rn = ROOT.TRestRun("../../data/R11567_00001_RawToTrack_Background_21hr_jgalan_2.3.15.root" )

# We print the information of the run metadata object
rn.PrintMetadata()

# We get the number of entries
nEntries =  rn.GetEntries()
print ( "The number of entries is : " + str( nEntries ) )
```

C++:
```c++
TRestRun* rn = new TRestRun("../../data/R11567_00001_RawToTrack_Background_21hr_jgalan_2.3.15.root" );
rn->PrintMetadata();
int nEntries =  rn->GetEntries();
cout << "The number of entries is : " <<  nEntries << endl;
```

There are also non-REST objects stored in the file. We can see by typing `.ls` in ROOT prompt and there are histograms shown also. Try to plot them.

### Exercise 2. Accessing the stored metadata objects using TRestRun

The following example uses the previous existing *rn* instance to retrieve a list of metadata objects found inside the file, prints the list with the metadata name together with its specific metadata class name, and calls the `PrintMetadata` method (present at any metadata class) to print information regarding one of the metadata objects in the list.

```python
# We recover the metadata object names that the user defined at the RML configuration file
mdNames = rn.GetMetadataNames()

# We iterate over the list and print the name together with the object class name
print("\n")
print ("The following metadata objects are found inside the file")
print ("--------------------------------------------------------")
for md in mdNames:
    print ( md + " is: " + rn.GetMetadata( md ).ClassName() )
print("\n") 

# We print the metadata information from the second element in the list
print ("We print the metadata information of the second element in the list:" )
print ("--------------------------------------------------------------------")
rn.GetMetadata( mdNames[1] ).PrintMetadata() 
```


We may now use any method defined at the recovered metadata instance.

You can alternativelly open the root file with restRoot through command line: `restRoot ../../data/R11567_00001_RawToTrack_Background_21hr_jgalan_2.3.15.root`
In this case, TRestRun objects and metadata objects can be directly accessed in the prompt.

```c++
root [0] run0->PrintMetadata()
root [1] md0_hitsAna->PrintMetadata()
root [2] md0_sAna->GetParameter("integralRange")
```

You can also have a look of the file contents through `TBrowser` GUI. By right clicking the metadata object in the list, you can retrieve data in it. 

### Exercise 3. Accessing data members from a stored metadata object

We can access the metadata members defined in each class (you may check the corresponding members by exploring the header file, the [class documentation](https://sultan.unizar.es/rest/classes.html) (recommended), or by invoking `Dump()` for a given metadata instance. For example, the [TRestRawVetoAnalysisProcess](https://sultan.unizar.es/rest/classTRestRawVetoAnalysisProcess.html) contains a metadata member named `fThreshold` which was used during the data processing.

We can recover the value using the name `veto` given by the user at the RML:

```python
rn.GetMetadataMember("veto::fThreshold")
```

or we can recover it using the class name of the process

```python
rn.GetMetadataMember("TRestRawVetoAnalysisProcess::fThreshold")
```

where the latest command will return the value of the first metadata instance that is of type `TRestRawVetoAnalysisProcess`. In our example there is only one instance, thus any of both ways will produce the same result.

The returned value will always be a string, and conversion to a numeric value might be necessary

```python
threshold = int( rn.GetMetadataMember("TRestRawVetoAnalysisProcess::fThreshold") )
```

**HINT:** Any metadata information is printed out inside the method `PrintMetadata` of each metadata class. In order to find out which relevant metadata members we can access inside the class it is a good idea to check the implementation of the method for the class we are interested in.


### Exercise 4. Reteieving event data from TRestRun

Besides metadata, TRestRun could also give access to the event data. We can acquire the event data object using `GetInputEvent()` method. When we request an entry or event id from the `TRestRun` instance, the data inside the variable(here we named *tckEv*) will be refreshed, while the address keeps the same. 

In this example we print the event data and analysis tree observables from 3 different event entries.

```python
tckEv = rn.GetInputEvent()

rn.GetEntry(10)
tckEv.GetID()
tckEv.PrintEvent()

# We get the next entry. It should be entry 11.
rn.GetNextEntry()
tckEv.GetID()
tckEv.PrintEvent()

# We get the event with id 6960 which we are certain exists inside the file
rn.GetEventWithID(6960)
tckEv.GetID()
tckEv.PrintEvent()

# We iterate over all the events to have a 
nEntries =  rn.GetEntries()
for n in range(nEntries):
	rn.GetEntry(n)
	print( "Number of Tracks: " + str( tckEv.GetNumberOfTracks() ))
```

**NOTE:** The event entry number is just the position of the entry within the file, but it does not serve to fully identify the event between different processing stages with different output files. The event ID in the other hand is unique, and it can be used all along the life of the data processing to identify the origin of the event.

**NOTE:** In the same way as `PrintMetadata` might help to understand where the information is stored inside the class data members, the `PrintEvent` method implementation of each specific event type might help to understand how the event information can be accessed directly.

**THINKING:** Can you write a loop to calculate the average number of tracks for this run?

### Exercise 5. Drawing events

In the same way as we print the event data information and the observable values of each event, we may visualize the event invoking the method `DrawEvent` available in any class inheriting from `TRestEvent`.

As we did previously, we need to pre-load the entry we wish to visualize, and have created a `TCanvas` window instance previously.

```python
c = ROOT.TCanvas()
rn.GetEntry(100)
tckEv.DrawEvent()
```

> The `DrawEvent` method might receive an optional argument that allows to control what information we want to visualize from the event data. See for example the [TRestGeant4Event::DrawEvent](https://sultan.unizar.es/rest/classTRestGeant4Event.html#acbfe704537c529ce05fa101719d34b56) or the [TRestRawSignalEvent::DrawEvent](https://sultan.unizar.es/rest/classTRestRawSignalEvent.html#a5445f401b350b1d952670a7323594f6d) documentation.







### Exercise 6. Accessing the analysis tree from TRestRun

Using the instance of TRestRun, *rn*, we can gain access to the analysis tree. The analysis tree contains all the observables added during the processing of the data for each event, and it can be operated as a standard ROOT `TTree` object. Important to read the [ROOT TTree documentation!](https://root.cern.ch/root/htmldoc/guides/users-guide/Trees.html).

For example, we could use the analysis tree to generate a histogram and draw the observable named `sAna_ThresholdIntegral`. In python this can be done as follows:

```python
# we retrieve analysis tree and have an overall look of its observables at entry 10
at = rn.GetAnalysisTree()
rn.GetEntry(10)
at.PrintObservables()

# we draw one of the observables inside the analysis tree (counting all the entries)
c = ROOT.TCanvas
histo1 = ROOT.TH1F(???histo1???,???histo1??? ,1000 ,0 ,500000)
at.Project("histo1", "sAna_ThresholdIntegral" )
histo1.GetEntries()
histo1.Draw()
```

This is similar as when we accessed the `TTree` compiled with a set of files using `TRestDataSet`, but this time we only access the data of a single run, the run which we opened at the beginning of this tutorial.


### Exercise 7. Retrieving specific observable values

Once we get access to the analysis tree in an iterative way we may recover the value of any of the observables inside the tree and do any calculation we are willing to, create a custom histogram, draw them, print their value, etc. Unfortunately in python we can only retrieve observable with type `double` or `int`.

```python
rn = ROOT.TRestRun("../../data/R11567_00001_RawToTrack_Background_21hr_jgalan_2.3.15.root" )
nEntries = rn.GetEntries()
for n in range(nEntries):
	rn.GetEntry(n)
	
	obsValue = aT.GetDblObservableValue("sAna_TripleMaxIntegral")
	print("g4Ana_totaleEdep: " + str(obsValue) + " keV" )
```

In C++ or restRoot prompt we can retrieve other kinds of observables.

```c++
auto at = rn->GetAnalysisTree();
at->GetEntry(0);
auto obs = at->GetObservable("sAna_SaturatedChannelID");
```

Note that this `obs` is a sealed data type of REST. It could be of any actual types. C++ couldn't tell. We need to convert it manually:

```c++
cout << "Observable: sAna_RiseSlopeAvg is of type: " << obs.type << endl;  // vector<int>

// we reconstruct and print index #0 of the vector typed observable
auto vec = *(vector<int>*)obs;
cout << "The first value of this observable is: " << vec[0] << endl;
```

**NOTE.** We now go a little deeper on how the vector or map observable works. These observables usually stores sub-entries of event data, e.g., multiple signal data of an event. In the opened file, we can see saturated channel ids of each event is stored in a vector. The length of this vector could vary from event to event. There are also `std::map` type observables stored. You can think them as a vector of two value pairs. 







### Exercise 8. Visualizing events in TRestBrowser

We can directly browse REST files with built-in shell command:

```bash
restViewEvents R11567_00001_RawToTrack_Background_21hr_jgalan_2.3.15.root
```

You can try to click-around the interface and browse different events.


### Exercise 9. Getting event entries that satisfy certain conditions

We have been working with a calibration file which registers reference events in our detector, such as X-rays produced by a radiactive source. Imagine that now we want to select background events that have properties similar to the properties from our reference calibration file.

Suppose then than we want to differentiate those events that have a `sAna_AmplitudeIntegralRatio` close to the average value obtained in the previous exercise (=72.87), and that we want the events happening in a given area of the detector, lets say those that produced an energy deposit on the first quadrant, where `x>0` and `y>0`.

We can open the **background** file and request the event entries that satisfy those conditions.

```python
rn = ROOT.TRestRun("../../data/R11567_00001_RawToTrack_Background_21hr_jgalan_2.3.15.root")
evList = rn.GetEventEntriesWithConditions("sAna_AmplitudeIntegralRatio>70 && sAna_AmplitudeIntegralRatio<75")
print( evList )
rn.GetEntry(118)
```

By typing `sAna_AmplitudeIntegralRatio>70 && sAna_AmplitudeIntegralRatio<75` also in the browser window and clicking "Next Event", you will see the entry goes in same numbers.







