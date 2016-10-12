// ################################################################
// # Program to analyse gain and s-curve files from Captan system #
// ################################################################
//.L CaptanPlots.C++
//GainCurve("Gain_RSPA_Rad_SmallPitch_Zone_linear_2_20_0.txt","RSPA_gain_linear.root",0)
//SCurve("SCurve_RSPA_SmallPitch_0_20_0.txt","SCurve_RSPA.root","")

#include <TROOT.h>
#include <TCanvas.h>
#include <TGaxis.h>
#include <TStyle.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TMath.h>
#include <TFitResult.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>


// ######################
// # Internal constants #
// ######################
#define PRINTMSG false // Print useful messages on screen
#define STATIONID 4
#define ROCID 0
#define ELEVCALLo  50  // Electrons per Vcal unit (low range)
#define ELEVCALHi 350  // Electrons per Vcal unit (high range = low range * 1800 / 260)
#define NROW 80
#define NCOL 52


// ##########################
// # Set histo layout style #
// ##########################
void SetStyle ()
{
  gROOT->SetStyle("Plain");
  gROOT->ForceStyle();
  gStyle->SetPalette(1);
  gStyle->SetTextFont(42);

  gStyle->SetOptTitle(0);
  gStyle->SetOptFit(1112);
  gStyle->SetOptStat(0);

  gStyle->SetPadRightMargin(0.10);
  gStyle->SetPadTopMargin(0.11);
  gStyle->SetPadBottomMargin(0.12);

  gStyle->SetTitleFont(42,"x");
  gStyle->SetTitleFont(42,"y");
  gStyle->SetTitleFont(42,"z");

  gStyle->SetTitleOffset(1.05,"x");
  gStyle->SetTitleOffset(0.95,"y");

  gStyle->SetTitleSize(0.05,"x");
  gStyle->SetTitleSize(0.05,"y");
  gStyle->SetTitleSize(0.05,"z");

  gStyle->SetLabelFont(42,"x");
  gStyle->SetLabelFont(42,"y");
  gStyle->SetLabelFont(42,"z");
  
  gStyle->SetLabelSize(0.05,"x");
  gStyle->SetLabelSize(0.05,"y");
  gStyle->SetLabelSize(0.05,"z");

  //  TGaxis::SetMaxDigits(3);
  gStyle->SetStatY(0.9);
  gStyle->SetStatW(0.15);
}


double scurveFunction (double* x, double* par)
// #######################
// # Function parameters #
// # par[0] = scale      #
// # par[1] = mean       #
// # par[2] = sigma      #
// #######################
{
  return 1. - 1./2. * TMath::Erfc((x[0] - par[0]) / par[1]);
}




