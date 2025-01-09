#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TH1F.h>
#include <TLatex.h>
#include <string>
#include <TGraphErrors.h>
#include <TGraph.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>
#include <vector>
#include <cmath>
using namespace std;

// Function to add a legend entry for each graph
void AddLegend(TLegend* legend, TGraph* graph, const char* legendText, Color_t color) {
    graph->SetLineColor(color);
    graph->SetMarkerColor(color);
    legend->AddEntry(graph, legendText, "lp");
}

void Mill_Mean_input() {
    const int n = 3;
    double stat_err1a[3] = {0.00364286, 0.00432946, 0.00274414};
    double stat_err1b[3] = {0.00104651, 0.00159867, 0.00138282};
    double stat_err5a[3] = {0.00215766, 0.00221904, 0.00145287};
    double stat_err5b[3] = {0.00104923, 0.00120701, 0.000867462};
    double stat_err10a[3] = {0.00150942, 0.00156117, 0.00107637};
    double stat_err10b[3] = {0.000933898, 0.000851371, 0.000798682};
    double stat_err100a[3] = {0.000513345, 0.000514673, 0.000468671};
    double stat_err100b[3] = {0.000353193, 0.000295203,  0.000246305};

    double RMS_1a[3] = {0.0133/4, 0.02027/4, 0.02753/4};
    double RMS_1b[3] = {0.01287/4, 0.01934/4, 0.08351/4};
    double RMS_5a[3] = {0.005219/4, 0.009144/4, 0.02904/4};
    double RMS_5b[3] = {0.004291/4, 0.00443/4, 0.007866/4};
    double RMS_10a[3] = {0.004665/4, 0.005321/4, 0.008393/4};
    double RMS_10b[3] = {0.003019/4, 0.005956/4, 0.003956/4};
    double RMS_100a[3] = {0.001237/4, 0.001134/4, 0.003294/4};
    double RMS_100b[3] = {0.000594/4, 0.0009473/4, 0.001637/4};

    TGraph* graphs[8];
    graphs[0] = new TGraph(n, stat_err1a, RMS_1a);
    graphs[1] = new TGraph(n, stat_err1b, RMS_1b);
    graphs[2] = new TGraph(n, stat_err5a, RMS_5a);
    graphs[3] = new TGraph(n, stat_err5b, RMS_5b);
    graphs[4] = new TGraph(n, stat_err10a, RMS_10a);
    graphs[5] = new TGraph(n, stat_err10b, RMS_10b);
    graphs[6] = new TGraph(n, stat_err100a, RMS_100a);
    graphs[7] = new TGraph(n, stat_err100b, RMS_100b);

    Color_t colors[8] = {kRed, kBlue, kGreen, kMagenta, kCyan, kOrange, kYellow, kViolet};

    // Create canvas for all graphs
    TCanvas* canvas = new TCanvas("AllGraphs", "Multiple Graphs Stacked with Fixed Ranges", 1200, 800);
    TLegend* legend = new TLegend(0.6, 0.7, 0.9, 0.9);
    legend->SetTextSize(0.025);

    const char* energyRanges[8] = {
        "1M Events (0.5-1.2 GeV)", "1M Events (1.4-3.0 GeV)",
        "5M Events (0.5-1.2 GeV)", "5M Events (1.4-3.0 GeV)",
        "10M Events (0.5-1.2 GeV)", "10M Events (1.4-3.0 GeV)",
        "100M Events (0.5-1.2 GeV)", "100M Events (1.4-3.0 GeV)"
    };

    for (int i = 0; i < 8; i++) {
        graphs[i]->SetMarkerStyle(21);
        graphs[i]->SetMarkerSize(1);
        graphs[i]->SetLineWidth(2);
        graphs[i]->SetLineColor(colors[i]);
        graphs[i]->SetMarkerColor(colors[i]);

        if (i == 0) {
            graphs[i]->Draw("AP");
            graphs[i]->GetXaxis()->SetTitle("Statistical Error");
            graphs[i]->GetYaxis()->SetTitle("RMS");
            graphs[i]->GetXaxis()->SetLimits(-0.07, 0.07);  // Fixed x-axis range
            graphs[i]->SetMinimum(-0.1);             // Fixed y-axis range
            graphs[i]->SetMaximum(0.1);
        } else {
            graphs[i]->Draw("P SAME");
        }

        // Add legend entry
        legend->AddEntry(graphs[i], energyRanges[i], "lp");

        // Add linear fit
        TF1* linearFit = new TF1("linearFit", "[0]*x + [1]", -1, 1);
        graphs[i]->Fit(linearFit, "Q");
        linearFit->SetLineColor(colors[i]);
        linearFit->Draw("SAME");

        // Print fit parameters
        double slope = linearFit->GetParameter(0);
        double intercept = linearFit->GetParameter(1);
        cout << "Graph " << i + 1 << ": " << energyRanges[i] << endl;
        cout << "  Slope: " << slope << " Intercept: " << intercept << endl;
    }

    legend->Draw();
    canvas->Update();
    canvas->SaveAs("Expo_RMS_vs_staterr_stacked_fixed_ranges.root");
}

void Run1() {
    Mill_Mean_input();
}



































































































































































