You will need to move to the directory `day2/session1` to execute the proposed commands.

### Exercise 1. Units

The units inside REST-for-Physics are available in the namespace `REST_Units`, a namespace defines a set of variables and methods that are accessible under that name when invoking `REST_Units::`.

Inside a `restRoot` session we can use the namespace to identify which public data members and methods are available to us. Now, just open a new `restRoot` session and checkout what it is inside the namespace.

```c++
restRoot
root [0] REST_Units:: [TAB]
```

In particular, you will be able to identify few of the REST basic units defined inside.

You may check now the scaling value associated to each unit.

```c++
root [0] REST_Units::degrees [ENTER]
(const double) 57.295780
```

Where the returned value corresponds to the degrees in one radian. The units inside REST work in a way that we can access the scaling factor by invoking `units("UNIT_STRING")`, which will allow us to make unit conversions inside our code.

For example, if we execute,

```c++
root [1] 1. * units("cm")
(double) 0.10000000
```

the result will be the number of `cm` inside 1 `mm` which is the default unit inside REST. We can also combine different units to achieve more complex results, such as:

```c++
root [2] 1. * units("g/cm^3")
(double) 1000000.0
```

If we know the value the units for the value we are providing, and this is not given in the default units, we will then need to write a more generic expression:

```c++
root [3] 180. * units("rads")/units("degrees")
(double) 3.1415927
```

REMEMBER: All these lines of code that you are using inside your `restRoot` session can be used inside a C++ compiled code, a C-macro, or even a python script that imports the REST-for-Physics libraries.

### Exercise 2. TRestPhysics

In the same way as units, REST-for-Physics provides access to its own physics constant definitions and geometry methods commonly exploited in particle tracking.

In this exercise, we will just check inside the `REST_Physics` namespace to see the available constants and print out their value.

```c++
root [4] REST_Physics:: [TAB]

root [5] REST_Physics::lightSpeed
(const double) 2.9979246e+08
```

### Exercise 3. TRestTools

