#include <TH1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TFile.h>
#include <TRandom.h>
#include <TMath.h>
#include <TCanvas.h>
#include <iostream>
#include <string>                                                           
using namespace std;


TH1F *fitf_master = 0;
TGraph *grff = 0;

Double_t fitf(Double_t *x, Double_t *par) {
    return par[0]*grff->Eval(x[0]*par[1]);
}

void tower_PolyDecay(int stats, double sf, string f_1, string f_2) {
     string f_e = ".png";
    TCanvas *c1 = new TCanvas("c1");
    int offset = 5;
    TH1F *htally = new TH1F("htally", "", 2000, 0, 2);
    int nhistobins = 100;

    for (int k = 0; k < 16; k++) {
        TF1 *mypoly = new TF1("mypoly", "90000.0*x^[0]", 0.3, 6);
        mypoly->SetParameters(-3.0);

        gRandom->SetSeed(9939 + offset + k);
        TH1F *h1 = new TH1F("h1", "", nhistobins, 0, 10);
        for (int i = 0; i < stats; i++) {
            h1->Fill(mypoly->GetRandom());
        }

        fitf_master = (TH1F *) h1->Clone("fmaster");
        fitf_master->Smooth(2);

        grff = new TGraph(fitf_master);

        TH1F *h2 = new TH1F("h2", "", nhistobins, 0, 10);
        gRandom->SetSeed(9939 + k);
        for (int j = 0; j < stats / 7; j++) {
            h2->Fill(mypoly->GetRandom() * sf);
        }

        float startFit = 0.5;
        float endFit = 1.3;
        int binsf = h1->FindBin(startFit);
        int binef = h1->FindBin(endFit);

        float amprat = h1->GetBinContent(binsf) / h2->GetBinContent(binsf);
        TF1 *myexpo = new TF1("myexpo", fitf, 0.1, 10, 2);
        
        
          // Seeding with the correct shift value before fitting
          myexpo->SetParameter(1, 0.7);

         // First fit
         h2->Fit("myexpo", "", "", 0.5, 1.2);

          // Retrieve the fit function
          TF1 *f1fit = h2->GetFunction("myexpo");

         // Set the parameter for the second fit
         f1fit->SetParameter(1, 1.0);

         // Second fit
         h2->Fit("myexpo", "", "", 0.5, 1.2);
    
        
        
        h1->SetLineColor(2);
        h1->Draw("same");
	 string savePath1 = f_1 + f_e;
        c1->SaveAs(savePath1.c_str());


        TF1 *ff2 = h2->GetFunction("myexpo");
        float basep1 = 1.0;
        float shiftp1 = ff2->GetParameter(1);

        cout << "==== ratio is " << basep1 / shiftp1 << endl;
        cout << "param 0 " << ff2->GetParameter(0) << endl;

        htally->Fill(basep1 / shiftp1);
    }

    TCanvas *c2 = new TCanvas("c2tally");
    htally->Draw();
     string savePath2 = f_2 + f_e;
    c2->SaveAs(savePath2.c_str());
}



void Run1() {
    tower_PolyDecay(7e5,  0.7,  "Pol_trials_1",  "Pol_tally_trials_1");
    tower_PolyDecay(7e5,  1.0,  "Pol_trials_2",  "Pol_tally_trials_2");
    tower_PolyDecay(7e5,  1.3,  "Pol_trials_3",  "Pol_tally_trials_3");
}




