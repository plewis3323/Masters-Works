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




TH1F* rebinHist1(TH1F* h1in, int rebin2) {
    cout << "in " << endl;
    long nbinsX = h1in->GetNbinsX();
    TH1F* h1out = 0;

    vector<float> binsb;
    vector<float> binct;
    vector<float> bincte;
    vector<float> binwt;

    int currentBin = 1;
    float binWidth = h1in->GetBinWidth(1);

    while (currentBin <= nbinsX) {
        float binContent = 0;
        float binError = 0;
        float weight = 0;
        float binStart = h1in->GetBinLowEdge(currentBin);

        while (currentBin <= nbinsX && h1in->GetBinLowEdge(currentBin) < binStart + binWidth) {
            binContent += h1in->GetBinContent(currentBin);
            binError += pow(h1in->GetBinError(currentBin), 2);
            weight += 1;
            currentBin++;
        }

        binct.push_back(binContent);
        bincte.push_back(sqrt(binError));
        binwt.push_back(weight);
        binsb.push_back(binStart + binWidth / 2);
        binWidth += h1in->GetBinWidth(currentBin);
    }

    TString nRebNm = h1in->GetName();
    nRebNm += "_reb";

    h1out = new TH1F(nRebNm.Data(), "", binsb.size() - 1, binsb.data());
    h1out->Sumw2();

    for (size_t ibk = 0; ibk < binsb.size() - 1; ibk++) {
        if (rebin2 >= 0) {
            if (binct[ibk] > 0 && binwt[ibk] > 0) {
                h1out->SetBinContent(ibk + 1, binct[ibk] / binwt[ibk]);
                h1out->SetBinError(ibk + 1, bincte[ibk] / binwt[ibk]);
            }
        } else {
            for (int subi = 0; subi < binct[ibk]; subi++) {
                h1out->Fill(h1out->GetBinCenter(ibk + 1), 1.0 / binwt[ibk]);
            }
        }
    }

    return h1out;
}

TH1F * fitf_master = 0;
TGraph * grff = 0;

Double_t fitf(Double_t *x,Double_t *par)
{

  return par[0]*grff->Eval(x[0]*par[1]);
  
}


void testslope(int stats, double shift_factor, double startR, double EndR, TCanvas* histCanvas, TCanvas* tallyCanvas, TCanvas* tallyCanvas2)
{
    int offset = 5;

    // Load the histogram from the hardcoded input file
    TFile* file = TFile::Open("l3all_oe.root");
    if (!file || file->IsZombie()) {
        cout << "Failed to open the input file: l3all_oe.root" << endl;
        return;
    }
    TH1F* histo = (TH1F*)file->Get("hcalout_eta_24");
    if (!histo) {
        cout << "Failed to get the histogram from: l3all_oe.root" << endl;
        return;
    }


    fitf_master = (TH1F*)histo->Clone("fmaster");
    fitf_master->Smooth(2);
    grff = new TGraph(fitf_master);

    TH1F* htally = new TH1F("htally", "", 2000, 0, 2);
    TH1F* htally2 = new TH1F("htally2", "", 2000, 0, 2);
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

        TH1F* rebinnedHist1 = rebinHist1(h1, 10);
        fitf_master = (TH1F*)rebinnedHist1->Clone("fmaster");
        fitf_master->Smooth(2);
        grff = new TGraph(fitf_master);

        TH1F* h2 = new TH1F("h2", "", nhistobins, 0, 10);
        for (int j = 0; j < stats / 7; j++) {
            h2->Fill(histo->GetRandom() * shift_factor);
        }

        TH1F* rebinnedHist2 = rebinHist1(h2, 10);
        TF1* myexpo = new TF1("myexpo", fitf, 0.1, 10, 2);
        myexpo->SetParameters(1e4, 1.0);
        rebinnedHist1->SetLineColor(2);
        rebinnedHist1->Draw("same");
        rebinnedHist2->Draw("same");
        rebinnedHist2->Fit("myexpo", "", "", startR, EndR);

        TF1* ff2 = rebinnedHist2->GetFunction("myexpo");

        shift_p1_measure += ff2->GetParameter(1);
        U_shift_p1_measure += ff2->GetParError(1);
        cout << "Iteration " << k << ": p_1shift = " << ff2->GetParameter(1) << " ± " << ff2->GetParError(1) << endl;
        htally->Fill(ff2->GetParError(1));
        htally2->Fill(1.0/(ff2->GetParameter(1)));
    }

    double final_shift_p1 = shift_p1_measure / 16.0; 
    double U_final_shift_p1 = U_shift_p1_measure / 16.0; 
    cout << "The mean p1_shift = " << final_shift_p1 << " +/- " << U_final_shift_p1 << endl; 

    tallyCanvas->cd();
    htally->Draw();
    tallyCanvas2->cd();
    htally2->Draw();
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
    TCanvas* c1b = new TCanvas("c1btally");
    TCanvas* c2b = new TCanvas("c2btally");
    TCanvas* c3b = new TCanvas("c3btally");
    TCanvas* c4b = new TCanvas("c4btally");
    TCanvas* c5b = new TCanvas("c5btally");
    TCanvas* c6b = new TCanvas("c6btally");

    // Struct to hold testslope parameters
    struct TestRun {
        double shift_factor;
        double startR;
        double EndR;
        TCanvas* histCanvas;
        TCanvas* tallyCanvas;
        TCanvas* tallyCanvas2;
    };

    // Array of test configurations
    TestRun testRuns[] = {
        {0.75, 0.5, 1.2, c1, c1a, c1b},
        {1.0,  0.5, 1.2, c2, c2a, c2b},
        {1.25, 0.5, 1.2, c3, c3a, c3b},
        {0.75, 1.4, 3.0, c4, c4a, c4b},
        {1.0,  1.4, 3.0, c5, c5a, c5b},
        {1.25, 1.4, 3.0, c6, c6a, c6b}
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
                  testRuns[l].histCanvas, testRuns[l].tallyCanvas, testRuns[l].tallyCanvas2);
    }

    // Save all canvases to the output ROOT file
    TFile* outFile = new TFile(output_file.c_str(), "RECREATE");
    for (int p = 0; p < 6; p++) {
        testRuns[p].histCanvas->Write();
        testRuns[p].tallyCanvas->Write();
        testRuns[p].tallyCanvas2->Write();
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
       
       
       




































































































































