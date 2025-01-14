#include "AliAnalysisTaskPtCorr.h"
#include "AliEventCuts.h"
#include "AliAnalysisManager.h"
#include "AliAnalysisFilter.h"
#include "AliAnalysisUtils.h"
#include "AliVEvent.h"
#include "AliAODEvent.h"
#include "AliESDEvent.h"
#include "AliVTrack.h"
#include "AliVParticle.h"
#include "AliAODTrack.h"
#include "AliESDtrack.h"
#include "AliVVertex.h"
#include "AliAODVertex.h"
#include "AliCentrality.h"
#include "AliExternalTrackParam.h"
#include "AliMultSelection.h"
#include "AliInputEventHandler.h"
#include "TList.h"
#include "TProfile.h"
#include "AliEventCuts.h"
#include "TTree.h"
#include "TH3.h"
#include "TH2.h"
#include "TH1.h"
#include "TClonesArray.h"
#include "AliStack.h"
#include "TClonesArray.h"
#include "TProfile.h"
#include "AliGFW.h"
#include "TClonesArray.h"
#include "AliGFWCuts.h"
#include "AliAODMCParticle.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TNamed.h"
#include "AliAODMCHeader.h"
#include "AliGenEventHeader.h"
#include "AliCollisionGeometry.h"
#include "AliGenHijingEventHeader.h"
#include "AliVVZERO.h"

ClassImp(AliAnalysisTaskPtCorr);

