#!/bin/sh
#december 2015 100x150
root -l <<EOF
.L makeSP_plot.C
main("../RSPA_dec/100x150/Chewie_Runs581_592_rangeRes2_credoOK.root","100","December2015","eff+charge")
main("../RSPA_dec/100x150/Chewie_Runs581_592_rangeRes2_credoOK.root","100","December2015","landaufitted")
main("../RSPA_dec/100x150/Chewie_Runs581_592_rangeRes2_credoOK.root","100","December2015","residualsfitted")
main("../RSPA_dec/100x150/Chewie_Runs581_592_nofit.root","100","December2015","landau")
main("../RSPA_dec/100x150/Chewie_Runs581_592_nofit.root","100","December2015","residuals")
.q
 EOF
EOF

#december 2015 50x300
root -l <<EOF
.L makeSP_plot.C
main("../RSPA_dec/50x300/Chewie_Runs581_592_newPlots.root","50","December2015","new")
main("../RSPA_dec/50x300/Chewie_Runs581_592_fits.root","50","December2015","eff+charge")
main("../RSPA_dec/50x300/Chewie_Runs581_592_fits.root","50","December2015","landaufitted")
main("../RSPA_dec/50x300/Chewie_Runs581_592_fits.root","50","December2015","residualsfitted")
main("../RSPA_dec/50x300/Chewie_Runs581_592_nofit.root","50","December2015","landau")
main("../RSPA_dec/50x300/Chewie_Runs581_592_nofit.root","50","December2015","residuals")
.q
 EOF
EOF

#december 2015 25x600
root -l <<EOF                                                                                                                                      
.L makeSP_plot.C                                                                                                                                    
main("../RSPA_dec/25x600/Chewie_Runs581_592_smallerWindows.root","25","December2015","eff+charge")
main("../RSPA_dec/25x600/Chewie_Runs581_592_newPlots.root","25","December2015","landau")                                                          
main("../RSPA_dec/25x600/Chewie_Runs581_592_newPlots.root","25","December2015","new")                                                           
main("../RSPA_dec/25x600/Chewie_Runs581_592_residuals.root","25","December2015","landaufitted")
main("../RSPA_dec/25x600/Chewie_Runs581_592_residuals.root","25","December2015","residualsfitted")
main("../RSPA_dec/25x600/Chewie_Runs581_592_smallerWindows.root","25","December2015","residuals")
.q
 EOF
EOF

#may 2016 50x300
root -l <<EOF                                                                                                                                        
.L makeSP_plot.C                                                                                                                                    
main("../IrradiatedSensors/May2016TestBeam/50x300/Chewie_Runs314_325newPlots.root","50","May2016","eff+charge")
main("../IrradiatedSensors/May2016TestBeam/50x300/Chewie_Runs314_325_residuals.root","50","May2016","landaufitted")
main("../IrradiatedSensors/May2016TestBeam/50x300/Chewie_Runs314_325_residuals.root","50","May2016","residualsfitted")
main("../IrradiatedSensors/May2016TestBeam/50x300/Chewie_Runs314_325_noFit.root","50","May2016","residuals")
main("../IrradiatedSensors/May2016TestBeam/50x300/Chewie_Runs314_325_noFit.root","50","May2016","landau")   
main("../IrradiatedSensors/May2016TestBeam/50x300/Chewie_Runs314_325_errors.root","50","May2016","err")
.X eff_thr.C
.q
 EOF
EOF

#june 2016 50x300
root -l <<EOF
.L makeSP_plot.C
main("../B2A/Chewie_Runs144_147.root","50","June2016","landaufitted")
.q
 EOF
EOF


#may 2016 25x600
root -l <<EOF
.L makeSP_plot.C                                                                                                                                     
main("../IrradiatedSensors/May2016TestBeam/25x600/Chewie_Runs315_318_newPlots.root","25","May2016","eff+charge")
main("../IrradiatedSensors/May2016TestBeam/25x600/Chewie_Runs315_318_newPlots.root","25","May2016","new")
main("../IrradiatedSensors/May2016TestBeam/25x600/Chewie_Runs315_318_newPlots.root","25","May2016","landau")
main("../IrradiatedSensors/May2016TestBeam/25x600/Chewie_Runs315_318_residuals.root","25","May2016","landaufitted")
main("../IrradiatedSensors/May2016TestBeam/25x600/Chewie_Runs315_318_residuals.root","25","May2016","residualsfitted")
main("../IrradiatedSensors/May2016TestBeam/25x600/Chewie_Runs315_318_noFit.root","25","May2016","residuals")
main("../IrradiatedSensors/May2016TestBeam/25x600/Chewie_Runs315_318_errors.root","25","May2016","err") 
.q 
EOF
EOF


#errors
root -l <<EOF
.L sigmaTel.C
main("../IrradiatedSensors/May2016TestBeam/50x300/Chewie_Runs314_325_errors.root","May2016","50")
main("../IrradiatedSensors/May2016TestBeam/25x600/Chewie_Runs315_318_errors.root","May2016","25")
main("../RSPA_dec/25x600/Chewie_Runs581_592_smallerWindows.root","December2015","25") 
main("../RSPA_dec/100x150/Chewie_Runs581_592_nofit.root","December2015","100")
main("../RSPA_dec/50x300/Chewie_Runs581_592_nofit.root","December2015","50") 
EOF
EOF
