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
    double Means1a[3] = {0.7512, 1.005, 1.113};
    double Means1b[3] = {0.7688, 0.7918, 0.8269};
    double Means5a[3] = {0.7538, 0.8946, 1.082};
    double Means5b[3] = {0.7784, 0.8345, 0.8105};
    double Means10a[3] = {0.7536, 0.9317, 0.9849};
    double Means10b[3] = {0.7823, 0.8463, 0.8765};
    double Means100a[3] = {0.7547, 1.021, 1.243};
    double Means100b[3] = {0.7297, 0.9282, 1.088};

    double std_err1a[3] = {0.01199/4.0, 0.07336/4.0, 0.2598/4.0};
    double std_err1b[3] = {0.1338/4.0, 0.133/4.0, 0.1299/4.0};
    double std_err5a[3] = {0.006538/4.0, 0.1445/4.0, 0.2616/4.0};
    double std_err5b[3] = {0.1037/4.0, 0.115/4.0, 0.07619/4.0};
    double std_err10a[3] = {0.006461/4.0, 0.1311/4.0, 0.2644/4.0};
    double std_err10b[3] = {0.03888/4.0, 0.05238/4.0, 0.1276/4.0};
    double std_err100a[3] = {0.001687/4.0, 0.001378/4.0, 0.1336/4.0};
    double std_err100b[3] = {0.0219/4.0, 0.06546/4.0, 0.1934/4.0};

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
    canvas->SaveAs("Var_means_input.root");
}

void Run1() {
    Mill_Mean_input();
}