AliAnalysisTaskPtCorr::AliAnalysisTaskPtCorr():
  AliAnalysisTaskSE(),
  fSystFlag(0),
  fEventCutFlag(0),
  fContSubfix(0),
  fCentEst(0),
  fIsMC(kFALSE),
  fBypassTriggerAndEventCuts(kFALSE),
  fDisablePileup(kFALSE),
  fUseOldPileup(kFALSE),
  fUseIP(kFALSE),
  fUseCentCalibration(kFALSE),
  fCMflag(0),
  fDCAxyFunctionalForm(0),
  fOnTheFly(false),
  fGenerator("AMPT"),
  fUseRecoNchForMC(kFALSE),
  fRndm(0),
  fNBootstrapProfiles(10),
  fPtAxis(0),
  fEtaAxis(0),
  fMultiAxis(0),
  fCentPtAxis(0),
  fMptAxis(0),
  fV0MMultiAxis(0),
  fPtBins(0),
  fNPtBins(0),
  fEtaBins(0),
  fNEtaBins(0),
  fMultiBins(0),
  fNMultiBins(0),
  fCentPtBins(0),
  fNCentPtBins(0),
  fMptBins(0),
  fNMptBins(0),
  fV0MBinsDefault(0),
  fNV0MBinsDefault(0),
  fUseNch(kFALSE),
  fUseV0M(kFALSE),
  fUseNUEOne(kFALSE),
  fPtMpar(8),
  fEtaAcceptance(0.8),
  fImpactParameterMC(-1.0),
  fQAList(0),
  fEventCount(0),
  fMultiDist(0),
  fMultiVsV0MCorr(0),
  fNchTrueVsReco(0),
  fESDvsFB128(0),
  fNchVsV0M(0),
  fptList(0),
  fPtCont(0),
  fMeanMultiVsCent(0),
  fTriggerType(AliVEvent::kMB+AliVEvent::kINT7),
  fDetectorResponse(0),
  fRunNo(0),
  fGFWSelection(0),
  fGFWNtotSelection(0),
  fEfficiencyList(0),
  fEfficiency(0),
  fEfficiencies(0),
  fCentcal(0),
  fPseudoEfficiency(2.),
  fDCAxyVsPt_noChi2(0),
  fWithinDCAvsPt_withChi2(0),
  fDCAxyVsPt_withChi2(0),
  fWithinDCAvsPt_noChi2(0),
  fMultVsMptVsCent(0),
  fMptVsCent(0),
  fMultVsCent(0),
  fV0MMulti(0),
  fITSvsTPCMulti(0),
  fIP(0),
  fSPDCutPU(0),
  fV0CutPU(0),
  fCenCutLowPU(0),
  fCenCutHighPU(0),
  fMultCutPU(0),
  fCentralPU(1500),
  EventNo(0),
  fStdTPCITS2011(0),
  fEventWeight(PtPtSpace::kTuples),
  fRequireReloadOnRunChange(kFALSE),
  fEnableFB768dcaxy(kFALSE),
  wp(0),
  wpcm(0)
{
};
AliAnalysisTaskPtCorr::AliAnalysisTaskPtCorr(const char *name, Bool_t IsMC, TString ContSubfix):
  AliAnalysisTaskSE(name),
  fSystFlag(0),
  fEventCutFlag(0),
  fContSubfix(0),
  fCentEst(0),
  fIsMC(IsMC),
  fBypassTriggerAndEventCuts(kFALSE),
  fDisablePileup(kFALSE),
  fUseOldPileup(kFALSE),
  fUseIP(kFALSE),
  fUseCentCalibration(kFALSE),
  fCMflag(0),
  fDCAxyFunctionalForm(0),
  fOnTheFly(false),
  fGenerator("AMPT"),
  fUseRecoNchForMC(kFALSE),
  fRndm(0),
  fNBootstrapProfiles(10),
  fPtAxis(0),
  fEtaAxis(0),
  fMultiAxis(0),
  fCentPtAxis(0),
  fMptAxis(0),
  fV0MMultiAxis(0),
  fPtBins(0),
  fNPtBins(0),
  fEtaBins(0),
  fNEtaBins(0),
  fMultiBins(0),
  fNMultiBins(0),
  fCentPtBins(0),
  fNCentPtBins(0),
  fMptBins(0),
  fNMptBins(0),
  fV0MBinsDefault(0),
  fNV0MBinsDefault(0),
  fUseNch(kFALSE),
  fUseV0M(kFALSE),
  fUseNUEOne(kFALSE),
  fPtMpar(8),
  fEtaAcceptance(0.8),
  fImpactParameterMC(-1.0),
  fQAList(0),
  fEventCount(0),
  fMultiDist(0),
  fMultiVsV0MCorr(0),
  fNchTrueVsReco(0),
  fESDvsFB128(0),
  fNchVsV0M(0),
  fptList(0),
  fPtCont(0),
  fMeanMultiVsCent(0),
  fTriggerType(AliVEvent::kMB+AliVEvent::kINT7),
  fDetectorResponse(0),
  fRunNo(0),
  fGFWSelection(0),
  fGFWNtotSelection(0),
  fEfficiencyList(0),
  fEfficiency(0),
  fEfficiencies(0),
  fCentcal(0),
  fPseudoEfficiency(2.),
  fDCAxyVsPt_noChi2(0),
  fWithinDCAvsPt_withChi2(0),
  fDCAxyVsPt_withChi2(0),
  fWithinDCAvsPt_noChi2(0),
  fMultVsMptVsCent(0),
  fMptVsCent(0),
  fMultVsCent(0),
  fV0MMulti(0),
  fITSvsTPCMulti(0),
  fIP(0),
  fSPDCutPU(0),
  fV0CutPU(0),
  fCenCutLowPU(0),
  fCenCutHighPU(0),
  fMultCutPU(0),
  fCentralPU(1500),
  EventNo(0),
  fStdTPCITS2011(0),
  fEventWeight(PtPtSpace::kTuples),
  fRequireReloadOnRunChange(kFALSE),
  fEnableFB768dcaxy(kFALSE),
  wp(0),
  wpcm(0)
{
  SetContSubfix(ContSubfix);
  fCentEst = new TString("V0M");
  if(!fIsMC) { //Efficiency and NUA only important for data
    DefineInput(1,TList::Class());  //NUE
  };
  if(fIsMC) {
    DefineInput(1,TH1::Class()); //Centrality calibration
  }
  DefineOutput(1,TList::Class());
  DefineOutput(2,TList::Class());
  SetNchCorrelationCut(1,0,kFALSE);
};
AliAnalysisTaskPtCorr::~AliAnalysisTaskPtCorr() {
};
void AliAnalysisTaskPtCorr::UserCreateOutputObjects(){
  if(!fGFWSelection) SetSystFlag(0);
  fGFWSelection->PrintSetup();
  fSystFlag = fGFWSelection->GetSystFlagIndex();
  if(fGFWSelection->GetSystFlagIndex() == 20) SetCentralityEstimator("CL0");
  else if(fGFWSelection->GetSystFlagIndex() == 21) SetCentralityEstimator("CL1");
  if(!fDCAxyFunctionalForm.IsNull()) { fGFWSelection->SetPtDepDCAXY(fDCAxyFunctionalForm); }
  OpenFile(1);
  SetupAxes();
  if(fOnTheFly)
  {
    printf("Creating OTF objects\n");
    printf("Generator is %s\n",fGenerator.Data());
    if(fUseCentCalibration) {
      fCentcal = (TH1*)GetInputData(1);
    }
    else if(centralitymap.empty() && fGenerator.EqualTo("AMPT")) {
      vector<double> b = {0.0,3.72,5.23,7.31,8.88,10.20,11.38,12.47,13.50,14.51,100.0};
      vector<double> cent = {0.0,5.0,10.0,20.0,30.0,40.0,50.0,60.0,70.0,80.0,100.0};
      for(size_t i(0); i<b.size(); ++i) centralitymap[b[i]]=cent[i];
    }
    else if(centralitymap.empty() && fGenerator.EqualTo("HIJING")) {
      vector<double> b = {0.0,1.60,2.27,2.79,3.22,3.60,5.09,7.20,8.83,10.20,11.40,12.49,13.49,14.44,15.46,100.0};
      vector<double> cent = {0.0,1.0,2.0,3.0,4.0,5.0,10.0,20.0,30.0,40.0,50.0,60.0,70.0,80.0,90.0,100.0};
      for(size_t i(0); i<b.size(); ++i) centralitymap[b[i]]=cent[i];
    }
    fIP = new TH1D("fIP","Impact parameter",1500,0.0,30.0);
    printf("OTF objects created\n");
  }
  fRndm = new TRandom(0);
  fRequireReloadOnRunChange = kFALSE;
  if(!fIsMC || fUseRecoNchForMC) LoadCorrectionsFromLists(); //Efficiencies and NUA are only for the data or if specified for pseudoefficiencies

  fptList = new TList();
  fptList->SetOwner(kTRUE);
  fPtCont = new AliPtPtContainer("ptcont_ch","ptcont_ch",fNMultiBins,fMultiBins,fPtMpar);
  fPtCont->SetEventWeight(fEventWeight);
  fptList->Add(fPtCont);
  if(fNBootstrapProfiles) fPtCont->InitializeSubsamples(fNBootstrapProfiles);

  printf("1\n");
  const Int_t nFineCentBins=90;
  Double_t *fineCentBins = new Double_t[nFineCentBins+1];
  for(Int_t i=0;i<=nFineCentBins; i++) fineCentBins[i] = i;
  fMultVsMptVsCent = new TH3D("fMultVsMptVsCent",Form(";%s; #LT[#it{p}_{T}]#GT;centrality (%)",(fUseNch)?"#it{N}_{ch}":(fUseV0M)?"V0M Amplitude":"Centrality (%)"),fNMultiBins,fMultiBins,fNMptBins,fMptBins,fNCentPtBins,fCentPtBins);
  fptList->Add(fMultVsMptVsCent);
  fMptVsCent = new TH2D("fMptVsCent",";centrality (%); #LT[#it{p}_{T}]#GT",nFineCentBins,fineCentBins,fNMptBins,fMptBins);
  fptList->Add(fMptVsCent);
  fMultiDist = new TH1D("MultiDistribution",Form("Multiplicity distribution; %s; Events",(fUseNch)?"#it{N}_{ch}":(fUseV0M)?"V0M Amplitude":"Centrality (%)"),fNMultiBins,fMultiBins);
  fV0MMulti = new TH1D("V0M_Multi","V0M_Multi",fNV0MBinsDefault,fV0MBinsDefault);
  fptList->Add(fMultiDist);
  fptList->Add(fV0MMulti);
  printf("2\n");
  fMultiVsV0MCorr = new TH2D*[2];
  fMultiVsV0MCorr[0] = new TH2D("MultVsV0M_BeforeConsistency","MultVsV0M_BeforeConsistency",103,0,103,fNMultiBins,fMultiBins[0],fMultiBins[fNMultiBins]);
  fMultiVsV0MCorr[1] = new TH2D("MultVsV0M_AfterConsistency","MultVsV0M_AfterConsistency",103,0,103,fNMultiBins,fMultiBins[0],fMultiBins[fNMultiBins]);
  fESDvsFB128 = new TH2D("ESDvsFB128","; N(FB128); N(ESD)",500,-0.5,4999.5,1500,-0.5,14999.5);
  fptList->Add(fMultiVsV0MCorr[0]);
  fptList->Add(fMultiVsV0MCorr[1]);
  fptList->Add(fESDvsFB128);
  //ITS vs TPC tracklets cut for PU
  printf("3\n");
  fITSvsTPCMulti = new TH2D("TPCvsITSclusters",";TPC clusters; ITS clusters",1000,0,10000,5000,0,50000);
  fptList->Add(fITSvsTPCMulti);
  if(fIsMC) {
    fNchTrueVsReco = new TH2D("NchTrueVsReco",";Nch (MC-true); Nch (MC-reco)",fNMultiBins,fMultiBins,fNMultiBins,fMultiBins);
    fptList->Add(fNchTrueVsReco);
  }
  printf("4\n");
  const Int_t nIPbins = 100;
  Double_t *ipBins = new Double_t[nIPbins];
  for(Int_t i=0;i<=nIPbins; i++) ipBins[i] = 0.2*i;
  fMeanMultiVsCent = new AliProfileBS("MeanMultiVsCent","Mean Multi vs centrality",(fUseIP)?nIPbins:nFineCentBins,(fUseIP)?ipBins:fineCentBins);
  fptList->Add(fMeanMultiVsCent);
  printf("5\n");
  fMultVsCent = new TH2D("fMultVsCent",Form(";%s; %s","Centrality (%)",(fUseNch)?"#it{N}_{ch}":(fUseV0M)?"V0M Amplitude":"Centrality (%)"),nFineCentBins,fineCentBins,fNMultiBins,fMultiBins);
  fptList->Add(fMultVsCent);
  printf("6\n");
  fNchVsV0M = new TH2D("fNchVsV0M","N_{ch} vs V0M amplitude; V0M amplitude; N_{ch}",1000,0,40000,2250,0,4500);
  fptList->Add(fNchVsV0M);
  printf("7\n");
  printf("Multiplicity objects created\n");
  PostData(1,fptList);

  printf("Creating QA objects\n");
  fQAList = new TList();
  fQAList->SetOwner(kTRUE);
  if(fOnTheFly) fQAList->Add(fIP);
  fEventCuts.AddQAplotsToList(fQAList,kTRUE);
  int nEventCutLabel = 7;
  fEventCount = new TH1D("fEventCount","Event counter",nEventCutLabel,0,nEventCutLabel);
  TString eventCutLabel[7]={"Input","Centrality","Trigger","AliEventCuts","Vertex","Pileup","Tracks"};
  for(int i=0;i<nEventCutLabel;++i) fEventCount->GetXaxis()->SetBinLabel(i+1,eventCutLabel[i].Data());
  fQAList->Add(fEventCount);
  fhQAEventsfMult32vsCentr = new TH2D("fhQAEventsfMult32vsCentr", "; centrality V0M; TPC multiplicity (FB32)", 100, 0, 100, 100, 0, 3000);
  fQAList->Add(fhQAEventsfMult32vsCentr);
  fhQAEventsMult128vsCentr = new TH2D("fhQAEventsfMult128vsCentr", "; centrality V0M; TPC multiplicity (FB128)", 100, 0, 100, 100, 0, 5000);
  fQAList->Add(fhQAEventsMult128vsCentr);
  fhQAEventsfMultTPCvsTOF = new TH2D("fhQAEventsfMultTPCvsTOF", "; TPC FB32 multiplicity; TOF multiplicity", 200, 0, 4000, 200, 0, 2000);
  fQAList->Add(fhQAEventsfMultTPCvsTOF);
  fhQAEventsfMultTPCvsESD = new TH2D("fhQAEventsfMultTPCvsESD", "; TPC FB128 multiplicity; ESD multiplicity", 200, 0, 7000, 300, -1000, 35000);
  fQAList->Add(fhQAEventsfMultTPCvsESD);
  printf("QA objects created!\n");
  PostData(2,fQAList);
  fEventCuts.OverrideAutomaticTriggerSelection(fTriggerType,true);

  //Creating cuts for 15o_pass2 and 18qr_pass3. 18qr_pass3 not implemented yet.
  //Would like to do that in a more elegant way, but not at this point, unfortunatelly
  if(fEventCutFlag) { //Only initialize them if necessary
    fSPDCutPU = new TF1("fSPDCutPU", "450. + 3.9*x", 0, 50000);
    if(!fV0CutPU) fV0CutPU = new TF1("fV0CutPU", "[0]+[1]*x - 6.*[2]*([3] + [4]*sqrt(x) + [5]*x + [6]*x*sqrt(x) + [7]*x*x)", 0, 100000); //Only if not initialized externally. Set to 0 for ESD MC, as that seems to be problematic?
    fCenCutLowPU = new TF1("fCenCutLowPU", "[0]+[1]*x - 5.5*([2]+[3]*x+[4]*x*x+[5]*x*x*x)", 0, 100);
    fCenCutHighPU = new TF1("fCenCutHighPU", "[0]+[1]*x + 5.5*([2]+[3]*x+[4]*x*x+[5]*x*x*x)", 0, 100);
    fMultCutPU = new TF1("fMultCutPU", "[0]+[1]*x+[2]*exp([3]-[4]*x) - 6.*([5]+[6]*exp([7]-[8]*x))", 0, 100);
    if(fEventCutFlag==1 || fEventCutFlag==101) {
      Double_t parV0[8] = {33.4237, 0.953516, 0.0712137, 227.923, 8.9239, -0.00319679, 0.000306314, -7.6627e-07};
      fV0CutPU->SetParameters(parV0);
      Double_t parV0CL0[6] = {0.0193587, 0.975914, 0.675714, 0.0292263, -0.000549509, 5.86421e-06};
      fCenCutLowPU->SetParameters(parV0CL0);
      fCenCutHighPU->SetParameters(parV0CL0);
      Double_t parFB32[9] = {-812.822, 6.41796, 5421.83, -0.382601, 0.0299686, -26.6249, 321.388, -0.82615, 0.0167828};
      fMultCutPU->SetParameters(parFB32);
    }
  };
  fGFWNtotSelection = new AliGFWCuts();
  fGFWNtotSelection->SetupCuts(0);
  fGFWNtotSelection->SetEta(fEtaAcceptance);
  printf("All output objects created!\n");
};

