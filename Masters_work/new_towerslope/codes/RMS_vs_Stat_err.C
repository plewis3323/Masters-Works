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



// Function to compute Spearman Rank Correlation
double SpearmanCorrelation(const vector<double>& x, const vector<double>& y) {
    // Ensure x and y have the same size
    if (x.size() != y.size()) {
        cerr << "Error: Input vectors must have the same size." << endl;
        return NAN;
    }

    int n = x.size();

    // Helper function to compute ranks
    auto computeRanks = [](const vector<double>& values) -> vector<double> {
        vector<pair<double, int>> valueIndex;
        for (int i = 0; i < values.size(); i++) {
            valueIndex.emplace_back(values[i], i);
        }

        // Sort values with their indices
        sort(valueIndex.begin(), valueIndex.end());

        // Assign ranks
        vector<double> ranks(values.size());
        int rank = 1;
        for (int i = 0; i < valueIndex.size(); i++) {
            if (i > 0 && valueIndex[i].first == valueIndex[i - 1].first) {
                ranks[valueIndex[i].second] = ranks[valueIndex[i - 1].second];
            } else {
                ranks[valueIndex[i].second] = rank;
            }
            rank++;
        }

        return ranks;
    };

    // Compute ranks for x and y
    vector<double> xRanks = computeRanks(x);
    vector<double> yRanks = computeRanks(y);

    // Calculate Spearman's rho
    double numerator = 0.0;
    double xDenominator = 0.0, yDenominator = 0.0;
    double xMean = accumulate(xRanks.begin(), xRanks.end(), 0.0) / n;
    double yMean = accumulate(yRanks.begin(), yRanks.end(), 0.0) / n;

    for (int i = 0; i < n; i++) {
        double xDiff = xRanks[i] - xMean;
        double yDiff = yRanks[i] - yMean;
        numerator += xDiff * yDiff;
        xDenominator += xDiff * xDiff;
        yDenominator += yDiff * yDiff;
    }

    double rho = numerator / sqrt(xDenominator * yDenominator);
    return rho;
}




// Function to calculate the Pearson correlation coefficient
double PearsonCorrelation(const double* x, const double* y, int n) {
    // Compute means of x and y
    double x_sum = 0, y_sum = 0;
    for (int i = 0; i < n; i++) {
        x_sum += x[i];
        y_sum += y[i];
    }
    double x_bar = x_sum / n;
    double y_bar = y_sum / n;

    // Compute numerator and denominators for Pearson r
    double numerator = 0;
    double x_denominator = 0;
    double y_denominator = 0;

    for (int i = 0; i < n; i++) {
        double x_diff = x[i] - x_bar;
        double y_diff = y[i] - y_bar;
        numerator += x_diff * y_diff;
        x_denominator += x_diff * x_diff;
        y_denominator += y_diff * y_diff;
    }

    // Compute r
    double r = numerator / sqrt(x_denominator * y_denominator);
    return r;
}




// Function to calculate population correlation coefficient
double PopulationCorrelation(const vector<double>& x, const vector<double>& y) {
    // Check if input vectors have the same size
    if (x.size() != y.size()) {
        cerr << "Error: Input vectors must have the same size." << endl;
        return NAN; // Return Not-a-Number if sizes don't match
    }

    // Calculate means of x and y
    double x_sum = 0.0, y_sum = 0.0;
    int n = x.size();
    for (int i = 0; i < n; i++) {
        x_sum += x[i];
        y_sum += y[i];
    }
    double x_mean = x_sum / n;
    double y_mean = y_sum / n;

    // Calculate covariance and variances
    double covariance = 0.0;
    double x_variance = 0.0, y_variance = 0.0;

    for (int i = 0; i < n; i++) {
        double x_diff = x[i] - x_mean;
        double y_diff = y[i] - y_mean;
        covariance += x_diff * y_diff;
        x_variance += x_diff * x_diff;
        y_variance += y_diff * y_diff;
    }

    // Calculate population correlation coefficient (rho)
    double rho = covariance / sqrt(x_variance * y_variance);
    return rho;
}





