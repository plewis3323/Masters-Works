#include <TH1.h>
#include <TGraph.h>
#include <TFile.h>
#include <iostream>
#include <cmath>
#include <string>
#include <TGraphErrors.h>
#include <TString.h>
#include <TF1.h>
#include <TRandom.h>
#include <TFitResultPtr.h>
#include <TMatrixDSym.h>
#include <TCanvas.h>
#include <vector>

using namespace std;

TH1F* fitf_master = 0;
TGraph* grff = 0;

Double_t fitf(Double_t* x, Double_t* par)
{
    return par[0] * grff->Eval(x[0] * par[1]);
}

void testslope(int stats, double shift_factor, double startR, double EndR, TCanvas* histCanvas, TCanvas* tallyCanvas)
{
    int offset = 5;

    // Load the histogram from the hardcoded input file
    TFile* file = TFile::Open("l3all_oe.root");
    if (!file || file->IsZombie()) {
        cout << "Failed to open the input file: l3all_oe.root" << endl;
        return;
    }
    TH1F* histo = (TH1F*)file->Get("hcalout_eta_24"); // Use the actual histogram name from the file
    if (!histo) {
        cout << "Failed to get the histogram from: l3all_oe.root" << endl;
        return;
    }

    // Rebin the histogram to reduce the number of bins
    int originalBins = histo->GetNbinsX();
    int targetBins = 100000;
    int rebinFactor = originalBins / targetBins;
    histo->Rebin(rebinFactor);

    fitf_master = (TH1F*)histo->Clone("fmaster");
    fitf_master->Smooth(2);

    grff = new TGraph(fitf_master);

    TH1F* htally = new TH1F("htally", "", 2000, 0, 2);
    int nhistobins = 400;

    // Use provided canvas for histograms
    histCanvas->cd();
   
   
   
    double shift_p1_measure = 0.0; 
    double U_shift_p1_measure = 0.0;
    for (int k = 0; k < 16; k++) {
        gRandom->SetSeed(9939 + offset + k);

        TH1F* h1 = new TH1F("h1", "", nhistobins, 0, 10);
        for (int i = 0; i < stats; i++) {
            h1->Fill(histo->GetRandom());
        }

        fitf_master = (TH1F*)h1->Clone("fmaster");
        fitf_master->Smooth(2);

        grff = new TGraph(fitf_master);

        TH1F* h2 = new TH1F("h2", "", nhistobins, 0, 10);
        gRandom->SetSeed(9939 + k);
        for (int j = 0; j < stats / 7; j++) {
            h2->Fill(histo->GetRandom() * shift_factor);
        }
        
        float startFit = 0.5;
        float endFit = 1.3;
        int binsf = h1->FindBin(startFit);
        int binef = h1->FindBin(endFit);
        float amprat = h1->GetBinContent(binsf) / h2->GetBinContent(binsf);


        TF1* myexpo = new TF1("myexpo", fitf, 0.1, 10, 2);
        myexpo->SetParameters(1e4, 1.0);
        h1->SetLineColor(2);
        h1->Draw("same");
        h2->Draw("same");
        h2->Fit("myexpo", "", "", startR, EndR);
        TF1* ff2 = h2->GetFunction("myexpo");
        shift_p1_measure += ff2->GetParameter(1);
        U_shift_p1_measure += ff2->GetParError(1);
        double shiftp1 = ff2->GetParameter(1);
        cout << "==== ratio is " << 1.0 / shiftp1 << endl;
        cout << "Iteration " << k << ": p_1shift = " << shiftp1  << " ± " << ff2->GetParError(1)  << endl;
        htally->Fill(ff2->GetParError(1));
        
        
    }
    
     double final_shift_p1 = shift_p1_measure/16.0; 
     double U_final_shift_p1 = U_shift_p1_measure/16.0; 
     cout << "The mean p1_shift = " << final_shift_p1 << " +/- " << U_final_shift_p1 << endl; 
     tallyCanvas->cd();
     htally->Draw();
    
}