void AliAnalysisTaskPtCorr::UserExec(Option_t*) {
  EventNo++;
  if(fOnTheFly) { ProcessOnTheFly(); return; }
  AliAODEvent *fAOD = dynamic_cast<AliAODEvent*>(InputEvent());
  if(!fAOD) return;
  if(fIsMC) {
    fMCEvent = dynamic_cast<AliMCEvent *>(MCEvent());
    if (!fMCEvent) return;
  }
  fEventCount->Fill("Input",1);

  AliMultSelection *l_MultSel = (AliMultSelection*)fInputEvent->FindListObject("MultSelection");
  if(!l_MultSel) { AliFatal("MultSelection not found\n"); return; }
  Double_t l_Cent  = l_MultSel->GetMultiplicityPercentile(fCentEst->Data());
  if(l_Cent<0) return;
  fEventCount->Fill("Centrality",1);

  if(!fBypassTriggerAndEventCuts)
    if(!CheckTrigger(l_Cent)) return;
  fEventCount->Fill("Trigger",1);

  Double_t vtxXYZ[] = {0.,0.,0.};
  if(!AcceptAOD(fAOD, vtxXYZ)) return;
  Double_t vz = fAOD->GetPrimaryVertex()->GetZ();
  if(!fGFWSelection->AcceptVertex(fAOD)) return;
  fEventCount->Fill("Vertex",1);

  if(fUseOldPileup && IsPileupEvent(fAOD,l_Cent)) return;
  if(l_Cent < 10) fEventCuts.fESDvsTPConlyLinearCut[0] = fCentralPU;
  else fEventCuts.fESDvsTPConlyLinearCut[0] = 15000.;
  fEventCount->Fill("Pileup",1);

  wp.clear(); wp.resize(fPtMpar+1,vector<double>(fPtMpar+1));
  Int_t iCent = fV0MMulti->FindBin(l_Cent);
  if(!iCent || iCent>fV0MMulti->GetNbinsX()) return;
  iCent--;
  Double_t ptMin = fPtBins[0];
  Double_t ptMax = fPtBins[fNPtBins];
  float nTotNoTracks=0;
  int nTotTracksFB128=0;
  AliAODTrack *lTrack;
  if(fIsMC) {
    float nTotNoTracksMC=0;
    float nTotNoTracksReco=0;
    if(fUseRecoNchForMC) nTotNoTracksReco = GetNtotTracks(fAOD,ptMin,ptMax,vtxXYZ,iCent);
    TClonesArray *tca = (TClonesArray*)fInputEvent->FindListObject("mcparticles");
    Int_t nPrim = tca->GetEntries();
    if(nPrim<1) return;
    AliAODMCParticle *lPart;
    for(Int_t ipart = 0; ipart < nPrim; ipart++) {
      lPart = (AliAODMCParticle*)tca->At(ipart);
      if (!lPart->IsPhysicalPrimary()) continue;
      if (lPart->Charge()==0.) continue;
      //Hardcoded cuts to inhereted from AcceptAODTrack
      Double_t leta = lPart->Eta();
      if(TMath::Abs(leta) > fEtaAcceptance) continue;
      Double_t pt = lPart->Pt();
      if(pt<ptMin || pt>ptMax) continue;
      nTotNoTracksMC++;
      FillWPCounter(wp,1,pt);
    };
    nTotNoTracks = fUseRecoNchForMC?nTotNoTracksReco:nTotNoTracksMC;
    if(fUseRecoNchForMC) fNchTrueVsReco->Fill(nTotNoTracksMC,nTotNoTracksReco);
  } else {
    Bool_t usingPseudoEff = (fPseudoEfficiency<1);
    for(Int_t lTr=0;lTr<fAOD->GetNumberOfTracks();lTr++) {
      if(usingPseudoEff) if(fRndm->Uniform()>fPseudoEfficiency) continue;
      lTrack = (AliAODTrack*)fAOD->GetTrack(lTr);
      if(!lTrack) continue;
      Double_t leta = lTrack->Eta();
      Double_t trackXYZ[] = {0.,0.,0.};
      //Counting FB128 for QA:
      if(lTrack->TestFilterBit(128)) nTotTracksFB128++;
      if(TMath::Abs(leta) > fEtaAcceptance) continue;
      if(!AcceptAODTrack(lTrack,trackXYZ,ptMin,ptMax,vtxXYZ)) continue;
      Double_t lpt = lTrack->Pt();
      Double_t weff = fEfficiencies[iCent]->GetBinContent(fEfficiencies[iCent]->FindBin(lpt));
      if(weff==0.0) continue;
      weff = 1./weff;
      nTotNoTracks += (fUseNUEOne)?1.:weff;
      FillWPCounter(wp,(fUseNUEOne)?1.0:weff,lpt);
    };
  };
  if(wp[1][0]==0) return; //if no single charged particles, then surely no PID either, no sense to continue
  fEventCount->Fill("Tracks",1);

  Double_t multVZERO = 0.;
  if(fUseV0M){
    AliVVZERO *vzero = (AliVVZERO*)fAOD->GetVZEROData();
    if(vzero) {
      for(int ich=0; ich < 64; ich++)
      multVZERO += vzero->GetMultiplicity(ich);
    }
  }
  Double_t l_Multi = fUseNch?(1.0*nTotNoTracks):(fUseV0M)?multVZERO:l_Cent;
  //A check in case l_Multi is completely off the charts (in MC, sometimes it ends up being... -Xe-310???)
  if(fUseNch && l_Multi<1) return;

  //Filling pt correlations and central moments
  Double_t l_Random = fRndm->Rndm();
  fPtCont->CalculateCorrelations(wp);
  fPtCont->FillProfiles(l_Multi,l_Random);
  if(fCMflag&1) fPtCont->FillCMProfiles(wp,l_Multi,l_Random);
  fMultVsMptVsCent->Fill(l_Multi,wp[1][1]/wp[1][0],l_Cent);
  fMptVsCent->Fill(l_Cent,wp[1][1]/wp[1][0]);

  //Some multiplicity plots
  fNchVsV0M->Fill(multVZERO,nTotNoTracks);
  fMultiVsV0MCorr[0]->Fill(l_Cent,nTotNoTracks);
  fMultiVsV0MCorr[1]->Fill(l_Cent,nTotNoTracks);
  fV0MMulti->Fill(l_Cent);
  fMultiDist->Fill(l_Multi);
  fMeanMultiVsCent->Fill(l_Cent,l_Multi);
  fMultVsCent->Fill(l_Cent,l_Multi);

  //Fetching number of ESD tracks -> for QA. Only after all the events are/were rejected
  AliAODHeader *head = (AliAODHeader*)fAOD->GetHeader();
  Int_t nESD = head->GetNumberOfESDTracks();
  fESDvsFB128->Fill(nTotTracksFB128,nESD);

  PostData(1,fptList);
  PostData(2,fQAList);
  return;
};
void AliAnalysisTaskPtCorr::NotifyRun() {
  if(!fEventCutFlag || fEventCutFlag>100) { //Only relevant if we're using the standard AliEventCuts
    //Reinitialize AliEventCuts (done automatically on check):
    Bool_t dummy = fEventCuts.AcceptEvent(InputEvent());
    if(!fDisablePileup) fEventCuts.SetRejectTPCPileupWithITSTPCnCluCorr(kTRUE);

    //Then override PU cut if required:
    if(fGFWSelection->GetSystFlagIndex()==22)
      fEventCuts.fESDvsTPConlyLinearCut[0] = 1500.;
  };
}
void AliAnalysisTaskPtCorr::Terminate(Option_t*) {
};
AliMCEvent *AliAnalysisTaskPtCorr::getMCEvent() {
  AliMCEvent* ev = dynamic_cast<AliMCEvent*>(MCEvent());
  if(!ev) { AliFatal("MC event not found!"); return 0; }
  AliGenEventHeader *header = dynamic_cast<AliGenEventHeader*>(ev->GenEventHeader());
  if(!header) { AliFatal("MC event not generated!"); return 0; }
  AliCollisionGeometry* headerH;
  TString genName;
  TList *ltgen = (TList*)ev->GetCocktailList();
  if (ltgen) {
  for(auto&& listObject: *ltgen){
    genName = Form("%s",listObject->GetName());
    if (genName.Contains("Hijing")) {
      headerH = dynamic_cast<AliCollisionGeometry*>(listObject);
      break;
      }
    }
  }
  else headerH = dynamic_cast<AliCollisionGeometry*>(ev->GenEventHeader());
  if(headerH){
      fImpactParameterMC = headerH->ImpactParameter();
  }
  return ev;
}
void AliAnalysisTaskPtCorr::ProcessOnTheFly() {
  fMCEvent = getMCEvent();
  fIP->Fill(fImpactParameterMC);
  Double_t l_Cent = getGeneratorCentrality();
  Int_t nTracks = fMCEvent->GetNumberOfPrimaries();
  if(nTracks < 1) { return; }
  wp.clear(); wp.resize(fPtMpar+1,vector<double>(fPtMpar+1));
  Double_t ptMin = fPtBins[0];
  Double_t ptMax = fPtBins[fNPtBins];
  Int_t nTotNoTracks=0;
  for(Int_t i=0;i<nTracks;i++) {
    AliMCParticle* lPart = dynamic_cast<AliMCParticle*>(fMCEvent->GetTrack(i));
    if(!lPart) { continue; };
    if (!lPart->IsPhysicalPrimary()) continue;
    if (lPart->Charge()==0.) continue;
    //Hardcoded cuts to inhereted from AcceptAODTrack
    Double_t leta = lPart->Eta();
    if(TMath::Abs(leta) > fEtaAcceptance) continue;
    Double_t pt = lPart->Pt();
    if(pt<ptMin || pt>ptMax) continue;
    nTotNoTracks++;
    FillWPCounter(wp,1,pt);
  };
  if(wp[1][0]==0) return; //if no single charged particles, then surely no PID either, no sense to continue

  fEventCount->Fill("Tracks",1);
  //Filling pT variance
  Double_t l_Multi = fUseNch?(1.0*nTotNoTracks):l_Cent;
  //A check in case l_Multi is completely off the charts (in MC, sometimes it ends up being... -Xe-310???)
  if(fUseNch && l_Multi<1) return;
  Double_t l_Random = fRndm->Rndm();

  fPtCont->CalculateCorrelations(wp);
  fPtCont->FillProfiles(l_Multi,l_Random);
  fPtCont->FillCMProfiles(wp,l_Multi,l_Random);
  fV0MMulti->Fill((fUseIP)?fImpactParameterMC:l_Cent);
  fMultiDist->Fill(l_Multi);
  fMeanMultiVsCent->Fill((fUseIP)?fImpactParameterMC:l_Cent,l_Multi);
  fMultVsMptVsCent->Fill(l_Multi,wp[1][1]/wp[1][0],(fUseIP)?fImpactParameterMC:l_Cent);
  fMptVsCent->Fill((fUseIP)?fImpactParameterMC:l_Cent,wp[1][1]/wp[1][0]);
  fMultVsCent->Fill((fUseIP)?fImpactParameterMC:l_Cent,l_Multi);
  PostData(1,fptList);
  PostData(2,fQAList);
  return;
}
double AliAnalysisTaskPtCorr::getGeneratorCentrality()
{
  if(fUseCentCalibration) return fCentcal->GetBinContent(fCentcal->GetXaxis()->FindBin(fImpactParameterMC));
  vector<double> b;
  if(centralitymap.empty()) AliFatal("Centralitymap is empty!");
  for (auto const& element : centralitymap) b.push_back(element.first);
  vector<double>::iterator it = upper_bound(b.begin(),b.end(),fImpactParameterMC);
  double l_cent = (fImpactParameterMC<0)?-1.0:(centralitymap[b[it-b.begin()]]+centralitymap[b[it-b.begin()-1]])/2.0;
  return l_cent;
}
Bool_t AliAnalysisTaskPtCorr::CheckTrigger(Double_t lCent) {
  UInt_t fSelMask = ((AliInputEventHandler*)(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler()))->IsEventSelected();
  //Apparently, MB trigger can also mark special triggers, leaving depleted regions in multi. To avoid this, pass true, if MB has been triggered.
  //This would fail if spec. triggers would also flag MB trigger, which seems to NOT be the case.
  if(!(fTriggerType&fSelMask)) { return kFALSE; }; //printf("Returning from the generic check\n");
  if(fSelMask&(fTriggerType&(AliVEvent::kINT7+AliVEvent::kMB))) {return kTRUE; }; //printf("Passed by MB trigger!\n");
  if((fSelMask&fTriggerType&AliVEvent::kCentral) && lCent>10) {return kFALSE; }; //printf("Returnning from kCent case\n");
  if((fSelMask&fTriggerType&AliVEvent::kSemiCentral) && (lCent<30 || lCent>50)) {return kFALSE; }; //printf("Returning from kSC case\n");
  return kTRUE;
};

