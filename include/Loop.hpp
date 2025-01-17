#ifndef _LOOP__HPP_
#define _LOOP__HPP_

#include <map>
#include <string>

#include "TApplication.h"
#include "TMacro.h"

#include "amschain.h"
#include "benoit.hpp"

class Loop{
 
public:
  virtual void go(){
    init();
    createHistos();
    loopOnEvents();
    draw();
    writePrivate();
    end();
  }

  ~Loop(){
    outputFile -> Close();
  }
    
  Loop( std::vector< std::string > _data = std::vector< std::string >() );
  Loop( std::string _data );

  void addTree( std::string treePath );
  void setEventList( std::string listName);
  void dumpCuts();

  std::map< std::string, TH1* > h;
  std::map< std::string, TH2* > h2;
  std::map< std::string, TGraph* > gr;
  std::map< std::string, TH1* >::iterator it;

protected:
  Long64_t nentries;
  std::vector< std::string > data;

  TFile* outputFile;
  TTree* outputTree;
  
  AMSChain *ch;
  AMSEventR* ev;
  MCEventgR* mc;
  EcalShowerR* shower;
  ParticleR* part;
  TrTrackR* tr;
  float EnergyE;

  bool writeList;
  int maxEntries;
  int maxRootFiles;
  Long64_t entry;

  unsigned int event;
  unsigned int run;

private:
  AMSEventList inputList;
  AMSEventList outputList;
  int firstRun, lastRun; // First and last run of the loop
  std::string listURL;
  std::string listName;
  std::string outputFileName;
  bool isSaveAMSTree;
  bool isOutputTree;
  std::map<int, int> cut;

public:
  void setListName( std::string _listName ){
    listName = _listName;
  }

  void setOutputFileName( std::string _ouputFileName ){
    outputFileName = _ouputFileName;
  }
  
  void saveAMSTree( bool _isSaveAMSTree ){
    isSaveAMSTree = _isSaveAMSTree;
  }

  void setWriteList( bool _writeList ){
    writeList = _writeList;
  }

  
protected:
  void loopOnEvents();
  
  virtual void createHistos(){};
  virtual int cutEvent() = 0; // must return true if event should be cut
  virtual bool process() = 0; // what has to be done in the loop (like filling histograms), return false if you don't wan't to save the event in the output tree
  virtual void draw(){};
  virtual void writePrivate();
  virtual void write(){};
  virtual void init();
  virtual void setBranchStatuses(){}
  virtual void setOutputBranches(){
    isOutputTree = false;
    delete outputTree;
    outputTree = NULL;
  }
  
  virtual void end(){
    dumpCuts();
  }
  void fill( std::string nameHisto, float var);
  void fill( std::string nameHisto, float varX, float varY);
  void addHisto( std::string nameHisto, int nBins = 100, float firstBin = 0, float lastBin = 0);
  void addHisto( std::string nameHisto, int nBinsX, float firstBinX, float lastBinX, int nBinsY, float firstBinY, float lastBinY );

private:
  bool useList;
  void loadChain();
  void registerSrcFilesInRootuple();
  void openOutputFiles();
  void addToList();

  int nentriesAfterCuts;
};

#endif

