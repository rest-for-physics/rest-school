
### Exercise 1. Launching a first processing chain

In this exercise we will test few processes that operate at the rawsignal processing domain and we will check how some metadata members can be initialized from the input filename.

The following example is a reduced version of the complete CAST experimental detector data processing and analysis. The rawdata contains waveforms that have been registered from the micromegas detector channels itself, and an additional signal coming from a muon veto system that helps discriminate events that are correlated with muon interactions. We will use the file `cast.rml`.

Inside our reduced event processing chain we define 6 processes:

#### Processes description

1. **virtualDAQ (MultiFEMINOSToSignal)**: It is responsible to read the binary file and fill in a `TRestRawSignalEvent`.
2. **rawChActivity (SignalChannelActivity)**: It pre-generates histograms of channel activity that will be written to the output file. \+
3. **rmChannels (SignalRemoveChannels)**: It removes the group of channels corresponding to the veto system.
4. **veto (VetoAnalysis)**: It is a dedicated process for analysing the signals from a veto system. \+\+ 
5. **sAna (SignalAnalysis)**: It will extract information from the pulse shape of each rawsignal.
6. **rateAna (EventRateAnalysis)**: It is a framework process that can be executed with any event type and extracts the event rate (Number of events that reached this process normalized with the time interval calculated using the event timestamps). \+\+\+

\+ A similar result could be achieved by using the `std::map` observables added by the `SignalAnalysis` process, leaving the user more freedom to create her/is own channel activity definitions later on. Still, it is interesting to fix those activity maps by a process that guarantees that every user produces them in the same way, and are univocally produced using metadata parameters that haven been stored to disk with a versioning system.

\+\+ Having dedicated processes for specific tasks leaves more flexibility to the process developer to decide the design of the process, metadata members, event data transformation, options, etc. Anyhow, re-exploiting processes and code is also a highly desired feature in a software project, we will see in the exercise 2 how the veto signal analysis can also be worked out using the SignalAnalysis process operating on a reduced group of signals.

\+\+\+ The processes that do not need to access specific event type information (e.g. events that only need access to `TRestEvent` header, or to the analysis tree) will be found directly in the framework and will be able to operate at any event processing stage with any event type.

#### Launching the event data processing

The input file that we will use for this exercise has been truncated. Therefore we have limited the event processing to 4900 events in order to avoid reaching the end of the file.

The `mainDataPath` defined inside the `<globals>` section defines where the output file will be stored. In the example is defined as `data/`. Thus, if it does not exist already we will need first to create a new data directory to host the new files generated.

```
mkdir data
```

Then we can launch the processing chain using `restManager`.

```
restManager --c cast.rml --f ../../data/R10513_Calibration_15min_Vm_340_Vd_113_Pr_1.4_Gain_0x1_Shape_0xD_Clock_0x02-000.aqs 
```

The output on screen will show us the result from invoking `PrintMetadata` for each of the processes we added in our event processing chain, defined inside the file `cast.rml`. Each process will show the metadata parameters used to define the process behaviour and a list of observables (if any) that were added by the process to the analysis tree.

We can now check inside the generated file and inspect the contents.

```
restRoot R10513_00000_BasicRaw_Calibration_15min_${USER}_2.3.15.root
```

In the generated output we will observe a list of the existing processes that have been registered together with the analysis tree and event data.

We may check for example the contents of the `TRestDetector` metadata class instance we created inside our RML, in the `<TRestRun>` section, and verify it has been filled with the filename format values we defined at the `inputFormat` parameter.

```
root [0] md_detector->PrintMetadata()
```

You may check now any other `md_` metadata class using `PrintMetadata` and inspect the different events (and corresponding analysis tree) using the methods that you learnt in session 2.3.

```
root [1] md_xyz->PrintMetadata()
root [2] run0->PrintMetadata()
root [3] run0->GetEntry(10)
root [3] run0->GetAnalysisTree()->PrintObservables()
...
```
### Exercise 2. Launching a processing chain with several rawsignal analysis processes

As soon as the input/output event types match, we have full flexibility to build any processing chain we need. One interesting feature is the potential to build event data chains where processes can be exploited more than once. For example, a rawsignal analysis process might be used to extract information before and after a given event transformation took place, such as before and after a smoothing process, or it can operate on different groups of signals.

