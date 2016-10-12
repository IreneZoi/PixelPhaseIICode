{
  //50
TFile * f1 = new TFile("../IrradiatedSensors/May2016TestBeam/50x300/Chewie_Runs314_325_residuals.root");//50 after                                   
TFile * f2 = new TFile("../RSPA_dec/50x300/Chewie_Runs581_592_fits.root");//50 pre                                           
//TFile * f3 = new TFile("../RSPA_dec/50x300/Chewie_Runs581_592_2500.root");//50 pre high thr
gROOT->SetStyle("Plain");
gStyle->SetPadGridX(0);
gStyle->SetPadGridY(0);
gStyle->SetPadGridX(0);
gStyle->SetPadGridY(0);
gStyle->SetOptStat(0);


TCanvas * c = new TCanvas("c","c",700,700);
c->cd();
c->SetFrameFillStyle(1000);
c->SetFrameFillColor(0);
 
 TDirectoryFile * d1  = f1->Get("Charge/Dut2/ClusterSize");
 d1->cd();
 TH1F * h22 = d1->Get("hClusterSize_Dut2");
 h22->SetTitle("");
 h22->SetLineColor(kRed);
 h22->DrawNormalized("same");
 std::cout<<h22->GetMean()<<std::endl;


 TDirectoryFile * d2  = f2->Get("Charge/Dut0/ClusterSize");
 d2->cd();
 TH1F * h11 = d2->Get("hClusterSize_Dut0");
 //h11->SetTitle("")
 h11->SetLineStyle(2); 
 h11->SetLineColor(kRed);
 h11->DrawNormalized("same");
 std::cout<<h11->GetMean()<<std::endl;
 /* TDirectoryFile * d3  = f3->Get("Charge/Dut0/ClusterSize");
 d2->cd();
 TH1F * h33 = d3->Get("hClusterSizeStandardCutsThreshold_Dut0");                                                                                    
 h33->SetLineStyle(3);
 h33->SetLineColor(kRed);
 h33->DrawNormalized("same");
 std::cout<<h11->GetMean()<<std::endl;
 */
 TLatex * tPrel = new TLatex();

 tPrel->SetNDC();
 tPrel->SetTextColor(kBlack);
 tPrel->SetTextSize(0.03027386);
 tPrel->SetTextFont(42);
 tPrel->DrawLatex(0.09821429,0.9194523,"Test Beam FNAL, December2015 vs May 2016, Dut=RSPA");
 TLegend *leg = new TLegend(0.4,0.671131,0.75,0.8809524);//,NULL,"brNDC");                                                                      
 leg->SetBorderSize(1);
 leg->SetLineColor(0);
 leg->SetLineStyle(1);
 leg->SetLineWidth(2);
 leg->SetFillColor(0);
 leg->SetFillStyle(0);
 leg->SetTextSize(0.02);
 leg->AddEntry(h22,"50x300 after irradiation","lpf");
 leg->AddEntry(h11,"50x300 before irradiation","lpf");
 // leg->AddEntry(h33,"50x300 before irradiation, threshold 2500e-","lpf");
 leg->Draw();
 c->SaveAs("../Tesi/results_images/clustersize50.eps");

 //25
 TFile * f4 = new TFile("../IrradiatedSensors/May2016TestBeam/25x600/Chewie_Runs315_318_noFit.root");//25 after                                   
 TFile * f5 = new TFile("../RSPA_dec/25x600/Chewie_Runs581_592_smallerWindows.root");//25 pre                                           
 // TFile * f6 = new TFile("../RSPA_dec/25x600/Chewie_Runs581_592_2500.root");//25 pre high thr
gROOT->SetStyle("Plain");
gStyle->SetPadGridX(0);
gStyle->SetPadGridY(0);
gStyle->SetPadGridX(0);
gStyle->SetPadGridY(0);
gStyle->SetOptStat(0);


TCanvas * c1 = new TCanvas("c","c",700,700);
c1->cd();
c1->SetFrameFillStyle(1000);
c1->SetFrameFillColor(0);
 
 TDirectoryFile * d4  = f4->Get("Charge/Dut2/ClusterSize");
 d4->cd();
 TH1F * h4 = d4->Get("hClusterSize_Dut2");
 h4->SetTitle("");
 h4->SetLineColor(kAzure);
 h4->DrawNormalized("same");
 std::cout<<h22->GetMean()<<std::endl;


 TDirectoryFile * d5  = f5->Get("Charge/Dut0/ClusterSize");
 d5->cd();
 TH1F * h5 = d5->Get("hClusterSize_Dut0");
 //h11->SetTitle("")
 h5->SetLineStyle(2); 
 h5->SetLineColor(kAzure);
 h5->DrawNormalized("same");
 // std::cout<<h11->GetMean()<<std::endl;
 // TDirectoryFile * d6  = f6->Get("Charge/Dut0/ClusterSize");
 //d5->cd();
 //TH1F * h6 = d6->Get("hClusterSizeStandardCutsThreshold_Dut0");
 //h11->SetTitle("")                                                                                                                                  
 // h6->SetLineStyle(3);
 // h6->SetLineColor(kAzure);
 // h6->DrawNormalized("same");
 // std::cout<<h11->GetMean()<<std::endl;

 TLatex * tPrel1 = new TLatex();

 tPrel1->SetNDC();
 tPrel1->SetTextColor(kBlack);
 tPrel1->SetTextSize(0.03);
 tPrel1->SetTextFont(42);
 tPrel1->DrawLatex(0.09821429,0.9194523,"Test Beam FNAL, December2015 vs May 2016, Dut=RSPA");
 TLegend *leg1 = new TLegend(0.45,0.671131,0.8,0.8809524);//,NULL,"brNDC");                                                                     
 leg1->SetTextSize(0.02);
 leg1->SetBorderSize(1);
 leg1->SetLineColor(0);
 leg1->SetLineStyle(1);
 leg1->SetLineWidth(2);
 leg1->SetFillColor(0);
 leg1->SetFillStyle(0);
 leg1->AddEntry(h4,"25x600 after irradiation","lpf");
 leg1->AddEntry(h5,"25x600 before irradiation","lpf");
 //leg1->AddEntry(h6,"25x600 before irradiation, threshold 2500e-","lpf");
 leg1->Draw();
 c1->SaveAs("../Tesi/results_images/clustersize25.eps");



}