void Mill_Mean_input() {
    const int n = 3;
    double stat_err1a[3] = {0.012, 0.016, 0.012};
    double stat_err1b[3] = {0.0101, 0.0050, 0.0034};
    double stat_err5a[3] = {0.011, 0.011, 0.008};
    double stat_err5b[3] = {0.0041, 0.0044, 0.0026};
    double stat_err10a[3] = {0.0086, 0.017, 0.011};
    double stat_err10b[3] = {0.0054, 0.0043, 0.0036};
    double stat_err100a[3] = {0.0078, 0.0075, 0.0034};
    double stat_err100b[3] = {0.0018, 0.0035, 0.0041};

    double RMS_1a[3] = {0.072, 0.0641, 0.133};
    double RMS_1b[3] = {0.197, 0.184, 0.206};
    double RMS_5a[3] = {0.038, 0.049, 0.047};
    double RMS_5b[3] = {0.082, 0.057, 0.054};
    double RMS_10a[3] = {0.0272, 0.0280, 0.0251};
    double RMS_10b[3] = {0.054, 0.0252, 0.060};
    double RMS_100a[3] = {0.0042, 0.0101, 0.0088};
    double RMS_100b[3] = {0.028, 0.018, 0.0233};

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

    // Combine all points into a single dataset for the global fit
    const int total_points = 8 * n;
    double global_x[total_points];
    double global_y[total_points];
    int idx = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < n; j++) {
            global_x[idx] = graphs[i]->GetX()[j];
            global_y[idx] = graphs[i]->GetY()[j];
            idx++;
        }
    }

    TGraph* globalGraph = new TGraph(total_points, global_x, global_y);

    // Create canvas for all graphs
    TCanvas* canvas = new TCanvas("AllGraphs", "L3_RMS_Staterror_comparison", 1200, 800);
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
            graphs[i]->SetTitle("l3 RMS vs. P1 statistical Error");
            graphs[i]->GetXaxis()->SetLimits(0.0, 1.5);  // Adjust x-axis range
            graphs[i]->SetMinimum(0.0);                 // Adjust y-axis range
            graphs[i]->SetMaximum(1.5);
        } else {
            graphs[i]->Draw("P SAME");
        }

        legend->AddEntry(graphs[i], energyRanges[i], "lp");
    }

    // Global fit using the combined dataset
    TF1* globalFit = new TF1("globalFit", "[0]*x + [1] ", 0.0, 0.02); // Linear fit
    globalGraph->Fit(globalFit, "R"); // Perform the fit on combined data
    globalFit->SetLineColor(kBlack);
    globalFit->SetLineStyle(1);
    globalFit->SetLineWidth(3);
    globalFit->Draw("ALP SAME");

    // Add global fit to the legend
    legend->AddEntry(globalFit, "Locus Trend of Points", "l");

    legend->Draw();
    canvas->Update();
    canvas->SaveAs("l3_RMS_vs_P1staterr_12-5_iteration.root");
    double r = PearsonCorrelation(global_x, global_y, total_points);
    cout << "Pearson Correlation Coefficient (r) for dataset 1: " << r << endl;
    // Calculate and print the population correlation coefficient
    // Convert arrays to vectors for PopulationCorrelation
    vector<double> global_x_vec(global_x, global_x + total_points);
    vector<double> global_y_vec(global_y, global_y + total_points);
    double rho = PopulationCorrelation(global_x_vec, global_y_vec);
    cout << "Population Correlation Coefficient (rho): " << rho << endl;
     // Calculate Spearman correlation
    double rho1 = SpearmanCorrelation(global_x_vec, global_y_vec);
    cout << "Spearman Rank Correlation Coefficient (rho): " << rho1 << endl;
    
}

void Run1() {
    Mill_Mean_input();
}


/*-----------------------------------------------------------------------------------------------------*/

