#include <TCanvas.h>
#include <TH1D.h>
#include <TMath.h>
#include "TSpectrum.h"
    
// Define the fit function
double fitf(double* x, double *par){
    double fitval = par[0] * TMath::Exp(-TMath::Power(*x - par[1],2)/(2*TMath::Power(par[2],2))) ;
    return fitval;
}
    

int calibrate_data(){
    
    // Load dataframe
    ROOT::RDataFrame df("AnalysisTree", "../../data/R01855/R01855_0*root");

    // Filter dataframe
    ROOT::RDF::RNode df_filtered = ROOT::RDataFrame(0);

    df_filtered = df.Define("radius2","hitsAna_xMean*hitsAna_xMean + hitsAna_yMean*hitsAna_yMean");
    df_filtered = df_filtered.Filter("radius2 < 100");
    
    // Create histogram
    auto h1 = df_filtered.Histo1D({"h","Raw data", 250, 0, 10000},"tckAna_MaxTrackEnergy");
    auto h = h1.GetValue(); // to convert the RDataFrame ROOT::RDF::RResultPtr<TH1D> to TH1D

    // Searching the peak candidates with TSpectrum
    TSpectrum *s = new TSpectrum();

    int sigma = 10; // a parameter of the peak finder that defines the search sensitivity

    Int_t nfound = s->Search(&h, sigma, "", 0.5); // searches for peaks in current histogram range
    printf("Found %d candidate peaks to fit\n", nfound);

    auto peak = s->GetPositionX(); // resturn the x position of the found peaks

    // Create a TF1 object using the fitf function. The last three parameters specify the range and the number of parameters for the function.
    TF1 *func = new TF1("fit",fitf,0,10000,3);

    // Set start parameters
    func->SetParameters(200,peak[0],peak[0]*0.2);

    // Set parameter limits on parameter 1
    func->SetParLimits(1,0,10000);

    // Give the parameters names.
    func->SetParNames("Constant","Mean_value","Sigma");

    // Call TH1::Fit with the name of the TF1 object.
    auto fitresult = h.Fit(func, "LS", "", 0.5*peak[0], 1.5*peak[0]);
    
    // Draw the histogram and fitted function
    TCanvas* c = new TCanvas();
    h.GetXaxis()->SetTitle("Energy [ADC]");
    h.GetYaxis()->SetTitle("Counts");
    h.DrawClone();
    
    // access the fit result to calculate the calibration factor
    double mean = fitresult->Parameter(1);
    double calfactor = 5.9 / mean;

    // edit the data in the dataframe to calibrate it
    df_filtered = df_filtered.Define("calibratedEnergy", [calfactor](double c){return c*calfactor;},{"tckAna_MaxTrackEnergy"});
    auto h2 = df_filtered.Histo1D({"h2","Calibrated data", 100, 0, 10},"calibratedEnergy");
    
    // create a new plot
    TCanvas* c2 = new TCanvas();
    h2->GetXaxis()->SetTitle("Energy [keV]");
    h2->GetYaxis()->SetTitle("Counts");

    h2->SetLineWidth(2);
    h2->SetLineColor(kBlack);
    h2->SetFillColor(kCyan-7);

    h2->DrawClone();

    // Plot a hitmap of unfiltered data
    auto h3 = df.Histo2D({"h3","Hit map",61,-30,30,61,-30,30},"hitsAna_xMean","hitsAna_yMean");

    TCanvas* c3 = new TCanvas();
    h3->GetXaxis()->SetTitle("x position [mm]");
    h3->GetYaxis()->SetTitle("y position [mm]");

    gStyle->SetPalette(kRainBow);

    h3->DrawClone("COLZ");
    return 0;
}


