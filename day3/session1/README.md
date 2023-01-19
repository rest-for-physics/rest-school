
### Exercise 1. Launching a first processing chain

In this exercise we will test few processes that operate at the rawsignal processing domain and we will check how some metadata members can be initialized from the input filename. 

The following example is a reduced version of the complete CAST experimental detector data analysis where we define 6 processes:

#### Processes description

1. **virtualDAQ (MultiFEMINOSToSignal)**: It is responsible to read the binary file and fill in a `TRestRawSignalEvent`.
2. **rawChActivity (SignalChannelActivity)**: It pre-generates histograms of channel activity that will be written to the output file. \+
3. **rmChannels (SignalRemoveChannels)**: It removes the group of channels corresponding to the veto system.
4. **veto (VetoAnalysis)**: It is a dedicated process for analysing the signals from a veto system. \+\+ 
5. **sAna (SignalAnalysis)**: It will extract information from the pulse shape of each rawsignal.
6. **rateAna (EventRateAnalysis)**: It is a framework process that can be executed with any event type and extracts the event rate (Number of events that reached this process normalized with the time interval calculated using the event timestamps). \+\+\+

\+ A similar result could be achieved by using the `std::map` observables added by the `SignalAnalysis` process, leaving the user more freedom to create her/is own channel activity definitions later on. Still, it is interesting to fix those activity maps by a process that guarantees that every user produces them in the same way, and are univocally produced using metadata parameters that haven been stored to disk with a versioning system.

\+\+ Having dedicated processes for specific tasks leaves more flexibility to the process developer to decide the design of the process, metadata members, event data transformation, options, etc. Anyhow, re-exploiting processes and code is also a highly desired feature in a software project, we will see in the exercise 2 how the veto signal analysis can also be worked out using the SignalAnalysis process on a reduced group of signals.

\+\+\+ The processes that do not need to access specific event type information (e.g. events that only need access to `TRestEvent` header, or to the analysis tree) will be found directly in the framework and will be able to operate at any event processing stage with any event type.

#### Launching the event data processing

The input file that we will use for this exercise has been truncated. Therefore we have limited the event processing to 4900 events in order to avoid reaching the end of the file.

The `mainDataPath` defined inside the `<globals>` section defines where the output file will be stored. In the example is defined as `data/`. Thus, if it does not exist already we will need first to create a new data directory to host the new files generated.

```
mkdir data
```

Then we can launch the processing chain using `restManager`.

```
restManager --c CAST.rml --f ../../data/R10513_Calibration_15min_Vm_340_Vd_113_Pr_1.4_Gain_0x1_Shape_0xD_Clock_0x02-000.aqs 
```

The output on screen will show us the result from invoking `PrintMetadata` for each of the processes we added in our event processing chain, defined inside the file `CAST.rml`.

Each process will usually show the metadata parameters used to define the process behaviour, and a list of observables (if any) that were added by the process to the analysis tree.

We can now check inside the generated file and inspect the contents.

```
restRoot R10513_00000_BasicRaw_Calibration_15min_${USER}_2.3.15.root
```

In the generated output we will observe a list of the existing processes that have been registered together with the analysis tree and event data.

We may check for example the contents of the `TRestDetector` metadata class instance we created inside our RML, in the `<TRestRun>` section, and verify it has been filled with the filename format values we defined at the `inputFormat` parameter.

```
root [0] md_detector->PrintMetadata()
```

You may check now any other `md_` metadata class using `PrintMetadata`, and inspect the different events using the methods that you learnt in session 2.3.

### Exercise 2. Launching a processing chain with several rawsignal analysis processes

As soon as the input/output event types match, we have full flexibility to build any processing chain we need. One interesting feature is the potential to build event data chains where processes can be exploited more than once. For example, a rawsignal analysis process might be used to extract information before and after a given event transformation took place, such as before and after a smoothing process, or it can operate on different groups of signals.

There are clear advantages on re-using processes for different applications, but also inside a particular application, such as:
- Reusing the same algorithms for different tasks.
- Increase the user feedback on a particular piece of code: bug identification, feature requests, know-how transfer.
- Reduce the maintenance costs thanks to code centralization.
- Increased benefit on bug correction and upgrades. More users and applications benefit from the changes (although some times also disturbed by! :) Of course, this depends on project maturity and the available resources for maintenance. )

#### Processes description

In the example we will use for this exercise we define 4 processes:

1. *virtualDAQ (MultiFEMINOSToSignal)*: It is responsible to read the binary file and fill in a `TRestRawSignalEvent`.
2. *veto (SignalAnalysis)*: It operates on a group of channels which are connected to a muon veto system, and it extracts the maximum amplitudes and peak times of each signal which is finally stored in a `std::map` inside the analysis tree.
3. *rmChannels (SignalRemoveChannels)*: It removes the group of channels corresponding to the veto system.
4. *mm (SignalAnalysis)*: It performs the pulse shape analysis on the remaining 

### Exercise 3. Re-processing events on a given event selection


### Exercise 4. Testing individual raw processes


### Exercise 5. Rawsignal operations and visualization


