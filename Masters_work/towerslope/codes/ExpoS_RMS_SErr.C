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
    double stat_err1a[3] = {0.01271, 0.01672, 0.007366};
    double stat_err1b[3] = {0.0043, 0.005196, 0.004654};
    double stat_err5a[3] = {0.00842, 0.008993, 0.003937};
    double stat_err5b[3] = {0.004633, 0.004808, 0.003};
    double stat_err10a[3] = {0.005895, 0.006077, 0.003313};
    double stat_err10b[3] = {0.00375, 0.0033, 0.003194};
    double stat_err100a[3] = {0.002055, 0.00206, 0.001813};
    double stat_err100b[3] = {0.001433, 0.0015, 0.0009437};

    double RMS_1a[3] = {0.0133 , 0.02027 , 0.0275};
    double RMS_1b[3] = {0.01287, 0.01934, 0.08351};
    double RMS_5a[3] = {0.00522, 0.00914, 0.02904};
    double RMS_5b[3] = {0.0043, 0.0044, 0.0079};
    double RMS_10a[3] = {0.00467, 0.00532, 0.00839};
    double RMS_10b[3] = {0.003019, 0.005956, 0.003956};
    double RMS_100a[3] = {0.00124, 0.00113, 0.0033};
    double RMS_100b[3] = {0.000594, 0.0009473, 0.001637};

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
            graphs[i]->SetTitle("Expo RMS vs. P1 statistical Error");
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
    canvas->SaveAs("Expo_RMS_vs_P1staterr_12-5_iteration.root");
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
    double stat_err1a[3] = {0.01271, 0.01672, 0.007366};
    double stat_err5a[3] = {0.00842, 0.008993, 0.003937};
    double stat_err10a[3] = {0.005895, 0.006077, 0.003313};
    double stat_err100a[3] = {0.002055, 0.00206, 0.001813};

    double RMS_1a[3] = {0.0133 , 0.02027 , 0.0275};
    double RMS_5a[3] = {0.00522, 0.00914, 0.02904};
    double RMS_10a[3] = {0.00467, 0.00532, 0.00839};
    double RMS_100a[3] = {0.00124, 0.00113, 0.0033};

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
            graphs[i]->SetTitle("Expo RMS vs. P1 statistical Error (0.5-1.2 GeV)");
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
    canvas->SaveAs("ExpoS_RMS_vs_P1staterr_LowR_iteration.root");
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
    double stat_err1b[3] = {0.0043, 0.005196, 0.004654};
    double stat_err5b[3] = {0.004633, 0.004808, 0.003};
    double stat_err10b[3] = {0.00375, 0.0033, 0.003194};
    double stat_err100b[3] = {0.001433, 0.0015, 0.0009437};


    double RMS_1b[3] = {0.01287, 0.01934, 0.08351};
    double RMS_5b[3] = {0.0043, 0.0044, 0.0079};
    double RMS_10b[3] = {0.003019, 0.005956, 0.003956};
    double RMS_100b[3] = {0.000594, 0.0009473, 0.001637};

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
            graphs[i]->SetTitle("Expo RMS vs. P1 statistical Error (1.4-3.0 Gev) ");
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
    canvas->SaveAs("ExpoS_RMS_vs_P1staterr_HighR_iteration.root");
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
/* ------------------------------------------------------------------------------------------*/
void Expo_L3_Mill_Mean_input() {
    const int n = 6;
    double stat_err1a[6] = {0.01271, 0.01672, 0.007366, 0.012, 0.016, 0.012};
    double stat_err1b[6] = {0.0043, 0.005196, 0.004654, 0.0101, 0.0050, 0.0034};
    double stat_err5a[6] = {0.00842, 0.008993, 0.003937, 0.011, 0.011, 0.008};
    double stat_err5b[6] = {0.004633, 0.004808, 0.003, 0.0041, 0.0044, 0.0026};
    double stat_err10a[6] = {0.005895, 0.006077, 0.003313, 0.0086, 0.017, 0.011};
    double stat_err10b[6] = {0.00375, 0.0033, 0.003194, 0.0054, 0.0043, 0.0036};
    double stat_err100a[6] = {0.002055, 0.00206, 0.001813, 0.0078, 0.0075, 0.0034};
    double stat_err100b[6] = {0.001433, 0.0015, 0.0009437, 0.0018, 0.0035, 0.0041};

    double RMS_1a[6] = {0.0133 , 0.02027 , 0.0275, 0.072, 0.0641, 0.133};
    double RMS_1b[6] = {0.01287, 0.01934, 0.08351, 0.197, 0.184, 0.206};
    double RMS_5a[6] = {0.00522, 0.00914, 0.02904, 0.038, 0.049, 0.047};
    double RMS_5b[6] = {0.0043, 0.0044, 0.0079, 0.082, 0.057, 0.054};
    double RMS_10a[6] = {0.00467, 0.00532, 0.00839, 0.0272, 0.0280, 0.0251};
    double RMS_10b[6] = {0.003019, 0.005956, 0.003956, 0.054, 0.0252, 0.060};
    double RMS_100a[6] = {0.00124, 0.00113, 0.0033, 0.0042, 0.0101, 0.0088};
    double RMS_100b[6] = {0.000594, 0.0009473, 0.001637, 0.028, 0.018, 0.0233};

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
    TCanvas* canvas = new TCanvas("AllGraphs", "L3-Expo_RMS_Staterror_comparison", 1200, 800);
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
            graphs[i]->SetTitle("Expo-L3 RMS vs. P1 statistical Error");
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
    canvas->SaveAs("Expo-L3_RMS_vs_P1staterr_12-5_iteration.root");
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

void Run4() {
    Expo_L3_Mill_Mean_input();
}















































































