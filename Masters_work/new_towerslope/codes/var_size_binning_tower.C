#include <TH1.h>
#include <TGraph.h>
#include <TFile.h>
#include <iostream>
#include <string>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <vector>
#include <string>
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


void testslope(int stats, double shift_factor, double startR, double EndR, TCanvas* histCanvas, TCanvas* tallyCanvas)
{ 



  int offset = 5;

  TH1F * htally = new TH1F("htally","",2000,0,2);

  int nhistobins = 400;
  histCanvas->cd(); // Use provided canvas for histograms

  for (int k = 0; k < 16; k++) {

  TF1 * myexp = new TF1("myexpNot","[0]/([1]+x)^[2]",0.3,6);
  myexp->SetParameters(1e8,1.1,6);
  gRandom->SetSeed(9939 + offset + k);
  TH1F * h1 = new TH1F("h1","",nhistobins,0,10);

  for (int i = 0; i<stats; i++)
    {

     

      h1->Fill( myexp->GetRandom() );     

    }

  fitf_master = (TH1F *) h1->Clone("fmaster");
  
  fitf_master->Smooth(2);

  grff = new TGraph(fitf_master);
 

  TH1F * h2 = new TH1F("h2","",nhistobins,0,10);

  gRandom->SetSeed(9939 + k);
  
  
  for (int j = 0; j<stats / 7; j++)
    {
      
      h2->Fill( myexp->GetRandom()*shift_factor);     
      

    }

     // Rebin the histograms
        TH1F* rebinnedHist1 = rebinHist1(h1,10);
        TH1F* rebinnedHist2 = rebinHist1(h2,10);

        float startFit = 0.5;
        float endFit = 1.3;

        int binsf = rebinnedHist1->FindBin(startFit);
        int binef = rebinnedHist1->FindBin(endFit);
        
        

        float amprat1 = rebinnedHist1->GetBinContent(binsf);
        float amprat2 = rebinnedHist2->GetBinContent(binsf);  
        float amprat = amprat1/amprat2;
      

        TF1* myexpo = new TF1("myexpo", fitf, 0.1, 10, 2);
        myexpo->SetParameters(1e4, 1.0);
        rebinnedHist2->Fit("myexpo", "", "", startR, EndR);
        rebinnedHist1->SetLineColor(2);
        rebinnedHist1->Draw("same");
        TF1* ff2 = rebinnedHist2->GetFunction("myexpo");

        float basep1 = 1.0;
        float shiftp1 = ff2->GetParameter(1);
   
        cout << "==== ratio is " << basep1 / shiftp1 << endl;
        cout << "param 0 " << ff2->GetParameter(0) << endl;
        cout << "Iteration " << k << ": p_1shift = " << shiftp1 << " ± " << ff2->GetParError(1) << endl;
       // htally->Fill(ff2->GetParError(1));
         htally->Fill(basep1 / shiftp1);
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


























   
