void SCurve(string fileIn, string fileOut, string quadrant)
{
  SetStyle();

  
  // #######################
  // # Internal parameters #
  // #######################
  double meanThr   = 2500.;
  double meanNoise =  300.;
  // # Cuts for fit #
  double probThr  = 0.0;  // Discard events below probThr [0.001]
  double ampliThr = 0.0;  // Discard events below ampliThr [0.5]
  double thrMin   = 1000; // Minimal threshold [e-]
  double thrMax   = 5000; // Maximal threshold [e-]
  // ################
  double minError   =   0.05; // Minimal error at saturation
  double boostError =   4.;   // Error scale factor
  double sigmaThr   =   5.;   // +/- sigmaThr*sigma fit range
  double xMin       =  0. * ELEVCALLo; // [e-]
  double xMax       = 100. * ELEVCALLo; // [e-]
  int nBins         = (xMax - xMin) / ELEVCALLo / 5; //5 should be the step size used for doing the curve at the test beam
  int beyondMaxBin  = 10; // # bins beyond max bin

  stringstream myString;
  ifstream fileInID;

  TF1* myFit;
  TH1D* hSCurves[NROW][NCOL];
  TH1D* hPar0_25x600 = new TH1D("hPar0_25x600","hPar0",60,1200,3500);
  TH1D* hPar0_50x300 = new TH1D("hPar0_50x300","hPar0",60,1200,3500);
  TH1D* hPar0_100x150 = new TH1D("hPar0_100x150","hPar0",60,1200,3500);
  TH1D* hPar1_25x600 = new TH1D("hPar1_25x600","hPar1",35,0,1000);
  TH1D* hPar1_50x300 = new TH1D("hPar1_50x300","hPar1",35,0,1000);
  TH1D* hPar1_100x150 = new TH1D("hPar1_100x150","hPar1",35,0,1000);
  TH2D* hGoodFitMap = new TH2D("hGoodFitMap","hGoodFitMap",NCOL,0,NCOL,NROW,0,NROW);
  hGoodFitMap->SetXTitle("column");
  hGoodFitMap->SetYTitle("row");
  TH2D* hPar0Map = new TH2D("hPar0Map","hPar0Map",NCOL,0,NCOL,NROW,0,NROW);
  hPar0Map->SetXTitle("column");
  hPar0Map->SetYTitle("row");
  TH2D* hPar1Map = new TH2D("hPar1Map","hPar1Map",NCOL,0,NCOL,NROW,0,NROW);
  hPar1Map->SetXTitle("column");
  hPar1Map->SetYTitle("row");


  // ###################
  // # Open input file #
  // ###################
  fileInID.open(fileIn.c_str(),ifstream::in);
  if (fileInID.good() == false)
    {
      cout << "[CaptanPlots::SCurve]\tError opening file: " << fileIn.c_str() << endl;
      exit (EXIT_FAILURE);
    }


  // ##########################
  // # Booking the histograms #
  // ##########################
  for (int i = 0; i < NROW; i++)
    {
      for (int j = 0; j < NCOL; j++)
	{
	  myString.clear(); myString.str("");
	  myString << "Station:"<< STATIONID << " Plaq: 2 - ROC:" << ROCID << " - r:" << i << " - c:" << j;
	  hSCurves[i][j] = new TH1D(myString.str().c_str(),myString.str().c_str(),nBins,xMin,xMax);
	  hSCurves[i][j]->SetXTitle("charge (electrons)");
	  hSCurves[i][j]->SetYTitle("efficiency");
	}
    }
  TF1* fitFun = new TF1("fitFun",scurveFunction,xMin,xMax,2);
  fitFun->SetLineColor(kBlue);


  // #######################
  // # Scan the input file #
  // #######################
  string line;
  double injCharge = 0;
  int Vcal;
  int nIter;
  int nInj;
  while (getline(fileInID,line))
    {
      string tmp;
      int nHits;
      int row;
      int col;

      myString.clear(); myString.str("");
      myString << line;

      if (line.find("ITERATIONS") != string::npos)
	{
	  myString >> tmp >> nIter;

	  if (PRINTMSG == true) cout << "[CaptanPlots::SCurve]\tRead total number of iterations: " << nIter << endl;
	}
      else if (line.find("INJECT_NUM") != string::npos)
	{
	  myString >> tmp >> nInj;

	  if (PRINTMSG == true) cout << "[CaptanPlots::SCurve]\tRead total number of injections: " << nInj << endl;
	}
      else if (line.find("Iteration") != string::npos)
	{
	  myString >> tmp >> tmp >> tmp >> tmp >> tmp >> Vcal;
	  
	  injCharge = Vcal * ELEVCALLo;
	  
	  if (PRINTMSG == true) cout << "[CaptanPlots::SCurve]\tRead injected charge: " << injCharge << endl;
	}
      else if ((line.find("r ") != string::npos) &&
	       (line.find("c ") != string::npos) &&
	       (line.find("h ") != string::npos) &&
	       (line.find("a ") != string::npos))
	{
	  myString >> tmp >> row >> tmp >> col >> tmp >> nHits;

	  hSCurves[row][col]->SetBinContent(hSCurves[row][col]->FindBin(injCharge),static_cast<double>(nHits) / static_cast<double>(nInj));
	}
    }


  // ####################
  // # Open output file #
  // ####################
  TFile* fileOutID = TFile::Open(fileOut.c_str(),"RECREATE");
  fileOutID->mkdir("SCurves/");
  fileOutID->cd("/SCurves/");


  // ####################
  // # Perform the fits #
  // ####################
  for (int i = 0; i < NROW; i++)
    {
      for (int j = 0; j < NCOL; j++)
	{
	  // #######################
	  // # Set binomial errors #
	  // #######################
	  for (int k = 0; k < hSCurves[i][j]->GetNbinsX(); k++)
	    {
	      double error = boostError * hSCurves[i][j]->GetBinContent(k+1) * (1. - hSCurves[i][j]->GetBinContent(k+1)) / nInj;
	      if (error < minError) error = minError;
	      hSCurves[i][j]->SetBinError(k+1,error);
	    }


	  // ############################
	  // # Find right range for fit #
	  // ############################
	  /*double getMax = hSCurves[i][j]->GetMaximum();
	  int maxBin    = (hSCurves[i][j]->GetMaximumBin() + beyondMaxBin < hSCurves[i][j]->GetNbinsX() ? hSCurves[i][j]->GetMaximumBin() + beyondMaxBin : hSCurves[i][j]->GetNbinsX());
	  fitFun->SetRange(xMin,hSCurves[i][j]->GetBinLowEdge(maxBin));*/
	  fitFun->SetParLimits(1,0,3000);
		
	  // #################
	  // # Start the fit #
	  // #################
	  fitFun->SetRange(xMin,xMax);
	  fitFun->SetParNames("#mu","#sigma");
	  fitFun->SetParameters(meanThr,meanNoise);
	  hSCurves[i][j]->Fit(fitFun,"QR");


	  // #############################
	  // # Recalibrate the fit range #
	  // #############################
	  /*fitFun->SetRange((fitFun->GetParameter(0) - sigmaThr * fitFun->GetParameter(1) < xMin ? xMin : fitFun->GetParameter(0) - sigmaThr * fitFun->GetParameter(1)),
			   (fitFun->GetParameter(0) + sigmaThr * fitFun->GetParameter(1) > hSCurves[i][j]->GetBinLowEdge(maxBin) ?
			    hSCurves[i][j]->GetBinLowEdge(maxBin) : fitFun->GetParameter(0) + sigmaThr * fitFun->GetParameter(1)));
	  hSCurves[i][j]->Fit(fitFun,"QR");*/

	      
	      //if (fitFun->GetParameter(1)< 50){
	      /*cout << "[CaptanPlots::SCurve]\tCell fit parameters r:" << i << " - c:" << j << "\t";
              cout << fitFun->GetParameter(1) << "\t";
	      cout << fitFun->GetChisquare() << "\t"<< endl;*/


	  if (PRINTMSG == true)
	    {
	      cout << "[CaptanPlots::SCurve]\tCell fit parameters r:" << i << " - c:" << j << "\t";
	      cout << fitFun->GetParameter(0) << "\t";
	      cout << fitFun->GetParameter(1) << "\t";
	      cout << fitFun->GetChisquare()  << "\t";
	      cout << fitFun->GetNDF()        << "\t";
	      cout << fitFun->GetProb()       << "\t" << endl;
	    }


	  // ##################################################
	  // # Filling overall threhsold and noise histograms #
	  // ##################################################
 	  //if ((hSCurves[i][j]->GetEntries() == 0) || (fitFun->GetProb() < probThr) || (getMax < ampliThr) || (fitFun->GetParameter(0) < thrMin) || (fitFun->GetParameter(0) > thrMax))
	  if ((hSCurves[i][j]->GetEntries() < 30))// || (fitFun->GetProb() < probThr) || (fitFun->GetParameter(0) < thrMin) || (fitFun->GetParameter(0) > thrMax))
	    {
	      if (PRINTMSG == true)
		{
		  cout << "[CaptanPlots::SCurve]\tFit probability lower than threshold (" << probThr << "): " << fitFun->GetProb();
		  cout << " or asymptotic value lower than " << ampliThr;
		  cout << " or amplitude greater than one " << endl;
		}
	    }
	  else
	    {
	      if (PRINTMSG == true) cout << "[CaptanPlots::SCurve]\tGood cell fit r:" << i << " - c:" << j << endl;
              
		//if(fitFun->GetChisquare()<10 && fitFun->GetChisquare()!=0){
 		if(i>49)
		  {
		    hPar0_25x600->Fill(fitFun->GetParameter(0));
		    hPar1_25x600->Fill(fitFun->GetParameter(1));
		    hGoodFitMap->Fill(j,i);
		    hPar0Map->SetBinContent(j+1,i+1,fitFun->GetParameter(0));
		    hPar1Map->SetBinContent(j+1,i+1,fitFun->GetParameter(1));
		  }
 		if(i>19 && i<50)
		  {
		    hPar0_50x300->Fill(fitFun->GetParameter(0));
		    hPar1_50x300->Fill(fitFun->GetParameter(1));
		    hGoodFitMap->Fill(j,i);
		    hPar0Map->SetBinContent(j+1,i+1,fitFun->GetParameter(0));
		    hPar1Map->SetBinContent(j+1,i+1,fitFun->GetParameter(1));
		  }
 		if(i<20)
		  {
		    hPar0_100x150->Fill(fitFun->GetParameter(0));
		    hPar1_100x150->Fill(fitFun->GetParameter(1));
		    hGoodFitMap->Fill(j,i);
		    hPar0Map->SetBinContent(j+1,i+1,fitFun->GetParameter(0));
		    hPar1Map->SetBinContent(j+1,i+1,fitFun->GetParameter(1));
		  }
	    
		//}
	    }
	  hSCurves[i][j]->Write();
	}
    }


  fileOutID->cd("/");


  

  // #####################
  // # Average in window #
  // #####################
  /*  unsigned int colStart = 0; // [ 2 - 28]
  unsigned int colEnd   = 0; // [27 - 50]
  unsigned int rowStart = 0; // [2  - 43]
  unsigned int rowEnd   = 0; // [42 - 78]
  if (quadrant == "q11")
    {
      colStart =  2;
      colEnd   = 27;
      rowStart =  2;
      rowEnd   = 42;
    }
  else if (quadrant == "q12")
    {
      colStart = 28;
      colEnd   = 50;
      rowStart =  2;
      rowEnd   = 42;
    }
  else if (quadrant == "q21")
    {
      colStart =  2;
      colEnd   = 27;
      rowStart = 43;
      rowEnd   = 78;
    }
  else if (quadrant == "q22")
    {
      colStart = 28;
      colEnd   = 50;
      rowStart = 43;
      rowEnd   = 78;
    }
  double avg = 0;
  unsigned int counter = 0;
  for (unsigned int i = 0; i < NROW; i++)
    for (unsigned int j = 0; j < NCOL; j++)
      if ((i >= rowStart) && (i <= rowEnd) && (j >= colStart) && (j <= colEnd) && hPar0Map->GetBinContent(j,i) > thrMin)
	{
	  avg += hPar0Map->GetBinContent(j,i);
	  counter += 1;
	}
  avg = avg / counter;
  cout << "Average threshold in quadrant (r:" << rowStart << "-" << rowEnd << " - c:" << colStart << "-" << colEnd << "): " << avg << endl;
  */



  // ###############################################
  // # Draw and fit threshold and noise histograms #
  // ###############################################
  //  TCanvas* c0_25x600 = new TCanvas("c0","c0_25x600",10,10,700,500);
  TCanvas* c0 = new TCanvas("c0","c0",10,10,700,500);
  //  c0_25x600->cd();
  c0->cd();

  //hPar0_25x600->Fit("gaus");
  //myFit = hPar0_25x600->GetFunction("gaus");
  //myFit->SetLineColor(kRed);
  hPar0_25x600->SetXTitle("threshold (electrons)");
  hPar0_25x600->SetYTitle("entries");
  hPar0_25x600->SetMaximum(400);
  hPar0_25x600->SetLineColor(kAzure);
  hPar0_25x600->Draw();
  hPar0_25x600->Write();
  //  c0_25x600->Modified();
  //c0_25x600->Update();
  //c0_25x600->Print("Threshold.pdf");

  //  TCanvas* c0_50x300 = new TCanvas("c0_50x300","c0_50x300",10,10,700,500);
  //c0_50x300->cd();

  //hPar0_50x300->Fit("gaus");
  //myFit = hPar0_50x300->GetFunction("gaus");
  //myFit->SetLineColor(kRed);
  hPar0_50x300->SetXTitle("threshold (electrons)");
  hPar0_50x300->SetYTitle("entries");
  hPar0_50x300->Write();
  hPar0_50x300->SetLineColor(kRed);
  hPar0_50x300->Draw("SAME");
  //c0_50x300->Modified();
  //c0_50x300->Update();
  //c0_50x300->Print("Threshold.pdf");

  //  TCanvas* c0_100x150 = new TCanvas("c0_100x150","c0_100x150",10,10,700,500);
  //c0_100x150->cd();

  //hPar0_100x150->Fit("gaus");
  //myFit = hPar0_100x150->GetFunction("gaus");
  //myFit->SetLineColor(kRed);
  hPar0_100x150->SetXTitle("threshold (electrons)");
  hPar0_100x150->SetYTitle("entries");
  hPar0_100x150->Write();
  hPar0_100x150->SetLineColor(kBlack);
  hPar0_100x150->Draw("SAME");
  //c0_100x150->Modified();
  //c0_100x150->Update();
  //  c0_100x150->Print("Threshold.pdf");


  TLatex * tPrel = new TLatex();

  tPrel->SetNDC();
  tPrel->SetTextColor(kBlack);
  tPrel->SetTextSize(0.03027386);
  tPrel->SetTextFont(42);
  tPrel->DrawLatex(0.09821429,0.9194523,"Test Beam FNAL, June 2016, SPb2a");
  TLegend *leg = new TLegend(0.7,0.7,0.85,0.8);//,NULL,"brNDC");                                                                       
  leg->SetBorderSize(1);
  leg->SetLineColor(0);
  leg->SetLineStyle(1);
  leg->SetLineWidth(2);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.03);
  //TLegendEntry *entry=                                                                                                                               
  leg->AddEntry(  hPar0_100x150,"100x150","lpf");
  leg->AddEntry(  hPar0_50x300,"50x300","lpf");
  leg->AddEntry(  hPar0_25x600,"25x600","lpf");
  leg->Draw();

  c0->Print("../Tesi/calibrations_images/June2016Threshold3conf.eps");

  /*  TCanvas* c1_25x600 = new TCanvas("c1_25x600","c1_25x600",10,10,700,500);
  c1_25x600->cd();
  hPar1_25x600->Draw();
  //hPar1_25x600->Fit("gaus");
  //myFit = hPar1_25x600->GetFunction("gaus");
  //myFit->SetLineColor(kRed);
  hPar1_25x600->SetXTitle("noise (electrons)");
  hPar1_25x600->SetYTitle("entries (#)");
  hPar1_25x600->Write();
  c1_25x600->Modified();
  c1_25x600->Update();
  c1_25x600->Print("Noise.pdf");

  TCanvas* c1_50x300 = new TCanvas("c1_50x300","c1_50x300",10,10,700,500);
  c1_50x300->cd();
  hPar1_50x300->Draw();
  //hPar1_50x300->Fit("gaus");
  //myFit = hPar1_50x300->GetFunction("gaus");
  //myFit->SetLineColor(kRed);
  hPar1_50x300->SetXTitle("noise (electrons)");
  hPar1_50x300->SetYTitle("entries (#)");
  hPar1_50x300->Write();
  c1_50x300->Modified();
  c1_50x300->Update();
  c1_50x300->Print("Noise.pdf");

  TCanvas* c1_100x150 = new TCanvas("c1_100x150","c1_100x150",10,10,700,500);
  c1_100x150->cd();
  hPar1_100x150->Draw();
  //hPar0_100x150->Fit("gaus");
  //myFit = hPar1_100x150->GetFunction("gaus");
  //myFit->SetLineColor(kRed);
  hPar1_100x150->SetXTitle("noise (electrons)");
  hPar1_100x150->SetYTitle("entries (#)");
  hPar1_100x150->Write();
  c1_100x150->Modified();
  c1_100x150->Update();
  c1_100x150->Print("Threshold.pdf");
  */

  TCanvas* c2 = new TCanvas("c2","c2",10,10,700,500);
  //  c0_25x600->cd();
  c2->cd();

  //hPar0_25x600->Fit("gaus");
  //myFit = hPar0_25x600->GetFunction("gaus");
  //myFit->SetLineColor(kRed);
  hPar1_25x600->SetXTitle("noise (electrons)");
  hPar1_25x600->SetYTitle("entries");
  hPar1_25x600->SetLineColor(kAzure);
  hPar1_25x600->SetMaximum(300);
  hPar1_25x600->Draw();
  hPar1_25x600->Write();
  //  c0_25x600->Modified();
  //c0_25x600->Update();
  //c0_25x600->Print("Threshold.pdf");

  //  TCanvas* c0_50x300 = new TCanvas("c0_50x300","c0_50x300",10,10,700,500);
  //c0_50x300->cd();

  //hPar0_50x300->Fit("gaus");
  //myFit = hPar0_50x300->GetFunction("gaus");
  //myFit->SetLineColor(kRed);
  hPar1_50x300->SetXTitle("noise (electrons)");
  hPar1_50x300->SetYTitle("entries");
  hPar1_50x300->Write();
  hPar1_50x300->SetLineColor(kRed);
  hPar1_50x300->Draw("SAME");
  //c0_50x300->Modified();
  //c0_50x300->Update();
  //c0_50x300->Print("Threshold.pdf");

  //  TCanvas* c0_100x150 = new TCanvas("c0_100x150","c0_100x150",10,10,700,500);
  //c0_100x150->cd();

  //hPar0_100x150->Fit("gaus");
  //myFit = hPar0_100x150->GetFunction("gaus");
  //myFit->SetLineColor(kRed);
  hPar1_100x150->SetXTitle("noise (electrons)");
  hPar1_100x150->SetYTitle("entries");
  hPar1_100x150->Write();
  hPar1_100x150->SetLineColor(kBlack);
  hPar1_100x150->Draw("SAME");
  //c0_100x150->Modified();
  //c0_100x150->Update();
  //  c0_100x150->Print("Threshold.pdf");


  TLatex * tPrel2 = new TLatex();

  tPrel2->SetNDC();
  tPrel2->SetTextColor(kBlack);
  tPrel2->SetTextSize(0.03027386);
  tPrel2->SetTextFont(42);
  tPrel2->DrawLatex(0.09821429,0.9194523,"Test Beam FNAL, June 2016, SPb2a");
  TLegend *leg2 = new TLegend(0.7,0.671131,0.85,0.8);//,NULL,"brNDC");                                                                       
  leg2->SetBorderSize(1);
  leg2->SetLineColor(0);
  leg2->SetLineStyle(1);
  leg2->SetLineWidth(2);
  leg2->SetFillColor(0);
  leg2->SetFillStyle(0);
  leg2->SetTextSize(0.03);  