void Run1(int stats, const string& output_file)
{
    // Create canvases
    TCanvas* c1 = new TCanvas("c1");
    TCanvas* c2 = new TCanvas("c2");
    TCanvas* c3 = new TCanvas("c3");
    TCanvas* c4 = new TCanvas("c4");
    TCanvas* c5 = new TCanvas("c5");
    TCanvas* c6 = new TCanvas("c6");
    TCanvas* c1a = new TCanvas("c1tally");
    TCanvas* c2a = new TCanvas("c2tally");
    TCanvas* c3a = new TCanvas("c3tally");
    TCanvas* c4a = new TCanvas("c4tally");
    TCanvas* c5a = new TCanvas("c5tally");
    TCanvas* c6a = new TCanvas("c6tally");

    // Struct to hold testslope parameters
    struct TestRun {
        double shift_factor;
        double startR;
        double EndR;
        TCanvas* histCanvas;
        TCanvas* tallyCanvas;
    };

    // Array of test configurations
    TestRun testRuns[] = {
        {0.75, 0.5, 1.2, c1, c1a},
        {1.0,  0.5, 1.2, c2, c2a},
        {1.25, 0.5, 1.2, c3, c3a},
        {0.75, 1.4, 3.0, c4, c4a},
        {1.0,  1.4, 3.0, c5, c5a},
        {1.25, 1.4, 3.0, c6, c6a}
    };

    // Event labels for readability
    string Event_S[6] = {
        "1Mill_0.75_LR", "1Mill_1.0_LR", "1Mill_1.25_LR",
        "1Mill_0.75_HR", "1Mill_1.0_HR", "1Mill_1.25_HR"
    };

    // Loop through the test configurations
    for (int l = 0; l < 6; l++) {
        cout << "Data Set Run: " << Event_S[l] << endl;
        cout << "---------------------------------------------------------------------" << endl;

        // Call testslope with the parameters from the struct
        testslope(stats, testRuns[l].shift_factor, testRuns[l].startR, testRuns[l].EndR,
                  testRuns[l].histCanvas, testRuns[l].tallyCanvas);
    }

    // Save all canvases to the output ROOT file
    TFile* outFile = new TFile(output_file.c_str(), "RECREATE");
    for (int p = 0; p < 6; p++) {
        testRuns[p].histCanvas->Write();
        testRuns[p].tallyCanvas->Write();
    }
    outFile->Close();
}



  
     /*
        // Check if the fits were successful
        if (fitResult1.Get() && fitResult2.Get()) {
            double basep1 = fitResult1->Parameter(1);
            double shiftp1 = fitResult2->Parameter(1);

            double sigma_basep1 = fitResult1->ParError(1);
            double sigma_shiftp1 = fitResult2->ParError(1);

            // Calculate C_rel and its uncertainty for this iteration
            double C_rel = basep1 / shiftp1;
            double sigma_C_rel = C_rel * sqrt(
                pow(sigma_basep1 / basep1, 2) +
                pow(sigma_shiftp1 / shiftp1, 2)
            );

            // Store the calculated C_rel and uncertainty
            C_rel_values.push_back(C_rel);
            C_rel_uncertainties.push_back(sigma_C_rel);

            cout << "Iteration " << k << ": C_rel = " << C_rel << " ± " << sigma_C_rel << endl;

            htally->Fill(sigma_C_rel);  // Fill htally with each C_rel
        } else {
            cout << "Warning: Fit failed, skipping this iteration." << endl;
        }
       */
       
       
       /*
        double basep1 = fitResult1->Parameter(1);
        double shiftp1 = fitResult2->Parameter(1);
        double sigma_basep1 = fitResult1->ParError(1);
        double sigma_shiftp1 = fitResult2->ParError(1);
        htally->Fill(basep1/shiftp1);  // Fill htally with each C_rel
        cout << "Iteration " << k << ": p_1shift = " << shiftp1  << " ± " << sigma_shiftp1  << endl;
       */ 
       
       
       


























































































































































































































































































































































































































































































































































































































