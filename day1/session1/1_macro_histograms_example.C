#include <TCanvas.h>
#include <TH1D.h>
    
int run_duration(){
    
    
    // initialize parameters
    int bincount;
    int run_duration = 0;

	double binw = 60; // bin width in seconds
	int rate_threshold = 10; // rate threshold in counts per binw. If the rate is below, the detector is regarded to be inactive, and the bin is not counted towards the measurement time

    // Load dataframe
    ROOT::RDataFrame data("AnalysisTree", "../data/R01855/R01855_0*root");
        
    // extract timestamps from dataframe
	auto result_ts = data.Take<double>("timeStamp");
    vector<double> timestamps = result_ts.GetValue();
    
    double tmax = data.Max("timeStamp").GetValue();
    double tmin = data.Min("timeStamp").GetValue();
    
    // calculate nbins to get desired bin width
     tmax = tmin + (round((tmax - tmin + binw / 2) / binw) * binw);
    
    // create histogram
    TH1D* h = new TH1D("timestamps", "timestamps", (tmax - tmin) / binw, tmin, tmax);
    for (auto i : timestamps) h->Fill(i);
    binw = h->GetBinWidth(1);
    // check if bins are above threshold
    int nbins = h->GetNbinsX();
    for (unsigned int i = 0; i < nbins; i++) {
		bincount = h->GetBinContent(i);
		if (bincount > rate_threshold) {
	    	run_duration += binw;
		}
    }

    cout << "Run duration:" << run_duration << " seconds = " << (double)run_duration/60 << " minutes = " << (double)run_duration/3600  << " hours" << endl;

    TCanvas* c = new TCanvas();
    h->GetXaxis()->SetTimeDisplay(1);  // The X axis is a time axis
    h->GetXaxis()->SetTitle("time");
    h->GetYaxis()->SetTitle("rate [counts / minute]");
    h->Draw();
    
    return 0;

}


