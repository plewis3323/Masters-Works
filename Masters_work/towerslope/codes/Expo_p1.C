#include <TH1.h>
#include <TGraph.h>
#include <TFile.h>
#include <iostream>
#include <cmath>
#include <string>
#include <TGraphErrors.h>
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

    histCanvas->cd(); // Use provided canvas for histograms

    for (int k = 0; k < 16; k++) {
        TF1* myexp = new TF1("myexp", "90000.0*exp([0]+[1]*x)", 0.3, 6);
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

        TF1* myexpo = new TF1("myexpo", fitf, 0.1, 10, 2);
        myexpo->SetParameters(stats / 7.0, 1.0);
        h2->Fit("myexpo", "", "", startR, EndR);
        h1->Fit("myexpo", "", "", startR, EndR);
        h1->SetLineColor(2);
        h2->SetLineColor(3);
        h1->Draw("same");
        h2->Draw("same");
        TF1* ff2 = h2->GetFunction("myexpo");
        float shiftp1 = ff2->GetParameter(1);
        cout << "==== ratio is " << 1.0 / shiftp1 << endl;
        cout << "Iteration " << k << ": p_1shift = " << shiftp1 << " ± " << ff2->GetParError(1) << endl;
        htally->Fill(ff2->GetParError(1));
    }

    tallyCanvas->cd();
    htally->Draw();
}

void Run1(int stats1, const string& output_file)
{
    TCanvas* c1 = new TCanvas("c1");
    TCanvas* c2 = new TCanvas("c2");
    TCanvas* c3 = new TCanvas("c3");
    TCanvas* c1a = new TCanvas("c1tally");
    TCanvas* c2a = new TCanvas("c2tally");
    TCanvas* c3a = new TCanvas("c3tally");
    TCanvas* c4 = new TCanvas("c4");
    TCanvas* c5 = new TCanvas("c5");
    TCanvas* c6 = new TCanvas("c6");
    TCanvas* c4a = new TCanvas("c4tally");
    TCanvas* c5a = new TCanvas("c5tally");
    TCanvas* c6a = new TCanvas("c6tally");

    // Call testslope with corrected arguments
    testslope(stats1, 0.75, 0.5, 1.2, c1, c1a);
    testslope(stats1, 1.0, 0.5, 1.2, c2, c2a);
    testslope(stats1, 1.25, 0.5, 1.2, c3, c3a);
    testslope(stats1, 0.75, 1.4, 3.0, c4, c4a);
    testslope(stats1, 1.0, 1.4, 3.0, c5, c5a);
    testslope(stats1, 1.25, 1.4, 3.0, c6, c6a);

    // Save canvases to the output file
    TFile* outFile = new TFile(output_file.c_str(), "RECREATE");
    c1->Write();
    c2->Write();
    c3->Write();
    c1a->Write();
    c2a->Write();
    c3a->Write();
    c4->Write();
    c5->Write();
    c6->Write();
    c4a->Write();
    c5a->Write();
    c6a->Write();
    outFile->Close();
}















/*
        vector<double> C_rel_values;
        vector<double> C_rel_uncertainties;
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
        htally->Fill(sigma_shiftp1);  // Fill htally with each C_rel
        cout << "Iteration " << k << ": p_1shift = " << shiftp1  << " ± " << sigma_shiftp1  << endl;
       */
       















































































































