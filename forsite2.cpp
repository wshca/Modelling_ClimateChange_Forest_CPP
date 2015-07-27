#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <math.h>
using namespace std;

//for MPI integration we use
#include "mpi.h"
#include <stdio.h>
#include <string>

// -----------------------------------------------------------------------------
// global variables for storage

// tree
const int treemax = 5000;
// tree storage - max number of trees is set by treemax; the number of collums should be reduced to a min setting
// 0 xcoord,     1 ycoord,   2 age,      3 height,         4 seize, 
// 5 color,      6 species,  7 Md,       8 M0,             9 Gd,         10 incrememtn, 
// 11 resource, 12 infcomp, 13 dispdist, 14 fecundity, 
// 15 maxH,     16 reproH,  17 supComp
int tree[treemax][18];

// cell
const int cellx = 100;
const int celly = 100;
const int cellmax = cellx * celly;
// x and y coordinates of the cell in the grid, number of trees in cell (later on more information about the cell like resources...
int cell[cellmax][2]; // 

// other ones
int totresources = 0; //initial number of resoucses is 0
int returenintercal = 0;
int gaps = 0;
int deathRes = 0;
int deathInv = 0;
int ddInv = 0;
int ddRes = 0;
int lsRes = 0;
int lsInv = 0;
int resN = 500;
int invN = 500;

int resVar = 10; //was 0.10 so times 100 to fit integer
int resMd = 90; //was 0.90 so times 100 to fit integer
int resM0 = 90; //was 0.90 so times 100 to fit integer
int resGd = 130; //was 1.30 so times 100 to fit integer
int resG0 = 25; //was 0.25 so times 100 to fit integer
int resF = 10;
int resIntrinsicM = 10; //was 0.010 so times 1000 to fit integer
int resMaxH = 2500;
int resReproH = 0;

int invVar = 10; //was 0.10 so times 100 to fit integer
int invMd = 50; //was 0.50 so times 100 to fit integer
int invM0 = 41; //was 0.41 so times 100 to fit integer
int invGd = 99; //was 0.99 so times 100 to fit integer
int invG0 = 10; //was 0.10 so times 100 to fit integer
int invF = 10;
int invIntrinsicM = 5; //was 0.0050 so times 1000 to fit integer
int invMaxH = 2300;
int invReproH = 300;
int resIntrinsic = 0;
int reproH = 0;

int compRed = 20; //was 0.20 so times 100 to fit integer
int hcompRatio = 100; //was 1.0 so times 100 to fit integer

int treecounter = 0;
int firstRes = 0;
int firstInv = 0;

int introT = 0;
int introP = 0;

int ticks = 0;
int ticksmax = 50;

int pDisturb = 10; //was 0.010 so times 1000 to fit integer
int distRadius = 15; //was 1.5 so times 10 to fit integer
int sddP = 99; //was 0.99 so times 100 to fit integer
int alpha = 6;


int rndcounter;
//double rnddouble;








// -----------------------------------------------------------------------------
// random number generator basic function
int randomgeneratorold()
{
  /* Initialize the random number generator with a seed */
  double rnd;
  int i;
  if (rndcounter < 1)
  {
    i = 1;
  }
  else
  {
    i = rndcounter;
  };
  rndcounter = rndcounter + 1;

  srand((unsigned)time(NULL)*i);
  //double rnd = 0;

  rnd= (double(rand())/RAND_MAX);
  // cout << "rand: " << rnd << endl;
  // cout << "A number between 0 and 50: " << rnd << endl;

  

  return rnd;
}


// -----------------------------------------------------------------------------
// random number generator basic function
double randomgenerator()
{
  /* Initialize the random number generator with a seed */
  double rnd;
  int i;
  if (rndcounter < 1)
  {
    i = 1;
  }
  else
  {
    i = rndcounter;
  };
  rndcounter = rndcounter + 1;

  srand((unsigned)time(NULL)*i);

  rnd = (double(rand())/RAND_MAX);
  /* 
  cout << "rand: " << rand() << endl;
  cout << "randdouble: " << double(rand()) << endl;
  cout << "randmax: " << RAND_MAX << endl;
  */
  return rnd;
}








// -----------------------------------------------------------------------------
// 
void computeDistance(void)
{
  double rnd;
  // rnd = randomgenerator((unsigned)time(NULL));
 
  // for disposing the seeds???

}


// -----------------------------------------------------------------------------
// 


