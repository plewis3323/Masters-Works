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
    double std1a[3] = {0.00364286, 0.00432946, 0.00274414};
    double std1b[3] = {0.00104651, 0.00159867, 0.00138282};
    double std5a[3] = {0.00215766, 0.00221904, 0.00145287};
    double std5b[3] = {0.00104923, 0.00120701, 0.000867462};
    double std10a[3] = {0.00150942, 0.00156117, 0.00107637};
    double std10b[3] = {0.000933898, 0.000851371, 0.000798682};
    double std100a[3] = {0.000513345, 0.000514673, 0.000468671};
    double std100b[3] = {0.000353193, 0.000295203,  0.000246305};
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

    // Draw graphs (points only, no lines)
    graph1a->Draw("AP");
    graph1b->Draw("P SAME");
    graph5a->Draw("P SAME");
    graph5b->Draw("P SAME");
    graph10a->Draw("P SAME");
    graph10b->Draw("P SAME");
    graph100a->Draw("P SAME");
    graph100b->Draw("P SAME");

    // Axis labels
    graph1a->GetXaxis()->SetTitle("Input");
    graph1a->GetYaxis()->SetTitle("Statistical Error");
    graph1a->SetTitle("RMS Input Comparison");
    graph1a->GetYaxis()->SetRangeUser(-1.0, 1.0); 

    // Add a legend to the canvas
    TLegend *legend = new TLegend(0.6, 0.7, 0.9, 0.9);
    legend->AddEntry(graph1a, "1 million events: 0.5-1.2 GeV", "p");
    legend->AddEntry(graph1b, "1 million events: 1.5-3.2 GeV", "p");
    legend->AddEntry(graph5a, "5 million events: 0.5-1.2 GeV", "p");
    legend->AddEntry(graph5b, "5 million events: 1.5-3.2 GeV", "p");
    legend->AddEntry(graph10a, "10 million events: 0.5-1.2 GeV", "p");
    legend->AddEntry(graph10b, "10 million events: 1.5-3.2 GeV", "p");
    legend->AddEntry(graph100a, "100 million events: 0.5-1.2 GeV", "p");
    legend->AddEntry(graph100b, "100 million events: 1.5-3.2 GeV", "p");
    legend->Draw();

    // Update the canvas to display all graphs
    Inputs1->Update();

    // Open a new root file to store canvases and tables
    TFile *Mean_inp = new TFile("expo_events_Stat_Error_inp.root", "RECREATE");

    // Write the main canvas (graphs)
    Inputs1->Write();

    // Create one large canvas for all tables of values
    CreateLargeTableCanvas(graph1a, graph1b, graph5a, graph5b, graph10a, graph10b, graph100a, graph100b,
                           "Table_AllGraphs", "Table of All Graph Points", Mean_inp);

    // Close the root file
    Mean_inp->Close();
}

void Run1() {
    TCanvas *c1 = new TCanvas("c1"); 
    Mill_Mean_input(c1);
    
    
    
   
}
















