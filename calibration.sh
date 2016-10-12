#!/bin/sh                                                                                                                                                                                                                                                                           
root -l <<EOF                                                                                                                                        
.L makeSPplot_pxarCal.C                                                                                                                              
main("../FMM12/pxar_calibration.root","100","FMM12","June2016","")
main("../B2A/pxar_calibrationPart1.root","100,50,25","SPb2a","June2016","")  
main("../IrradiatedSensors/RSPA/RSPA_afterIrradiation/pxar_20160511_143437.root","50,25","RSPA","May2016","pretest")
main("../IrradiatedSensors/RSPA/RSPA_afterIrradiation/pxar.root","50,25","RSPA","May2016","pixelalive")
main("../IrradiatedSensors/RSPA/Trim/RSPA_mergedTrimMap.root","50,25","RSPA","May2016","trim")
.q
 EOF
EOF
root -l <<EOF
.L makePlotGain.C
main("../RSPA_dec/RSPA_Dec_linear.root",1,"200","December2015")
main("../IrradiatedSensors/May2016TestBeam/RSPA_New_linear.root",1,"400","May2016")
.q 
 EOF
EOF
root -l <<EOF
.L thrIrr.C
SCurve("../IrradiatedSensors/May2016TestBeam/Scurve/newCal/Scurve_RSPA_Rad_SmallPitch_ParametersRun299_344_2_10_0.txt","../IrradiatedSensors/May2016TestBeam/Scurve/newCal/SCurve_RSPA_New_linear.root","")
.q 
 EOF
EOF
root -l <<EOF
.L thrNotIrr.C
SCurve("../B2A/B2A_Scurve_143_2_10_0.txt","../B2A/SCurve_B2A.root","")
.q 
 EOF
EOF
