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


void testslope(int stats, double shift_factor, string f_1, string f_2)
{ 


    string f_e = ".root";
    TCanvas *c1 = new TCanvas("c1");

  int offset = 5;

  TH1F * htally = new TH1F("htally","",2000,0,2);

  int nhistobins = 400;

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

        rebinnedHist2->Fit("myexpo", "", "", 0.5, 3.5);

        rebinnedHist1->SetLineColor(2);
        rebinnedHist1->Draw("same");
         string savePath1 = f_1 + f_e;
         c1->SaveAs(savePath1.c_str());

        TF1* ff2 = rebinnedHist2->GetFunction("myexpo");

        float basep1 = 1.0;
        float shiftp1 = ff2->GetParameter(1);

        cout << "==== ratio is " << basep1 / shiftp1 << endl;
        cout << "param 0 " << ff2->GetParameter(0) << endl;

        htally->Fill(basep1 / shiftp1);
    }

    TCanvas* c2 = new TCanvas("c2tally");
    htally->Draw();
    string savePath2 = f_2 + f_e;
    c2->SaveAs(savePath2.c_str());
}




void Run1 () {

    testslope(1e6, 0.75, "Var_Shape1", "Var_tally1");
    testslope(1e6, 1.0, "Var_Shape2", "Var_tally2");
    testslope(1e6, 1.25, "Var_Shape3", "Var_tally3"); 


}




























   
























