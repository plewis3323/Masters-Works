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
void CreateLargeTableCanvas(TGraph* graph1a, TGraph* graph5a, 
                            TGraph* graph10a, TGraph* graph100a, 
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
    writeGraphPoints(graph5a, "Graph 5a");
    writeGraphPoints(graph10a, "Graph 10a");
    writeGraphPoints(graph100a, "Graph 100a");

    // Update and write the large table canvas to the root file
    tableCanvas->Update();
    file->cd();
    tableCanvas->Write();
}

void Mill_Mean_input(TCanvas* Inputs1) {
    Inputs1->cd(); 
    const int n = 16; 
    double p1a[n] =  {1.24616, 1.10527,  1.28169,  1.01087, 1.31325, 1.16842,  1.19782, 1.34483, 1.38961,  1.19178,  1.33803, 1.11766,  1.3019, 1.16092, 1.36916, 1.19099};
    double Er1[n] =  {0.0115415, 0.00232639, 0.000287891,  0.0511752, 0.00069254, 0.000623185, 0.00310153, 0.0028869,  0.00862097, 0.000378624, 0.000630047,  0.00336307, 0.0047848,  0.000320773,  0.040422,  0.0687415};
double p1b[n] = {0.171337, 0.207653, 0.191361, 0.176388, 0.1611, 0.186657, 0.155636, 0.206695, 0.172951, 0.154303, 0.183076, 0.15212, 0.179294, 0.215838, 0.169008, 0.222991};
double Er2[n] = {0.0108315, 0.00208388, 0.00192374, 0.0017617, 0.00160531, 0.0114687, 0.00171259, 0.012014, 0.0110788, 0.0015381, 0.00182094, 0.00153831, 0.00178538, 0.00216133, 0.011779, 0.00225711};
double p1c[n] = {1.28905, 1.29091, 1.27586, 1.29508, 1.38645, 1.36066, 1.29333, 1.31579, 1.30588, 1.25942, 1.22535, 1.2716, 1.29787, 1.41758, 1.3551, 1.32184};
double Er3[n] = {0.0248754, 0.00224828, 0.000628587, 0.00168212, 0.02805, 0.000198572, 0.00051522, 0.00148363, 0.000796056, 0.0259342, 0.000600399, 0.000365055, 0.00861193, 0.000321685, 0.0286162, 0.0120347};
double p1d[n] = {0.190558, 0.188316, 0.184395, 0.195606, 0.189408, 0.193445, 0.18982, 0.190897, 0.188462, 0.18706, 0.192451, 0.186861, 0.191125, 0.19006, 0.189778, 0.186351};
double Er4[n] = {0.00401518, 0.000491678, 0.00406509, 0.000448951, 0.00451598, 0.000464687, 0.0042778, 0.00452595, 0.000439306, 0.00376851, 0.00449896, 0.000436876, 0.00421304, 0.00421594, 0.0044486, 0.000631565};
    double Inp[n];
   for (int i = 0; i < 16; ++i) {
    Inp[i] = 1.0;
    }

    // Create TGraphErrors
    TGraphErrors* graph1a = new TGraphErrors(n, Inp, p1a, 0, Er1);
    TGraphErrors* graph5a = new TGraphErrors(n, Inp, p1b, 0, Er2);
    TGraphErrors* graph10a = new TGraphErrors(n, Inp, p1c, 0, Er3);
    TGraphErrors* graph100a = new TGraphErrors(n, Inp, p1d, 0, Er4);

    // Customize appearance of each graph (use points only, no lines)
    graph1a->SetMarkerStyle(20);  // Full circle
    graph1a->SetMarkerSize(1.0);
    graph1a->SetMarkerColor(kRed+1);


    graph5a->SetMarkerStyle(22);  // Full triangle up
    graph5a->SetMarkerSize(1.0);
    graph5a->SetMarkerColor(kGreen+1);


    graph10a->SetMarkerStyle(24);  // Open circle
    graph10a->SetMarkerSize(1.0);
    graph10a->SetMarkerColor(kCyan+1);


    graph100a->SetMarkerStyle(26);  // Open triangle up
    graph100a->SetMarkerSize(1.0);
    graph100a->SetMarkerColor(kRed+2);

    // Draw graphs (points only, no lines)
    graph1a->Draw("AP");
    graph5a->Draw("P SAME");
    graph10a->Draw("P SAME");
    graph100a->Draw("P SAME");

    // Axis labels
    graph1a->GetXaxis()->SetTitle("Input");
    graph1a->GetYaxis()->SetTitle("p1");
    graph1a->SetTitle("p1 Input Comparison");
    graph1a->GetYaxis()->SetRangeUser(0.0, 1.6); 

    // Add a legend to the canvas
    TLegend *legend = new TLegend(0.6, 0.7, 0.9, 0.9);
    legend->AddEntry(graph1a, "1 million events: 0.5-1.2 GeV (Inp = 0.75)", "p");
    legend->AddEntry(graph5a, "5 million events: 0.5-1.2 GeV (Inp = 0.75)", "p");
    legend->AddEntry(graph10a, "10 million events: 0.5-1.2 GeV (Inp = 0.75)", "p");
    legend->AddEntry(graph100a, "100 million events: 0.5-1.2 GeV (Inp = 0.75)", "p");
    legend->Draw();
    

    // Update the canvas to display all graphs
    Inputs1->Update();

    // Open a new root file to store canvases and tables
    TFile *Mean_inp = new TFile("p1_inp.root", "RECREATE");

    // Write the main canvas (graphs)
    Inputs1->Write();

    // Create one large canvas for all tables of values
    CreateLargeTableCanvas(graph1a,  graph5a,  graph10a, graph100a,
                           "Table_AllGraphs", "Table of All Graph Points", Mean_inp);

    // Close the root file
    Mean_inp->Close();
}

void Run1() {
    TCanvas *c1 = new TCanvas("c1"); 
    
    
    
    
    
    Mill_Mean_input(c1);
}




