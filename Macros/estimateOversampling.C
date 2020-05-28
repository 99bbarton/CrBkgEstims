/*
  The code below should replace the contents of the Loop() function produced by make class. It will produce a histogram of resampled events.
 */



if (fChain == 0) return;
   
   TCut cuts("deent.mom>100 && deent.mom<110&&ue.status<0&&dequal.TrkQual>0.8&&deent.td>0.57735027 && deent.td<1&&deent.d0>-80 && deent.d0<105&&(deent.d0+2.0/deent.om)>450. && (deent.d0+2.0/deent.om)<680.&&dequal.TrkPID>0.95");

   fChain->Draw(">>events",cuts);

   TEventList *events = (TEventList*)gDirectory->Get("events"); //Get the list of entries passing cuts
   
   int nEvents = events->GetN();
   
   double epsilon = std::numeric_limits<float>::epsilon();
   int event;
   double xPos_min,xPos_max, zPos_min, zPos_max;
   int count = 0;
   string lowStr_x = "crvinfomc._primaryX>";
   string upStr_x = "&&crvinfomc._primaryX<";
   string lowStr_z = "&&crvinfomc._primaryZ>";
   string upStr_z = "&&crvinfomc._primaryZ<";
   TH1F *hist = new TH1F("hist", "Event Duplication | 2025 | Hi | ExpMom Cuts", 10, 0.5, 10.5);
   for (int i = 0; i < nEvents; i++)
     {
       event = events->GetEntry(i);
       fChain->GetEntry(event); //Read the event
       
       xPos_min = *crvinfomc__primaryX - epsilon; // Define the bounding box of a resample match
       xPos_max = *crvinfomc__primaryX + epsilon;
       zPos_min = *crvinfomc__primaryZ - epsilon;
       zPos_max = *crvinfomc__primaryZ + epsilon;
       TCut matchCut(lowStr_x + xPos_min + upStr_x + xPos_max + lowStr_z + zPos_min + upStr_z + zPos_max);

       
       count = fChain->GetEntries(cuts + matchCut); 
       if (count != 0)
	 hist->Fill(count, 1.0/count);
       count = 0;// cout << count << endl;   
     }
   
   gStyle->SetOptStat("111111");

   TCanvas *canv = new TCanvas("canv", "Estimation of Resampling", 800,600);
   hist->Draw("hist");
   canv->SaveAs("duplication.png");