Bool_t AliAnalysisTaskPtCorr::IsPileupEvent(AliAODEvent* ev, double centrality){
  // Check for additional pile-up rejection in Run 2 Pb-Pb collisions (15o, 17n)
  // based on multiplicity correlations
  // ***************************************************************************

  Bool_t bIs17n = kFALSE;
  Bool_t bIs15o = kFALSE;
  Bool_t bIs18qr = kFALSE;

  Int_t iRunNumber = ev->GetRunNumber();
  if(iRunNumber >= 244824 && iRunNumber <= 246994) { bIs15o = kTRUE; }
  else if(iRunNumber == 280235 || iRunNumber == 20234) { bIs17n = kTRUE; }
  else if(iRunNumber >= 295585 && iRunNumber <= 297595 ) { bIs18qr = kTRUE; }
  else { return kFALSE; }

  // recounting multiplcities
  const Int_t multESD = ((AliAODHeader*) ev->GetHeader())->GetNumberOfESDTracks();
  const Int_t nTracks = ev->GetNumberOfTracks();
  Int_t multTPC32 = 0;
  Int_t multTPC128 = 0;
  Int_t multTOF = 0;
  Int_t multTrk = 0;
  Double_t multESDTPCdif = 0.0;
  Double_t v0Centr = 0.0;

  for(Int_t it(0); it < nTracks; it++)
  {
    AliAODTrack* track = (AliAODTrack*) ev->GetTrack(it);
    if(!track) { continue; }

    if(track->TestFilterBit(32))
    {
      multTPC32++;
      if(TMath::Abs(track->GetTOFsignalDz()) <= 10.0 && track->GetTOFsignal() >= 12000.0 && track->GetTOFsignal() <= 25000.0) { multTOF++; }
      if((TMath::Abs(track->Eta())) < 0.8 && (track->GetTPCNcls() >= 70) && (track->Pt() >= 0.2) && (track->Pt() < 3)) { multTrk++; }
    }

    if(track->TestFilterBit(128)) { multTPC128++; }
  }
  int fPileupCut = 15000;
  if(centrality < 10) fPileupCut = fCentralPU;
  if(bIs17n)
  {
    multESDTPCdif = multESD - (6.6164 + 3.64583*multTPC128 + 0.000126397*multTPC128*multTPC128);
    if(multESDTPCdif > 1000) { return kTRUE; }
    if( ((AliAODHeader*) ev->GetHeader())->GetRefMultiplicityComb08() < 0) { return kTRUE; }
  }

  if(bIs15o)
  {
    multESDTPCdif = multESD - 3.38*multTPC128;
    if(multESDTPCdif > fPileupCut) { return kTRUE; }

    TF1 fMultTOFLowCut = TF1("fMultTOFLowCut", "[0]+[1]*x+[2]*x*x+[3]*x*x*x - 4.*([4]+[5]*x+[6]*x*x+[7]*x*x*x+[8]*x*x*x*x+[9]*x*x*x*x*x)", 0, 10000);
    fMultTOFLowCut.SetParameters(-1.0178, 0.333132, 9.10282e-05, -1.61861e-08, 1.47848, 0.0385923, -5.06153e-05, 4.37641e-08, -1.69082e-11, 2.35085e-15);
    if(Double_t(multTOF) < fMultTOFLowCut.Eval(Double_t (multTPC32))) { return kTRUE; }

    TF1 fMultTOFHighCut = TF1("fMultTOFHighCut", "[0]+[1]*x+[2]*x*x+[3]*x*x*x + 4.*([4]+[5]*x+[6]*x*x+[7]*x*x*x+[8]*x*x*x*x+[9]*x*x*x*x*x)", 0, 10000);
    fMultTOFHighCut.SetParameters(-1.0178, 0.333132, 9.10282e-05, -1.61861e-08, 1.47848, 0.0385923, -5.06153e-05, 4.37641e-08, -1.69082e-11, 2.35085e-15);
    if(Double_t(multTOF) > fMultTOFHighCut.Eval(Double_t (multTPC32))) { return kTRUE; }

    AliMultSelection* multSelection = (AliMultSelection*) ev->FindListObject("MultSelection");
    if(!multSelection) { AliError("AliMultSelection object not found! Returning -1"); return -1; }
    v0Centr = multSelection->GetMultiplicityPercentile("V0M");

    TF1 fMultCentLowCut = TF1("fMultCentLowCut", "[0]+[1]*x+[2]*exp([3]-[4]*x) - 5.*([5]+[6]*exp([7]-[8]*x))", 0, 100);
    fMultCentLowCut.SetParameters(-6.15980e+02, 4.89828e+00, 4.84776e+03, -5.22988e-01, 3.04363e-02, -1.21144e+01, 2.95321e+02, -9.20062e-01, 2.17372e-02);
    if(Double_t(multTrk) < fMultCentLowCut.Eval(v0Centr)) { return kTRUE; }
  }

  if(bIs18qr)
  {
    multESDTPCdif = multESD - 3.38*multTPC128;
    if(multESDTPCdif > fPileupCut) { return kTRUE; }

  }

  // QA Plots
  fhQAEventsfMult32vsCentr->Fill(v0Centr, multTrk);
  fhQAEventsMult128vsCentr->Fill(v0Centr, multTPC128);
  fhQAEventsfMultTPCvsTOF->Fill(multTPC32, multTOF);
  fhQAEventsfMultTPCvsESD->Fill(multTPC128, multESD);

  return kFALSE;
}
Bool_t AliAnalysisTaskPtCorr::AcceptAOD(AliAODEvent *inEv, Double_t *lvtxXYZ) {
  if(!fBypassTriggerAndEventCuts) {
    if(!fEventCutFlag) { if(!fEventCuts.AcceptEvent(inEv)) return 0; } //Don't perform AcceptEvent if not relevant
    else if(!AcceptCustomEvent(inEv)) return 0;
    if(fEventCutFlag>100) Bool_t dummy = fEventCuts.AcceptEvent(inEv); //if flag > 100, then also store QA output from AcceptEvent
  };
  fEventCount->Fill("AliEventCuts",1);
  const AliAODVertex* vtx = dynamic_cast<const AliAODVertex*>(inEv->GetPrimaryVertex());
  if(!vtx || vtx->GetNContributors() < 1)
    return kFALSE;
  const AliAODVertex* vtxSPD = dynamic_cast<const AliAODVertex*>(inEv->GetPrimaryVertexSPD());
  Double_t dMaxResol = 0.25; // suggested from DPG
  Double_t cov[6] = {0};
  vtxSPD->GetCovarianceMatrix(cov);
  Double_t zRes = TMath::Sqrt(cov[5]);
  if ( vtxSPD->IsFromVertexerZ() && (zRes > dMaxResol)) return kFALSE;
  const Double_t aodVtxZ = vtx->GetZ();
  if(TMath::Abs(aodVtxZ) > 10)
    return kFALSE;
  vtx->GetXYZ(lvtxXYZ);

  return kTRUE;
};
Bool_t AliAnalysisTaskPtCorr::AcceptCustomEvent(AliAODEvent* fAOD) { //From Alex
  Float_t v0Centr    = -100.;
  Float_t cl1Centr   = -100.;
  Float_t cl0Centr   = -100.;
  AliMultSelection* MultSelection = 0x0;
  MultSelection = (AliMultSelection*)fAOD->FindListObject("MultSelection");
  if(!MultSelection) {
    AliWarning("AliMultSelection object not found!");
    return kFALSE;
  } else {
    v0Centr = MultSelection->GetMultiplicityPercentile("V0M");
    cl1Centr = MultSelection->GetMultiplicityPercentile("CL1");
    cl0Centr = MultSelection->GetMultiplicityPercentile("CL0");
  }
  if(v0Centr>=80.||v0Centr<0) return kFALSE; //This would have to be adjusted for vs. V0M
  Int_t nITSClsLy0 = fAOD->GetNumberOfITSClusters(0);
  Int_t nITSClsLy1 = fAOD->GetNumberOfITSClusters(1);
  Int_t nITSCls = nITSClsLy0 + nITSClsLy1;
  AliAODTracklets *aodTrkl = (AliAODTracklets*)fAOD->GetTracklets();
  Int_t nITSTrkls = aodTrkl->GetNumberOfTracklets(); //ESD: esd->GetMultiplicity()->GetNumberOfTracklets()
  const Int_t nTracks = fAOD->GetNumberOfTracks(); //ESD: est->GetNumberOfTracks()
  Int_t multTrk = 0;
  for (Int_t it = 0; it < nTracks; it++) {
    AliAODTrack* aodTrk = (AliAODTrack*)fAOD->GetTrack(it);
    if(!aodTrk){
        delete aodTrk;
        continue;
    }
    if(aodTrk->TestFilterBit(32)) multTrk++; //GetStandardITSTPCTrackCuts2011()
  }
  AliAODVZERO* aodV0 = fAOD->GetVZEROData();
  Float_t multV0a = aodV0->GetMTotV0A();
  Float_t multV0c = aodV0->GetMTotV0C();
  Float_t multV0Tot = multV0a + multV0c;
  UShort_t multV0aOn = aodV0->GetTriggerChargeA();
  UShort_t multV0cOn = aodV0->GetTriggerChargeC();
  UShort_t multV0On = multV0aOn + multV0cOn;
  //pile-up cuts
  if(cl0Centr<fCenCutLowPU->Eval(v0Centr)) return kFALSE;
  if (cl0Centr > fCenCutHighPU->Eval(v0Centr)) return kFALSE;
  if(Float_t(nITSCls)>fSPDCutPU->Eval(nITSTrkls)) return kFALSE;
  if(multV0On<fV0CutPU->Eval(multV0Tot)) return kFALSE;
  if(Float_t(multTrk)<fMultCutPU->Eval(v0Centr)) return kFALSE;
  if(((AliAODHeader*)fAOD->GetHeader())->GetRefMultiplicityComb08()<0) return kFALSE;
  if(fAOD->IsIncompleteDAQ()) return kFALSE;
  return kTRUE;
}
Bool_t AliAnalysisTaskPtCorr::AcceptCustomEvent(AliESDEvent* fESD) { //From Alex
  Float_t v0Centr    = -100.;
  Float_t cl1Centr   = -100.;
  Float_t cl0Centr   = -100.;
  AliMultSelection* MultSelection = 0x0;
  MultSelection = (AliMultSelection*)fESD->FindListObject("MultSelection");
  if(!MultSelection) {
    AliWarning("AliMultSelection object not found!");
    return kFALSE;
  } else {
    v0Centr = MultSelection->GetMultiplicityPercentile("V0M");
    cl1Centr = MultSelection->GetMultiplicityPercentile("CL1");
    cl0Centr = MultSelection->GetMultiplicityPercentile("CL0");
  }
  if(v0Centr>=80.||v0Centr<0) return kFALSE; //This would have to be adjusted for vs. V0M
  Int_t nITSClsLy0 = fESD->GetNumberOfITSClusters(0);
  Int_t nITSClsLy1 = fESD->GetNumberOfITSClusters(1);
  Int_t nITSCls = nITSClsLy0 + nITSClsLy1;
  Int_t nITSTrkls = fESD->GetMultiplicity()->GetNumberOfTracklets();
  const Int_t nTracks = fESD->GetNumberOfTracks();
  Int_t multTrk = 0;
  AliESDtrack *esdTrack;
  for (Int_t it = 0; it < nTracks; it++) {
    esdTrack = (AliESDtrack*)fESD->GetTrack(it);
    if(!esdTrack) continue;
    if(fStdTPCITS2011->AcceptTrack(esdTrack)) multTrk++;
  }
  AliESDVZERO* esdV0 = fESD->GetVZEROData();
  Float_t multV0a = esdV0->GetMTotV0A();
  Float_t multV0c = esdV0->GetMTotV0C();
  Float_t multV0Tot = multV0a + multV0c;
  UShort_t multV0aOn = esdV0->GetTriggerChargeA();
  UShort_t multV0cOn = esdV0->GetTriggerChargeC();
  UShort_t multV0On = multV0aOn + multV0cOn;
  //pile-up cuts
  if(cl0Centr<fCenCutLowPU->Eval(v0Centr)) return kFALSE;
  if (cl0Centr > fCenCutHighPU->Eval(v0Centr)) return kFALSE;
  if(Float_t(nITSCls)>fSPDCutPU->Eval(nITSTrkls)) return kFALSE;
  if(multV0On<fV0CutPU->Eval(multV0Tot)) return kFALSE; //Problematic for MC for whatever reason? On AODs work perfectly fine
  if(Float_t(multTrk)<fMultCutPU->Eval(v0Centr)) return kFALSE;
  AliESDtrackCuts::MultEstTrackType estType = fESD->GetPrimaryVertexTracks()->GetStatus() ? AliESDtrackCuts::kTrackletsITSTPC : AliESDtrackCuts::kTracklets;
  if(AliESDtrackCuts::GetReferenceMultiplicity(fESD,estType,0.8) < 0) return kFALSE;
  if(fESD->IsIncompleteDAQ()) return kFALSE;
  return kTRUE;
}