//TLegendEntry *entry=                                                                                                                               
//  leg2->AddEntry(  hPar1_100x150,"100x150","lpf");
  leg2->AddEntry(  hPar1_50x300,"50x300","lpf");
  leg2->AddEntry(  hPar1_25x600,"25x600","lpf");
  leg2->Draw();

  c2->Print("../Tesi/calibrations_images/June2016Noise3conf.eps");


  TCanvas* cMap = new TCanvas("cMap","cGOODfitMap",10,10,700,500);
  cMap->cd();
  hGoodFitMap->Draw("gcolz");
  hGoodFitMap->Write();
  cMap->Modified();
  cMap->Update();
  cMap->Print("GoodFitMap.pdf");
  
  TCanvas* cP0Map = new TCanvas("cP0Map","cP0Map",10,10,700,500);
  cP0Map->cd();


  hPar0Map->GetZaxis()->SetRangeUser(1000,7000);
  TPaletteAxis *palette = new TPaletteAxis(hPar0Map->GetXaxis()->GetXmax()+0.01,hPar0Map->GetYaxis()->GetXmin(),hPar0Map->GetXaxis()->GetXmax()+hPar0Map->GetXaxis()->GetXmax()*0.048,hPar0Map->GetYaxis()->GetXmax(),hPar0Map);
  palette->SetLabelColor(1);
  palette->SetLabelFont(42);
  palette->SetLabelOffset(0.005);
  palette->SetLabelSize(0.025);
  //    palette->SetTitleOffset(1);                                                                                                                     
  palette->SetTitleSize(0.04);
  palette->SetFillColor(100);
  palette->SetFillStyle(1001);

  palette->SetX1NDC(0.9);
  palette->SetX2NDC(0.95);
  palette->SetY1NDC(0.2);
  palette->SetY2NDC(0.8);
  hPar0Map->GetListOfFunctions()->Add(palette,"br");
  hPar0Map->Draw("gcolz");
  hPar0Map->Write();
  cP0Map->Modified();
  cP0Map->Update();
  TLatex * tPrel3 = new TLatex();
  tPrel3->SetNDC();
  tPrel3->SetTextColor(kBlack);
  tPrel3->SetTextSize(0.03027386);
  tPrel3->SetTextFont(42);
  tPrel3->DrawLatex(0.09821429,0.9194523,"Test Beam FNAL, May 2016, Dut2=RSPA");
  cP0Map->Print("../../../../Tesi/results_images/May2016/Par0Map.eps");

  TCanvas* cP1Map = new TCanvas("cP1Map","cP1Map",10,10,700,500);
  cP1Map->cd();
  hPar1Map->GetZaxis()->SetRangeUser(0,1000);
  TPaletteAxis *palette = new TPaletteAxis(hPar1Map->GetXaxis()->GetXmax()+0.01,hPar1Map->GetYaxis()->GetXmin(),hPar1Map->GetXaxis()->GetXmax()+hPar1Map->GetXaxis()->GetXmax()*0.048,hPar1Map->GetYaxis()->GetXmax(),hPar1Map);
  palette->SetLabelColor(1);
  palette->SetLabelFont(42);
  palette->SetLabelOffset(0.005);
  palette->SetLabelSize(0.025);
  //    palette->SetTitleOffset(1);                                                                                                                  \
                                                                                                                                                      
  palette->SetTitleSize(0.04);
  palette->SetFillColor(100);
  palette->SetFillStyle(1001);

  palette->SetX1NDC(0.9);
  palette->SetX2NDC(0.95);
  palette->SetY1NDC(0.2);
  palette->SetY2NDC(0.8);
  hPar1Map->GetListOfFunctions()->Add(palette,"br");
  hPar1Map->Draw("gcolz");
  hPar1Map->Write();
  cP1Map->Modified();
  cP1Map->Update();
  TLatex * tPrel4 = new TLatex();
  tPrel4->SetNDC();
  tPrel4->SetTextColor(kBlack);
  tPrel4->SetTextSize(0.03027386);
  tPrel4->SetTextFont(42);
  tPrel4->DrawLatex(0.09821429,0.9194523,"Test Beam FNAL, May 2016, Dut2=RSPA");

  cP1Map->Print("../../../../Tesi/results_images/May2016/Par1Map.eps");


  fileOutID->Close();
}


