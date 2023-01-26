### Final exercise. Exam for getting an official REST-for-Physics certificate!

Suppose now we are going to establish a set of analysis schema for a new experiment. 
To simplify, we assume the detector only contains one readout module with one readout channel. Like an HPGe detector. The daq system triggers
at each event, and records one single waveform(TRestRawSignal) within 512 sampling window. There could be multiple pulses inside one window.

The analysis for such detector needs: 
* A new event class containing energy(s), rise time(s), triggered time(s), pulse FWHM(s) and pulse width 1/10 height(s).
* A new process to convert TRestRawSignalEvent to our new event type, and extract observables like baseline noise level, first trigger position, etc.

For the exercise we suggest you to write new c++ codes of you own. Don't use the existing methods like `GetPointsOverThreshold()` from TRestRawSignal.

Here are the examples:
