//Plot Hi and Lo samples on a single stacked plot
//Based off old_compareAll.CRY & plotCRY_standardized.C
//Ben Barton
//bbarton@virginia.edu

/**********************************************************************************************************************************************************************************/
///////////////////////////////////////////////////////////////   Control Params & Livetimes  //////////////////////////////////////////////////////////////////////////////////////

const int NCUTS = 1; //Number of cut variations to plot (not including no cuts as a variation)
const bool INC_TRKQUAL_SFX = false; //Prior to mid-July 2019, dequal leaves were named with suffixes - set to true to read trees prior to this date
const bool INC_CRV_SUMMARIES = false; //Set to true if examining tree with Ralf's CRV summary leaves


/**********************************************************************************************************************************************************************************/
///////////////////////////////////////////////////////////////   Input Files    ///////////////////////////////////////////////////////////////////////////////////////////////////

// 2025 Sample
TFile file_2025_hi_summs("/mu2e/data/users/bbarton/CR_BKGDS/TrkAna_2025/hi2025_ralfsAdds.root"); //summs sufix denotes addition of Ralf's CRV summary branches
TFile file_2025_lo_summs("/mu2e/data/users/bbarton/CR_BKGDS/TrkAna_2025/lo2025_ralfsAdds.root");
//TFile file_2025_hi("/mu2e/data/users/bbarton/CR_BKGDS/TrkAna_2025/hi2025.root");
//TFile file_2025_lo("/mu2e/data/users/bbarton/CR_BKGDS/TrkAna_2025/lo2025.root");

//CRY1 and CRY2 samples
TFile  file_CRY2("/mu2e/data/users/bbarton/CR_BKGDS/TrkAna_CRY2/cry2_ralfsAdds.root");
TFile  file_CRY1("/mu2e/data/users/bbarton/CR_BKGDS/TrkAna_CRY1/cry1_ralfsAdds.root");
//TFile  file_CRY2("/mu2e/data/users/bbarton/CRY2/TrkAnaTrees/cry2_new15July2019.root");
//TFile  file_CRY1("/mu2e/data/users/bbarton/CRY1/TrkAnaTrees/cry1_new24July2019.root");

//DBY Sample
//TFile file_DBY("/mu2e/data/users/oksuzian/CRVnTupleAllTracks.root");


//Livetimes of samples with various cuts [effective 2.46e6s years]
const double LT_2025_HI_EXPMOM = 49.5;
const double LT_2025_LO_EXPMOM = 1479.9;
const double LT_2030_HI_EXPMOM = 49.5;
const double LT_2030_LO_EXPMOM = 1483.6;
const double LT_2025_HI_NOCUTS = 12334;
const double LT_2025_LO_NOCUTS = 471745;
const double LT_2030_HI_NOCUTS = 11516;
const double LT_2030_LO_NOCUTS = 488709;
const double LT_CRY1_NOCUTS = 367; 
const double LT_CRY1_EXPMOM = 2.0; 
const double LT_CRY2_NOCUTS = 1083; 
const double LT_CRY2_EXPMOM = 5.0; 


/**********************************************************************************************************************************************************************************/



/**********************************************************************************************************************************************************************************/
//////////////////////////////////////////////////////    Define Standard Parameters for Each Variable   ///////////////////////////////////////////////////////////////////////////

//Struct to hold parameters for a histograms in a contained unit 
//Arrays should be the length of the number of cut variations to be plotted
typedef struct{
  double xMins[NCUTS + 1];
  double xMaxs[NCUTS + 1];
  int nBins[NCUTS + 1];
  string xTitle;
  string title;
  string varExpr; 
} histParams;

//This seems clunky but is the most efficient way to define a scalable number of histograms with predefined characteristics. Initializing new Histograms for each would be
//innefficient and messy.To add a new histogram

//Set the parameters for the histograms
vector<histParams> hist_params;

void defHistParams()
{
  histParams params_crvinfomc_x0;
  params_crvinfomc_x0.varExpr = "crvinfomc._x[0]";
  params_crvinfomc_x0.title = "Primary x-posiition at the CRV";
  params_crvinfomc_x0.xTitle = "MC Truth: x at CRV (mm)";
  params_crvinfomc_x0.xMins[0] = -7000; //No cut limits
  params_crvinfomc_x0.xMaxs[0] = 0;
  params_crvinfomc_x0.nBins[0] = 100;
  params_crvinfomc_x0.xMins[1] = -7000; //Signal cut limits
  params_crvinfomc_x0.xMaxs[1] = -800;
  params_crvinfomc_x0.nBins[1] = 100;
  // params_crvinfomc_x0.xMins[2] = -7000; //Expanded mom window limits
  // params_crvinfomc_x0.xMaxs[2] = -800;
  // params_crvinfomc_x0.nBins[2] = 100;
  hist_params.push_back(params_crvinfomc_x0);
  
  histParams params_crvinfomc_y0;
  params_crvinfomc_y0.title = "crvinfomc._y[0]";
  params_crvinfomc_y0.xTitle = "MC Truth: y at CRV (mm)";
  params_crvinfomc_y0.xMins[0] = -2000; //No cut limits
  params_crvinfomc_y0.xMaxs[0] = 3000;
  params_crvinfomc_y0.nBins[0] = 100;
  params_crvinfomc_y0.xMins[1] = -2000; //Signal cut limits
  params_crvinfomc_y0.xMaxs[1] = 3000;
  params_crvinfomc_y0.nBins[1] = 100;
  // params_crvinfomc_y0.xMins[2] = -2000; //Expanded mom window limits
  // params_crvinfomc_y0.xMaxs[2] = 3000;
  // params_crvinfomc_y0.nBins[2] = 100;
  hist_params.push_back(params_crvinfomc_y0);

  histParams params_crvinfomc_z0;
  params_crvinfomc_z0.title = "crvinfomc._z[0]";
  params_crvinfomc_z0.xTitle = "MC Truth: z at CRV (mm)";
  params_crvinfomc_z0.xMins[0] = -3000; //No cut limits
  params_crvinfomc_z0.xMaxs[0] = 20000;
  params_crvinfomc_z0.nBins[0] = 100;
  params_crvinfomc_z0.xMins[1] = 0; //Signal cut limits
  params_crvinfomc_z0.xMaxs[1] = 19000;
  params_crvinfomc_z0.nBins[1] = 100;
  // params_crvinfomc_z0.xMins[2] = -2000; //Expanded mom window limits
  // params_crvinfomc_z0.xMaxs[2] = 19000;
  // params_crvinfomc_z0.nBins[2] = 100;
  hist_params.push_back(params_crvinfomc_z0);

  histParams params_crvinfomc_primaryX;
  params_crvinfomc_primaryX.title = "crvinfomc._primaryX";
  params_crvinfomc_primaryX.xTitle = "Primary Particle x Position (mm)";
  params_crvinfomc_primaryX.xMins[0] = -80000; //No cut limits
  params_crvinfomc_primaryX.xMaxs[0] = 80000;
  params_crvinfomc_primaryX.nBins[0] = 100;
  params_crvinfomc_primaryX.xMins[1] = -80000; //Signal cut limits
  params_crvinfomc_primaryX.xMaxs[1] = 80000;
  params_crvinfomc_primaryX.nBins[1] = 100;
  // params_crvinfomc_primaryX.xMins[2] = -80000; //Expanded mom window limits
  // params_crvinfomc_primaryX.xMaxs[2] = 80000;
  // params_crvinfomc_primaryX.nBins[2] = 100;
  hist_params.push_back(params_crvinfomc_primaryX);

  histParams params_crvinfomc_primaryY;
  params_crvinfomc_primaryY.title = "crvinfomc._primaryY";
  params_crvinfomc_primaryY.xTitle = "Primary Particle y Position (mm)";
  params_crvinfomc_primaryY.xMins[0] = 15364; //No cut limits
  params_crvinfomc_primaryY.xMaxs[0] = 15410;
  params_crvinfomc_primaryY.nBins[0] = 100;
  params_crvinfomc_primaryY.xMins[1] = 15364; //Signal cut limits
  params_crvinfomc_primaryY.xMaxs[1] = 15410;
  params_crvinfomc_primaryY.nBins[1] = 100;
  // params_crvinfomc_primaryY.xMins[2] = 15365; //Expanded mom window limits
  // params_crvinfomc_primaryY.xMaxs[2] = 15410;
  // params_crvinfomc_primaryY.nBins[2] = 100;
  hist_params.push_back(params_crvinfomc_primaryY);

  histParams params_crvinfomc_primaryZ;
  params_crvinfomc_primaryZ.title = "crvinfomc._primaryZ";
  params_crvinfomc_primaryZ.xTitle = "Primary Particle z Position (mm)";
  params_crvinfomc_primaryZ.xMins[0] = -80000; //No cut limits
  params_crvinfomc_primaryZ.xMaxs[0] = 80000;
  params_crvinfomc_primaryZ.nBins[0] = 100;
  params_crvinfomc_primaryZ.xMins[1] = -80000; //Signal cut limits
  params_crvinfomc_primaryZ.xMaxs[1] = 80000;
  params_crvinfomc_primaryZ.nBins[1] = 100;
  // params_crvinfomc_primaryZ.xMins[2] = -80000; //Expanded mom window limits
  // params_crvinfomc_primaryZ.xMaxs[2] = 80000;
  // params_crvinfomc_primaryZ.nBins[2] = 100;
  hist_params.push_back(params_crvinfomc_primaryZ);

  histParams params_crvinfomc_primaryPdgId;
  params_crvinfomc_primaryPdgId.title = "crvinfomc._primaryPdgId";
  params_crvinfomc_primaryPdgId.xTitle = "Primary Particle PDG ID";
  params_crvinfomc_primaryPdgId.xMins[0] = -225; //No cut limits
  params_crvinfomc_primaryPdgId.xMaxs[0] = 2225;
  params_crvinfomc_primaryPdgId.nBins[0] = 100;
  params_crvinfomc_primaryPdgId.xMins[1] = -15; //Signal cut limits
  params_crvinfomc_primaryPdgId.xMaxs[1] = 15;
  params_crvinfomc_primaryPdgId.nBins[1] = 30;
  // params_crvinfomc_primaryPdgId.xMins[2] = -15; //Expanded mom window limits
  // params_crvinfomc_primaryPdgId.xMaxs[2] = 30;
  // params_crvinfomc_primaryPdgId.nBins[2] = 15;
  hist_params.push_back(params_crvinfomc_primaryPdgId);

  histParams params_crvinfomc_primaryE;
  params_crvinfomc_primaryE.title = "crvinfomc._primaryE";
  params_crvinfomc_primaryE.xTitle = "Primary Particle Energy (MeV)";
  params_crvinfomc_primaryE.xMins[0] = 0; //No cut limits
  params_crvinfomc_primaryE.xMaxs[0] = 4300000;
  params_crvinfomc_primaryE.nBins[0] = 100;
  params_crvinfomc_primaryE.xMins[1] = 0; //Signal cut limits
  params_crvinfomc_primaryE.xMaxs[1] = 1000000;
  params_crvinfomc_primaryE.nBins[1] = 100;
  // params_crvinfomc_primaryE.xMins[2] = 0; //Expanded mom window limits
  // params_crvinfomc_primaryE.xMaxs[2] = 1000000;
  // params_crvinfomc_primaryE.nBins[2] = 100;
  hist_params.push_back(params_crvinfomc_primaryE);

  histParams params_deent_mom;
  params_deent_mom.title = "deent.mom";
  params_deent_mom.xTitle = "Downstream e^- : Momentum (MeV/c)";
  params_deent_mom.xMins[0] = 40; //No cut params
  params_deent_mom.xMaxs[0] = 300;
  params_deent_mom.nBins[0] = 100;
  params_deent_mom.xMins[1] = 100; //Signal cut params
  params_deent_mom.xMaxs[1] = 110;
  params_deent_mom.nBins[1] = 20; 
  // params_deent_mom.xMins[2] = 60; //Expanded mom window cut params
  // params_deent_mom.xMaxs[2] = 160;
  // params_deent_mom.nBins[2] = 100;
  hist_params.push_back(params_deent_mom);
  
  histParams params_deent_d0;
  params_deent_d0.title = "deent.d0";
  params_deent_d0.xTitle = "Downstream e^- : Distance to Center (mm)";
  params_deent_d0.xMins[0] = -600; //No cut params
  params_deent_d0.xMaxs[0] = 600;
  params_deent_d0.nBins[0] = 100;
  params_deent_d0.xMins[1] = -80; //Signal cut params
  params_deent_d0.xMaxs[1] = 110;
  params_deent_d0.nBins[1] = 100;
  // params_deent_d0.xMins[2] = -80; //Expanded mom window cut params
  // params_deent_d0.xMaxs[2] = 110;
  // params_deent_d0.nBins[2] = 100;
  hist_params.push_back(params_deent_d0);

  histParams params_deent_td; 
  params_deent_td.title = "deent.td";
  params_deent_td.xTitle = "Downstream e^- : Pitch Angle";
  params_deent_td.xMins[0] = 0; //No cut params
  params_deent_td.xMaxs[0] = 5;
  params_deent_td.nBins[0] = 100;
  params_deent_td.xMins[1] = 0.55; //Signal cut params
  params_deent_td.xMaxs[1] = 1.05;
  params_deent_td.nBins[1] = 100;
  // params_deent_td.xMins[2] = 0.55; //Expanded mom window cut params
  // params_deent_td.xMaxs[2] = 1.05;
  // params_deent_td.nBins[2] = 100;
  hist_params.push_back(params_deent_td);

  histParams params_de_t0;
  params_de_t0.title = "de.t0";
  params_de_t0.xTitle = "Downstream e^- : t_0 (s)";
  params_de_t0.xMins[0] = 400; //No cut params
  params_de_t0.xMaxs[0] = 1700;
  params_de_t0.nBins[0] = 100;
  params_de_t0.xMins[1] = 500; //Signal cut params
  params_de_t0.xMaxs[1] = 1700;
  params_de_t0.nBins[1] = 100;
  // params_de_t0.xMins[2] = 500; //Expanded mom window cut params
  // params_de_t0.xMaxs[2] = 1700;
  // params_de_t0.nBins[2] = 100;
  hist_params.push_back(params_de_t0);

  histParams params_dequal_trkQualDeM;
  params_dequal_trkQualDeM.title = "dequal.TrkQual";
  params_dequal_trkQualDeM.xTitle = "Downstream e^- : Track Quality";
  params_dequal_trkQualDeM.xMins[0] = 0; //No cut params
  params_dequal_trkQualDeM.xMaxs[0] = 1;
  params_dequal_trkQualDeM.nBins[0] = 100;
  params_dequal_trkQualDeM.xMins[1] = 0.5; //Signal cut params
  params_dequal_trkQualDeM.xMaxs[1] = 1;
  params_dequal_trkQualDeM.nBins[1] = 100;
  // params_dequal_trkQualDeM.xMins[2] = 0.5; //Expanded mom window cut params
  // params_dequal_trkQualDeM.xMaxs[2] = 1;
  // params_dequal_trkQualDeM.nBins[2] = 100;
  hist_params.push_back(params_dequal_trkQualDeM);

  histParams params_demcent_d0;
  params_demcent_d0.title = "demcent.d0";
  params_demcent_d0.xTitle = "Downstream e^- : MC Truth Distance to Center (mm)";
  params_demcent_d0.xMins[0] = -800; //No cut params
  params_demcent_d0.xMaxs[0] = 600;
  params_demcent_d0.nBins[0] = 100;
  params_demcent_d0.xMins[1] = -250; //Signal cut params
  params_demcent_d0.xMaxs[1] = 150;
  params_demcent_d0.nBins[1] = 100;
  // params_demcent_d0.xMins[2] = -250; //Expanded mom window cut params
  // params_demcent_d0.xMaxs[2] = 150;
  // params_demcent_d0.nBins[2] = 100;
  hist_params.push_back(params_demcent_d0);

  histParams params_delta_det0_CRVtimeWindowStart;
  params_delta_det0_CRVtimeWindowStart.title = "de.t0 - crvinfo._timeWindowStart";
  params_delta_det0_CRVtimeWindowStart.xTitle = "Time Delta (s)";
  params_delta_det0_CRVtimeWindowStart.xMins[0] = -200; //No cut params
  params_delta_det0_CRVtimeWindowStart.xMaxs[0] = 200;
  params_delta_det0_CRVtimeWindowStart.nBins[0] = 100;
  params_delta_det0_CRVtimeWindowStart.xMins[1] = -200; //Signal cuts params
  params_delta_det0_CRVtimeWindowStart.xMaxs[1] = 200;
  params_delta_det0_CRVtimeWindowStart.nBins[1] = 100;
  // params_delta_det0_CRVtimeWindowStart.xMins[2] = -200; //Expanded mom cut params
  // params_delta_det0_CRVtimeWindowStart.xMaxs[2] = 200;
  // params_delta_det0_CRVtimeWindowStart.nBins[2] = 100;
  hist_params.push_back(params_delta_det0_CRVtimeWindowStart);

  histParams params_pz_p;
  params_pz_p.title = "p_z / p = cos(#theta)";
  params_pz_p.varExpr = "demcpri.momz/sqrt((demcpri.momx*demcpri.momx)+(demcpri.momy*demcpri.momy)+(demcpri.momz*demcpri.momz))";
  params_pz_p.xTitle = "cos(#theta)";
  params_pz_p.xMins[0] = -1; //No cuts params
  params_pz_p.xMaxs[0] = 1;
  params_pz_p.nBins[0] = 100;
  params_pz_p.xMins[1] = -1; //Signal cuts params
  params_pz_p.xMaxs[1] = 1;
  params_pz_p.nBins[1] = 100;
  // params_pz_p.xMins[2] = -1; //Expanded mom cut params
  // params_pz_p.xMaxs[2] = 1;
  // params_pz_p.nBins[2] = 100;
  hist_params.push_back(params_pz_p);

  histParams params_noCRV_demc_pdg;
  params_noCRV_demc_pdg.title = "demc.pdg";
  params_noCRV_demc_pdg.xTitle = "Particle PDG";
  params_noCRV_demc_pdg.xMins[0] = -14.5; //No cuts params
  params_noCRV_demc_pdg.xMaxs[0] = 15.5;
  params_noCRV_demc_pdg.nBins[0] = 30;
  params_noCRV_demc_pdg.xMins[1] = -14.5; //Signal cuts params
  params_noCRV_demc_pdg.xMaxs[1] = 15.5;
  params_noCRV_demc_pdg.nBins[1] = 30;
  // params_noCRV_demc_pdg.xMins[2] = -14.5; //Expanded mom cut params
  // params_noCRV_demc_pdg.xMaxs[2] = 15.5;
  // params_noCRV_demc_pdg.nBins[2] = 30;
  hist_params.push_back(params_noCRV_demc_pdg); 

  histParams params_demcpri_posz;
  params_demcpri_posz.title = "demcpri.posz";
  params_demcpri_posz.xTitle = "MC Truth Primary z Position (mm)";
  params_demcpri_posz.xMins[0] = -80000; //No cut params
  params_demcpri_posz.xMaxs[0] = 80000;
  params_demcpri_posz.nBins[0] = 100;
  params_demcpri_posz.xMins[1] = -80000; //Signal cut params
  params_demcpri_posz.xMaxs[1] = 80000;
  params_demcpri_posz.nBins[1] = 100;
  // params_demcpri_posz.xMins[2] = -80000; //Expanded mom window cut params
  // params_demcpri_posz.xMaxs[2] = 80000;
  // params_demcpri_posz.nBins[2] = 100;
  hist_params.push_back(params_demcpri_posz);

  histParams params_demcpri_posx;
  params_demcpri_posx.title = "demcpri.posx";
  params_demcpri_posx.xTitle = "MC Truth Primary x Position (mm)";
  params_demcpri_posx.xMins[0] = -80000; //No cut params
  params_demcpri_posx.xMaxs[0] = 80000;
  params_demcpri_posx.nBins[0] = 100;
  params_demcpri_posx.xMins[1] = -80000; //Signal cut params
  params_demcpri_posx.xMaxs[1] = 80000;
  params_demcpri_posx.nBins[1] = 100;
  // params_demcpri_posx.xMins[2] = -80000; //Expanded mom window cut params
  // params_demcpri_posx.xMaxs[2] = 80000;
  // params_demcpri_posx.nBins[2] = 100;
  hist_params.push_back(params_demcpri_posx);

  histParams params_demcpri_mom;
  params_demcpri_mom.title = "sqrt((demcpri.momx*demcpri.momx)+(demcpri.momy*demcpri.momy)+(demcpri.momz*demcpri.momz))";
  params_demcpri_mom.xTitle = "MC Truth Primary Momentum (MeV/c)";
  params_demcpri_mom.xMins[0] = 0; //No cut params
  params_demcpri_mom.xMaxs[0] = 1000000;
  params_demcpri_mom.nBins[0] = 100;
  params_demcpri_mom.xMins[1] = 0; //Signal cut params
  params_demcpri_mom.xMaxs[1] = 500000;
  params_demcpri_mom.nBins[1] = 100;
  // params_demcpri_mom.xMins[2] = 0; //Expanded mom window cut params
  // params_demcpri_mom.xMaxs[2] = 500000;
  // params_demcpri_mom.nBins[2] = 100;
  hist_params.push_back(params_demcpri_mom);


  histParams params_CRVtop;
  params_CRVtop.varExpr = "crvinfomc._z[0]/1000.";
  params_CRVtop.title = "MCTruth: Number of Events/Module/2.46e6s";
  params_CRVtop.xTitle = "z [m]";
  params_CRVtop.xMins[0] = -2.1526; 
  params_CRVtop.xMaxs[0] = 18.5474;
  params_CRVtop.nBins[0] = 25;
  params_CRVtop.xMins[1] = -2.1526;
  params_CRVtop.xMaxs[1] = 18.5474;
  params_CRVtop.nBins[1] = 25;
  hist_params.push_back(params_CRVtop);

}