Bool_t AliAnalysisTaskPtCorr::AcceptAODTrack(AliAODTrack *mtr, Double_t *ltrackXYZ, const Double_t &ptMin, const Double_t &ptMax, Double_t *vtxp) {
  if(mtr->Pt()<ptMin) return kFALSE;
  if(mtr->Pt()>ptMax) return kFALSE;
  if(ltrackXYZ && vtxp) {
    mtr->GetXYZ(ltrackXYZ);
    ltrackXYZ[0] = ltrackXYZ[0]-vtxp[0];
    ltrackXYZ[1] = ltrackXYZ[1]-vtxp[1];
    ltrackXYZ[2] = ltrackXYZ[2]-vtxp[2];
  } else return kFALSE; //DCA cut is a must for now
  return fGFWSelection->AcceptTrack(mtr,(fSystFlag==1&&!fEnableFB768dcaxy)?0:ltrackXYZ,0,kFALSE); //All complementary DCA track cuts for FB768 are disabled
};
Bool_t AliAnalysisTaskPtCorr::AcceptAODTrack(AliAODTrack *mtr, Double_t *ltrackXYZ, const Double_t &ptMin, const Double_t &ptMax, Double_t *vtxp, Int_t &nTot) {
  if(mtr->Pt()<ptMin) return kFALSE;
  if(mtr->Pt()>ptMax) return kFALSE;
  if(ltrackXYZ && vtxp) {
    mtr->GetXYZ(ltrackXYZ);
    ltrackXYZ[0] = ltrackXYZ[0]-vtxp[0];
    ltrackXYZ[1] = ltrackXYZ[1]-vtxp[1];
    ltrackXYZ[2] = ltrackXYZ[2]-vtxp[2];
  } else return kFALSE; //DCA cut is a must for now
  if(fGFWNtotSelection->AcceptTrack(mtr,ltrackXYZ,0,kFALSE)) nTot++;
  return fGFWSelection->AcceptTrack(mtr,(fSystFlag==1&&!fEnableFB768dcaxy)?0:ltrackXYZ,0,kFALSE); //All complementary DCA track cuts for FB768 are disabled
};
int AliAnalysisTaskPtCorr::GetNtotTracks(AliAODEvent* lAOD, const Double_t &ptmin, const Double_t &ptmax, Double_t *vtxp, Int_t iCent) {
  Double_t ltrackXYZ[3];
  AliAODTrack *lTrack;
  int nTotNoTracks=0;
  for(Int_t lTr=0;lTr<lAOD->GetNumberOfTracks();lTr++) {
    lTrack = (AliAODTrack*)lAOD->GetTrack(lTr);
    if(!lTrack) continue;
    if(TMath::Abs(lTrack->Eta()) > fEtaAcceptance) continue;
    if(!AcceptAODTrack(lTrack,ltrackXYZ,ptmin,ptmax,vtxp)) continue;
    Double_t lpt = lTrack->Pt();
    Double_t weff = fEfficiencies[iCent]->GetBinContent(fEfficiencies[iCent]->FindBin(lpt));
    if(weff==0.0) continue;
    nTotNoTracks += (fUseNUEOne)?1.:weff;
  };
  return nTotNoTracks;
}
void AliAnalysisTaskPtCorr::FillWPCounter(vector<vector<double>> &inarr, double w, double p)
{
  for(int i=0;i<=fPtMpar;++i)
  {
    for(int j=0;j<=fPtMpar;++j)
    {
      inarr[i][j] += pow(w,i)*pow(p,j);
    }
  }
  return;
}
void AliAnalysisTaskPtCorr::LoadCorrectionsFromLists(){
  fEfficiencyList = (TList*)GetInputData(1);
  fEfficiencies = new TH1D*[fNV0MBinsDefault];
  for(Int_t i=0;i<fNV0MBinsDefault;i++) {
      fEfficiencies[i] = (TH1D*)fEfficiencyList->FindObject(Form("EffRescaled_Cent%i%s",i,fGFWSelection->GetSystPF()));
      if(fEfficiencies[i] && fPseudoEfficiency<1) fEfficiencies[i]->Scale(fPseudoEfficiency);
      if(!fEfficiencies[i]) {
        if(!i) AliFatal("Could not fetch efficiency!\n");
        printf("Could not find efficiency for V0M bin no. %i! Cloning the previous efficiency instead...\n",i);
        fEfficiencies[i] = (TH1D*)fEfficiencies[i-1]->Clone(Form("EffRescaled_Cent%i%s",i,fGFWSelection->GetSystPF()));
      };
  }
  return;
}
void AliAnalysisTaskPtCorr::SetupAxes() {
 const Int_t temp_NV0MBinsDefault=10;
  Double_t temp_V0MBinsDefault[11] = {0,5,10,20,30,40,50,60,70,80,90};
  if(!fV0MMultiAxis) SetV0MBins(temp_NV0MBinsDefault,temp_V0MBinsDefault);
  fV0MBinsDefault=GetBinsFromAxis(fV0MMultiAxis);
  fNV0MBinsDefault=fV0MMultiAxis->GetNbins();
  if(!fMultiAxis) SetMultiBins(fNV0MBinsDefault,fV0MBinsDefault);
  fMultiBins = GetBinsFromAxis(fMultiAxis);
  fNMultiBins = fMultiAxis->GetNbins();
  const Int_t l_NPtBinsDefault = 25;
  Double_t l_PtBinsDefault[l_NPtBinsDefault+1] = {0.20, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95,
                     1.00, 1.10, 1.20, 1.30, 1.40, 1.50, 1.60, 1.70, 1.80, 1.90,
                     2.00, 2.20, 2.40, 2.60, 2.80, 3.00};
  if(!fPtAxis) SetPtBins(l_NPtBinsDefault,l_PtBinsDefault);
  fPtBins = GetBinsFromAxis(fPtAxis);
  fNPtBins = fPtAxis->GetNbins();
  int Neta_Default = 1;
  double l_eta_Default[] = {-0.8,0.8};
  if(!fEtaAxis) { printf("Setting default eta bins\n"); SetEtaBins(Neta_Default,l_eta_Default);}
  fEtaBins=GetBinsFromAxis(fEtaAxis);
  fNEtaBins=fEtaAxis->GetNbins();
  const int nCentBinsMpt = 4;
  double centbinsMpt[] = {0,1,5,10,90};
  if(!fCentPtAxis) { printf("Setting default cent bins for mpt fluctuations\n"); SetCentBinsForPt(nCentBinsMpt,centbinsMpt);}
  fCentPtBins=GetBinsFromAxis(fCentPtAxis);
  fNCentPtBins=fCentPtAxis->GetNbins();

  const int nMptBins = 500;
  double mptlow = 0.5;
  double mpthigh = 1;
  if(!fMptAxis) { printf("Setting default mpt bins\n"); SetMptBins(nMptBins,mptlow,mpthigh); }
  fNMptBins = fMptAxis->GetNbins();
  fMptBins = GetBinsFromAxis(fMptAxis);
  return;
}
void AliAnalysisTaskPtCorr::SetPtBins(Int_t nPtBins, Double_t *PtBins) {
  if(fPtAxis) delete fPtAxis;
  fPtAxis = new TAxis(nPtBins, PtBins);
}
void AliAnalysisTaskPtCorr::SetEtaBins(Int_t nbins, Double_t *etabins) {
  if(fEtaAxis) delete fEtaAxis;
  fEtaAxis = new TAxis(nbins,etabins);
}
void AliAnalysisTaskPtCorr::SetMultiBins(Int_t nMultiBins, Double_t *multibins) {
  if(fMultiAxis) delete fMultiAxis;
  fMultiAxis = new TAxis(nMultiBins, multibins);
}
void AliAnalysisTaskPtCorr::SetMultiBins(Int_t nMultiBins, Double_t low, Double_t high) {
  if(fMultiAxis) delete fMultiAxis;
  fMultiAxis = new TAxis(nMultiBins, low, high);
}
void AliAnalysisTaskPtCorr::SetCentBinsForPt(Int_t nBins, Double_t *centbins) {
  if(fCentPtAxis) delete fCentPtAxis;
  fCentPtAxis = new TAxis(nBins, centbins);
}
void AliAnalysisTaskPtCorr::SetV0MBins(Int_t nV0MBins, Double_t *V0Mbins) {
  if(fV0MMultiAxis) delete fV0MMultiAxis;
  fV0MMultiAxis = new TAxis(nV0MBins, V0Mbins);
}
void AliAnalysisTaskPtCorr::SetMptBins(Int_t nMptBins, Double_t mptlow, Double_t mpthigh) {
  if(fMptAxis) delete fMptAxis;
  fMptAxis = new TAxis(nMptBins, mptlow, mpthigh);
}
Double_t *AliAnalysisTaskPtCorr::GetBinsFromAxis(TAxis *inax) {
  Int_t lBins = inax->GetNbins();
  Double_t *retBins = new Double_t[lBins+1];
  for(Int_t i=0;i<lBins;i++)
    retBins[i] = inax->GetBinLowEdge(i+1);
  retBins[lBins] = inax->GetBinUpEdge(lBins);
  return retBins;
}
