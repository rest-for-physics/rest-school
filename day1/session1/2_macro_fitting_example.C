#include <TCanvas.h>
#include <TH1D.h>
#include <TMath.h>
    
// Define the fit function
double fitf(double* x, double *par){
    double fitval = par[0] * TMath::Exp(-TMath::Power(*x - par[1],2)/(2*TMath::Power(par[2],2))) ;
    return fitval;
}
    

int fit_calibration(){
    
    
    // initialize parameters
    int bincount;
    int run_duration = 0;

	double binw = 60; // bin width in seconds
	int rate_threshold = 10; // rate threshold in counts per binw. If the rate is below, the detector is regarded to be inactive, and the bin is not counted towards the measurement time

    // Load dataframe
    ROOT::RDataFrame df("AnalysisTree", "../data/R01855/R01855_0*root");
    
    // Create histogram
    auto h = df.Histo1D({"h","calibration", 250, 0, 10000},"tckAna_MaxTrackEnergy");

    // Print histogram
    TCanvas* c = new TCanvas();
    h->GetXaxis()->SetTitle("energy [ADC]");
    h->GetYaxis()->SetTitle("counts");
    
    /*
    ROOT::RDF::RNode df2 = ROOT::RDataFrame(0);
    df2 = df.Define("energy","tckAna_MaxTrackEnergy * 0.00145")
    df2 = df2.Define("radius","hitsAna_xMean*hitsAna_xMean + hitsAna_yMean*hitsAna_yMean")
    df2 = df2.Filter("radius<100")
    h = df2.Histo1D({"h","energy",100,0,10},"energy")
    h->Draw()
    */

    // Create a TF1 object using the fitf function. The last three parameters specify the range and the number of parameters for the function.
    TF1 *func = new TF1("fit",fitf,0,10000,3);

    // Set start parameters
    func->SetParameters(200,4000,800);

    // Set parameter limits on parameter 1
    func->SetParLimits(1,0,10000);

    // Give the parameters names.
    func->SetParNames("Constant","Mean_value","Sigma");

    // Call TH1::Fit with the name of the TF1 object.
    h->Fit(func, "L", "", 2000, 6000);

    h->DrawClone();
    return 0;

}