/**********************************************************************************************************************************************************************************/
/////////////////////////////////////////////////////////////////    Make the Plots    /////////////////////////////////////////////////////////////////////////////////////////////

//Makes stacked plots of hi and lo for each histParam struct in defHistParams()
//Files are saved as pngs/pdfs
void plotAllComparisons()
{
  if (NCUTS < 1)
    {
      cout << "NCUTS must be >= 1" << endl;
      exit(-1);
    }

  //////////////////////////////////////////////////////////////    Define Cuts    ///////////////////////////////////////////////////////////////////////////////////////////////////

   //Signal cuts
  string expanded_momentum = "deent.mom>100 && deent.mom<110"; //Use wider momentum window than experimental signal window
  string physics_mom = "deent.mom>103.85 && deent.mom<104.9";

  string no_upstream = "ue.status<0";
  string trk_qual;
  string trk_cut_pid;
  if (INC_TRKQUAL_SFX)
    {
      trk_qual = "dequal.TrkQualDeM>0.8";; //For original CRY1 analysis this was 0.4
      trk_cut_pid = "dequal.TrkPIDDeM>0.95"; //Was 0.9 until 4/27
    }
  else
    {
      trk_qual = "dequal.TrkQual>0.8";; //For original CRY1 analysis this was 0.4
      trk_cut_pid = "dequal.TrkPID>0.95";
    }
  string pitch_angle = "deent.td>0.57735027 && deent.td<1"; //  Excludes beam particles
  string min_trans_R = "deent.d0>-80 && deent.d0<105"; //  Consistent with coming from the target
  string max_trans_R = "(deent.d0+2.0/deent.om)>450. && (deent.d0+2.0/deent.om)<680."; //  Inconsistent with hitting the proton absorber
  string timing_cut = "de.t0>700 && de.t0<1695"; // This is the standard window
  string all_cuts_MDC = expanded_momentum + "&&" + no_upstream + "&&" + trk_qual + "&&" + pitch_angle + "&&" + min_trans_R + "&&" + max_trans_R;
  string expMomCuts = all_cuts_MDC + "&&" + trk_cut_pid;// + "&&" + timing_cut;
  string noMomCuts = no_upstream + "&&" + trk_qual + "&&" + pitch_angle + "&&" + min_trans_R + "&&" + max_trans_R + "&&" + trk_cut_pid;// + "&&" + timing_cut; 
  string physCuts = no_upstream + "&&" + trk_qual + "&&" + pitch_angle + "&&" + min_trans_R + "&&" + max_trans_R + "&&" + trk_cut_pid +/* "&&" + timing_cut + */"&&" + physics_mom;
  


  //Cut lists & identifiers // Add any additional cuts that you want to make to this list
  string cuts[NCUTS + 1];
  string cutIDs[NCUTS + 1];
  cuts[0] = "";
  cutIDs[0] = "noCuts";
  cuts[1] = expMomCuts;
  cutIDs[1] = "expMomCuts";
  // cuts[2] = physCuts;
  // cutIDs[2] = "physCuts";

  defHistParams(); //Define the histograms to be made

  gStyle->SetOptStat(0);

  //Read the trees
  TTree *tree_hi = (TTree*) file_2025_hi_summs.Get("TrkAnaNeg/trkana");
  TTree *tree_lo = (TTree*) file_2025_lo_summs.Get("TrkAnaNeg/trkana");
  TTree *tree_CRY1 = (TTree*) file_CRY1.Get("TrkAnaNeg/trkana");
  TTree *tree_CRY2 = (TTree*) file_CRY2.Get("TrkAnaNeg/trkana");
  // TTree *tree_DBY = (TTree*) file_DBY.Get("allTracks");

  TCanvas *canv = new TCanvas("canv","Sample Comparisons",1200,600);

  for (int cutN = 0; cutN <= NCUTS; cutN++) //For each cut
    {
      for (int v = 0; v < hist_params.size(); v++) //For each variable
	{

	  histParams params = hist_params.at(v);

	  //Define the histograms based on predefined settings
	  TH1F *hHi = new TH1F("hHi", params.title.c_str(), params.nBins[cutN], params.xMins[cutN], params.xMaxs[cutN]);
	  TH1F *hLo = new TH1F("hLo", params.title.c_str(), params.nBins[cutN], params.xMins[cutN], params.xMaxs[cutN]);
	  TH1F *hCRY1 = new TH1F("hCRY1", params.title.c_str(), params.nBins[cutN], params.xMins[cutN], params.xMaxs[cutN]);
	  TH1F *hCRY2 = new TH1F("hCRY2", params.title.c_str(), params.nBins[cutN], params.xMins[cutN], params.xMaxs[cutN]);
	  // TH1F *hDBY = new TH1F("hDBY", params.title.c_str(), params.nBins[cutN], params.xMins[cutN], params.xMaxs[cutN]);

	  //Get data from trees and store in histograms
	  string expr;
	  if (params.varExpr != "")
	    expr = params.varExpr;
	  else
	    expr = params.title;
	  tree_hi->Draw(Form("%s>>+hHi", expr.c_str()), cuts[cutN].c_str() , "goff");
	  tree_lo->Draw(Form("%s>>+hLo", expr.c_str()), cuts[cutN].c_str(), "goff");
	  tree_CRY1->Draw(Form("%s>>+hCRY1", expr.c_str()), cuts[cutN].c_str() , "goff");
	  tree_CRY2->Draw(Form("%s>>+hCRY2", expr.c_str()), cuts[cutN].c_str(), "goff");
	  // tree_DBY->Draw(Form("%s>>+hDBY", expr.c_str()), cuts[cutN].c_str() , "goff");
	  
	  
	  hHi = (TH1F*) gDirectory->Get("hHi");
	  hLo = (TH1F*) gDirectory->Get("hLo");
	  //  hDBY = (TH1F*)gDirectory->Get("hDBY");
	  hCRY2 = (TH1F*)gDirectory->Get("hCRY2");
	  hCRY1 = (TH1F*)gDirectory->Get("hCRY1");

	  //Scale by livetimes if cuts are applied
	  double lt_hi = 1; //Livetimes to scale num of entries by
	  double lt_lo = 1; 
	  double lt_CRY1 = 1;
	  double lt_CRY2 = 1;
	  //  double lt_DBY = 1;
	  if (cutN == 1){
	    lt_hi = LT_2025_HI_EXPMOM;
	    lt_lo = LT_2025_LO_EXPMOM;
	    //   lt_DBY = LT_DBY_EXPMOM;
	    lt_CRY1 = LT_CRY1_EXPMOM;
	    lt_CRY2 = LT_CRY2_EXPMOM;
	  }
	  else if(cutN == 0){
	    lt_hi = LT_2025_HI_NOCUTS;
	    lt_lo = LT_2025_LO_NOCUTS;
	    // lt_DBY = LT_DBY_NOCUTS;
	    lt_CRY1 = LT_CRY1_NOCUTS;
	    lt_CRY2 = LT_CRY2_NOCUTS;
	   
	  }
	  hHi->Scale(1.0 / lt_hi);
	  hLo->Scale(1.0 / lt_lo);
	  hCRY1->Scale(1.0 / lt_CRY1);
	  hCRY2->Scale(1.0 / lt_CRY2);
	  //  hDBY->Scale(1.0 / lt_DBY);

	  //Set plot style
	  hHi->SetLineColor(kBlack);
	  hLo->SetLineColor(kBlue);
	  hCRY1->SetLineColor(kMagenta);
	  hCRY2->SetLineColor(kRed);
	  //	  hDBY->SetLineColor(kGreen);

	  
	  //hHi->SetFillColor(kBlack);
	  hLo->SetFillColor(kBlue);
	  //hCRY1->SetFillColor(kMagenta);
	  //hCRY2->SetFillColor(kRed);
	  //hDBY->SetFillColor(kGreen);

	  hCRY1->Draw("hist");

	  //Build the stack of hi and lo samples and draw all of the histograms
	  THStack *stack = new THStack("stack",(params.title + " : " + cutIDs[cutN]).c_str());
	  stack->Add(hHi);
	  stack->Add(hLo);
	  stack->Draw("same hist");
	  stack->GetXaxis()->SetTitle(params.xTitle.c_str());
	  
	 
	  hCRY2->Draw("same hist");
	  // hDBY->Draw("same hist");
	 
	  
	  //Add legend
	  TLegend *leg = new TLegend(0.7,0.7,0.85,0.9, "Entries/Year:");
	  leg->AddEntry(hHi,Form("Hi: %.1lf", hHi->Integral()),"l");
	  leg->AddEntry(hLo,Form("Lo: %.1f", hLo->Integral()),"l");
	  //  leg->AddEntry("hDBY",Form("DBY: %0.1f events", hDBY->Integral()),"l");
	  leg->AddEntry("hCRY2",Form("CRY2: %0.1f events", hCRY2->Integral()),"l");
	  leg->AddEntry("hCRY1",Form("CRY1: %0.1f events", hCRY1->Integral()),"l");
	  leg->Draw("same");

	  //Build a filename and save plots
	  std::string plot_name;
	  plot_name.reserve(params.title.size() + 11);
	  plot_name += "comparison_";
	  for(size_t i = 0; i < params.title.size(); ++i)
	    {
	      if(params.title[i] != ']' && params.title[i] != '[' && params.title[i] != '.' && params.title[i] != ' ' && params.title[i] != '/') 
		plot_name += params.title[i];
	      if((params.title[i] == '.' && params.title[i+1] != '_') || params.title[i] == '/')
		plot_name += "_";
	    }

	  if (params.title == "p_z / p = cos(#theta)")
	    plot_name = "comp_pz_p";

	  if (cutN > 0)
	    canv->SaveAs(Form("../Plots/ComparisonPlots/%s_%s.png", plot_name.c_str(), cutIDs[cutN].c_str()));
	  else
	    canv->SaveAs(Form("../Plots/ComparisonPlots/%s.png", plot_name.c_str()));
	   

	  //Clean up
	  hHi->Delete();
	  hLo->Delete();
	  hCRY1->Delete();
	  hCRY2->Delete();
	  //	  hDBY->Delete();
	  stack->Delete();
	  leg->Delete();

	}
    }


}


/**********************************************************************************************************************************************************************************/
