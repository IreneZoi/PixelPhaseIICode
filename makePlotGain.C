//go in the right directory and choose the correct file
//root -l
//.L makePlotGain.C++
//main("Gain_RSPA_linear.root",1,"900")
//main("Gain_RSPA_tanh.root",0,"900")





#include <stdio.h>
#include "TCanvas.h"
#include "TH2.h"
#include "TH1.h"


void DrawTH2(TString vbias, TH2D* h2, TString name, double range=-1., TString testbeam);
void DrawTH1(TString vbias, TH1D* h1, TString name, TString testbeam);


int main(TString inputfile, unsigned int fitType, TString vbias, TString testbeam){


    gROOT->SetStyle("Plain");
    gStyle->SetPadGridX(0);
    gStyle->SetPadGridY(0);
    gStyle->SetPalette(1);
    gStyle->SetOptStat(0);
//    gStyle->SetOptFit(1111);



    TFile * f = new TFile(inputfile);//"pxar.root");
    f->cd();

/*    //Pretest
   
    TDirectoryFile*d0 =(TDirectoryFile*)f->Get("Pretest");
//    double range1 =0.95;

    DrawTH2(zone,(TH2D*)d0->Get("pretestVthrCompCalDel_c11_r70_C0_V0"), "VthrCompCaldel_c11_r70");
    DrawTH2(zone,(TH2D*)d0->Get("pretestVthrCompCalDel_c11_r20_C0_V0"), "VthrCompCaldel_c11_r20");
    //h0->GetZaxis()->SetRangeUser(0.95,1);
   
    //PixelAlive

    TDirectoryFile*d =(TDirectoryFile*)f->Get("PixelAlive");

    DrawTH2(zone,(TH2D*)d->Get("PixelAlive_C0_V0"), "PixelAlive");


    //PhOptimization

    TDirectoryFile*d1 =(TDirectoryFile*)f->Get("PhOptimization");

    DrawTH1(zone,(TH1D*)d1->Get("PH_c3_r26_C0_V0"),"PulseHeight_c3_r26");
    DrawTH1(zone,(TH1D*)d1->Get("PH_c49_r75_C0_V0"),"PulseHeight_c49_r75");

    //Scurves

    TDirectoryFile*d2 =(TDirectoryFile*)f->Get("Scurves");

    DrawTH1vcal(zone,(TH1D*)d2->Get("dist_sig_scurvevcal_vcal_C0_V0"),"vcal_noise");
    */

    //**********Calibrations************


    if(testbeam == "December2015")
      {

	TDirectoryFile*d3 =(TDirectoryFile*)f->Get("GainCurves");
	if(fitType == 1)
	  {
	    DrawTH1(vbias,(TH1D*)d3->Get("Station:4 - Plaq: 0 - ROC:0 - r:44 - c:25"),"Gain_44_25_linear", testbeam);
	    DrawTH1(vbias,(TH1D*)d3->Get("Station:4 - Plaq: 0 - ROC:0 - r:56 - c:9"),"Gain_56_9_linear", testbeam);
	    DrawTH2(vbias,(TH2D*)f->Get("hGoodFitMap"),"FitMap_linear",-1, testbeam);
	    DrawTH2(vbias,(TH2D*)f->Get("hPar0Map"),"slope_linear", -1,testbeam);
	    DrawTH2(vbias,(TH2D*)f->Get("hPar1Map"),"intercept_linear",-1, testbeam);
	  }
	else if
	  {
	    DrawTH1(vbias,(TH1D*)d3->Get("Station:4 - Plaq: 2 - ROC:0 - r:21 - c:42"),"Gain_21_42_tanh", testbeam);
	    DrawTH1(vbias,(TH1D*)d3->Get("Station:4 - Plaq: 2 - ROC:0 - r:56 - c:9"),"Gain_56_9_tanh", testbeam);
	    DrawTH2(vbias,(TH2D*)f->Get("hGoodFitMap"),"FitMap_tanh", testbeam);
	    DrawTH2(vbias,(TH2D*)f->Get("hPar1Map"),"mean_tanh", testbeam);
	  }
      }

    if(testbeam=="May2016")
      {
	TDirectoryFile*d3 =(TDirectoryFile*)f->Get("GainCurves");
	if(fitType == 1)
	  {
	    DrawTH1(vbias,(TH1D*)d3->Get("Station:4 - Plaq: 2 - ROC:0 - r:44 - c:25"),"Gain_44_25_linear", testbeam);
	    DrawTH1(vbias,(TH1D*)d3->Get("Station:4 - Plaq: 2 - ROC:0 - r:56 - c:9"),"Gain_56_9_linear", testbeam);
	    DrawTH2(vbias,(TH2D*)f->Get("hGoodFitMap"),"FitMap_linear",-1, testbeam);
	    DrawTH2(vbias,(TH2D*)f->Get("hPar0Map"),"slope_linear", -1,testbeam);
	    DrawTH2(vbias,(TH2D*)f->Get("hPar1Map"),"intercept_linear",-1, testbeam);

	  }
	else if
	  {
	    DrawTH1(vbias,(TH1D*)d3->Get("Station:4 - Plaq: 2 - ROC:0 - r:21 - c:42"),"Gain_21_42_tanh", testbeam);
	    DrawTH1(vbias,(TH1D*)d3->Get("Station:4 - Plaq: 2 - ROC:0 - r:56 - c:9"),"Gain_56_9_tanh", testbeam);
	    DrawTH2(vbias,(TH2D*)f->Get("hGoodFitMap"),"FitMap_tanh", testbeam);
	    DrawTH2(vbias,(TH2D*)f->Get("hPar1Map"),"mean_tanh", testbeam);
	  }

      }




}


