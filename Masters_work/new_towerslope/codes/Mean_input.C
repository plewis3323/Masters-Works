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

// Function to create a TLegend for a canvas and graph at center-top (lowered slightly)
void AddLegend(TGraphErrors* graph, const char* legendText) {
    // Adjusted coordinates for the legend at the center-top, but lower
    TLegend *legend = new TLegend(0.35, 0.80, 0.65, 0.90);
    legend->AddEntry(graph, legendText, "p");  // "p" indicates the points
    legend->Draw();
}

// Function to create a table of points in a TTree for each graph
void CreateTableForGraph(TGraphErrors* graph, const char* treeName, const char* title, TFile* file) {
    // Create a new TTree
    TTree* tree = new TTree(treeName, title);
    
    // Variables to hold the data
    double xVal, yVal, yErr;

    // Create branches in the tree for x-values, y-values, and y-errors
    tree->Branch("x", &xVal, "x/D");  // "D" means double
    tree->Branch("y", &yVal, "y/D");
    tree->Branch("yErr", &yErr, "yErr/D");

    // Fill the tree with the graph's data points
    for (int i = 0; i < graph->GetN(); ++i) {
        xVal = graph->GetX()[i];
        yVal = graph->GetY()[i];
        yErr = graph->GetErrorY(i);
        tree->Fill();
    }

    // Write the tree to the file
    file->cd();  // Ensure we are in the correct file directory
    tree->Write();
}

// Function to create and store a canvas for the table
void CreateTableCanvas(TGraphErrors* graph, const char* canvasName, const char* tableTitle, TFile* file) {
    // Create a new canvas for the table
    TCanvas* tableCanvas = new TCanvas(canvasName, tableTitle, 600, 400);
    
    // Set up text rendering using TLatex
    TLatex latex;
    latex.SetTextSize(0.03); // Set text size for readability

    // Title for the table
    latex.DrawLatexNDC(0.1, 0.9, tableTitle);
    
    // Draw the table of values (X, Y, Error Y)
    for (int i = 0; i < graph->GetN(); ++i) {
        double xVal = graph->GetX()[i];
        double yVal = graph->GetY()[i];
        double yErr = graph->GetErrorY(i);
        
        // Create a formatted string with the point values
        TString text;
        text.Form("Point %d: X = %.3f, Y = %.3f, Error Y = %.3f", i+1, xVal, yVal, yErr);
        
        // Draw the table rows on the canvas
        latex.DrawLatexNDC(0.1, 0.85 - i*0.05, text);
    }

    // Update and write the table canvas to the root file
    tableCanvas->Update();
    tableCanvas->Write();
}

// Function to draw the graph on a canvas and write to ROOT file
void DrawGraphOnCanvas(TGraphErrors* graph, const char* canvasName, const char* title, const char* xTitle, const char* yTitle, const char* legendText, TFile* file) {
    // Create a canvas
    TCanvas *canvas = new TCanvas(canvasName, title, 800, 600);
    
    // Draw the graph
    graph->Draw("AP");
    
    // Set axis titles
    graph->GetXaxis()->SetTitle(xTitle);
    graph->GetYaxis()->SetTitle(yTitle);

    // Add legend at center-top, slightly lower
    AddLegend(graph, legendText);
    
    // Update canvas to show graph and legend
    canvas->Update();

    // Write the canvas to the root file
    canvas->Write();

    // Create a table (TTree) for the points in the graph and save it in the root file
    TString treeName = TString("Table_") + canvasName;
    CreateTableForGraph(graph, treeName, title, file);
    
    // Create a separate canvas for the table of values
    TString tableCanvasName = TString("TableCanvas_") + canvasName;
    CreateTableCanvas(graph, tableCanvasName, TString("Table for ") + title, file);
}