// -----------------------------------------------------------------------------
// 
void setTraits(int treeType)
{
  int Md;
  int M0;
  int Gd;
  int F;
  int IntrinsicM;
  int MaxH;
  int ReproH;
  int Var;

  if (treeType = 0)
  {
  Md = resMd;
  M0 = resM0;
  Gd = resGd;
  F = resF;
  IntrinsicM = resIntrinsic;
  MaxH = resMaxH;
  ReproH = resReproH;
  Var = resVar;
  }
  else
  {
  Md = invMd;
  M0 = invM0;
  Gd = invGd;
  F = invF;
  IntrinsicM = invIntrinsicM;
  MaxH = invMaxH;
  ReproH = invReproH;
  Var = invVar;
  };

  // set the random levels
  tree[treecounter][7]= Md * Md * Var;
  tree[treecounter][8]= M0 * M0 * Var;
  tree[treecounter][9]= Gd * Gd * Var;
  tree[treecounter][14]= F * F * Var;
  tree[treecounter][10]= IntrinsicM * IntrinsicM * Var;
  tree[treecounter][15]= MaxH * MaxH * Var;
  tree[treecounter][16]= ReproH * ReproH * Var; 

}


// -----------------------------------------------------------------------------
// 
void initSeedLocally(int treeType)
{
   tree[treecounter][2]=0; //age
   tree[treecounter][3]=2; //height
   tree[treecounter][4]=20; //size in 1/100 so we can use integer
   tree[treecounter][5]=1; //color blue = 1
   setTraits(treeType);
}


// -----------------------------------------------------------------------------
// 
void initSeed(int treeType)
{
   // local vars
   double xdouble = 0;
   double ydouble = 0;
   int xcoord = 0;
   int ycoord = 0;
   int intrinsicM = 0;

   if (treeType = 0)
     {
       intrinsicM= invIntrinsicM;
     }
   else
     {
       intrinsicM = resIntrinsicM;
     };
   
   tree[treecounter][2]=0; //age
   tree[treecounter][3]=2; //height
   tree[treecounter][4]=50; //size in 1/100 so we can use integer
   tree[treecounter][5]=0; //color white = 0.
   tree[treecounter][6]=treeType; //set initial seeds to resident = 0 species or 1 for invader
   
   // call setTraits if Md <=0, M0 <=0, Gd<=0, or intrinsicM <=0
   while (tree[treecounter][7] + 1 < 0 || tree[treecounter][8] +1 < 0|| tree[treecounter][9] +1 < 0 || intrinsicM +1 < 0)
     {
     setTraits(treeType);
     };
   
    //get random x and y coord
   xdouble = randomgenerator();
   ydouble = randomgenerator();
  
   //xdouble = randomgenerator((unsigned)time(NULL));
   //ydouble = randomgenerator((unsigned)time(NULL)*int(xcoord));
   // adapt numbers to grid
   xcoord = int(xdouble * cellx) ;
   ycoord = int(ydouble * celly) ;  
   
   if (xcoord < 0 || xcoord > 99 || ycoord < 0 || ycoord > 99)
     {
       cout << "There is a error in the grid!";
      
     };

   //set x,y position of the tree
   tree[treecounter][0] = xcoord;
   tree[treecounter][1] = ycoord;
}

// -----------------------------------------------------------------------------
// 
void createTrees(int treeType)
{
  // local vars
  int xdouble;
  int ydouble;
  int xcoord;
  int ycoord;
  int maxproduction;
  //int i;

  

  //setTraits();

  if (treeType = 0) 
    {     
    firstRes = treecounter;
    maxproduction = resN;
    }
  else
    {
    firstInv = treecounter;
    maxproduction = invN;
    };

  //get random x and y coord
  xdouble = randomgenerator();
  ydouble = randomgenerator();
  // adapt numbers to grid
  xcoord = int(xdouble * cellx) ;
  ycoord = int(ydouble * celly) ;  

  // set x and y coordinates of the tree in the tree file
  for (treecounter; treecounter < maxproduction; treecounter++)
  {
  tree[treecounter][0] = xcoord;
  tree[treecounter][1] = ycoord;
  // set the number tree in the cell file
  cell[xcoord][ycoord] = cell[xcoord][ycoord]+1;
  initSeed(treeType);
  setTraits(treeType);
  
  };

}


// -----------------------------------------------------------------------------
// 
void treesDie(void)
{
  int i;
  double rnd;
  rnd = randomgenerator();
 
  //cout << " i am here at trees die " << endl;
  for (i=0; i < treecounter; i++)
  {
    if (tree[i][3] < tree[i][15] * 90)
    {
      if (rnd < tree[i][8] * exp ( tree[i][10] * -1 * tree[i][7]))
      {
        if (tree[i][6] = 0)
        {
          ddRes = ddRes + 1;
        };
        if (tree[i][6] = 1)
        {
          ddInv = ddInv + 1;
        };
      };
    }; 

  /*if (tree[i][6] = 0)
  {
    if (rnd < resIntrinsicM)
    //left out
  };
  */
  if (tree[i][6] = 0)
  {
    lsRes = lsRes + tree[i][2];
    deathRes = deathRes + tree[i][2]; 
  };

  if (tree[i][6] = 1)
  {
    lsInv = lsInv + tree[i][2];
    deathInv = deathInv + tree[i][2]; 
  };

  };
}


