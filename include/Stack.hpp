#ifndef _STACK__H_
#define _STACK__H_
#include <string>
#include <iostream>
#include <map>

#include "TFile.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2F.h"
#include "TLegendEntry.h"
#include "TStyle.h"
#include "TLine.h"
#include "TAttFill.h"
#include "TColor.h"

#include "benoit.hpp"

class Stack  // : public TObject
{
public:
  // Default constructor
  Stack( std::string _name = "Default" );

  // Construct Stack and push_back the histo : histoName from file : fileName
  Stack(std::string fileName, std::string histoName);

  // Construct Stack and push_back the histo h
  Stack(TH1 *h);

  // Stack all histograms called histoName from various files from a map
  // First parameter of the map is path and the second is the TLegend entry
  // Usage :
  //
  // std::string histoName = "var1";
  // map< std::string, std::string > vec;
  // vec["output/tree1.root"] = "electron";
  // vec["output/tree2.root"] = "positron";
  // vec["output/tree3.root"] = "proton";
  // Stack *st = new Stack( vec, histoName );
  // st -> draw();
  Stack( std::map< std::string, std::string > fileNameAndTLegendLabel, std::string histoName );

  // A destructor, nobody uses it anyway
  virtual ~Stack();

  //////////////////////////////////////////////////////////
  //
  // Various way of stacking histos
  //
  //////////////////////////////////////////////////////////
  // call TTree::Draw(var,cut) and push the histo, axis title will be the variables name
  TH1* push_back(TTree* tree, std::string varexp, std::string selection = "", std::string option = "", Long64_t nentries = 1000000000, Long64_t firstentry = 0);

  // call TTree::Draw(var,cut) and push the histo, axis title will be the variables name
  TH1* push_back(std::string filename, std::string treename, std::string varexp, std::string selection = "", std::string option = "", Long64_t nentries = 1000000000, Long64_t firstentry = 0);

  // push_back the histo, label is the TLegend label
  TH1* push_back(TH1* h, std::string tlegendLabel = "");

  // push_back the histo called  histoNamed if present in the visited TFile
  TH1* push_back(std::string histoName, std::string tlegendLabel = "");

  // push_back the TGraph, label is the TLegend label
  TGraph* push_back(TGraph* gr, std::string tlegendLabel = "");

  // push_back the TLine, label is the TLegend label
  TLine* push_back(TLine* line, std::string tlegendLabel = "");

  // push_back a vertical TLine
  TLine* pushVerticalLine( float x, std::string tlegendLabel = "");

  // push_back a horizontal TLine
  TLine* pushHorizontalLine( float x, std::string tlegendLabel = "");


  // Stack all histograms called histoName from various files from a map
  // First parameter of the map is path and the second is the TLegend entry
  // For usage, see constructor :  Stack::Stack( std::map< std::string, std::string > fileNameAndTLegendLabel, std::string histoName )
  void Load( std::map< std::string, std::string > fileNameAndTLegendLabel, std::string histoName );


  //////////////////////////////////////////////////////////
  //
  // Display methods
  //
  //////////////////////////////////////////////////////////

  // Draws all stacked histos, computes nice axis, draws TLegend
  void draw();
  virtual void Draw(){
    draw();
  }

  // Draws in selected can all stacked histos, computes nice axis, draws TLegend
  void draw(TVirtualPad *customCanvas);

  // Change the tlgend label refering to histogram h
  void editLegendEntry(TH1* h, std::string newLabel);

  // Change the drawing option for obj
  // in addition to standard root option, you can use:
  //
  // for all, nolegend to make a plot without legend
  // for TGraph, customColor to use your own TMarkerStyle for the drawing
  // for TH2, black for not colz
  
  void setDrawingOption( TObject* obj, std::string theDrawingOption );

  // Set the frame used to display histograms
  void setFrame( float infX, float supX, float infY, float supY );

  // Change labels size on X and Y axis
  void setXLabelSize( float _sizeX );
  void setYLabelSize( float _sizeY );
  void setLabelsSize( float _sizeX, float _sizeY );

  // Change labels size on X and Y axis
  void setXTitleSize( float _sizeX );
  void setYTitleSize( float _sizeY );
  void setTitlesSize( float _sizeX, float _sizeY );

  // Change labels on X and Y axis
  void setXLabel( std::string label );
  void setYLabel( std::string label );
  void setLabels( std::string labelX, std::string labelY );

  // Set the TFrame title
  void setTitle( std::string title );

  // Normalize all histograms
  void normalize(float xinf = 0, float xsup = 0);

  // Write the canvas in the current TFile
  void write();

  // Write in .eps and .root format, one can specify another extension
  void write( std::string nameWithoutExtension, std::string extension = "" );

  // Enable or disable the TLegend (default is enabled)
  void setLegendDisplay( bool _display );

  // Change the position of the TLegend box
  void setLegendPosition( double xInf, double yInf, double xSup, double ySup );

  // Return the legend
  TLegend* getLegend(){
    return leg;
  }

private:
  std::string name;
  std::string title;
  std::string titleX;
  std::string titleY;
  float labelSizeX;
  float labelSizeY;
  float titleSizeX;
  float titleSizeY;
  std::vector<TH1*> vec;
  std::vector<TGraph*> vecGraph;
  std::vector<TLine*> vecLine;  
  std::map< TObject*, std::string > drawingOption;
  std::map< TObject*, TLegendEntry* > mapLegEntry;
  std::pair<double, double> limitsX;
  std::pair<double, double> limitsY;
  
  static std::map< std::string, int> canvasNameNumberOfInstance;
  bool _displayLegend;

  TLegend *leg;
  TPad *can;
  TH2 *frame;
  static const float topMarginFactor = 0.1;
  static const float bottomMarginFactor = 0.1;  
  void Init();
  float getMaximumInRange(TH1* h, float xinf, float xsup);
  void draw2D();
  void draw1D();
  void ComputeLimits();
  bool checkIfDrawLegend();
  bool checkIfCustomColor( TGraph* gr );
  void drawGraph( TGraph* gr, int objectNumber );
  void drawLine( TLine* line );
  
  int getColor(int n, bool fill = 0);

  static const int debug = 0;

// public:
//   ClassDef(Stack,0);  // The Stacking class
};

#endif
