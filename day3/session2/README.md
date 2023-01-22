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


### Exercise 3. Diffusing and smearing punctual detector energy deposits


### Exercise 4. Event detector reconstruction

