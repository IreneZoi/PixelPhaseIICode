//root -l
//.L makeSP_plot.C
//main("Chewie_Runs1359_1369_25x600.root","25600","Runs1359to1361")
#include <stdio.h>
#include "TCanvas.h"
void DrawTH2(TString zone,TH2F* h2, TString name, double range=-1., TString device, TString testbeam);
void DrawTH1(TString zone,TH1F* h1, TString name, TString device, int entries, TString testbeam);

int main(TString inputfile, TString zone, TString device, TString testbeam, TString option){ //zone: 100#mumx150#mum


    gROOT->SetStyle("Plain");
    gStyle->SetPadGridX(0);
    gStyle->SetPadGridY(0);
    gStyle->SetPalette(1);
    gStyle->SetOptStat(0);


    //    std::cout<<" WARNING "<< "make sure directory plots"+zone+runs<<"  exixsts"<<std::endl;

    TFile * f = new TFile(inputfile);//"pxar_calibration.root"); //FMM12
    f->cd();

    if(device == "RSPA")
      {
	if(option== "pretest")
	  {
	    TDirectoryFile*d0 =(TDirectoryFile*)f->Get("Pretest");                                                                                 
	    DrawTH2(zone,(TH2F*)d0->Get("pretestVthrCompCalDel_c11_r70_C0_V0"), "VthrCompCaldel_c11_r70", -1, device, testbeam);                     
	    DrawTH2(zone,(TH2F*)d0->Get("pretestVthrCompCalDel_c11_r20_C0_V0"), "VthrCompCaldel_c11_r20", -1, device, testbeam);                     
	  }
	if(option== "pixelalive")
	  {
	    TDirectoryFile*d =(TDirectoryFile*)f->Get("PixelAlive");
	    DrawTH2(zone,(TH2F*)d->Get("PixelAlive_C0_V0"), "PixelAlive_all", -1, device, testbeam);
	  }
	if(option == "trim")
	  {
	    DrawTH2(zone,(TH2F*)f->Get("TrimMap"),"TrimMap", -1, device, testbeam);
	//	    DrawTH1(zone,(TH1D*)f->Get("TrimDistribution"),"TrimBits");
	  }


      }

    if(device == "SPb2a")
      {
    TDirectoryFile*d =(TDirectoryFile*)f->Get("PixelAlive");

    DrawTH2(zone,(TH2F*)d->Get("PixelAlive_C0_V1"), "PixelAlive_wbc1", -1, device, testbeam);
    DrawTH2(zone,(TH2F*)d->Get("PixelAlive_C0_V3"), "PixelAlive_wbc3", -1, device, testbeam);
      }





    if(device=="FMM12")
      {
    //Pretest
    
    TDirectoryFile*d0 =(TDirectoryFile*)f->Get("Pretest");
    //    double range1 =0.95;
    DrawTH2(zone,(TH2F*)d0->Get("pretestVthrCompCalDel_c12_r22_C0_V1"), "pretestVthrCompCalDel_c12_r22", -1, device, testbeam);
    //h0->GetZaxis()->SetRangeUser(0.95,1);
    
    //PixelALive

    TDirectoryFile*d =(TDirectoryFile*)f->Get("PixelAlive");

    DrawTH2(zone,(TH2F*)d->Get("PixelAlive_C0_V0"), "PixelAlive", -1, device, testbeam);
    //    DrawTH1(zone,(TH1F*)d->Get("h1DCellCharge4RowsClusterSizeYProfile_Dut0"),"4RowsClusterSizeYProfile", device, testbeam);

    //Trimming
    TDirectoryFile*d1 =(TDirectoryFile*)f->Get("Trim");
    DrawTH2(zone,(TH2F*)d1->Get("TrimMap_C0_V0"),"TrimMap",15, device, testbeam);
    DrawTH1(zone,(TH1F*)d1->Get("dist_TrimMap_C0_V0"),"dist_TrimMap",  device, 1, testbeam);
    DrawTH2(zone,(TH2F*)d1->Get("thr_TrimThrFinal_vcal_C0_V0"),"thr_TrimThrFinal_vcal",50, device, testbeam);
    DrawTH1(zone,(TH1F*)d1->Get("dist_thr_TrimThrFinal_vcal_C0_V0"),"dist_thr_TrimThrFinal_vcal",  device, 1, testbeam);

    //PhOptimization
    TDirectoryFile*d2 =(TDirectoryFile*)f->Get("PhOptimization");
    DrawTH2(zone,(TH2F*)d2->Get("maxphmap_C0_V0"),"maxphmap", 250, device, testbeam);
    DrawTH1(zone,(TH1F*)d2->Get("dist_maxphmap_C0_V0"),"dist_maxphmap",  device, 1, testbeam);
    DrawTH2(zone,(TH2F*)d2->Get("minphmap_C0_V0"),"minphmap", 250, device, testbeam);
    DrawTH1(zone,(TH1F*)d2->Get("dist_minphmap_C0_V0"),"dist_minphmap", device, 1, testbeam);
    DrawTH2(zone,(TH2F*)d2->Get("maxphvsdacdac_th2_C0"),"maxphvsdacdac_th2_C0",-1,  device, testbeam);
    DrawTH2(zone,(TH2F*)d2->Get("minphvsdacdac_th2_C0"),"minphvsdacdac_th2_C0",-1,  device, testbeam);
    DrawTH1(zone,(TH1F*)d2->Get("PH_c3_r7_C0_V0"),"PH_c3_r7_C0_V0",  device, 0, testbeam);
    DrawTH1(zone,(TH1F*)d2->Get("PH_c7_r43_C0_V0"),"PH_c7_r43_C0_V0",  device, 0, testbeam);

    //GainPedestal
    TDirectoryFile*d3 =(TDirectoryFile*)f->Get("GainPedestal");
    DrawTH1(zone,(TH1F*)d3->Get("gainPedestalP0_C0_V1"),"gainPedestalP0",  device, 1, testbeam);
    DrawTH1(zone,(TH1F*)d3->Get("gainPedestalP1_C0_V1"),"gainPedestalP1",  device, 1, testbeam);
    DrawTH1(zone,(TH1F*)d3->Get("gainPedestalP2_C0_V1"),"gainPedestalP2",  device, 1, testbeam);
    DrawTH1(zone,(TH1F*)d3->Get("gainPedestalP3_C0_V1"),"gainPedestalP3",  device, 1, testbeam);
    DrawTH1(zone,(TH1F*)d3->Get("gainPedestal_c0_r7_C0"),"gainPedestal_c0_r7",  device, 0, testbeam);
    DrawTH1(zone,(TH1F*)d3->Get("gainPedestalNonLinearity_C0_V1"),"gainPedestalNonLinearity",  device, 1, testbeam);

    //SCurves
    TDirectoryFile*d4 =(TDirectoryFile*)f->Get("Scurves");
    DrawTH1(zone,(TH1F*)d4->Get("dist_thr_scurveVcal_Vcal_C0_V0"),"dist_thr_scurveVcal_Vcal",  device, 1, testbeam);
    DrawTH1(zone,(TH1F*)d4->Get("dist_sig_scurveVcal_Vcal_C0_V0"),"dist_sig_scurveVcal_Vcal",  device, 1, testbeam);
    DrawTH1(zone,(TH1F*)d4->Get("scurve_Vcal_c50_r57_C0"),"scurve_Vcal_c50_r57",  device, 0, testbeam);
      }
}


