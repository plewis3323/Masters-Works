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

void RMS_input() {                                                                                                                                                                                                                                                            
    const int n = 3;
    double Input[3] = {0.75, 1.0, 1.25};
    double std_err1a[3] = {0.01199, 0.07336, 0.2598};
    double std_err1b[3] = {0.1338, 0.133, 0.1299};
    double std_err5a[3] = {0.006538, 0.1445, 0.2616};
    double std_err5b[3] = {0.1037, 0.115, 0.07619};
    double std_err10a[3] = {0.006461, 0.1311, 0.2644};
    double std_err10b[3] = {0.03888, 0.05238, 0.1276};
    double std_err100a[3] = {0.001687, 0.001378, 0.1336};
    double std_err100b[3] = {0.0219, 0.06546, 0.1934};
   

    TGraph* graphs[8];
    graphs[0] = new TGraph(n, Input, std_err1a);
    graphs[1] = new TGraph(n, Input,  std_err1b);
    graphs[2] = new TGraph(n, Input,  std_err5a);
    graphs[3] = new TGraph(n, Input,  std_err5b);
    graphs[4] = new TGraph(n, Input,  std_err10a);
    graphs[5] = new TGraph(n, Input,  std_err10b);
    graphs[6] = new TGraph(n, Input,  std_err100a);
    graphs[7] = new TGraph(n, Input,  std_err100b);

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
            graphs[i]->GetYaxis()->SetTitle("RMS");
        } else {
            graphs[i]->Draw("P SAME");
        }

        // Add legend entry with energy range
        AddLegend(legend, graphs[i], energyRanges[i], colors[i]);
    }

    // Draw legend and save canvas
    legend->Draw();
    canvas->Update();
    canvas->SaveAs("Var_RMS_input.root");
}

void Run1() {
    RMS_input(); 
}






























































































































































































































































































































































































































