There are clear advantages on re-using processes for different applications, but also inside a particular application, such as:
- Reusing the same algorithms for different tasks.
- Increase the user feedback on a particular piece of code: bug identification, feature requests, know-how transfer.
- Reduce the maintenance costs thanks to code centralization.
- Increased benefit on bug correction and upgrades. More users and applications benefit from the changes (although some times also disturbed by! :) Of course, this depends on project maturity and the available resources for maintenance. )

#### Processes description

In the example we will use for this exercise we define 4 processes:

1. **virtualDAQ (MultiFEMINOSToSignal)**: It is responsible to read the binary file and fill in a `TRestRawSignalEvent`.
2. **veto (SignalAnalysis)**: It operates on a group of channels which are connected to a muon veto system, and it extracts the maximum amplitudes and peak times of each signal which is finally stored in a `std::map` inside the analysis tree.
3. **rmChannels (SignalRemoveChannels)**: It removes the group of channels corresponding to the veto system.
4. **mm (SignalAnalysis)**: It performs the pulse shape analysis on the remaining channels that belong to the micromegas detector.

#### Launching the event data processing

This time we will use another file coming from some tests that were performed at the IAXO-D0 prototype including a more sophisticated muon veto system. The preliminar tests were carried including 14 veto signals. This example has been extracted from the rawlib validation pipeline, correspoding to the [Veto analysis](https://github.com/rest-for-physics/rawlib/blob/master/.github/workflows/validation.yml) validation. As it is the case in the previous example the original file has been truncated in order to save disk space.

Now we can process our file to produce some observables that will be used for analysis later on.

```
restManager --c veto.rml --f R01208_Ar2Iso_Background14h_14Vetos_IccubFEC-000.aqs
```

Note that the output filename is now much more shorter since we have chosen it to be this way at the `TRestRun` section. We will open now the file for quick inspection.

```
restRoot R01208_quickData.root
```

And we will check the observables we can find inside some of the events:

```
root [0] run0->GetAnalysisTree()->PrintObservables()
root [1] run0->GetEntry(11)
root [2] run0->GetAnalysisTree()->PrintObservables()
```

One important difference in this processed file is that we have added only a selection of observables in each of the processes. This particular selection is nothing special, and it has been chosen just for the shake of illustrating this example.

As we have seen in session 2.3 we may now create a selection of events using a particular condition at the analysis tree. We will use the maximum pulse amplitude between all the veto signals, and the peak time averaged for those veto signals that were found above threshold.

We get then a list of events, you may try to get a different list by changing the values:

```
root [3] run0->GetEventIdsWithConditions("vetoRaw_MaxPeakAmplitude > 1000 && vetoRaw_AveragePeakTime > 200")
root [4] run0->GetEventIdsWithConditions("vetoRaw_MaxPeakAmplitude > 2000 && vetoRaw_AveragePeakTime > 200")
```

You should see now on screen a vector with the event ids that have been selected. The constrains we imposed are our particular definition to decide when the veto system has detected a cosmic muon. This is done just to illustrate this example, in practice a more sophisticated analysis is required to enhance our muon discrimination capabilitites.

As in the previous example you may check now any other `md_` metadata class contents, or inspect the different events using the methods that you learnt in session 2.3.

#### Exporting the ids to a text file

Before closing the previous ROOT session lets write a file few selected ids, and write them to a file:

```
root [5] std::vector <int> ids = run0->GetEventIdsWithConditions("vetoRaw_MaxPeakAmplitude > 3500 && vetoRaw_AveragePeakTime > 200")
root [6] FILE *f = fopen("ids.txt", "wt");
root [7] for( const auto & x:ids ) fprintf( f, "%d\n", x );
root [8] fclose( f );
```

The event ids selected should be know registered inside our `ids.txt` file.

### Exercise 3. Re-processing events on a given event selection

Sometimes we will be willing to run a computationally expensive analysis on a particular selection of events. It is not worth to run this expensive data processing on each event (e.g. we could be wasting our precious CPU time to process noisy events, or we just want to consider events from a population that we are not interested in - as it could be the events at a given energy range) we can just use the results of a quick data analysis to select that population, or use an external event id list obtained by some other means.

The file `vetoOnSelection.rml` implements a new process `TRestEventSelectionProcess`. This process is a framework process and thus can be applied at any stage of the data processing. Although in our case it makes sense that it is applied as soon as possible in order to reduce the event processing load.

**NOTE:** The event id is read from the AQS file, thus the selection process cannot be placed before!

Looking to the `TRestEventSelectionProcesss` defined inside `vetoOnSelection.rml` we can see how we have introduced a condition similar to the one applied in the previous exercise.

We can now test the process and verify that only the selected entries are present at the output file.

```
restManager --c vetoOnSelection.rml --f R01208_Ar2Iso_Background14h_14Vetos_IccubFEC-000.aqs
```

We have reduced the number of events to be processed to the number of events in our selection, otherwise, since our aqs file has been truncated we will run into troubles, thats the reason we reduced the number of events to be processed to 19 in our example, inside `vetoOnSelection.rml`.

We may know check inside the new file and verify that the event ids found at the output file correspond with those we were expecting to be processed.

```
restRoot data/R01208_quickData_Selection.root
root [0] for( int n = 0; n < run0->GetEntries(); n++ ) { run0->GetEntry(n); std::cout << "id:" << ana_tree0->GetEventID() << std::endl; }
```

If we have a look now to the analysis tree we will see that this time we do not have `veto` observables, and for the `mm` process all the observables have been added, instead of a particular selection.

### Exercise 4. Testing individual raw processes

On top of using a pre-defined event data processing chain, it is also possible to use those processes standalone, so that we can do tests in an isolated manner.

#### Exercise 4.1 Adding signal noise 

In this first example we will create a new python script that uses the TRestRawSignalAddNoiseProcess to mimic the white electronic noise we observe in experimental data.

Open a new file, `addNoise.py`, and create a dummy rawsignal event (TRestRawSignalEvent) where we will add just one signal with 512 points initialized to zero.

```
import ROOT
import REST

rawEv = ROOT.TRestRawSignalEvent()

sgnl = ROOT.TRestRawSignal()

sgnl.SetSignalID(13)
for x in range(0,512):
    sgnl.AddPoint(0)

rawEv.AddSignal(sgnl)
```

Now we just need to initialize the process using a configuration file

```
addNoiseProcess = ROOT.TRestRawSignalAddNoiseProcess("metadata.rml")
```

then we are ready to test our event processing for our dummy generated event

```
outEv = addNoiseProcess.ProcessEvent( rawEv )
```

we may check now some of the properties of the signal to check the effect of processing the event, such as the baseline fluctuations

```
outEv.GetSignal(0).CalculateBaseLine(50,450)
print( "The baseline fluctuation sigma is : " + outEv.GetSignal(0).GetBaseLineSigma() )
```

or draw the event to visualize the result

```
c = ROOT.TCanvas()
outEv.DrawEvent()
c.Print("noise.png")
```

#### Exercise 4.2. Signal convolution

In this example we will introduce the shaping of the electronics through the process `TRestRawSignalShapingProcess`. This time we will create  a C-macro process our event.

We may create a new file `shaping.C` where we will define a method,

```
Int_t shaping( ) {

}
```

although another option would be to test it inside a `restRoot` session directly.

In a similar way as we did in the previous example we will first create a dummy event with a signal that contains two discrete energy deposits. Inside the method definition we start writting:

```
    TRestRawSignalEvent* ev = new TRestRawSignalEvent();

    TRestRawSignal* sgnl = new TRestRawSignal(512);

    sgnl->IncreaseBinBy(170, 100);
    sgnl->IncreaseBinBy(250, 250);

    ev->AddSignal(*sgnl);
```

Then we initialize the shaping process using an RML file,

```
    string cfgFile = "shaper.rml";
    TRestRawSignalShapingProcess* shaper = new TRestRawSignalShapingProcess((char*)cfgFile.c_str());
    shaper->PrintMetadata();
```

We may visualize the event before processing it:

```
    TCanvas c;
    ev->DrawEvent();
```

and then we can process and visualize it again:

```
TRestRawSignalEvent* shapedEvent = (TRestRawSignalEvent*) shaper->ProcessEvent(ev);
shapedEvent->DrawEvent()
```

#### Exercise 4.3. Rawlib pipeline validation

The previous exercises are based on the examples found at the rawlib [pipeline directory](https://github.com/rest-for-physics/rawlib/tree/master/pipeline) which are used for Continous Integration tests. Each directory inside that directory defines a working example. We highly encouragee you to have a look there for more complete and up-to-date examples.

### Example 5: Using pyRoot and numpy to quickly visualize rawsignals and derived parameters 

This example, `PyRoot_SignalPlotter.ipynb`, is a Jupyter notebook. It demonstrates how we can combine ROOT and REST with python mathematical libraries to examine raw signals, while quickly visualizing noise reduction parameters. This could be a more convenient way for those who havce already some experience using `numpy` and `matplotlib.pyplot` libraries.

**TODO**: Since this notebook already uses many REST functions, it will be presented in more detail in another session.