void DrawTH2(TString zone, TH2F* h2, TString name, double range, TString device, TString testbeam){

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

    if(device=="RSPA" && name == "TrimMap")
      TPaletteAxis *palette = new TPaletteAxis(h2->GetXaxis()->GetXmax()+0.01,h2->GetYaxis()->GetXmin(),h2->GetXaxis()->GetXmax()+h2->GetXaxis()->GetXmax()*0.048,h2->GetYaxis()->GetXmax(),h2);
    else
      TPaletteAxis *palette = (TPaletteAxis*)h2->GetListOfFunctions()->FindObject("palette");
    //    TPaletteAxis *palette = new TPaletteAxis(h2->GetXaxis()->GetXmax()+0.01,h2->GetYaxis()->GetXmin(),h2->GetXaxis()->GetXmax()+h2->GetXaxis()->GetXmax()*0.048,h2->GetYaxis()->GetXmax(),h2);
    //    TPaletteAxis *palette = new TPaletteAxis(h2->GetXaxis()->GetXmax()+1,h2->GetYaxis()->GetXmin(),h2->GetXaxis()->GetXmax()+2,h2->GetYaxis()->GetXmax(),h2);
    palette->SetLabelColor(1);
    palette->SetLabelFont(42);
    palette->SetLabelOffset(0.005);
    palette->SetLabelSize(0.02);
//    palette->SetTitleOffset(1);
    palette->SetTitleSize(0.04);
    palette->SetFillColor(100);
    palette->SetFillStyle(1001);

    //    palette->SetX1NDC(0.9);
    //    palette->SetX2NDC(0.95);
    //    palette->SetY1NDC(0.2);
    //    palette->SetY2NDC(0.8);
    //    h2->GetListOfFunctions()->Add(palette,"br");
    if(range>0) {std::cout<<range<<std::endl; h2->GetZaxis()->SetRangeUser(0.,range);}

    //    h2->Draw("COLZ");

    if(name == "TrimMap" || name == "thr_TrimThrFinal_vcal" )
      {
	h2->GetXaxis()->SetTitle("col");
        h2->GetYaxis()->SetTitle("row");

      }
    if(name == "TrimMap")
      {
	h2->GetZaxis()->SetRangeUser(0.,15.);
      }

    h2->Draw("COLZ");
    TLatex * tPrel = new TLatex();

    tPrel->SetNDC();
    tPrel->SetTextColor(kBlack);
    tPrel->SetTextSize(0.025);
    tPrel->SetTextFont(42);
    tPrel->DrawLatex(0.09821429,0.9194523,"Calibration, "+testbeam+", "+device+", pitch:  "+zone+"#mum");



    if(zone=="50,25" )
      c->SaveAs("../Tesi/calibrations_images/"+device+"/"+name+".eps");
    if(zone=="100,50,25" )
      c->SaveAs("../Tesi/calibrations_images/"+device+"/"+device+"_3zones_"+name+".eps");
     if(zone=="100" )
      c->SaveAs("../Tesi/calibrations_images/"+device+"/"+device+"_100x150_"+name+".eps");
    if(zone=="50" )
      c->SaveAs("../Tesi/calibrations_images/"+device+"/"+device+"_50x300_"+name+".eps");
    if(zone=="25" )
      c->SaveAs("../Tesi/calibrations_images/"+device+"/"device+"_25x600_"+name+".eps");
    delete c;
    delete palette;

}