// -----------------------------------------------------------------------------
// 
void treesDisperse(void)
{
  int i;

  for (i=0; i < treecounter; i++)
  {
    if (tree[i][3] > tree[i][16])
    {
      //find out how it works
      // hatch trees: tree[i][14]
    };
  };
}


// -----------------------------------------------------------------------------
void treesGrowheight(void)
{
  int i;
  double increment;

  for (i=0; i < treecounter; i++)
  {
    // set increment
    increment = tree[i][8] * log (tree[i][18] / tree[i][3]) * tree[i][3] * tree[i][11];
    // set height
    tree[i][2] = tree[i][2] + int(increment*100);
  }
}


// -----------------------------------------------------------------------------
void treesDefineResource(void)
{
  int i;
  double increment;

  for (i=0; i < treecounter; i++)
  {
    tree[i][17] = tree[i][3] > hcompRatio * tree[i][17] * compRed;

    tree[i][11] = int(exp ( -tree[i][17] * tree[i][9]));
  }
}


// -----------------------------------------------------------------------------
void treesGrowold(void)
{
  int i;
  for (i = 0; i < treecounter; i++)
  {

    if (tree[i][3] > 1) //height
    {
    
    tree[i][2] = tree[i][2] + 1; //set the age plus
    tree[i][4] = 15; //size /100
    tree[i][5] = 2; // color 2 = blue
//cout << "i went into this loop!" << tree[i][2] << endl;
    };

    if (tree[i][3] > reproH)
    {
      if (tree[i][6] = 0) // species 0 = resident
      {
      tree[i][5] = 3; // color 3 = green
      }      
      else
      {
      tree[i][5] = 4; // color 4 = red
      };
    };
  };
  

  

}


// -----------------------------------------------------------------------------
// 
void introTwoSpecies(int treetype)
{
  // local vars

  // call functions



  //setTraits();
  createTrees(0); //res = 0
  createTrees(1); //inv = 1
  
}


// -----------------------------------------------------------------------------
// 
void disturbPatch(void)
{

}


// -----------------------------------------------------------------------------
// 
void changePatch(void)
{
  //cout << "test3";
  //return 0;
}


// -----------------------------------------------------------------------------
// output of some data
// tree storage - max number of trees is set by treemax; the number of collums should be reduced to a min setting
// 0 xcoord,     1 ycoord,   2 age,      3 height,         4 seize, 
// 5 color,      6 species,  7 Md,       8 M0,             9 Gd,         10 incrememtn, 
// 11 resource, 12 infcomp, 13 dispdist, 14 fecundity, 
// 15 maxH,     16 reproH,  17 supComp
void outputdata(void)
{
  int i;
  /*for (i = 0; i< treecounter; i++)
    {
    cout << "x: " << tree[i][0];
    cout << " y: " << tree[i][1];
    cout << " age: " << tree[i][2];
    cout << " height: " << tree[i][3];
    cout << " seize: " << tree[i][4] << endl;
    };
*/
  //cout << "Treecount is: " << treecounter << endl;
}

// -----------------------------------------------------------------------------
// set up the field
void setup(void)
{
  system("CLS");

  //ask patches

  //change patches
  changePatch();

  //return 0;
}


// -----------------------------------------------------------------------------
// run the system
void runsystem(void)
{

  if (ticks > introT -1) 
    if (ticks < introT + introP +1)
      {    
      introTwoSpecies(0);
      introTwoSpecies(1);   
      
  for (ticks = 0; ticks < ticksmax; ticks++)
  {   
      treesGrowold();
      treesDefineResource();
      treesGrowheight();
      treesDisperse();
      treesDie();
      disturbPatch();
      changePatch();
      ticks = ticks + 1;
//outputdata();
      };
  //
  };
outputdata();
  //return 0;
}

// -----------------------------------------------------------------------------
// main routine which calls all the other routines
int main(void)
{
  double rnd = 0;
  // this function calls the routine calls all the subroutines
  int i;
  for (i=0; i < ticksmax ; i ++)
    {
      runsystem();
      //randomgeneratorold(rnd);
      //cout << "rnd is: " << rnddouble << endl;
    };  
   cout << "Number of trees is: " << treecounter << endl;
  return 0;
}



// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------














