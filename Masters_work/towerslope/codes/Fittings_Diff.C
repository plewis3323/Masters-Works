#include "testslope.h"
#include <TH1.h>
#include <TF1.h>
#include <TFile.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <TMath.h>
#include <iostream>

void Fittings_Diff(int stats) {
    // Setting up histograms and functions
    TH1F *htally = new TH1F("htally", "", 2000, 0, 2);
    int nhistobins = 400;
    TF1 *fits[3];
    fits[0] = new TF1("fit1", "expo", 0.1, 10); // Exponential fit
    fits[1] = new TF1("fit2", "gaus", 0.1, 10); // Gaussian fit
    fits[2] = new TF1("fit3", "[0]*TMath::Landau(x,[1],[2])", 0.1, 10); // Landau fit

    for (int k = 0; k < 5; k++) {
        TF1 *myexp = new TF1("myexpNot", "[0]/([1]+x)^[2]", 0.3, 6);
        myexp->SetParameters(1e8, 1.1, 6);
        gRandom->SetSeed(9939 + k);

        TH1F *h1 = new TH1F("h1", "", nhistobins, 0, 10);
        TH1F *h2 = new TH1F("h2", "", nhistobins, 0, 10);

        for (int i = 0; i < stats; i++) {
            h1->Fill(myexp->GetRandom());
            h2->Fill(myexp->GetRandom() * 1.04); // Adjusted scaling factor
        }

        // Fit and evaluate each function
        double chi2[3], ndf[3], aic[3], bic[3];

        for (int i = 0; i < 3; i++) {
            h2->Fit(fits[i], "Q"); // Quiet fit
            chi2[i] = fits[i]->GetChisquare();
            ndf[i] = fits[i]->GetNDF();
            aic[i] = 2 * fits[i]->GetNpar() + chi2[i];
            bic[i] = fits[i]->GetNpar() * log(stats) + chi2[i];
        }

        // Print results
        for (int i = 0; i < 3; i++) {
            std::cout << "Fit " << i + 1 << " - Chi2: " << chi2[i] << ", NDF: " << ndf[i]
                      << ", AIC: " << aic[i] << ", BIC: " << bic[i] << std::endl;
        }

        // Plotting
        TCanvas *c = new TCanvas("c", "Fits", 800, 600);
        h2->Draw();
        for (int i = 0; i < 3; i++) {
            fits[i]->SetLineColor(i + 1);
            fits[i]->Draw("same");
        }
        c->SaveAs(Form("fit_comparison_%d.png", k));
    }

    TCanvas *c2 = new TCanvas("c2tally");
    htally->Draw();
}