void Low_Mill_Mean_input() {
    const int n = 3;
    double stat_err1a[3] = {0.0644101, 0.0762856, 0.0437919};
    double stat_err5a[3] = {0.023324, 0.0353681, 0.0148592};
    double stat_err10a[3] = {0.0168207, 0.027008, 0.0101488};
    double stat_err100a[3] = {0.00487283, 0.00835389, 0.00310641};

    double RMS_1a[3] = {0.02451, 0.04408, 0.04332};
    double RMS_5a[3] = {0.01516, 0.02064, 0.02069};
    double RMS_10a[3] = {0.01122, 0.009633, 0.007666};
    double RMS_100a[3] = {0.00264, 0.002397, 0.0042};

    TGraph* graphs[4];
    graphs[0] = new TGraph(n, stat_err1a, RMS_1a);
    graphs[1] = new TGraph(n, stat_err5a, RMS_5a);
    graphs[2] = new TGraph(n, stat_err10a, RMS_10a);
    graphs[3] = new TGraph(n, stat_err100a, RMS_100a);

    Color_t colors[8] = {kRed, kBlue, kGreen, kMagenta};

    // Combine all points into a single dataset for the global fit
    const int total_points = 4 * n;
    double global_x[total_points];
    double global_y[total_points];
    int idx = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < n; j++) {
            global_x[idx] = graphs[i]->GetX()[j];
            global_y[idx] = graphs[i]->GetY()[j];
            idx++;
        }
    }

    TGraph* globalGraph = new TGraph(total_points, global_x, global_y);

    // Create canvas for all graphs
    TCanvas* canvas = new TCanvas("AllGraphs", "L3_RMS_Staterror_comparison", 1200, 800);
    TLegend* legend = new TLegend(0.6, 0.7, 0.9, 0.9);
    legend->SetTextSize(0.025);



    const char* energyRanges[4] = {
        "1M Events (0.5-1.2 GeV)", 
        "5M Events (0.5-1.2 GeV)", 
        "10M Events (0.5-1.2 GeV)", 
        "100M Events (0.5-1.2 GeV)"
    };


    for (int i = 0; i < 4; i++) {
        graphs[i]->SetMarkerStyle(21);
        graphs[i]->SetMarkerSize(1);
        graphs[i]->SetLineWidth(2);
        graphs[i]->SetLineColor(colors[i]);
        graphs[i]->SetMarkerColor(colors[i]);

        if (i == 0) {
            graphs[i]->Draw("AP");
            graphs[i]->GetXaxis()->SetTitle("Statistical Error");
            graphs[i]->GetYaxis()->SetTitle("RMS");
            graphs[i]->SetTitle("l3 RMS vs. P1 statistical Error (0.5-1.2 GeV)");
            graphs[i]->GetXaxis()->SetLimits(0.0, 1.5);  // Adjust x-axis range
            graphs[i]->SetMinimum(0.0);                 // Adjust y-axis range
            graphs[i]->SetMaximum(1.5);
        } else {
            graphs[i]->Draw("P SAME");
        }

        legend->AddEntry(graphs[i], energyRanges[i], "lp");
    }

    // Global fit using the combined dataset
    TF1* globalFit = new TF1("globalFit", "[0]*x + [1] ", 0.0, 0.02); // Linear fit
    globalGraph->Fit(globalFit, "R"); // Perform the fit on combined data
    globalFit->SetLineColor(kBlack);
    globalFit->SetLineStyle(1);
    globalFit->SetLineWidth(3);
    globalFit->Draw("ALP SAME");

    // Add global fit to the legend
    legend->AddEntry(globalFit, "Locus Trend of Points", "l");

    legend->Draw();
    canvas->Update();
    canvas->SaveAs("l3_Rebin_RMS_vs_P1staterr_Lowpt_Highp1erros_iteration.root");
    double r = PearsonCorrelation(global_x, global_y, total_points);
    cout << "Pearson Correlation Coefficient (r) for dataset 1: " << r << endl;
    // Calculate and print the population correlation coefficient
    // Convert arrays to vectors for PopulationCorrelation
    vector<double> global_x_vec(global_x, global_x + total_points);
    vector<double> global_y_vec(global_y, global_y + total_points);
    double rho = PopulationCorrelation(global_x_vec, global_y_vec);
    cout << "Population Correlation Coefficient (rho): " << rho << endl;
    // Calculate Spearman correlation
    double rho1 = SpearmanCorrelation(global_x_vec, global_y_vec);
    cout << "Spearman Rank Correlation Coefficient (rho): " << rho1 << endl;
    
    
}

void Run2() {
    Low_Mill_Mean_input();
}



/* -------------------------------------------------------------------------------------------*/



