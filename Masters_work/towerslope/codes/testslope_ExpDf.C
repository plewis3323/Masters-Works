#include <TH1.h>
#include <TGraph.h>
#include <TFile.h>
#include <iostream>
#include <string>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <iostream>
#include <string>
using namespace std;

TH1F *fitf_master = 0;
TGraph *grff = 0;

Double_t fitf(Double_t *x, Double_t *par) {
    return par[0]*grff->Eval(x[0]*par[1]);
}

void testslope(int stats, double shift_factor, string f_1, string f_2, double startR, double EndR, TCanvas* histCanvas, TCanvas* tallyCanvas) {
    int offset = 5;
    TH1F *htally = new TH1F("htally", "", 2000, 0, 2);
    int nhistobins = 400;

    histCanvas->cd();  // Use provided canvas for histograms

    for (int k = 0; k < 16; k++) {
        TF1 *myexp = new TF1("myexp", "90000.0*exp([0]+[1]*x)", 0.3, 6);
        myexp->SetParameters(0, -1.0);

        gRandom->SetSeed(9939 + offset + k);
        TH1F *h1 = new TH1F("h1", "", nhistobins, 0, 10);
        for (int i = 0; i < stats; i++) {
            h1->Fill(myexp->GetRandom());
        }

        fitf_master = (TH1F *) h1->Clone("fmaster");
        fitf_master->Smooth(2);

        grff = new TGraph(fitf_master);

        TH1F *h2 = new TH1F("h2", "", nhistobins, 0, 10);
        gRandom->SetSeed(9939 + k);
        for (int j = 0; j < stats / 7; j++) {
            h2->Fill(myexp->GetRandom() * shift_factor);
        }

        float startFit = 0.5;
        float endFit = 1.3;
        int binsf = h1->FindBin(startFit);
        int binef = h1->FindBin(endFit);

        float amprat = h1->GetBinContent(binsf) / h2->GetBinContent(binsf);

        TF1 *myexpo = new TF1("myexpo", fitf, 0.1, 10, 2);
        myexpo->SetParameters(stats / 7.0, 1.0);

        h2->Fit(myexpo, "", "", startR, EndR);
        h1->SetLineColor(2);
        h1->Draw("same");

        TF1 *ff2 = h2->GetFunction("myexpo");
        float basep1 = 1.0;
        float shiftp1 = ff2->GetParameter(1);

        cout << "==== ratio is " << basep1 / shiftp1 << endl;
        cout << "param 0 " << ff2->GetParameter(0) << endl;

        htally->Fill(basep1 / shiftp1);
    }

    tallyCanvas->cd();  // Use provided canvas for tally
    htally->Draw();
    htally->Write();  // Save tally histogram to file
}




    
void mill () { 
    //Canvas for reference histograms 
    TCanvas *c1 = new TCanvas("c1");
    TCanvas *c2 = new TCanvas("c2");
    TCanvas *c3 = new TCanvas("c3");
    
    //Canvas for reference histograms 
    TCanvas *c4 = new TCanvas("c4");
    TCanvas *c5 = new TCanvas("c5");
    TCanvas *c6 = new TCanvas("c6");
    
    //Canvas for tally histograms 
    TCanvas* c1a = new TCanvas("c2tally");
    TCanvas* c2a = new TCanvas("c2tally2");
    TCanvas* c3a = new TCanvas("c2tally3");
    
    //Canvas for tally histograms 
    TCanvas* c4b = new TCanvas("c2tally4");
    TCanvas* c5b = new TCanvas("c2tally5");
    TCanvas* c6b = new TCanvas("c2tally6");
    
    testslope(1e6, 0.75, "expo_1", "expo_tally1", 0.5, 1.2, c1, c1a);
    testslope(1e6, 1.0, "expo_Shape1", "expo_tally1", 0.5, 1.2, c2, c2a);
    testslope(1e6, 1.25, "expo_Shape1", "expo_tally1", 0.5, 1.2, c3, c3a);
    testslope(1e6, 0.75, "expo_Shape1", "expo_tally1", 1.4, 3.0, c4, c4b);
    testslope(1e6, 1.0, "expo_Shape1", "expo_tally1", 1.4, 3.0, c5, c5b);
    testslope(1e6, 1.25, "expo_Shape1", "expo_tally1", 1.4, 3.0, c6, c6b);
    
    TFile *expo_data = new TFile("expo_mill_events.root", "RECREATE"); 
    
    c1->Write(); 
    c2->Write();
    c3->Write(); 
    c4->Write(); 
    c5->Write(); 
    c6->Write(); 
    c1a->Write(); 
    c2a->Write(); 
    c3a->Write(); 
    c4b->Write(); 
    c5b->Write(); 
    c6b->Write(); 
    
    expo_data->Close(); 
}

