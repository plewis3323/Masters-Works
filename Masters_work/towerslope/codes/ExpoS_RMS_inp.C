#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TH1F.h>
#include <TLatex.h>
#include <string>
#include <TGraphErrors.h>
#include <TGraph.h>
#include <TF1.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <TString.h>
#include <vector>
#include <TMatrixD.h>
#include <TVirtualFitter.h>
#include <string>
#include <TLegend.h>
#include <cmath>
using namespace std; 

// Function to create a large table of all graph points on one canvas
void CreateLargeTableCanvas(TGraph* graph1a, TGraph* graph1b, TGraph* graph5a, TGraph* graph5b, 
                            TGraph* graph10a, TGraph* graph10b, TGraph* graph100a, TGraph* graph100b, 
                            const char* canvasName, const char* tableTitle, TFile* file) {
    // Create a new canvas for the large table
    TCanvas* tableCanvas = new TCanvas(canvasName, tableTitle, 1200, 800);
    
    // Set up text rendering using TLatex
    TLatex latex;
    latex.SetTextSize(0.025); // Set text size for readability
    latex.DrawLatexNDC(0.05, 0.92, tableTitle);

    // Table header
    latex.DrawLatexNDC(0.05, 0.87, "Graph     X-Value       Y-Value");

    // Counter to adjust the y position for each row
    double yPos = 0.82;

    // Function to write graph points
    auto writeGraphPoints = [&](TGraph* graph, const char* graphName) {
        for (int i = 0; i < graph->GetN(); ++i) {
            double xVal = graph->GetX()[i];
            double yVal = graph->GetY()[i];
            TString text;
            text.Form("%s   X = %.3f, Y = %.3f", graphName, xVal, yVal);
            latex.DrawLatexNDC(0.05, yPos, text);
            yPos -= 0.03;  // Adjust the vertical position for the next line
        }
    };

    // Write points from all graphs
    writeGraphPoints(graph1a, "Graph 1a");
    writeGraphPoints(graph1b, "Graph 1b");
    writeGraphPoints(graph5a, "Graph 5a");
    writeGraphPoints(graph5b, "Graph 5b");
    writeGraphPoints(graph10a, "Graph 10a");
    writeGraphPoints(graph10b, "Graph 10b");
    writeGraphPoints(graph100a, "Graph 100a");
    writeGraphPoints(graph100b, "Graph 100b");

    // Update and write the large table canvas to the root file
    tableCanvas->Update();
    file->cd();
    tableCanvas->Write();
}

void Mill_Mean_input(TCanvas* Inputs1) {
    Inputs1->cd(); 
    const int n = 3; 
    double std1a[3] =  {0.0133 , 0.02027 , 0.0275};
    double std1b[3] =  {0.01287, 0.01934, 0.08351};
    double std5a[3] =  {0.00522, 0.00914, 0.02904};
    double std5b[3] =  {0.0043, 0.0044, 0.0079};
    double std10a[3] =  {0.00467, 0.00532, 0.00839};
    double std10b[3] =  {0.003019, 0.005956, 0.003956};
    double std100a[3] =  {0.00124, 0.00113, 0.0033 };
    double std100b[3] =  {0.000594, 0.0009473, 0.001637};
    double Input[3]  = {0.75, 1.0, 1.25};


    // Create TGraphErrors
    TGraph* graph1a = new TGraph(n, Input, std1a);
    TGraph* graph1b = new TGraph(n, Input, std1b);
    TGraph* graph5a = new TGraph(n, Input, std5a);
    TGraph* graph5b = new TGraph(n, Input, std5b);
    TGraph* graph10a = new TGraph(n, Input, std10a);
    TGraph* graph10b = new TGraph(n, Input, std10b);
    TGraph* graph100a = new TGraph(n, Input, std100a);
    TGraph* graph100b = new TGraph(n, Input, std100b);

    // Customize appearance of each graph (use points only, no lines)
    int colors[] = {kRed+1, kBlue+1, kGreen+1, kMagenta+1, kCyan+1, kOrange+1, kRed+2, kBlue+2};
    int markers[] = {20, 21, 22, 23, 24, 25, 26, 27};

    TGraph* graphs[] = {graph1a, graph1b, graph5a, graph5b, graph10a, graph10b, graph100a, graph100b};
    const char* labels[] = {
        "1 million events: Low Pt (0.5-1.2 GeV)", "1 million events: High Pt (1.5-3.2 GeV)",
        "5 million events: Low Pt (0.5-1.2 GeV)", "5 million events: High Pt (1.5-3.2 GeV)",
        "10 million events: Low Pt (0.5-1.2 GeV)", "10 million events: High Pt (1.5-3.2 GeV)",
        "100 million events: Low Pt (0.5-1.2 GeV)", "100 million events: High Pt (1.5-3.2 GeV)"
    };

    for (int i = 0; i < 8; ++i) {
        graphs[i]->SetMarkerStyle(markers[i]);
        graphs[i]->SetMarkerSize(1.0);
        graphs[i]->SetMarkerColor(colors[i]);
        if (i == 0) graphs[i]->Draw("AP");
        else graphs[i]->Draw("P SAME");
    }

    // Axis labels
    graph1a->GetXaxis()->SetTitle("Input (Decal)");
    graph1a->GetYaxis()->SetTitle("RMS");
    graph1a->SetTitle("RMS Input Comparison");
    graph1a->GetYaxis()->SetRangeUser(-0.3, 0.4); 

    // Add a legend to the canvas
    TLegend *legend = new TLegend(0.6, 0.5, 0.9, 0.9);
    legend->SetTextSize(0.03);
    for (int i = 0; i < 8; ++i) {
        legend->AddEntry(graphs[i], labels[i], "p");
    }
    legend->Draw();

    // Update the canvas to display all graphs
    Inputs1->Update();

    // Open a new root file to store canvases and tables
    TFile *Mean_inp = new TFile("Dec-13_ExpoS_RMS_inp.root", "RECREATE");

    // Write the main canvas (graphs)
    Inputs1->Write();

    // Create one large canvas for all tables of values
    CreateLargeTableCanvas(graph1a, graph1b, graph5a, graph5b, graph10a, graph10b, graph100a, graph100b,
                           "Table_AllGraphs", "Table of All Graph Points", Mean_inp);

    // Close the root file
    Mean_inp->Close();
}

void Run1() {
    TCanvas *c1 = new TCanvas("c1", "All Graphs", 1200, 800); 
    Mill_Mean_input(c1);
}


































































































































































































