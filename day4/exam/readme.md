### Final exercise. Exam for getting an official REST-for-Physics certificate!

Suppose now we are going to establish a set of analysis schema for a new experiment. 
To simplify, we assume the detector only contains one readout module with one readout channel. Like an HPGe detector. The daq system triggers
at each event, and records one single waveform(TRestRawSignal) within 512 sampling window. There could be multiple pulses inside one window. 
The following figure shows an example event.

![xx](sample_event.png)

In this exercise, you will need to write your own analysis code for such detector. It should contain: 
* A new event class containing energy(s), rise time(s), triggered time(s), pulse FWHM(s) and pulse width 1/10 height(s).
* A new process to convert TRestRawSignalEvent to our new event type, and extract observables like baseline noise level, first trigger position, etc.

**Hint** You can take a look at the code of `TRestRawSignalAnalysisProcess` and `TRestRawToDetectorSignalProcess` for reference. You can also use the existing methods from `TRestRawSignalEvent` class. The template code for this exercise can be found in this directory.