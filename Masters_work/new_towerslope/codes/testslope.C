#include <TH1.h>
#include <TGraph.h>
#include <TFile.h>
#include <iostream>
#include <string>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TFile.h>
#include <TRandom.h>
#include <TCanvas.h>
using namespace std; 

TH1F * fitf_master = 0;
TGraph * grff = 0;

Double_t fitf(Double_t *x,Double_t *par)
{

  return par[0]*grff->Eval(x[0]*par[1]);
  
}


void testslope(int stats)
{ 

  int offset = 5;

  TH1F * htally = new TH1F("htally","",2000,0,2);

  int nhistobins = 400;

  for (int k = 0; k < 5; k++) {

    TF1 * myexp = new TF1("myexpNot","[0]/([1]+x)^[2]",0.3,6);
    myexp->SetParameters(1e8,1.1,6);
  gRandom->SetSeed(9939 + offset + k);


  TH1F * h1 = new TH1F("h1","",nhistobins,0,10);

  for (int i = 0; i<stats; i++)
    {

     

      h1->Fill( myexp->GetRandom() );     

    }

  fitf_master = (TH1F *) h1->Clone("fmaster");
  
  fitf_master->Smooth(2);

  grff = new TGraph(fitf_master);
 

  TH1F * h2 = new TH1F("h2","",nhistobins,0,10);

  gRandom->SetSeed(9939 + k);
  
  
  for (int j = 0; j<stats / 7; j++)
    {
      
      h2->Fill( myexp->GetRandom()*1.155);     
      

    }

  float startFit = 0.5;
  float endFit = 1.3;

  
  int binsf = h1->FindBin(startFit);
  int binef = h1->FindBin(endFit);


  float amprat = 
    h1->GetBinContent(binsf)/
    h2->GetBinContent(binsf);
 
  
  TF1 * myexpo = new TF1("myexpo",fitf,0.1,10,2);
  myexpo->SetParameters(1e4,1.0);
  


  h2->Fit("myexpo","","",0.5,1.2);

  h1->SetLineColor(2);
  h1->Draw("same");

  TF1 * ff2 = h2->GetFunction("myexpo");


  
  float basep1 = 1.0;

  float shiftp1 = ff2->GetParameter(1);


 

  cout << "==== ratio is " << basep1/shiftp1 << endl;
  cout << "param 0 "<< ff2->GetParameter(0) << endl;

  htally->Fill(basep1/shiftp1);

  }
   
  TCanvas * c2 = new TCanvas("c2tally");
  htally->Draw();


}