void Mill_Mean_input() {
    const int n = 3; 
    double Means1a[3] =  {0.817, 1.003, 1.174};
    double Means1b[3] =  {0.7924, 0.8013, 0.8407};
    double Means5a[3] =  {0.7667, 0.9875, 1.24};
    double Means5b[3] =  {0.859, 0.928, 0.984};
    double Means10a[3] =  {0.7643, 0.9942, 1.236};
    double Means10b[3] =  {0.8883, 0.9477, 1.004};
    double Means100a[3] =  {0.7517, 1.004, 1.252};
    double Means100b[3] =  {0.7798, 0.9851, 1.233};

    double Input[3]  = {0.75, 1.0, 1.25};

    double std_err1a[3] = {0.018, 0.016, 0.033};
    double std_err1b[3] = {0.049, 0.046, 0.051};
    double std_err5a[3] = {0.0095, 0.012, 0.012};
    double std_err5b[3] = {0.021, 0.014, 0.014};
    double std_err10a[3] = {0.0068, 0.0070, 0.0063};
    double std_err10b[3] = {0.014, 0.0063, 0.015};
    double std_err100a[3] = {0.0011, 0.0025, 0.0022};
    double std_err100b[3] = {0.0069, 0.0044, 0.0058};

    // Create TGraphErrors
    TGraphErrors* graph1a = new TGraphErrors(n, Input, Means1a, 0, std_err1a);
    TGraphErrors* graph1b = new TGraphErrors(n, Input, Means1b, 0, std_err1b);
    TGraphErrors* graph5a = new TGraphErrors(n, Input, Means5a, 0, std_err5a);
    TGraphErrors* graph5b = new TGraphErrors(n, Input, Means5b, 0, std_err5b);
    TGraphErrors* graph10a = new TGraphErrors(n, Input, Means10a, 0, std_err10a);
    TGraphErrors* graph10b = new TGraphErrors(n, Input, Means10b, 0, std_err10b);
    TGraphErrors* graph100a = new TGraphErrors(n, Input, Means100a, 0, std_err100a);
    TGraphErrors* graph100b = new TGraphErrors(n, Input, Means100b, 0, std_err100b);

    // Customize appearance of each graph (use points only, no lines)
    graph1a->SetMarkerStyle(20);  // Full circle
    graph1a->SetMarkerSize(1.0);
    graph1a->SetMarkerColor(kRed+1);

    graph1b->SetMarkerStyle(21);  // Full square
    graph1b->SetMarkerSize(1.0);
    graph1b->SetMarkerColor(kBlue+1);

    graph5a->SetMarkerStyle(22);  // Full triangle up
    graph5a->SetMarkerSize(1.0);
    graph5a->SetMarkerColor(kGreen+1);

    graph5b->SetMarkerStyle(23);  // Full triangle down
    graph5b->SetMarkerSize(1.0);
    graph5b->SetMarkerColor(kMagenta+1);

    graph10a->SetMarkerStyle(24);  // Open circle
    graph10a->SetMarkerSize(1.0);
    graph10a->SetMarkerColor(kCyan+1);

    graph10b->SetMarkerStyle(25);  // Open square
    graph10b->SetMarkerSize(1.0);
    graph10b->SetMarkerColor(kOrange+1);

    graph100a->SetMarkerStyle(26);  // Open triangle up
    graph100a->SetMarkerSize(1.0);
    graph100a->SetMarkerColor(kRed+2);

    graph100b->SetMarkerStyle(27);  // Open triangle down
    graph100b->SetMarkerSize(1.0);
    graph100b->SetMarkerColor(kBlue+2);

    // Open a new root file to store canvases and tables
    TFile *Mean_inp = new TFile("Spectral_events_mean_inp.root", "RECREATE");

    // Draw each graph on its own canvas, add legend, and write to the ROOT file along with the corresponding table
    DrawGraphOnCanvas(graph1a, "c1_1M_0_5_1_2_GeV", "1 Million Events (0.5-1.2 GeV)", "Input", "Mean", "1 million events: 0.5-1.2 GeV", Mean_inp);
    DrawGraphOnCanvas(graph1b, "c2_1M_1_5_3_2_GeV", "1 Million Events (1.5-3.2 GeV)", "Input", "Mean", "1 million events: 1.5-3.2 GeV", Mean_inp);
    DrawGraphOnCanvas(graph5a, "c3_5M_0_5_1_2_GeV", "5 Million Events (0.5-1.2 GeV)", "Input", "Mean", "5 million events: 0.5-1.2 GeV", Mean_inp);
    DrawGraphOnCanvas(graph5b, "c4_5M_1_5_3_2_GeV", "5 Million Events (1.5-3.2 GeV)", "Input", "Mean", "5 million events: 1.5-3.2 GeV", Mean_inp);
    DrawGraphOnCanvas(graph10a, "c5_10M_0_5_1_2_GeV", "10 Million Events (0.5-1.2 GeV)", "Input", "Mean", "10 million events: 0.5-1.2 GeV", Mean_inp);
    DrawGraphOnCanvas(graph10b, "c6_10M_1_5_3_2_GeV", "10 Million Events (1.5-3.2 GeV)", "Input", "Mean", "10 million events: 1.5-3.2 GeV", Mean_inp);
    DrawGraphOnCanvas(graph100a, "c7_100M_0_5_1_2_GeV", "100 Million Events (0.5-1.2 GeV)", "Input", "Mean", "100 million events: 0.5-1.2 GeV", Mean_inp);
    DrawGraphOnCanvas(graph100b, "c8_100M_1_5_3_2_GeV", "100 Million Events (1.5-3.2 GeV)", "Input", "Mean", "100 million events: 1.5-3.2 GeV", Mean_inp);

    // Close the root file
    Mean_inp->Close();
}

void Run1() {
    Mill_Mean_input();
}