void DrawTH2(TString vbias, TH2D* h2, TString name, double range, TString testbeam){

    TCanvas * c = new TCanvas("c","c",700,700);
    c->cd();
    c->Range(-1.5,35.125,13.5,183.875);
    c->SetFrameFillStyle(1000);
    c->SetFrameFillColor(0);

    h2->SetTitle("");
    h2->GetXaxis()->SetLabelFont(42);
    h2->GetXaxis()->SetLabelSize(0.025);
    h2->GetXaxis()->SetTitleSize(0.035);
    h2->GetXaxis()->SetTitleOffset(0.8);
    h2->GetXaxis()->SetTitleFont(42);
    h2->GetYaxis()->SetLabelFont(42);
    h2->GetYaxis()->SetLabelSize(0.025);
    h2->GetYaxis()->SetTitleSize(0.035);
    h2->GetYaxis()->SetTitleOffset(1.4);
    h2->GetYaxis()->SetTitleFont(42);


    TPaletteAxis *palette = new TPaletteAxis(h2->GetXaxis()->GetXmax()+0.01,h2->GetYaxis()->GetXmin(),h2->GetXaxis()->GetXmax()+h2->GetXaxis()->GetXmax()*0.04,h2->GetYaxis()->GetXmax(),h2);
    palette->SetLabelColor(1);
    palette->SetLabelFont(42);
    palette->SetLabelOffset(0.01);
    palette->SetLabelSize(0.02);
//    palette->SetTitleOffset(1);
    palette->SetTitleSize(0.04);
    palette->SetFillColor(100);
    palette->SetFillStyle(1001);

    palette->SetX1NDC(0.9);
    palette->SetX2NDC(0.93);
    palette->SetY1NDC(0.2);
    palette->SetY2NDC(0.8);
    h2->GetListOfFunctions()->Add(palette,"br");
    if(range>0) {std::cout<<range<<std::endl; h2->GetZaxis()->SetRangeUser(range,1.);}
    if(name== "mean_tanh")
      {
	h2->SetMaximum(30000);
        h2->SetMinimum(21000);
      }
    if(name == "slope_linear")
      {
      h2->SetMaximum(0.0165);
      h2->SetMinimum(0);
      }

    if(name== "intercept_linear")
      {
	h2->SetMaximum(300);
        h2->SetMinimum(110);
      }




    h2->Draw("colz");

    TLatex * tPrel = new TLatex();

    tPrel->SetNDC();
    tPrel->SetTextColor(kBlack);
    tPrel->SetTextSize(0.02027386);
    tPrel->SetTextFont(42);
    tPrel->DrawLatex(0.09821429,0.9194523,"Test Beam FNAL, "+testbeam+", RSPA, Vbias=-"+ vbias+"V");




    c->SaveAs("../Tesi/calibrations_images/RSPA/"+testbeam+name+".eps");
    delete c;
    delete palette;

}


void DrawTH1(TString vbias,TH1D* h1, TString name, TString testbeam){

    TCanvas * c = new TCanvas("c","c",700,700);
    c->cd();
    c->SetFrameFillStyle(1000);
    c->SetFrameFillColor(0);

    h1->SetTitle("");
    h1->GetXaxis()->SetLabelFont(42);
    h1->GetXaxis()->SetLabelSize(0.025);
    h1->GetXaxis()->SetTitleSize(0.035);
    h1->GetXaxis()->SetTitleOffset(0.8);
    h1->GetXaxis()->SetTitleFont(42);
    h1->GetYaxis()->SetLabelFont(42);
    h1->GetYaxis()->SetLabelSize(0.025);
    h1->GetYaxis()->SetTitleSize(0.035);
    h1->GetYaxis()->SetTitleOffset(1.4);
    h1->GetYaxis()->SetTitleFont(42);



    h1->Draw("");

    TLatex * tPrel = new TLatex();

    tPrel->SetNDC();
    tPrel->SetTextColor(kBlack);
    tPrel->SetTextSize(0.02027386);
    tPrel->SetTextFont(42);
    tPrel->DrawLatex(0.09821429,0.9194523,"Test Beam FNAL, "+testbeam+", RSPA, Vbias=-"+ vbias+"V");




    c->SaveAs("../Tesi/calibrations_images/RSPA/"+testbeam+name+".eps");
    delete c;

}