TRestTools is a class that defines static methods that can be invoked directly. It contains generic methods that can be of utility at some point in the process of building a new REST metadata class or process, or inside REST macros/scripts. The different methods can be found documented in [the following page](https://sultan.unizar.es/rest/classTRestTools.html).

Some of the methods found in TRestTools will allow us to operate with tables. The `data` directory inside the root [rest-school repository](https://github.com/rest-for-physics/rest-school) contains a binary table with extension `N150f` indicating us that it is a table with 150 columns, and its values are written with `float` precision.

We can read this table into a `std::vector <std::vector> >` previously generated data holder as follows:

```c++
root [6] std::vector <std::vector <float> > d;
root [7] TRestTools::ReadBinaryTable( "../../data/XenonNeon_25Pct_1bar.N150f", d )
```

Then, we may access this table using the standard c++ libraries. To print on screen one element we may use:

```c++
root [8] d[10][10];
```

or to print the full table or show several rows on screen, we can do:

```c++
root [9] TRestTools::PrintTable ( d );
root [10] TRestTools::PrintTable ( d, 10, 11 );
```

Other interesting methods inside `TRestTools` allow us to operate with filenames, such as substracting the filename from a full path, extracting the extension, or checking if the file exists or is accessible. For example:

```c++
root [11] TRestTools::GetFileNameRoot("../../data/XenonNeon_25Pct_1bar.N150f")
(std::string) "XenonNeon_25Pct_1bar"
```

Now, we can create a ROOT macro, that uses those methods to fill a TH1D object with the sums from each row. Open a new file and test the following code:

```c++
Int_t GenerateHistogram( std::string fname ) {

    /// Reading the table
    std::vector <std::vector <float> > myTable;
    TRestTools::ReadBinaryTable( "../../data/XenonNeon_25Pct_1bar.N150f", myTable );

    /// Creating the histogram to be filled
    TH1D *h = new TH1D( "Response", "XenonNeon_25Pct", myTable.size(), 0, 15);

    int n = 0;
    for( const auto &d : myTable ) {

        /// We add all the values in a row to set the value of each histogram bin
        float result = 0;
        for( const auto &elem : d )
            result += elem;

        n++; // First bin is 1

		/// We fill the histogram
        h->SetBinContent( n, result );
    }

    std::string fnameRoot = TRestTools::GetFileNameRoot( fname );

	/// We must create the canvas where to draw before drawing!!
    TCanvas c;

    h->Draw();
    h->GetXaxis()->SetTitle("Energy root [keV]");
    h->GetYaxis()->SetTitle("Efficiency");

    c.Print( (TString) fnameRoot + ".png" );

    return 0;
}
```

Where in the above code we have used the fact that the data is a response matrix defined in the 0-15keV range.

We can execute the macro inside the `restRoot` session by loading the generated file, and executing the method:

```c++
root [12] .L GenerateHistogram.C
root [13] GenerateHistogram( "../../data/XenonNeon_25Pct_1bar.N150f" );
```

If the macro execution succeeds, you should have a new png image showing the resulting histogram.


### Exercise 4. TRestAnalysisPlot

In this exercise we will launch restManager to produce a combined plot using the `TRestAnalysisPlot` metadata class. This class allows to define plots through a configuration file so that we can produce the same systematic plots for different datasets.

#### First simple test

We just provide the definition inside the rml config file. All the input files that satisfy the glob pattern will be combined.

```c++
restManager --c analysisPlots.rml --f "../../data/run_*rayTracing*root"
```


#### Changing the output verbose level

To see the effect of output levels you may try to launch now `restManager` with a higher verbose level adding the `--v` flag in the arguments.

You may use:

```c++
restManager --v 2 --c analysisPlots.rml --f "../../data/run_*rayTracing*root"
```

The `--v 2` flag is equal to `info` output level and it will override the verboseLevel given by the configuration file for each individual section or class definition.

#### Modifying the RML behaviour using system variables

In the `<globals>` section inside the RML file we have defined a variable named `FORMAT`. This variable is used inside the configuration file but can be overwritten and modified externally using sytem environment variables. This allows to control externally the behaviour of the RML (configuration file) and define templates with the most relevant parameters that need to be modified/controlled externally.

You may try to generate an output using the different formats supported by ROOT (e.g. pdf, C, root, jpeg). Try to generate a different image format using the following code.

```c++
export FORMAT=pdf
restManager --c analysisPlots.rml --f "../../data/run_*rayTracing*root"
```

Hint: Open the RML file to inspect it and see how the `FORMAT` variable is used inside the RML file.

#### Modifying the plots

We may now explore the different `<plot>` sections and decide to change one of the plots to draw something else.

In this exercise you may decide to use *any* of the variables that are listed inside the `data/README.md` description in the root [rest-school repository](https://github.com/rest-for-physics/rest-school).

A possible proposal:
 - Replace the variables inside the plot named `FocalSpot` by the variables `offset_posX` and `offset_posY`.
 - Remove the variables inside the plot named `SolarDist` and replace by a single variable histogram `axionPhoton_fieldAverage`.

Additionally you may update the title and labels to match what we are drawing "Offset" instead of "Spot" or "Field average [T]". You may also play around with the histogram limits using the `range` field, or the binning number using the `nbins` field.


#### Adding new plots

You can also add a new plot, just copy/paste a complete `<plot>` entry and modify it as you did in the previous section. The only difference now is that you need to allocate an additional space inside the `<canvas` entry to define a grid that can host at least 5 plots, such as 3x2 or 2x3. The canvas size should be also updated according to the new number of plots to keep the aspect ratio.

### Exercise 5. TRestMetadataPlot

In this exercise we will launch restManager to produce a combined plot using `TRestMetadataPlot`. In a similar way to `TRestAnalysisPlot`, this class will allow to generate systematic plots, this time using the values of the metadata members found in a set of input files. Thus, different metadata members can be plotted against the other metadata members, just as the run number or the run timestamp, as we will see in the following example.

We provide the plot definitions inside the `metadataPlot.rml` config file. We will use now pre-generated processed files that correspond to real detector data that were generated using `TRestProcessRunner::inputEventStorage=False` and `TRestProcessRunner::outputEventStorage=False` so that only the `TRestAnalysisTree` and metadata objects are present inside the file.

Now just try the following command to test the plot production. The first time we will need to create a directory `plots` where the plots will be created.

```c++
mkdir plots
restManager --c metadataPlot.rml --f "../../data/*RawToTrack*root"
```

### Exercise 6. TRestDataSet

A `TRestDataSet` instance will allow us to define certain conditions that need to be satisfied by a group of REST processed files. Once the object has been initialized it will give us access to an internal `TTree` and `RDataFrame` instances built with the selected files.

Inside the `dataset.rml` file you will find different dataset definitions that can be used to create a selection of files. For example, the dataset named `CalibrationsJune` will pick-up from the `RawTrack` files all those calibration runs that were measured in June.

In this example we will initialize that particular dataset and access the `RDataFrame` and `TTree` defined inside `TRestDataSet`.

```c++
root [0] TRestDataSet calJune("dataset.rml", "CalibrationsJune");
root [1] calJune.Initialize()
```

We can verify now which files have been finally selected by the dataset definition,

```c++
root [2] calJune.GetFileSelection()
```

and we can access the combined `TTree` as we do with any other `TTree`. For example we can check the number of entries inside the tree,

```c++
root [3] calJune.GetTree()->GetEntries()
```

or check the column names (branches) that have been added to the dataset (the ones that we have chosen inside the RML).

```c++
root [4] calJune.GetDataFrame().GetColumnNames()
```

We can create a histogram from the combined data and draw it on a canvas, using the `TTree` instance,

```c++
root [5] TCanvas c1;
root [6] calJune.GetTree()->Draw("sAna_ThresholdIntegral>>myHisto1(1000,0,500000)");
root [7] myHisto1->GetEntries()
root [7] myHisto1->GetNbinsX()
root [8] c1.Print("myHisto.png");
```

or using the `RDataFrame` instance,

```c++
root [9] TCanvas c2;
root [10] auto myHisto2 = calJune.GetDataFrame().Histo1D("sAna_NumberOfGoodSignals")
root [11] myHisto2->Draw();
root [12] c2.Print("myHisto2.png")
```

Now we can operate with those objects to perform a more sophisticated analysis in our code, or even use it in combination of other REST classes that require as input a `TRestDataSet`.

Another interesting feature of `TRestDataSet` is the capability to export the analysis tree entries, and other relevant information, into different output formats so that the next access to the dataset we do not need to compute the file selection. Currently we support the ROOT output format which will dump to disk a `TRestDataSet` describing the parameters used for file selection, and a ROOT `TTree`, and a ASCII plain-text format which could be useful to import in other software packages.

```c++
root [13] calJune.Export("JuneCalibrations.txt")
root [14] calJune.Export("JuneCalibrations.root")
root [15] .q
```

You may now check the file contents inside the TXT file,

```c++
head -n 25 JuneCalibrations.txt
```

or inside the exported ROOT file.

```c++
restRoot
root [0] TFile *f = TFile::Open("JuneCalibrations.root");
root [3] TTree *tr = (TTree *) f->Get("AnalysisTree");
root [4] tr->GetEntries()
(long long) 34782

root [5] TRestDataSet *dSet = (TRestDataSet *) f->Get("CalibrationsJune");
root [6] dSet->PrintMetadata();
```

