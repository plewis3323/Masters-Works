#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TH1F.h>
#include <TLatex.h>
#include <string>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <TString.h>
#include <vector>
#include <TMatrixD.h>
#include <TVirtualFitter.h>
#include <TLegend.h>
#include <TTree.h>
#include <TText.h>
#include <TLatex.h>
#include <cmath>
using namespace std;

void Mill_Mean_input() {
    const int n = 3;
    double Means1a[3] = {0.7563, 1.003, 1.237};
    double Means1b[3] = {0.7561, 0.998, 1.226};
    double Means5a[3] = {0.7489, 0.9991, 1.236};
    double Means5b[3] = {0.7496, 1.0, 1.249};
    double Means10a[3] = {0.75, 1.001, 1.234};
    double Means10b[3] = {0.7501, 1.0, 1.248};
    double Means100a[3] = {0.7503, 0.9994, 1.238};
    double Means100b[3] = {0.75, 1.0, 1.25};

    double Input[3] = {0.75, 1.0, 1.25};

    double std_err1a[3] = {0.0133 / 4.0, 0.02027 / 4.0, 0.0275 / 4.0};
    double std_err1b[3] = {0.01287 / 4.0, 0.01934 / 4.0, 0.08351 / 4.0};
    double std_err5a[3] = {0.00522 / 4.0, 0.00914 / 4.0, 0.02904 / 4.0};
    double std_err5b[3] = {0.0043 / 4.0, 0.0044 / 4.0, 0.0079 / 4.0};
    double std_err10a[3] = {0.00467 / 4.0, 0.00532 / 4.0, 0.00839 / 4.0};
    double std_err10b[3] = {0.003019 / 4.0, 0.005956 / 4.0, 0.003956 / 4.0};
    double std_err100a[3] = {0.00124 / 4.0, 0.00113 / 4.0, 0.0033 / 4.0};
    double std_err100b[3] = {0.000594 / 4.0, 0.0009473 / 4.0, 0.001637 / 4.0};

    // Create TGraphErrors
    TGraphErrors *graphs[8];
    graphs[0] = new TGraphErrors(n, Input, Means1a, 0, std_err1a);
    graphs[1] = new TGraphErrors(n, Input, Means1b, 0, std_err1b);
    graphs[2] = new TGraphErrors(n, Input, Means5a, 0, std_err5a);
    graphs[3] = new TGraphErrors(n, Input, Means5b, 0, std_err5b);
    graphs[4] = new TGraphErrors(n, Input, Means10a, 0, std_err10a);
    graphs[5] = new TGraphErrors(n, Input, Means10b, 0, std_err10b);
    graphs[6] = new TGraphErrors(n, Input, Means100a, 0, std_err100a);
    graphs[7] = new TGraphErrors(n, Input, Means100b, 0, std_err100b);

    // Customize appearance
    int colors[] = {kRed + 1, kBlue + 1, kGreen + 1, kMagenta + 1, kCyan + 1, kOrange + 1, kRed + 2, kBlue + 2};
    int markers[] = {20, 21, 22, 23, 24, 25, 26, 27};
    int fitColors[] = {kBlack, kGray + 1, kGreen + 2, kMagenta + 2, kCyan + 2, kOrange + 2, kRed, kBlue};

    for (int i = 0; i < 8; ++i) {
        graphs[i]->SetMarkerStyle(markers[i]);
        graphs[i]->SetMarkerSize(1.0);
        graphs[i]->SetMarkerColor(colors[i]);
        graphs[i]->GetXaxis()->SetTitle("Input (Decal)");
        graphs[i]->GetYaxis()->SetTitle("Mean (Decal)");
        graphs[i]->SetTitle("Plot of Mean Decal and Input Decal"); 
    }

    // Open a new root file
    TFile *Mean_inp = new TFile("Dec-13_Expo_Mean_Inp_plot.root", "RECREATE");

    // Create canvas and draw graphs
    TCanvas *canvas = new TCanvas("CombinedCanvas", "All Graphs Combined", 1200, 800);
    canvas->Divide(1, 1);

    canvas->cd(1);
    TLegend *legend = new TLegend(0.1, 0.6, 0.5, 0.9);
    legend->SetTextSize(0.03);

    const char *labels[] = {
        "1M Low Pt (0.5-1.2 GeV)", "1M High Pt (1.2-3.0 GeV)",
        "5M Low Pt (0.5-1.2 GeV)", "5M High Pt (1.2-3.0 GeV)",
        "10M Low Pt (0.5-1.2 GeV)", "10M High Pt (1.2-3.0 GeV)",
        "100M Low Pt (0.5-1.2 GeV)", "100M High Pt (1.2-3.0 GeV)"
    };

    for (int i = 0; i < 8; ++i) {
        graphs[i]->Draw(i == 0 ? "AP" : "P SAME");

        // Fit function
        TF1 *fit = new TF1(Form("fit_%d", i), "[0]*x + [1]", 0.7, 1.3);
        fit->SetLineColor(fitColors[i]);
        graphs[i]->Fit(fit, "R");

        // Add to legend
        legend->AddEntry(fit, Form("Fit: %s", labels[i]), "l");
    }

    legend->Draw();

    // Write the canvas to the file
    canvas->Write();

    // Close the file
    Mean_inp->Close();
}

void Run1() {
    Mill_Mean_input();
}


















