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
    TH1F* htally = new TH1F("htally", "", 2000, 0, 2);
    int nhistobins = 400;

    // Use provided canvas for histograms
    histCanvas->cd();
   
    double shift_p1_measure = 0.0; 
    double U_shift_p1_measure = 0.0;
    for (int k = 0; k < 16; k++) {
        TF1 *myexp = new TF1("myexp", "90000.0*exp([0]+[1]*x)", 0.3, 6);
        myexp->SetParameters(0, -1.0);
        gRandom->SetSeed(9939 + offset + k);
        TH1F* h1 = new TH1F("h1", "", nhistobins, 0, 10);
        for (int i = 0; i < stats; i++) {
            h1->Fill(myexp->GetRandom());
        }

        fitf_master = (TH1F*)h1->Clone("fmaster");
        fitf_master->Smooth(2);
        grff = new TGraph(fitf_master);

        TH1F* h2 = new TH1F("h2", "", nhistobins, 0, 10);
        gRandom->SetSeed(9939 + k);
        for (int j = 0; j < stats / 7; j++) {
            h2->Fill(myexp->GetRandom() * shift_factor);
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
        "100Mill_0.75_LR", "100Mill_1.0_LR", "100Mill_1.25_LR",
        "100Mill_0.75_HR", "100Mill_1.0_HR", "100Mill_1.25_HR"
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
















































































































































































































































