void ConvertDTB2Captan (string fileIn, string fileOut)
{
  ifstream fileInID;
  ofstream fileOutID;

  std::vector<int> VCalLoRange;
  std::vector<int> VCalHiRange;

  stringstream myString;
  string line;
  string tmp;

  unsigned int nVCalSteps;


  // ###################
  // # Open input file #
  // ###################
  fileInID.open(fileIn.c_str(),ifstream::in);
  if (fileInID.good() == false)
    {
      cout << "[CaptanPlots::ConvertDTB2Captan]\tError opening file: " << fileIn.c_str() << endl;
      exit (EXIT_FAILURE);
    }


  // ####################
  // # Open output file #
  // ####################
  fileOutID.open(fileOut.c_str(),ifstream::out);
  if (fileOutID.good() == false)
    {
      cout << "[CaptanPlots::ConvertDTB2Captan]\tError opening file: " << fileOut.c_str() << endl;
      exit (EXIT_FAILURE);
    }


  // ######################################
  // # Reading parameters from input file #
  // ######################################
  bool VCalLoDone = false;
  bool VCalHiDone = false;
  while (getline(fileInID,line))
    {
      myString.clear(); myString.str("");
      myString << line;

      if (line.find("Low range") != string::npos)
	{
	  // ############################
	  // # Read VCal bins low range #
	  // ############################
	  while (true)
	    {
	      myString >> tmp;

	      // #############################
	      // # Check if it is an integer #
	      // #############################
	      char* p;
	      strtol(tmp.c_str(), &p, 10);
	      if (*p == 0) break;
	    }
	  while (myString)
	    {
	      VCalLoRange.push_back(atoi(tmp.c_str()));
	      myString >> tmp;
	    }

	  VCalLoDone = true;
	}
      else if (line.find("High range") != string::npos)
	{
	  // #############################
	  // # Read VCal bins high range #
	  // #############################
	  while (true)
	    {
	      myString >> tmp;

	      // #############################
	      // # Check if it is an integer #
	      // #############################
	      char* p;
	      strtol(tmp.c_str(), &p, 10);
	      if (*p == 0) break;
	    }
	  while (myString)
	    {
	      VCalHiRange.push_back(atoi(tmp.c_str()));
	      myString >> tmp;
	    }

 	  VCalHiDone = true;
	}

      if ((VCalLoDone == true) && (VCalHiDone == true)) break;
    }


  // #####################################
  // # Writing parameters to output file #
  // #####################################
  nVCalSteps = VCalLoRange.size() + VCalHiRange.size();

  myString.clear(); myString.str("");
  myString << "ITERATIONS: " << nVCalSteps << "\n";
  fileOutID << myString.str();

  myString.clear(); myString.str("");
  myString << "INJECT_NUM: 1\n";
  fileOutID << myString.str();


  // ########################
  // # Reading&Writing data #
  // ########################
  unsigned int it = 0;
  int pH;
  int row;
  int col;
  while (it < nVCalSteps)
    {
      fileInID.clear();
      fileInID.seekg(0,ios::beg);

      getline(fileInID,line);
      getline(fileInID,line);
      getline(fileInID,line);
      getline(fileInID,line);


      myString.clear(); myString.str("");
      if (it < VCalLoRange.size())
	{
	  myString << "Iteration " << it+1 << " --- reg = " << VCalLoRange[it] * ELEVCALLo / ELEVCALHi << "\n";
	}
      else
	{
	  myString << "Iteration " << it+1 << " --- reg = " << VCalHiRange[it-VCalLoRange.size()] << "\n";
	}
      fileOutID << myString.str();


      for (int i = 0; i < NROW; i++)
	{
	  for (int j = 0; j < NCOL; j++)
	    {
	      getline(fileInID,line);

	      myString.clear(); myString.str("");
	      myString << line;


	      // ########################
	      // # Reading pulse height #
	      // ########################
	      unsigned int n = 0;
	      while (n <= it)
		{
		  myString >> tmp;
		  n++;
		}
	      pH = atoi(tmp.c_str());


	      // ##########################
	      // # Reading row and column #
	      // ##########################
	      while (n <= nVCalSteps+1)
		{
		  myString >> tmp;
		  n++;
		}
 	      col = atoi(tmp.c_str());
	      myString >> tmp;
 	      row = atoi(tmp.c_str());
	      
	      myString.clear(); myString.str("");
	      myString << "r " << row << " c " << col << " h " << 1 << " a " << pH << "\n";
	      fileOutID << myString.str();
	    }
	}

      it++;
    }


  fileOutID.close();
  fileInID.close();
}