void DrawTH1(TString zone,TH1F* h1, TString name, TString device, int entries, TString testbeam){

    TCanvas * c = new TCanvas("c","c",700,700);
    c->cd();
    c->SetFrameFillStyle(1000);
    c->SetFrameFillColor(0);

    //        gStyle->SetOptStat(0);
        gStyle->SetStatY(0.1);
        gStyle->SetStatX(0.1);
        gStyle->SetStatW(0.15);
        gStyle->SetStatH(0.2);


    h1->SetTitle("");
    if(entries ==1)
      h1->GetYaxis()->SetTitle("Entries");
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

    if( name =="dist_maxphmap" || name == "dist_minphmap")
      {
	gStyle->SetOptStat(0);
      h1->GetXaxis()->SetTitle("PulseHeight (ADC)");
      }
    if(name =="PH_c3_r7_C0_V0" ||  name == "PH_c7_r43_C0_V0")
      {
	h1->GetXaxis()->SetTitle("Vcal");
	gStyle->SetOptStat(0);
      }
    if(name == "dist_sig_scurveVcal_Vcal" || name == "dist_thr_scurveVcal_Vcal" || name == "dist_thr_TrimThrFinal_vcal" || name == "scurve_Vcal_c50_r57")
      h1->GetXaxis()->SetTitle("Vcal");
    if(name == "dist_TrimMap")
      {
	h1->GetXaxis()->SetTitle("TrimBits");
	h1->GetXaxis()->SetRangeUser(0.,16.);
      }
    if(name== "gainPedestal_c0_r7")
      {

        gStyle->SetOptFit(0);

	h1->GetXaxis()->SetTitle("Vcal (DAC)");
	h1->GetYaxis()->SetTitle("Pulse Height");
      }
    if(name == "gainPedestalNonLinearity")
      gStyle->SetOptStat(0);



    TLatex * tPrel = new TLatex();

    tPrel->SetNDC();
    tPrel->SetTextColor(kBlack);
    tPrel->SetTextSize(0.025);
    tPrel->SetTextFont(42);
    tPrel->DrawLatex(0.09821429,0.9194523,"Calibration, "+testbeam+", "+device+", pitch:  "+zone+"#mum");


    if(zone=="50,25" )
      c->SaveAs("../Tesi/calibrations_images/"+device+"/"+name+".eps");
    if(zone=="100,50,25" )
      c->SaveAs("../Tesi/calibrations_images/"+device+"/"+device+"_3zones_"+name+".eps");
    if(zone=="100" )
      c->SaveAs("../Tesi/calibrations_images/"+device+"/"+device+"_100x150_"+name+".eps");
    if(zone=="50" )
      c->SaveAs("../Tesi/calibrations_images/"+device+"/"+device+"_50x300_"+name+".eps");
    if(zone=="25" )
      c->SaveAs("../Tesi/calibrations_images/"+device+"/"+device+"_25x600_"+name+".eps");

    delete c;

}
