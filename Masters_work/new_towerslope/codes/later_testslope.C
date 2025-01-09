/* #include <TH1.h> */
/* #include <TGraph.h> */
/* #include <TGraphErrors.h> */
/* #include <TF1.h> */
/* #include <TFile.h> */
/* #include <TRandom.h> */
/* #include <TCanvas.h> */

TH1F * fitf_master = 0;
TGraph * grff = 0;

TGraph * grff_orig = 0;

double fitf(Double_t * f, Double_t * p);

double origfn(Double_t * f, Double_t * p);

//TH1F * rebinHist1(TH1F * h1);
TH1F *  rebinHist1(TH1F * h1in, int rebin2=0);
void uniformizeErrs(TH1F * );

void drawInterp(TH1* hin, TH1*hout);

//TF1 * f1 = 0;

void testslope(char * nofile, int niter, int stats, float fitminx, float fitmaxx)
{


  ROOT::Math::MinimizerOptions::SetDefaultStrategy(2);

  //  int offset = 100;
  int offset = 5;


  //  float stats = 1e5;
  
  //  if (f1 == 0)
  //    f1= new TF1("f_h1",fitf,0.001,10,2);

  cout << "hello" << endl;
   TFile * inputfnf = new TFile("l3all_oe.root");
  cout << "file " << inputfnf << endl;
  //inputfnf->ls();
  TH1F * hc24= (TH1F *) inputfnf->Get("hcalout_eta_24");
  cout << hc24 << " hc24" << endl;
  hc24->Rebin(10);
  cout << "got " << endl;
  //  TH1F * hreb24 = rebinHist1(hc24,0);
  // grff_orig = (TGraph *) new TGraphErrors(hreb24);
 

  //  TF1 * myexp = new TF1("myexpNot",origfn, 0.001, 10,2);
  TF1 * myexp = new TF1("myexpNot","[0]*exp(-1.0*x^2/(2+[1])^2)/([2]+x)^[3]+[4]*exp([5]*x)",0.02,10.0);
  myexp->SetNpx(10000);
  
  myexp->SetParameters(2.5*6e4,2,0.1,2.7,750,-1);
  //  TF1 * myexp = new TF1("myexpNot","[0]/([1]+x)^[2]",0.001,10);
  //myexp->SetParameters(10^8,1.1,6);


  //  myexp->SetParameters(10^8,1.0);
  

  TFile * nnewof = new TFile(nofile,"RECREATE");

  cout << ".... " << endl;
  
  TH1F * htally = new TH1F("htally","",2000,0,2);

  TH1F * h1, *h2;

  int nhistobins = 1000;

  TDatime td;
  int dadseed = td.GetTime(); 
  cout << "seed is "<<  dadseed << endl;


  for (int k = 0; k < niter; k++) {

    cout << "iter " << k << endl;
    //underlying spectra shape in myexp ----------

    // perfect conditions, underlying spectra is really exponential
    //    TF1 * myexp = new TF1("myexp","expo",0.3,6);
    //myexp->SetParameters(10,-9.0);

    //    the following is a function from RHIC known to fit data
    // that is a more likely realistic underlying spectra shape
    // using this mimics more like real world currently
    /*
    TF1 * myexp = new TF1("myexpNot","[0]/([1]+x)^[2]",0.3,6);
    myexp->SetParameters(10^8,1.1,6);
    */

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

  gRandom->SetSeed(9939 + dadseed + offset + k);


  if (k > -1) 
    {
      TString nmh1 = "h1";
      nmh1 += k;
      h1 = new TH1F(nmh1.Data(),"",nhistobins,0,10);
      
      
      for (int i = 0; i<1.*stats; i++)
	{
	  if (i%100000 == 0)
	    cout << " stats1: " << i << endl; 
	  //cout << myexp->GetRandom() << endl;
	  
	  //	  h1->Fill( myexp->GetRandom() );     
	  h1->Fill( hc24->GetRandom() );     
	  
	}

      //  cout << endl << "end " << endl;

      //      h1 = rebinHist1(h1,1);
  
  fitf_master = (TH1F *) h1->Clone("fmaster");
  //  fitf_master->Rebin(2);
  //  fitf_master->Smooth(2);



  grff = new TGraph(fitf_master);
  //  grff->SetName("grf_fmast");

    }


  TString nmh2 = "h2";
  nmh2 += k;
  h2 = new TH1F(nmh2.Data(),"",nhistobins,0,10);

  gRandom->SetSeed(9939 + dadseed + k);
  //  myexp = new TF1("myexp2","expo",0,4);
  
  for (int j = 0; j<stats; j++)
    {
      if (j%100000 == 0)
	cout << " numevs2: " << j << endl; 

      //cout << "sec:  " << myexp->GetRandom() << endl;
      //      h2->Fill( myexp->GetRandom()*1.05 );     
      h2->Fill( hc24->GetRandom()*1.05 );     
      

    }


  //  h2 = rebinHist1(h2,1);

  //uniformizeErrs(h2);
  //  cout << endl << " endNumevs" << endl; 
 
  float startFit = 0.5;
  float endFit = 2.0;

  //  h1->Fit("expo","","",startFit,2.0);
  //  TF1 * ff1 = h1->GetFunction("expo");

  int binsf = h1->FindBin(startFit);
  int binef = h1->FindBin(endFit);


  float amprat = 
    h1->GetBinContent(binsf)/
    h2->GetBinContent(binsf);
 
  //  h2->Scale(amprat);

 
  //  TF1 * myexpo = new TF1("myexpo","expo",0.1,10);
  TF1 * myexpo = new TF1("myexpo",fitf,0.05,9,2);
  myexpo->SetParameters(0.6,1.0);
  /* myexpo->SetParLimits(0,1e-8,1e10); */
  
  /* myexpo->SetParLimits(1,1.0/1.05,1.0/1.05); */

  h2->Fit("myexpo","","",fitminx,fitmaxx); 
  //  h2->Fit("myexpo","","",0.5,1.2);
  
  //myexpo->SetParLimits(1,0.7,1.4);
  
  /*
  cout << " par is " << ff1->GetParameter(0) << endl;
  myexpo->SetParameter(0,ff1->GetParameter(0)); 
  cout << " par is " << myexpo->GetParameter(0) << endl;
  //  myexpo->SetParameter(0,ff1->GetParameter(0)); 
  //  myexpo->SetParLimits(0,ff1->GetParameter(0),ff1->GetParameter(0)); 
  myexpo->FixParameter(0,ff1->GetParameter(0)); 
  cout << " par is " << myexpo->GetParameter(0) << endl;
  */

  //  myexpo->SetParameter(1,1.0/1.05);

  //  h2->Fit("myexpo","","",fitminx,fitmaxx);

  cout << "fit " << fitminx<< " "  << fitmaxx << endl;
  //h2->Fit("myexpo","","",0.07,1.0);

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



  /* h1->Write(); */
  /* h2->Write(); */
  /* htally->Write(); */
  /* hreb24->Write(); */

  nnewof->Write();  

  TCanvas * c2 = new TCanvas("c2tally");
  htally->Draw();

  

}

void uniformizeErrs(TH1F * hin)
{

  for (int i = 1; i< hin->GetNbinsX()+1;  i++) 
    {
      if (hin->GetBinError(i)/hin->GetBinContent(i) < 0.08)
	hin->SetBinError(i, 0.08*hin->GetBinContent(i));
    }

}


double fitf(Double_t *x,Double_t *par)
{

  return par[0]*grff->Eval(x[0]*par[1]);
  
}


double origfn(Double_t *x,Double_t *par)
{

  return par[0]*grff_orig->Eval(x[0]*par[1]);
  
}


void drawInterp(TH1* hin, TH1*hout)
{

  fitf_master = (TH1F *) hin->Clone("fInterp");
  grff = new TGraph(fitf_master);
 
  hout = new TH1F("hinterp","",5e6, 0,10.0);
  for (long i =0; i< 1e6; i++)
    {
      if (i%100000 == 0)
	cout << "progress " << i << endl; 
      hout->SetBinContent(i+1,grff->Eval(hout->GetBinCenter(i+1) ));
    }

  cout <<"made, drawing" << endl;
  hout->SetLineColor(2);
  hout->Draw("same");

}

TH1F *  rebinHist1(TH1F * h1in, int rebin2)
{

  cout << "in " << endl;
  long nbinsX = h1in->GetNbinsX();
  TH1F * h1out =0; 
  
  float barrs[7]  = {0.1,0.5,1.0, 2.5, 4.0,  7.0,  10.0};
  int barMult[7]=   {1,    5 ,50, 200, 1000,  5000,10000};
  // int barMult[7]=   {1,  10 ,100, 1000,5000, 10000,50000};

  if (rebin2)
    {
      cout << "alt rebin" << endl;
      // for 1000 bins
      barMult[0] = 1;
      barMult[1] = 1; // 20 times (100/5)
      barMult[2] = 5; // 10 times bigger
      barMult[3] = 10; // 5 times  1000/
      barMult[4] = 50; // 5 times 100/5
      barMult[5] = 50; // times
      barMult[6] = 100; //1 times
      //barMult[0] = 1;
     

    }

  float barrsNum[20] = {0};

  int numBarrs = 7;

  int itb = 0;

  cout << "in rebin " << endl;

  float binsb[50000] = {0};
  float  binct[50000] = {0};
  float  bincte[50000] = {0};
  float  binwt[50000] = {0};
  
  cout << "in rebin 2" << endl;

  
  int itbins = 0;

  float binwv = 1e-5;

  cout << " rebin var sized " << endl;
  
  for (int ik = 1; ik < nbinsX+1; ik++)
    {

      float ptb = h1in->GetBinCenter(ik);
      if (ptb > barrs[itb])
	itb++;

      int svik = ik;
      for (int ikj = svik; ikj < svik+barMult[itb]; ikj++)
	{
	  //float ptbj = h1in->GetBinCenter(ik);
	  binct[itbins] += h1in->GetBinContent(ikj);
	  //	  ik++;	      
	}
      bincte[itbins] = sqrt(binct[itbins]);
      binwt[itbins] = 1.0*barMult[itb];
      //      binct[itbins] /= 1.0*barMult[itb];
      // bincte[itbins] /= 1.0*barMult[itb];
      
      ik = svik + barMult[itb]-1;
      binsb[itbins+1] = h1in->GetBinCenter(ik) + binwv/2.0;

      if (itb > 1)
	cout << itbins << "::" << binsb[itbins+1] << "[" << barMult[itb] << "] ";
      itbins++;

    }

  TString nRebNm = h1in->GetName();
  nRebNm += "_reb";

  h1out = new TH1F(nRebNm.Data(), "", itbins,binsb);
  h1out->Sumw2();
  
  for (int ibk = 1; ibk < itbins+1; ibk++)
    {
      if (rebin2 >= 0)
	{
	  if (binct[ibk-1] > 0 && binwt[ibk-1] > 0)
	    {
	      h1out->SetBinContent(ibk,binct[ibk-1]/binwt[ibk-1]);
	      h1out->SetBinError(ibk,bincte[ibk-1]/binwt[ibk-1]);
	    }
	}
      else 
	{
	  for (int subi =0; subi < binct[ibk]; subi++)
	    {
	      h1out->Fill(h1out->GetBinCenter(ibk), 1.0/binwt[ibk]);
	    }

	}  

    }

  return h1out;

}


TH1F * makeLog(TH1*hin)
{

  TString tsn = hin->GetName();
  tsn = tsn + "_log";
  int kNbins = hin->GetNbinsX();
  TH1F * hout = (TH1F *) hin->Clone(tsn.Data());
  hout->Reset();
    //= new TH1D(tsn.Data(),"",
    //			 ,hin->GetBinLowEdge(1)
    //			 ,hin->GetBinLowEdge()+hin->GetBinWidth(
  
    for (int ibk = 1; ibk < hin->GetNbinsX()+1; ibk++)
    {

      if (hin->GetBinContent(ibk) > 0)
	hout->SetBinContent(ibk,log10(hin->GetBinContent(ibk)));
      //else
      //cout << ibk << " bin lteq 0" << endl;
      //hout->SetBinError(ibk,log10(hin->GetBinError(ibk)));;
    }

    return hout;

}


TH1F * undoLog(TH1*hin)
{

  TString tsn = hin->GetName();
  tsn = tsn + "_unlog";
  int kNbins = hin->GetNbinsX();
  TH1F * hout = (TH1F *) hin->Clone(tsn.Data());
  hout->Reset();
    //= new TH1D(tsn.Data(),"",
    //			 ,hin->GetBinLowEdge(1)
    //			 ,hin->GetBinLowEdge()+hin->GetBinWidth(
  
  for (int ibk = 1; ibk < hin->GetNbinsX()+1; ibk++)
    {
      
      hout->SetBinContent(ibk,pow(10.0,hin->GetBinContent(ibk)));
      hout->SetBinError(ibk, pow(10.0,hin->GetBinError(ibk)));;
      
    }
  
    return hout;

}


TH1F * giveHistFromGraph(TH1F * hin, TGraph * tgin)
{

  TString tsn = hin->GetName();
  tsn = tsn + "_h1ftg";
  int kNbins = hin->GetNbinsX();
  
  TH1F * hout = (TH1F *) hin->Clone(tsn.Data());
  hout->Reset();

  for (int ibk = 1; ibk < kNbins+1; ibk++)
    {      
      hout->SetBinContent(ibk,tgin->Eval(hin->GetBinCenter(ibk)));
      //      hout->SetBinError(ibk,1
    }
  
  return hout;

} 

