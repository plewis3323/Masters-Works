#include <TH1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TFile.h>
#include <TRandom.h>
#include <TCanvas.h>

TH1F * fitf_master = 0;
TGraph * grff = 0;

//double fitf(Double_t * f, Double_t p);


Double_t fitf(Double_t *x,Double_t *par)
{

  return par[0]*grff->Eval(x[0]*par[1]);
  
}


//TF1 * f1 = 0;

void testslope(int stats)
{

  //  int offset = 100;
  int offset = 5;

  TH1F * htally = new TH1F("htally","",2000,0,2);

  int nhistobins = 400;
  
  //  if (f1 == 0)
  //    f1= new TF1("f_h1",fitf,0.001,10,2);



  for (int k = 0; k < 5; k++) {


    //underlying spectra shape in myexp ----------

    // perfect conditions, underlying spectra is really exponential
    //    TF1 * myexp = new TF1("myexp","expo",0.3,6);
    //myexp->SetParameters(10,-9.0);

    //    the following is a function from RHIC known to fit data
    // that is a more likely realistic underlying spectra shape
    // using this mimics more like real world currently
    TF1 * myexp = new TF1("myexpNot","[0]/([1]+x)^[2]",0.3,6);
    myexp->SetParameters(1e8,1.1,6);

    //    TF1 * myexp = new TF1("myGaus","gaus",0.2,6);
    //myexp->SetParameters(10^8,-0.3,0.5);

    //---------------------------


  // if offset = 0, reseeding the random number generator the 2nd time
  // (with the same number) in root
  // ensures that the _exact_ same list of energy random numbers are obtained
  // for shift and non-shift .. this eliminates ~half of the statistical error
  // in the ratio, [because there is no statistical fluctuation between the 
  // two sets of energy numbers, they are exactly the same set of numbers!] 
  // and is like what is done in Justin B's first
  // shift studies using the data because he shifts the 
  // same set of data energies

  gRandom->SetSeed(9939 + offset + k);


  TH1F * h1 = new TH1F("h1","",nhistobins,0,10);

  for (int i = 0; i<stats; i++)
    {

      //cout << myexp->GetRandom() << endl;

      h1->Fill( myexp->GetRandom() );     

    }

  fitf_master = (TH1F *) h1->Clone("fmaster");
  //  fitf_master->Rebin(2);
  fitf_master->Smooth(2);

  grff = new TGraph(fitf_master);
 

  TH1F * h2 = new TH1F("h2","",nhistobins,0,10);

  gRandom->SetSeed(9939 + k);
  //  myexp = new TF1("myexp2","expo",0,4);
  
  for (int j = 0; j<stats / 7; j++)
    {
      //cout << "sec:  " << myexp->GetRandom() << endl;
      h2->Fill( myexp->GetRandom()*1.015 );     
      

    }

  float startFit = 0.5;
  float endFit = 1.3;

  //  h1->Fit("expo","","",startFit,2.0);
  //  TF1 * ff1 = h1->GetFunction("expo");

  int binsf = h1->FindBin(startFit);
  int binef = h1->FindBin(endFit);


  float amprat = 
    h1->GetBinContent(binsf)/
    h2->GetBinContent(binsf);
 
  //  h2->Scale(amprat);

  //  TF1 * myexpo = new TF1("myexpo","expo",0.1,10);
  TF1 * myexpo = new TF1("myexpo",fitf,0.1,10,2);
  myexpo->SetParameters(1e4,1.0);
  
  /*
  cout << " par is " << ff1->GetParameter(0) << endl;
  myexpo->SetParameter(0,ff1->GetParameter(0)); 
  cout << " par is " << myexpo->GetParameter(0) << endl;
  //  myexpo->SetParameter(0,ff1->GetParameter(0)); 
  //  myexpo->SetParLimits(0,ff1->GetParameter(0),ff1->GetParameter(0)); 
  myexpo->FixParameter(0,ff1->GetParameter(0)); 
  cout << " par is " << myexpo->GetParameter(0) << endl;
  */

  h2->Fit("myexpo","","",0.5,1.2);

  h1->SetLineColor(2);
  h1->Draw("same");

  TF1 * ff2 = h2->GetFunction("myexpo");


  //  float basep1 = ff1->GetParameter(1);
  float basep1 = 1.0;

  float shiftp1 = ff2->GetParameter(1);


  /*
  //trunc mean 
  float basep1 = 0;
  float shiftp1 = 0; 
jkj
  float basep1w = 0;
  float shiftp1w = 0;


  for (int jj = binsf; jj < binef+1; jj++) {
    basep1 += h1->GetBinCenter(jj)*h1->GetBinContent(jj);
    shiftp1 += h2->GetBinCenter(jj)*h2->GetBinContent(jj);
    basep1w += h1->GetBinContent(jj);
    shiftp1w += h2->GetBinContent(jj);
  }
  basep1 = basep1/basep1w - startFit;
  shiftp1 = shiftp1/shiftp1w - startFit;

  */

  cout << "==== ratio is " << basep1/shiftp1 << endl;
  cout << "param 0 "<< ff2->GetParameter(0) << endl;

  htally->Fill(basep1/shiftp1);

  }
   
  TCanvas * c2 = new TCanvas("c2tally");
  htally->Draw();


}




// double fitf(Double_t *x,Double_t *par)
// {

//   return par[0]*grff->Eval(x[0]*par[1]);
  
// }

