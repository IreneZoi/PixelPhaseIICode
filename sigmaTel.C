#include <stdio.h>
#include "TCanvas.h"
#include "TF1.h"
#include <TTreeFormula.h>
#include <TMath.h>
#include <Rtypes.h>
#include <TROOT.h>
#include <TVectorT.h>
#include <TMap.h>
#include <TAxis.h>

double error(TH1F* h1);



int main(TString inputfile, TString testbeam, TString option){

  TFile * f = new TFile(inputfile);//"Chewie_Runs1359_1369_25x600.root");                                                                           
  f->cd();
  TString dut;
  double sigmad[4],sigmam[4];

  if(testbeam == "December2015") 
    {
      dut="Dut0";
      if(option=="100")
	{
	  sigmam[0]= 23.8 ;
          sigmam[1]= 24.8 ;
          sigmam[2]=  7.6;
          sigmam[3]=  7.7 ;
	}
      if(option=="50")
	{
          sigmam[0]= 11.8 ;
          sigmam[1]= 12.5 ;
          sigmam[2]= 7.8 ;
          sigmam[3]= 7.8 ;
	}
      if(option=="25")
	{
          sigmam[0]= 9.6 ;
          sigmam[1]= 0 ;
          sigmam[2]= 8.4 ;
          sigmam[3]= 7.2 ;
	}
    }


  if(testbeam == "May2016")
    {
      dut="Dut2";
      if(option=="50")
	{
	  sigmam[0]= 15.3 ;
	  sigmam[1]= 19.6 ;
	  sigmam[2]=  8.5 ;
	  sigmam[3]=  8.1 ;
	}
      if(option=="25")
	{
	  
	  sigmam[0]= 10.1 ;
	  sigmam[1]=  0   ;
	  sigmam[2]=  9.7 ;
	  sigmam[3]=  7.7 ;
	}

      
    }

 if(testbeam == "June2016") dut="Dut1";

  double error;



  TDirectoryFile*d8 =(TDirectoryFile*)f->Get("Resolution/"+dut+"/Errors");
  error=error(  (TH1F*)d8->Get("hPredictedYErrors_"+dut));
  std::cout<< " error " << error << std::endl; 

  for(int j=0;j<4;j++)
    {
      if(sigmam[j] ==0)
        sigmad[j]=0;
      else
	sigmad[j]=TMath::Sqrt( sigmam[j]*sigmam[j]-error*error);
    }

  std::cout << "$" << setprecision(3) << sigmad[0] << "$ & $" 
	           << setprecision(3) << sigmad[1] << "$ & $"
	           << setprecision(2) << sigmad[2] << "$ & $" 
	           << setprecision(2)<< sigmad[3] << "$ \ \ " << std::endl;


}



double error(TH1F* h1){

  int n;
  n=h1->GetEntries();
  float bin;
  float sigma;
  Int_t ncells = h1->GetSize();
  //  std::cout<< h1->GetSize()<<std::endl;
  float sum=0;

  for(int i=0; i<(ncells-2); i++)
    {
      //      std::cout<< h1->GetBinCenter(i) <<std::endl;
      sum+= h1->GetBinCenter(i)*h1->GetBinCenter(i)*h1->GetBinContent(i);

    }

  sigma = TMath::Sqrt((double)sum/n);
  //  std::cout<<n<<std::endl;
  //  std::cout<<sigma<<std::endl;

    return sigma;


}
