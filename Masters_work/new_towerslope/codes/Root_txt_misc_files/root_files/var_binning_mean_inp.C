#include <iostream>
#include <fstream>
#include <sstream>
#include <TFile.h>
#include <TH1F.h>
#include <TLatex.h>
#include <string>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TFormula.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <TString.h>
#include <vector>
#include <TMatrixD.h>
#include <TVirtualFitter.h>
#include <TLegend.h>
#include <cmath>
using namespace std;

// Helper function to evaluate mathematical expressions
double EvaluateExpression(const string& expression) {
    TFormula formula("formula", expression.c_str());
    return formula.Eval(0);  // Evaluate at x = 0
}

// Function to take user input for means and standard errors, allowing math expressions
void GetUserInput(const string& label, vector<double>& values, int n) {
    cout << "Enter " << label << " (you can use math expressions like 1+0.5):\n";
    for (int i = 0; i < n; ++i) {
        string input;
        cout << label << "[" << i << "]: ";
        cin >> input;
        values[i] = EvaluateExpression(input);
    }
}

void Mill_Mean_input() {
    const int n = 3;
    double Input[n] = {0.75, 1.0, 1.25};

    vector<string> graphLabels = {
        "1 million events: 0.5-1.2 GeV", "1 million events: 1.5-3.2 GeV",
        "5 million events: 0.5-1.2 GeV", "5 million events: 1.5-3.2 GeV",
        "10 million events: 0.5-1.2 GeV", "10 million events: 1.5-3.2 GeV",
        "100 million events: 0.5-1.2 GeV", "100 million events: 1.5-3.2 GeV"
    };

    vector<TGraphErrors*> graphs(8);
    vector<TF1*> fits(8);

    // Prompt user for the ROOT file name
    string rootFileName;
    cout << "Enter the name of the ROOT file to save the graphs (e.g., output.root): ";
    cin >> rootFileName;

    // Open the ROOT file
    TFile* Mean_inp = new TFile(rootFileName.c_str(), "RECREATE");

    // Create a canvas for all graphs
    TCanvas* canvas = new TCanvas("AllGraphs", "All Graphs on One Canvas", 1000, 800);
    canvas->Divide(1, 1);  // Single pad for all graphs
    canvas->cd();

    TLegend* legend = new TLegend(0.7, 0.1, 0.9, 0.4);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);

    for (int g = 0; g < 8; ++g) {
        vector<double> Means(n), StdErr(n);

        // Get user-defined means and standard errors
        GetUserInput("means for " + graphLabels[g], Means, n);
        GetUserInput("standard errors for " + graphLabels[g], StdErr, n);

        // Create TGraphErrors for this graph
        graphs[g] = new TGraphErrors(n, Input, Means.data(), nullptr, StdErr.data());

        // Customize appearance
        graphs[g]->SetMarkerStyle(20 + g);
        graphs[g]->SetMarkerSize(1.0);
        graphs[g]->SetMarkerColor(kRed + g);

        // Draw the graph
        if (g == 0) {
            graphs[g]->Draw("AP");  // Draw axes with the first graph
            graphs[g]->GetXaxis()->SetTitle("Input");
            graphs[g]->GetYaxis()->SetTitle("Mean");
        } else {
            graphs[g]->Draw("P SAME");  // Overlay subsequent graphs
        }

        // Add a linear fit
        fits[g] = new TF1(Form("fit%d", g), "pol1", 0, 2);  // Linear fit
        graphs[g]->Fit(fits[g], "Q");  // Quiet mode
        fits[g]->SetLineColor(kRed + g);
        fits[g]->SetLineStyle(2);

        // Add the graph to the legend
        legend->AddEntry(graphs[g], graphLabels[g].c_str(), "P");
        legend->AddEntry(fits[g], Form("Fit: %s", graphLabels[g].c_str()), "L");
    }

    // Draw the legend
    legend->Draw();

    // Update the canvas
    canvas->Update();

    // Save the canvas and close the file
    canvas->Write();
    Mean_inp->Close();
}

// Main function to run the program
void Run1() {
    Mill_Mean_input();
}





























































































































































































































































































