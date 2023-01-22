### Exercise 1. Accessing the gas properties through TRestDetectorGas

In REST-for-Physics we use the Garfield++ interface to Magboltz to generate Magboltz gas files and load the gas properties, such as the electron drift velocity or the electron diffusion coefficients. The gas files are produced by Magboltz through MonteCarlo, which is a computationally expensive operation.

We can use [TRestDetectorGas](https://sultan.unizar.es/rest/classTRestDetectorGas.html) to create a gas definition through a metadata description and generate the gas files for given gas mixtures and conditions that can be recovered afterwards without having to perform again the calculation.

You may use `TRestDetectorGas` with any definition using the gas components supported by Magboltz. We have generated a database with pregenerated gas files using the `TRestDetectorGas` interface. You may download the `gases.rml` definition where all the existing pre-generated gas mixtures can be found.

```
wget https://sultan.unizar.es/gasFiles/gases.rml
```

Then, we just print on screen the first line of `TRestDetectorGas` definitions to quickly inspect the available definitions.

```
cat gases.rml | grep "<TRestDetectorGas"
```

which will print on screen all the lines inside the file that contain `<TRestDetectorGas`.

We may chose now the name of any of those gases and load it in our `restRoot` environment.

```
restRoot
root [0] TRestDetectorGas gas("server", "Xenon-TMA 1.5Pct 10-10E3Vcm");
```

This time, instead of proving a local RML file to the `TRestMetadata` constructor we have provided the `server` keyword that will internally retrieve the `gases.rml` file from the database.

We can now print out some of the properties from our gas, including the definitions used to generate the gas.

```
gas.PrintMetadata()
```

before retrieving the gas properties we need to define an electric field, you may try to retrieve the drift velocity without doing it.

```
gas.GetDriftVelocity()
```

A warning will appear on screen indicating that you must first define the value of the electric field. We may define the value using the `SetElectricField` method. Remember that if no units are provided, the default units will be assumed. In this particular case, the relevant default units are `V` and `mm`. Thus, if we do not indicate the units, `V/mm` will be assumed.

```
gas.SetElectricField(100)
gas.GetElectricField()
gas.SetElectricField( 1. / units("kV/cm") );
gas.GetElectricField()
```

Therefore, both returned values from `GetElectricField` method should be the same, the value is always returned in the default units `V/mm`.

We may now retrieve the physical properties from the gas we loaded, such as the electron drift velocity which will be returned in the default REST-for-Physics units also `mm/us`,

```
gas.GetDriftVelocity()
```

or the transversal and longitudinal diffusion coefficients that will be returned in `cm^1/2`.

```
gas.GetTransversalDiffusion()
gas.GetLongitudinalDiffusion()
```

This time the returned value is not given in the default units, there are some exceptions that will be always indicated in the [method documentation](https://sultan.unizar.es/rest/classTRestDetectorGas.html#afdede862e3382b8bc6dacb139d828938).

**TODO**: Optionally generate the curves for a couple of gas mixtures as a function of the electric field.

### Exercise 2. Generating a detector readout using a TRestDetectorReadout

A readout can be included inside a processing chain, and it serves to find the relation between the electronic channels of the detector and the physical readout channels, as well as to define the topology of the readout. Typical processes that use the readout are `TRestDetectorHitsToSignalProcess` and `TRestDetectorSignalToHitsProcess` for event reconstruction, or `TRestDetectorElectronDiffusion` for diffusing the the charge produced by a Geant4 simulation.

In this exercise we will use the pixel detector readout that can be found at the [basic-readouts](https://github.com/rest-for-physics/basic-readouts) repository (we highly encourage to look on that repository to get further insights related to the readout generation and usage).

We have copied the file `pixelReadouts.rml` into this repository so that is is more accessible. We will just need to update it so that it covers the proper detector area that we will use in the next exercise. In the next exercise we define a box centered in the `(0,0,0)` with a total drift distance of 1-m defined in the Z-axis, and a total active area of 0.5x0.5-m2. Thus, in our readout we will need to adapt the readout area by updating the number of pixels and the size per pixel. We will also define a unique readout plane that will be placed at `(0,0,-500)`.

Follow the following steps to update your readout:
1. Remove one of the `readoutPlane` sections found inside the `readouts.rml` file.
2. Update the `position` in the remaining readout plane to `(0,0,-500)mm`.
3. Update the `cathodePosition` to `(0,0,500)mm`.
4. Update the default value of the pixel pitch and pixel size so that it covers by default an area of 0.5x0.5-m2.

Once this is done just launch the readout generation:

```
restRoot
root [0] TRestDetectorReadout readout("readouts.rml");
root [1] TFile *f = TFile::Open("readouts.root", "RECREATE" );
root [2] readout.Write("pixel");
root [3] f->Close();
.q
``` 

**NOTE:** We are pre-generating the readout metadata for the same reason we pre-generated gas files, the readout generation is computationally expensive, and the optimization of the readout is necessary for complex irregular readouts that need to generate a regular mapping that is associated with arbitrary pixel positions and sizes. The regular mapping is then used to fasten the localization of the pixel.

We may test now our readout to check that it is as we expected:

```
restRoot
root [0] TFile *f = TFile::Open("readouts.root");
root [1] auto readout = f->Get<TRestDetectorReadout>("pixel");
root [2] readout->PrintMetadata();
root [3] readout->GetNumberOfChannels()
```

And test some of the methods implemented inside the readout, such as retrieving the X and Y positions of a given, `planeId=0`, `moduleId=0` and `channelId=2250`.

```
root [3] Double_t x = readout->GetX(0, 0, 2250); 
root [4] Double_t y = readout->GetY(0, 0, 2250); 
```

or recover the corresponding daq channel id corresponding to a given `(x,y)` coordinates.

```
Int_t planeId = -1;
Int_t moduleId = -1;
Int_t channelId = -1;

Int_t daqId = readout->GetHitsDaqChannel(TVector3(50, 70, 12.5), planeId, moduleId, channelId);
std::cout << "Channel id: " << daqId << std::endl;
```

**NOTE:** The channel id returned is the value corresponding to the daq channel id that is associated with the electronics acquisition system. If no decoding file was provided, then the physical readout channel number will be equal to the daq channel id.


### Exercise 3. Diffusing and smearing punctual detector energy deposits

For testing some of the processes present at the detector library we will generate a dummy Geant4 Monte Carlo simulation where we have a simple gas box filled with gas at high pressure.

We will use the geometry defined at the `geometry.gdml` file, and the simulation conditions described inside `electron.rml`. The geometry defines a variable `GDML_PRESSURE` that allows us to play externally with the pressure/density of the gas, and we need to define before executing the example. On top of that we will need to create a directory where the output files will be generated.

```
mkdir data
export GDML_PRESSURE=10
restG4 electrons.rml
```

Once it has been 




### Exercise 4. Detector event reconstruction

