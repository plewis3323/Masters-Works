#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TH1F.h>
#include <TLatex.h>
#include <string>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>
#include <vector>
#include <cmath>
using namespace std;

// Function to add a legend entry for each graph
void AddLegend(TLegend* legend, TGraphErrors* graph, const char* legendText, Color_t color) {
    graph->SetLineColor(color);
    graph->SetMarkerColor(color);
    legend->AddEntry(graph, legendText, "lp");
}

void Mill_Mean_input() {
    const int n = 3;
    double Input[3] = {0.75, 1.0, 1.25};
    double Means1a[3] = {0.7563, 1.003, 1.237};
    double Means1b[3] = {0.7561, 0.998, 1.226};
    double Means5a[3] = {0.7489, 0.9991, 1.236};
    double Means5b[3] = {0.7496, 1.00, 1.249};
    double Means10a[3] = {0.75, 1.001, 1.234};
    double Means10b[3] = {0.7501, 1.00, 1.248};
    double Means100a[3] = {0.7503, 0.9994, 1.238};
    double Means100b[3] = {0.75, 1.0, 1.25};

    double std_err1a[3] = {0.0133/4, 0.02027/4, 0.02753/4};
    double std_err1b[3] = {0.01287/4, 0.01934/4, 0.08351/4};
    double std_err5a[3] = {0.005219/4, 0.009144/4, 0.02904/4};
    double std_err5b[3] = {0.004291/4, 0.00443/4, 0.007866/4};
    double std_err10a[3] = {0.004665/4, 0.005321/4, 0.008393/4};
    double std_err10b[3] = {0.003019/4, 0.005956/4, 0.003956/4};
    double std_err100a[3] = {0.001237/4, 0.001134/4, 0.003294/4};
    double std_err100b[3] = {0.000594/4, 0.0009473/4, 0.001637/4};

    TGraphErrors* graphs[8];
    graphs[0] = new TGraphErrors(n, Input, Means1a, 0, std_err1a);
    graphs[1] = new TGraphErrors(n, Input, Means1b, 0, std_err1b);
    graphs[2] = new TGraphErrors(n, Input, Means5a, 0, std_err5a);
    graphs[3] = new TGraphErrors(n, Input, Means5b, 0, std_err5b);
    graphs[4] = new TGraphErrors(n, Input, Means10a, 0, std_err10a);
    graphs[5] = new TGraphErrors(n, Input, Means10b, 0, std_err10b);
    graphs[6] = new TGraphErrors(n, Input, Means100a, 0, std_err100a);
    graphs[7] = new TGraphErrors(n, Input, Means100b, 0, std_err100b);

    Color_t colors[8] = {kRed, kBlue, kGreen, kMagenta, kCyan, kOrange, kYellow, kViolet};

    // Create canvas for all graphs
    TCanvas *canvas = new TCanvas("AllGraphs", "Multiple Graphs with Linear Fits", 1200, 800);
    TLegend *legend = new TLegend(0.15, 0.7, 0.35, 0.9);
    legend->SetTextSize(0.02);

    // Define energy ranges for the legend text
    const char* energyRanges[8] = {
        "1M Events (0.5-1.2 GeV)", "1M Events (1.4-3.0 GeV)",
        "5M Events (0.5-1.2 GeV)", "5M Events (1.4-3.0 GeV)",
        "10M Events (0.5-1.2 GeV)", "10M Events (1.4-3.0 GeV)",
        "100M Events (0.5-1.2 GeV)", "100M Events (1.4-3.0 GeV)"
    };

    // Draw each graph with unique color and add linear fit
    for (int i = 0; i < 8; i++) {
        graphs[i]->SetMarkerStyle(21);
        graphs[i]->SetMarkerSize(1);
        graphs[i]->SetLineWidth(2);
        graphs[i]->SetLineColor(colors[i]);
        graphs[i]->SetMarkerColor(colors[i]);
        
        // Draw the first graph to set the axes; others will be overlaid
        if (i == 0) {
            graphs[i]->Draw("AP");
            graphs[i]->GetXaxis()->SetTitle("Input");
            graphs[i]->GetYaxis()->SetTitle("Mean");
        } else {
            graphs[i]->Draw("P SAME");
        }

        // Add legend entry with energy range
        AddLegend(legend, graphs[i], energyRanges[i], colors[i]);

        // Add linear fit to each graph
        TF1 *linearFit = new TF1("linearFit", "pol1", 0.0, 4.0);
        graphs[i]->Fit(linearFit, "Q");
        linearFit->SetLineColor(colors[i]);
        linearFit->Draw("SAME");
    }

    // Draw legend and save canvas
    legend->Draw();
    canvas->Update();
    canvas->SaveAs("Exp_means_fit_with_labels.root");
}

void Run1() {
    Mill_Mean_input();
}






































