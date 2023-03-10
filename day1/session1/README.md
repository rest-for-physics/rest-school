### Macro 1: Example of producing a histogram and performing a simple analysis

In this example we look at the timestamps of a calibration measurement with the IAXO-D0 detector. The goal is to rebin the raw data so that we can see the event rate at a glance, and to spot detector down-time. We use the histogram to determine the effective measurement duration.

Even though the macro uses only ROOT and not REST, to execute it we have to launch `restRoot`, as the data we use contains some REST objects that cannot be loaded by Root. You can launch the macro by typing `.L 1_macro_histograms_example.C` and then call the function with  `run_duration()`.

After initializing some variables, we load the data into a `RDataFrame` and extract the timestamps into a `std::vector`:
```
	ROOT::RDataFrame data("AnalysisTree", "../../data/R01855/R01855_0*root");
	auto result_ts = data.Take<double>("timeStamp");
	vector<double> timestamps = result_ts.GetValue();
```
Since the histogram cannot be created with a bin width option, but only with a range and number of bins, we have to calculate the required number of bins to get the desired bin width of 60 seconds, so that the y-axis of the histogram shows a meaningful rate. In addition, the number of bins has to be an integer, so we round up the upper range limit `tmax` to the next higher multiple of 60 added to `tmin`:

```
	double tmax = data.Max("timeStamp").GetValue();
	double tmin = data.Min("timeStamp").GetValue();
	tmax = tmin + (round((tmax - tmin + binw / 2) / binw) * binw);
```

In the next step we create the histogram, using the rounded `tmax` to calculate the number of bins, and fill it with the content of the vector. Note that the histogram could also be drawn directly from the dataframe, but for the sake of generality we chose to fill it with the content of a vector.
```
	TH1D* h = new TH1D("timestamps", "timestamps", (tmax - tmin) / binw, tmin, tmax);
	for (auto i : timestamps) h->Fill(i);
```
We can call the `GetBinWidth` method and display its result if desired to check if the above calculation was correct (it was ;-)).
```
	binw = h->GetBinWidth(1);
```
Finally, in the data we can see that there was a short down time in the detector due to a voltage trip. To estimate the effective measurement duration, we want to exclude this period. To achieve this, we loop over all the bins and count how many have a count higher than a predefined threshold (in this example 10 counts per minute): 
```
	int nbins = h->GetNbinsX();
	for (unsigned int i = 0; i < nbins; i++) {
		bincount = h->GetBinContent(i);
		if (bincount > rate_threshold) {
	    	run_duration += binw;
		}
	}
```
Finally we set up the histogram and plot the data, and display the effective measurement duration.

### Macro 2: Example of fitting a histogram with a custom function
We want to fit the energy spectrum of a 55-Fe calibration run with the IAXO-D0 detector. 

To run this macro, we have to launch `restRoot` and then execute:
```
	.L 2_macro_fitting_example.C
	fit calibration()
```
In a separate function in the macro we define the fitting function. In this case we simply define a Gaussian distribution. In general this function comes predefined in root, but for the sake of learning, we define it ourselves:
```
	double fitf(double* x, double *par){
    		double fitval = par[0] * TMath::Exp(-TMath::Power(*x - par[1],2)/(2*TMath::Power(par[2],2))) ;
    		return fitval;
	}
```
In the main function we again load the data into a dataframe, but then create the histogram directly from there by calling the `Histo1D` method:
```
	auto h = df.Histo1D({"h","calibration", 250, 0, 10000},"tckAna_MaxTrackEnergy");
```
To use the custom function in the fit, we have to wrap it into a `TF1` object:
```
	TF1 *func = new TF1("fit",fitf,0,10000,3);
```
In the following steps we set the start parameters, give limits to the mean of the Gaussian between 1 and 10 000, and name the parameters. The fit itself is executed by the following:
```
	h->Fit(func, "L", "", 2000, 6000);
```
The option "L" sets the fitting method to chi-squared. In addition we only perform the fit between 2000 and 6000. Finally we draw the histogram including the fit.

### Example 3: Final exercise
In this macro, we again analyze a calibration run of IAXO-D0. First we want to fit the 55-Fe peak to obtain the calibration factor, use this to calibrate the data in the dataframe, and then produce a histogram showing the calibrated energy spectrum. In addition we produce a 2-dimensional hit map of the events in the detector.

1. Create a macro. Include the necessary classes.

2. Define the fitting function.

3. Define the main function.

4. Load the data into a `RDataFrame`.

5. Initialize a filtered dataframe.

6. Filter the data. Only keep the data from a spot with 10 mm radius in the detector center (necessary columns: `hitsAna_xMean` and `hitsAna_yMean`).

7. Create a histogram of the energy (column `tckAna_MaxTrackEnergy`).

8. Use `TSpectrum::Search()` to find peak candidates. Get the position of the peak to use it to define the start parameters of the fit.

9. Create a `TF1`-object from the fitting function

10. Configure the fit by interacting with the `TF1`-object: setting parameter values (start parameters), setting parameter constraints, naming the parameters, ...

11. Execute the fit by calling `TH1::Fit()`.

12. Draw the histogram and the fitted function on a canvas.

13. Access the best fit parameters and calculate the calibration factor. Note that the fit has to be performed with the "S" option to extract the parameters.

14. Define a new column in the dataframe with the calibrated energy. Use a C++ lambda function in the definition.

15. Create a new plot with the calibrated energy. Try out some options to adjust style and colors to your liking.

16. (BONUS) Create a 2-D histogram with the detector hit map.