void ten_mill () { 
    //Canvas for reference histograms 
    TCanvas *c1 = new TCanvas("c1");
    TCanvas *c2 = new TCanvas("c2");
    TCanvas *c3 = new TCanvas("c3");
    
    //Canvas for reference histograms 
    TCanvas *c4 = new TCanvas("c4");
    TCanvas *c5 = new TCanvas("c5");
    TCanvas *c6 = new TCanvas("c6");
    
    //Canvas for tally histograms 
    TCanvas* c1a = new TCanvas("c2tally");
    TCanvas* c2a = new TCanvas("c2tally2");
    TCanvas* c3a = new TCanvas("c2tally3");
    
    //Canvas for tally histograms 
    TCanvas* c4b = new TCanvas("c2tally4");
    TCanvas* c5b = new TCanvas("c2tally5");
    TCanvas* c6b = new TCanvas("c2tally6");
    
    testslope(1e7, 0.75, "expo_Shape1", "expo_tally1", 0.5, 1.2, c1, c1a);
    testslope(1e7, 1.0, "expo_Shape1", "expo_tally1", 0.5, 1.2, c2, c2a);
    testslope(1e7, 1.25, "expo_Shape1", "expo_tally1", 0.5, 1.2, c3, c3a);
    testslope(1e7, 0.75, "expo_Shape1", "expo_tally1", 1.4, 3.0, c4, c4b);
    testslope(1e7, 1.0, "expo_Shape1", "expo_tally1", 1.4, 3.0, c5, c5b);
    testslope(1e7, 1.25, "expo_Shape1", "expo_tally1", 1.4, 3.0, c6, c6b);
    
    TFile *expo_data2 = new TFile("expo_ten_mill_events.root", "RECREATE"); 
    
    c1->Write(); 
    c2->Write();
    c3->Write(); 
    c4->Write(); 
    c5->Write(); 
    c6->Write(); 
    c1a->Write(); 
    c2a->Write(); 
    c3a->Write(); 
    c4b->Write(); 
    c5b->Write(); 
    c6b->Write(); 
    
    expo_data2->Close(); 
}

void hun_mill_low () { 
    //Canvas for reference histograms 
    TCanvas *c1 = new TCanvas("c1");
    TCanvas *c2 = new TCanvas("c2");
    TCanvas *c3 = new TCanvas("c3");
    
    //Canvas for tally histograms 
    TCanvas* c1a = new TCanvas("c2tally");
    TCanvas* c2a = new TCanvas("c2tally2");
    TCanvas* c3a = new TCanvas("c2tally3");
    
    testslope(1e8, 0.75, "expo_Shape1", "expo_tally1", 0.5, 1.2, c1, c1a);
    testslope(1e8, 1.0, "expo_Shape1", "expo_tally1", 0.5, 1.2, c2, c2a);
    testslope(1e8, 1.25, "expo_Shape1", "expo_tally1", 0.5, 1.2, c3, c3a);
    
    TFile *expo_data3 = new TFile("expo_mill_eventsL.root", "RECREATE"); 
    
    c1->Write(); 
    c2->Write();
    c3->Write(); 
    c1a->Write(); 
    c2a->Write(); 
    c3a->Write(); 
    
    expo_data3->Close(); 
}

void hun_mill_high() {
    //Canvas for reference histograms 
    TCanvas *c4 = new TCanvas("c4");
    TCanvas *c5 = new TCanvas("c5");
    TCanvas *c6 = new TCanvas("c6");
    
    //Canvas for tally histograms 
    TCanvas* c4b = new TCanvas("c2tally4");
    TCanvas* c5b = new TCanvas("c2tally5");
    TCanvas* c6b = new TCanvas("c2tally6");
    
    testslope(1e8, 0.75, "expo_Shape1", "expo_tally1", 1.4, 3.0, c4, c4b);
    testslope(1e8, 1.0, "expo_Shape1", "expo_tally1", 1.4, 3.0, c5, c5b);
    testslope(1e8, 1.25, "expo_Shape1", "expo_tally1", 1.4, 3.0, c6, c6b);
    
    TFile *expo_data4 = new TFile("expo_mill_eventsH.root", "RECREATE"); 

    c4->Write(); 
    c5->Write(); 
    c6->Write(); 
    c4b->Write(); 
    c5b->Write(); 
    c6b->Write(); 

    expo_data4->Close();
}

void five_mill () { 
    //Canvas for reference histograms 
    TCanvas *c1 = new TCanvas("c1");
    TCanvas *c2 = new TCanvas("c2");
    TCanvas *c3 = new TCanvas("c3");
    
    //Canvas for reference histograms 
    TCanvas *c4 = new TCanvas("c4");
    TCanvas *c5 = new TCanvas("c5");
    TCanvas *c6 = new TCanvas("c6");
    
    //Canvas for tally histograms 
    TCanvas* c1a = new TCanvas("c2tally");
    TCanvas* c2a = new TCanvas("c2tally2");
    TCanvas* c3a = new TCanvas("c2tally3");
    
    //Canvas for tally histograms 
    TCanvas* c4b = new TCanvas("c2tally4");
    TCanvas* c5b = new TCanvas("c2tally5");
    TCanvas* c6b = new TCanvas("c2tally6");
    
    testslope(5e6, 0.75, "expo_Shape1", "expo_tally1", 0.5, 1.2, c1, c1a);
    testslope(5e6, 1.0, "expo_Shape1", "expo_tally1", 0.5, 1.2, c2, c2a);
    testslope(5e6, 1.25, "expo_Shape1", "expo_tally1", 0.5, 1.2, c3, c3a);
    testslope(5e6, 0.75, "expo_Shape1", "expo_tally1", 1.4, 3.0, c4, c4b);
    testslope(5e6, 1.0, "expo_Shape1", "expo_tally1", 1.4, 3.0, c5, c5b);
    testslope(5e6, 1.25, "expo_Shape1", "expo_tally1", 1.4, 3.0, c6, c6b);
    
    TFile *expo_dataA = new TFile("expo_five_mill_events.root", "RECREATE"); 
    
    c1->Write(); 
    c2->Write();
    c3->Write(); 
    c4->Write(); 
    c5->Write(); 
    c6->Write(); 
    c1a->Write(); 
    c2a->Write(); 
    c3a->Write(); 
    c4b->Write(); 
    c5b->Write(); 
    c6b->Write(); 
    
    expo_dataA->Close(); 
}