void H_Mill_Mean_input() {
    const int n = 3;
    double stat_err1b[3] = {0.20129, 0.154158, 0.119315};
    double stat_err5b[3] = {0.0520258, 0.0837859, 0.099437};
    double stat_err10b[3] = {0.0348044, 0.0572511, 0.154149};
    double stat_err100b[3] = {0.000531931, 0.0202416, 0.0523513};


    double RMS_1b[3] = {0.08718, 0.1812, 0.1529};
    double RMS_5b[3] = {0.02853, 0.06441, 0.07561};
    double RMS_10b[3] = {0.003154, 0.03499, 0.04906};
    double RMS_100b[3] = {0.0009493, 0.01597, 0.05799};

    TGraph* graphs[4];
    graphs[0] = new TGraph(n, stat_err1b, RMS_1b);
    graphs[1] = new TGraph(n, stat_err5b, RMS_5b);
    graphs[2] = new TGraph(n, stat_err10b, RMS_10b);
    graphs[3] = new TGraph(n, stat_err100b, RMS_100b);

    Color_t colors[4] = {kCyan, kOrange, kYellow, kViolet};

    // Combine all points into a single dataset for the global fit
    const int total_points = 4 * n;
    double global_x[total_points];
    double global_y[total_points];
    int idx = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < n; j++) {
            global_x[idx] = graphs[i]->GetX()[j];
            global_y[idx] = graphs[i]->GetY()[j];
            idx++;
        }
    }

    TGraph* globalGraph = new TGraph(total_points, global_x, global_y);

    // Create canvas for all graphs
    TCanvas* canvas = new TCanvas("AllGraphs", "L3_RMS_Staterror_comparison", 1200, 800);
    TLegend* legend = new TLegend(0.6, 0.7, 0.9, 0.9);
    legend->SetTextSize(0.025);



    const char* energyRanges[8] = {
         "1M Events (1.4-3.0 GeV)",
         "5M Events (1.4-3.0 GeV)",
         "10M Events (1.4-3.0 GeV)",
         "100M Events (1.4-3.0 GeV)"
    };


    for (int i = 0; i < 4; i++) {
        graphs[i]->SetMarkerStyle(21);
        graphs[i]->SetMarkerSize(1);
        graphs[i]->SetLineWidth(2);
        graphs[i]->SetLineColor(colors[i]);
        graphs[i]->SetMarkerColor(colors[i]);

        if (i == 0) {
            graphs[i]->Draw("AP");
            graphs[i]->GetXaxis()->SetTitle("Statistical Error");
            graphs[i]->GetYaxis()->SetTitle("RMS");
            graphs[i]->SetTitle("l3 RMS vs. P1 statistical Error (1.4-3.0 Gev) ");
            graphs[i]->GetXaxis()->SetLimits(0.0, 1.5);  // Adjust x-axis range
            graphs[i]->SetMinimum(0.0);                 // Adjust y-axis range
            graphs[i]->SetMaximum(1.5);
        } else {
            graphs[i]->Draw("P SAME");
        }

        legend->AddEntry(graphs[i], energyRanges[i], "lp");
    }

    // Global fit using the combined dataset
    TF1* globalFit = new TF1("globalFit", "[0]*x + [1] ", 0.0, 0.02); // Linear fit
    globalGraph->Fit(globalFit, "R"); // Perform the fit on combined data
    globalFit->SetLineColor(kBlack);
    globalFit->SetLineStyle(1);
    globalFit->SetLineWidth(3);
    globalFit->Draw("ALP SAME");

    // Add global fit to the legend
    legend->AddEntry(globalFit, "Locus Trend of Points", "l");

    legend->Draw();
    canvas->Update();
    canvas->SaveAs("l3_rebin_RMS_vs_High_P1staterr_HighR_iteration.root");
    double r = PearsonCorrelation(global_x, global_y, total_points);
    cout << "Pearson Correlation Coefficient (r) for dataset 1: " << r << endl;
    // Calculate and print the population correlation coefficient
    // Convert arrays to vectors for PopulationCorrelation
    vector<double> global_x_vec(global_x, global_x + total_points);
    vector<double> global_y_vec(global_y, global_y + total_points);
    double rho = PopulationCorrelation(global_x_vec, global_y_vec);
    cout << "Population Correlation Coefficient (rho): " << rho << endl;
    // Calculate Spearman correlation
    double rho1 = SpearmanCorrelation(global_x_vec, global_y_vec);
    cout << "Spearman Rank Correlation Coefficient (rho): " << rho1 << endl;
    
    
}

