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
    double Means1a[3] =  {0.8456, 0.9856, 1.091};
    double Means1b[3] =  {0.8735, 1.025, 1.152};
    double Means5a[3] =  {0.8416, 0.9987, 1.079};
    double Means5b[3] =  {0.8661, 1.028, 1.217};
    double Means10a[3] =  {0.84, 0.9973, 1.083};
    double Means10b[3] =  {0.8697, 0.987, 1.212};
    double Means100a[3] =  {0.8373, 1.004, 1.085};
    double Means100b[3] =  {0.87, 0.9898, 1.209};

    double Input[3]  = {0.75, 1.0, 1.25};

    double std_err1a[3] = {0.02451/4.0, 0.04408/4.0, 0.04332/4.0};
    double std_err1b[3] = {0.08718/4.0, 0.1812/4.0, 0.1529/4.0};
    double std_err5a[3] = {0.01516/4.0, 0.02064/4.0, 0.02069/4.0};
    double std_err5b[3] = {0.02853/4.0, 0.06441/4.0, 0.07561/4.0};
    double std_err10a[3] = {0.01122/4.0, 0.009633/4.0, 0.007666/4.0};
    double std_err10b[3] = {0.003154/4.0, 0.03499/4.0, 0.04906/4.0};
    double std_err100a[3] = {0.00264/4.0, 0.002397/4.0, 0.0042/4.0};
    double std_err100b[3] = {0.0009493/4.0, 0.01597/4.0, 0.05799/4.0};
    
    
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
        TF1 *linearFit = new TF1("linearFit", "pol1", 0.0, 2.0);
        graphs[i]->Fit(linearFit, "R");
        linearFit->SetLineColor(colors[i]);
        linearFit->Draw("SAME");
    }

    // Draw legend and save canvas
    legend->Draw();
    canvas->Update();
    canvas->SaveAs("l3_rebin_mean_inp.root");
}

void Run1() {
    Mill_Mean_input();
}