void Run3() {
    H_Mill_Mean_input();
}









/* 


- Steps to Run Pearson Test: r. 
- it's a Linear Correlation test for two variables. 


*    -1 < |r| < 1: test is inconclusive. 
*    |r| = -1 and 1. : Negative or positive correlation 
*    find means for x-data set define as such:   x_bar = (x_1 + x_2 + x_3 + x_4 + .....)/N 
*    find means for y-data set define as such:   y_bar = (y_1 + y_2 + y_3 + y_4 + .....)/N
*    assuming summations: r = (x_i - x_bar)*(y_i - y_bar)/sqrt((x_i - x_bar)^2) * sqrt((y_i - y_bar)^2)
*    Print out r. 



*/






/*
#include <iostream>
#include <TFile.h>
#include <TGraph.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>
#include <TRandom.h>
#include <cmath>

using namespace std;

void Best_Trend_Points() {
    const int n = 7; // Number of datasets
    const int points_per_graph = 3;
    const int total_points = n * points_per_graph;

    // Data points from your graphs
    double stat_err1a[3] = {};
    double stat_err5a[3] = {};
    double stat_err5b[3] = {0.00210688, 0.00184591, 0.0015304};
    double stat_err10a[3] = {0.00405335, 0.00458086, 0.00339226};
    double stat_err10b[3] = {0.00150586, 0.00148788, 0.00137583};
    double stat_err100a[3] = {0.00238462, 0.00187959, 0.00105203};
    double stat_err100b[3] = {0.000772954, 0.00167842, 0.00137789};

    double RMS_1a[3] = {};
    double RMS_5a[3] = {};
    double RMS_5b[3] = {};
    double RMS_10a[3] = {};
    double RMS_10b[3] = {};
    double RMS_100a[3] = {};
    double RMS_100b[3] = {};

    // Combine all data points into a single array for the global fit
    double global_x[total_points];
    double global_y[total_points];
    int idx = 0;

    auto addPoints = [&](double* x_arr, double* y_arr) {
        for (int i = 0; i < points_per_graph; ++i) {
            global_x[idx] = x_arr[i];
            global_y[idx] = y_arr[i];
            ++idx;
        }
    };

    // Add all datasets to the global arrays
    addPoints(stat_err1a, RMS_1a);
    addPoints(stat_err5a, RMS_5a);
    addPoints(stat_err5b, RMS_5b);
    addPoints(stat_err10a, RMS_10a);
    addPoints(stat_err10b, RMS_10b);
    addPoints(stat_err100a, RMS_100a);
    addPoints(stat_err100b, RMS_100b);

    // Create a global TGraph for all points
    TGraph* globalGraph = new TGraph(total_points, global_x, global_y);
    globalGraph->SetTitle("Best Trend Line for All Points;Statistical Error;RMS");
    globalGraph->SetMarkerStyle(20);
    globalGraph->SetMarkerSize(1);
    globalGraph->SetMarkerColor(kBlue);

    // Create a canvas to draw the graph and fit
    TCanvas* canvas = new TCanvas("canvas", "Best Trend for Points", 1200, 800);
    globalGraph->Draw("AP");

    // Fit a linear function to the combined data
    TF1* bestFit = new TF1("bestFit", "pol1", 0.0, 0.5); // Linear trend
    globalGraph->Fit(bestFit, "R"); // "Q" suppresses fitting messages
    bestFit->SetLineColor(kRed);
    bestFit->SetLineWidth(2);
    bestFit->Draw("SAME");

    // Add legend
    TLegend* legend = new TLegend(0.6, 0.7, 0.9, 0.9);
    legend->AddEntry(globalGraph, "Data Points", "p");
    legend->AddEntry(bestFit, "Best Fit (Linear Trend)", "l");
    legend->Draw();

    // Save the canvas
    canvas->SaveAs("best_trend_points.png");
}
*/





























































