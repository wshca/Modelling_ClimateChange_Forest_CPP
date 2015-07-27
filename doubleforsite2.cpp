#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <math.h>
#include <fstream>
using namespace std;

//for MPI integration we use
#include "mpi.h"
#include <stdio.h>
#include <string>

// -----------------------------------------------------------------------------
// global variables for storage

// tree
const long int treemax = 1000000;
// tree storage - max number of trees is set by treemax; the number of collums should be reduced to a min setting
// 0 xcoord,     1 ycoord,   2 age,      3 height,         4 seize,
// 5 color,      6 species,  7 Md,       8 M0,             9 Gd,         10 incrememtn,
// 11 resource, 12 infcomp, 13 dispdist, 14 fecundity,
// 15 maxH,     16 reproH,  17 supComp
long double tree[treemax][18];
// double should be substituted by int so save memory and cpu time
//int tree[treemax][18];
// furthermore the structure should be change with adaptive memory allocation

// cell
const int cellx = 20;
const int celly = 20;
const long int cellmax = cellx * celly;
// x and y coordinates of the cell in the grid, number of trees in cell (later on more information about the cell like resources...
int cell[cellmax][2];

// patches
// 0 xcoord, 1 ycoord, value of age is stored there
int patches[cellmax][2];
// idea for change: used patchID to get patches of bigger size - maybe unnesseray

// other ones
double totresources = 0; //initial number of resoucses is 0
double returninterval = 0;
int gaps = 0;
double deathRes = 0;
double deathInv = 0;
double ddInv = 0;
double ddRes = 0;
double lsRes = 0;
double lsInv = 0;
double resN = 500;
double invN = 500;

double resVar = 0.10; //was 0.10 so times 100 to fit integer
double resMd = 0.90; //was 0.90 so times 100 to fit integer
double resM0 = 0.90; //was 0.90 so times 100 to fit integer
double resGd = 1.30; //was 1.30 so times 100 to fit integer
double resG0 = 0.25; //was 0.25 so times 100 to fit integer
double resF = 10;
double resIntrinsicM = 0.010; //was 0.010 so times 1000 to fit integer
double resMaxH = 2500.0;
double resReproH = 0;

double invVar = 0.10; //was 0.10
double invMd = 0.50; //was 0.50
double invM0 = 0.41; //was 0.41
double invGd = 0.99; //was 0.99
double invG0 = 0.10; //was 0.10
double invF = 5.0;
double invIntrinsicM = 0.005; //was 0.0050
double invMaxH = 2300.0;
double invReproH = 300.0;
double resIntrinsic = 0;
double reproH = 0;

double compRed = 0.20; //was 0.20
double hcompRatio = 1.00; //was 1.0

int treecounter = 0;
double firstRes = 0;
double firstInv = 0;

double introT = 50;
double introP = 100;

int ticks = 0; //0;
int ticksmax = 500; //500;

double pDisturb = 0.010; //was 0.010
double distRadius = 1.5; //was 1.5
double sddP = 0.99; //was 0.99
double alpha = 6; //was 6

double resNlocal;
double invNlocal;
int rndcounter = 1;

char storeChar[100];
//double rnddouble;

// -----------------------------------------------------------------------------
// variable changing section
// -----------------------------------------------------------------------------
int changeVariable(int i, double bufferdouble)
{

  // switch routine to set the variable to the new value
  switch(i)
  {
    case 0:
    totresources = bufferdouble; //initial number of resoucses is 0
    break;

    case 1:
    returninterval = bufferdouble;
    break;

    case 2:
    gaps = bufferdouble;
    break;

    case 3:
    deathRes = bufferdouble;
    break;

    case 4:
    deathInv = bufferdouble;
    break;

    case 5:
    ddInv = bufferdouble;
    break;

    case 6:
    ddRes = bufferdouble;
    break;

    case 7:
    lsRes = bufferdouble;
    break;

    case 8:
    lsInv = bufferdouble;
    break;

    case 9:
    resN = bufferdouble;
    break;

    case 10:
    invN = bufferdouble;
    break;

    case 11:
    resVar = bufferdouble; //was 0.10 so times 100 to fit integer
    break;

    case 12:
    resMd = bufferdouble; //was 0.90 so times 100 to fit integer
    break;

    case 13:
    resM0 = bufferdouble; //was 0.90 so times 100 to fit integer
    break;

    case 14:
    resGd = bufferdouble; //was 1.30 so times 100 to fit integer
    break;

    case 15:
    resG0 = bufferdouble; //was 0.25 so times 100 to fit integer
    break;

    case 16:
    resF = bufferdouble;
    break;

    case 17:
    resIntrinsicM = bufferdouble; //was 0.010 so times 1000 to fit integer
    break;

    case 18:
    resMaxH = bufferdouble;
    break;

    case 19:
    resReproH = bufferdouble;
    break;

    case 20:
    invVar = bufferdouble; //was 0.10
    break;

    case 21:
    invMd = bufferdouble; //was 0.50
    break;

    case 22:
    invM0 = bufferdouble; //was 0.41
    break;

    case 23:
    invGd = bufferdouble; //was 0.99
    break;

    case 24:
    invG0 = bufferdouble; //was 0.10
    break;

    case 25:
    invF = bufferdouble;
    break;

    case 26:
    invIntrinsicM = bufferdouble; //was 0.0050
    break;

    case 27:
    invMaxH = bufferdouble;
    break;

    case 28:
    invReproH = bufferdouble;
    break;

    case 29:
    resIntrinsic = bufferdouble;
    break;

    case 30:
    reproH = bufferdouble;
    break;

    case 31:
    compRed = bufferdouble; //was 0.20
    break;

    case 32:
    hcompRatio = bufferdouble; //was 1.0
    break;

    case 33:
    firstRes = bufferdouble;
    break;

    case 34:
    firstInv = bufferdouble;
    break;

    case 35:
    introT = bufferdouble;
    break;

    case 36:
    introP = bufferdouble;

    case 37:
    ticksmax = int(bufferdouble); //500
    break;

    case 38:
    pDisturb = bufferdouble; //was 0.010
    break;

    case 39:
    distRadius = bufferdouble; //was 1.5
    break;

    case 40:
    sddP = bufferdouble; //was 0.99
    break;

    case 41:
    alpha = bufferdouble; //was 6
    break;

  };
  return 0;

}

// -----------------------------------------------------------------------------
// coverting section
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// convert inputline to double function
double convertInputline(char* inputline)
{
  double inputdouble;




  return inputdouble;
}

// -----------------------------------------------------------------------------
// file handling section
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// remove old files
void fileremoveold(const char* filename)
{
  // open the file for writting - resulting in overwritting
  // the old files so we can append to a empty file later on
  FILE *outputfile = fopen(filename, "w");

  // close the file so there is no problem with the OS
  fclose(outputfile);
}


// -----------------------------------------------------------------------------
// open file and do the settings of the variables
int fileGetSettings(const char* filename)
{
  // local varibales
  int i;
  const int buffersize = 100; // buffer size
  char buffer[buffersize];    // temporary storage of string
  char *charpointer;
  char buffervariable[15];    // temporaray storage for the variable name
  double bufferdouble;        // temporary storage for the double value of the varibale

  // open the file for reading
  ifstream inputfile(filename);

  // error catching for the case the file does not exist
  if (inputfile == NULL)
    {
      cout << "Input file was not specified!" << endl;
      return 1; // return a error code so the calling routinge knows there is a error
    };

  // read the file and store it in the variables

  // read the file into a char array for further processing
  i = 0;
  while (inputfile.getline(buffer,buffersize)) // let the loop run through the file until it reaches EOF
    {

    // recognizing the variables and storing

    // ingnore commentaries starting with "/"
    //char keys[] = "/";
    if (strcspn(buffer, "/") < 1)
      {
      // commentar is removed
      }
      else
      {
      // commentar free area

      // convert the string to double and store it
      bufferdouble = atof(buffer);

      // call function to set the variables
      changeVariable(i,bufferdouble);
      i++;

      }; //end of the if function to ignore commentaries

    }; //end of the while llop going through the lines

  return 0; // default return value so we know there was no error in the file
}



// -----------------------------------------------------------------------------
// function section
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// random number generator basic function
int randomgeneratorold()
{
  // local variables
  double rnd;
  int i;

  // failsafe for the first call where it would be 0
  if (rndcounter < 1)
  {
    i = 1;
  }
  else
  {
    i = rndcounter;
  };

  // increment the global variable
  rndcounter = rndcounter + 1;

  // make a new seed for the random generator every run
  srand((unsigned)time(NULL)*i);

  // get a random double value and send it to the local variable rnd
  rnd= (double(rand())/RAND_MAX);


  //cout << "rand: " << rnd << endl;
  //cout << "A number between 0 and 50: " << rnd << endl;

  // hand out the random number to the calling routine
  return rnd;
}

// -----------------------------------------------------------------------------
// random number generator basic function
double randomgenerator()
{
  // local variables
  double rnd;
  int i;

  // everytime the generator is called the rndcounter variables is incremented
  // this is necessary to get differen random numbers
  if (rndcounter < 1)
  {
    i = 1;
  }
  else
  {
    i = rndcounter;
  };

  // increment the global variable rndcounter
  rndcounter = rndcounter + 1;

  //Initialize the random number generator with the seed
  srand((unsigned)time(NULL)*i);

  // Call the C routine rand() for a random number
  rnd = double(rand()/RAND_MAX);
  /*
  cout << "rand: " << rand() << endl;
  cout << "randdouble: " << double(rand()) << endl;
  cout << "randmax: " << RAND_MAX << endl;
  */
  //cout << "rand: " << rnd << endl;

  // hand out the random number to the calling routine
  return rnd;
}

// -----------------------------------------------------------------------------
// random normal distribution function copied from
// http://www.sitmo.com/doc/Calculating_the_Cumulative_Normal_Distribution
// Algorithm 26.2.17 from Abromowitz and Stegun, Handbook of Mathematical Functions
// not used!
double randomnormaldist(double x)
{
  const double b1 =  0.319381530;
  const double b2 = -0.356563782;
  const double b3 =  1.781477937;
  const double b4 = -1.821255978;
  const double b5 =  1.330274429;
  const double p  =  0.2316419;
  const double c  =  0.39894228;

  if(x >= 0.0) {
      double t = 1.0 / ( 1.0 + p * x );
      return (1.0 - c * exp( -x * x / 2.0 ) * t *
      ( t *( t * ( t * ( t * b5 + b4 ) + b3 ) + b2 ) + b1 ));
  }
  else {
      double t = 1.0 / ( 1.0 - p * x );
      return ( c * exp( -x * x / 2.0 ) * t *
      ( t *( t * ( t * ( t * b5 + b4 ) + b3 ) + b2 ) + b1 ));
    }

}

// -----------------------------------------------------------------------------
// random normal function
// be aware that too big values of x generate a buffer overflow! Use numbers smaller than 1000!!!!
double randomnormal(double variance,double mean)
//double randomnormal(double mean, double variance)
{
  double x = randomgenerator();
  const double pi = 3.14159265358979323846264338327950288;
  double sigma = 0.0;
  double rnd = 0.0;
  double exponent = 0.0;
  double meantemp = mean;
  double sigmatemp = sqrt(variance);

  double mu = 0.0;
  int changeType = 0;

  while (x < 0.0)
    {
      x = randomgenerator();
    };

  // preprocessing to get values in the range of [0,1]
  if (mean<1)
    {
    mu = mean;
    sigma = sqrt(variance);
    changeType = 1;
    //cout << "tets";
    };

  if (mean==1 || (mean>1 && mean<10))
    {
    mu = mean/10;
    sigma = sqrt(variance/10);
    changeType = 10;
    //cout << "aadddaa";
    };

  if (mean==10 || (mean>10 && mean<100))
    {
    mu = mean/100;
    sigma = sqrt(variance/100);
    changeType = 100;
    };

  if (mean==100 || (mean>100 && mean<1000))
    {
    mu = mean/1000;
    sigma = sqrt(variance/1000);
    changeType = 1000;
    //cout << "test!" << endl;
    };

  if (mean==1000 || (mean>1000 && mean<10000))
    {
    mu = mean/10000;
    sigma = sqrt(variance/10000);
    changeType = 10000;
    };

  if (mean==10000 || (mean>10000 ))//&& mean<100000))
    {
    mu = mean/100000;
    sigma = sqrt(variance/100000);
    changeType = 100000;
    };

  //cout << "sigma is: " << sigma;
  //cout << " mu is: " << mu;
  //cout << " mean was: " << mean;
  // MATH stuff
  // p(x) = 1/ (sigma * sqrt(2pi)) * exp ( - (x-mean)^2 / 2*sigma^2)

  // We write a function so we simulate the random-normal function in netlogo
  rnd = 1 / (sigma * sqrt(2 * pi));
  exponent =  -1 * pow( (x - mu),2) / (2 * pow(sigma,2));
  rnd = rnd * exp(exponent);

  //cout << " rnd is: " << rnd;
  //cout << " output is: " << rnd*changeType << endl;

  //cout << "mean original: " << meantemp << endl;
  //cout << "mean manipula: " << mu * changeType << endl;
  //cout << "sigma original: " << sigmatemp << endl;
  //cout << "sigma manipula: " << sigma << endl;
  //cout << "returnvalue is: " << rnd * changeType << endl;

  rnd = rnd * changeType;



  return rnd;

}



// -----------------------------------------------------------------------------
//
void computeDistance(void)
{
  int i = 0;
  double rnd = 0;

  // Original function out of nlogo
  // ifelse random-float 1 < sddP
  // [set dispdist ln( random-float 1) * (- alpha)]
  // [set dispdist ln( random-float 1) * (- [height] of myself * 2.5 ^ 1.5)]




  for (i = 0; i < treecounter; i++)
  {
    // removing the dead trees out of the list
    //if (tree[i][0] < 0 || tree[i][1] < 0)
    //  {
	// tree i is removed from this loop
    //  }
    //else
    //  {

	// call the random generator and store it so we can use it later on
	rnd = randomgenerator();
	//ifelese function
        if (rnd < sddP)
        {
        // set dispdist ln( random-float 1) * (- alpha)
        tree[i][13] = log(rnd) * (-alpha);
        }
        else
        {
        // set dispdist ln( random-float 1) * (- [height] of myself * 2.5 ^ 1.5)
        // note that 'pow' is the C function for power to
        tree[i][13] = log(rnd) * (- tree[i][3] * pow(2.5,1.5));
        };

     //};
 };


}

// -----------------------------------------------------------------------------
//
void setTraits(double treeType)
{
  // local varibales needed so we can copy the necessary global one into it and have only to check the complex functions once
  // and not for every treetype we have
  double Md;
  double M0;
  double Gd;
  double F;
  double IntrinsicM;
  double MaxH;
  double ReproH;
  double Var;


  // copy the global variable to the local one
  if (treeType < 0.5)
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


  /* Old function from nlogo
    ifelse species = "resident"
    [set Md random-normal resMd (resMd * resVar)
    set M0 random-normal resM0 (resM0 * resVar)
    set Gd random-normal resGd (resGd * resVar)
    set G0 random-normal resG0 (resG0 * resVar)
    set fecundity int random-normal resF (resF * resVar)
    set intrinsicM random-normal resIntrinsicM (resIntrinsicM * resVar)
    set maxH random-normal resMaxH (resMaxH * resVar)
    set reproH random-normal resReproH (resReproH * resVar)
    ]
    [set Md random-normal invMd (invMd * invVar)
    set M0 random-normal invM0 (invM0 * invVar)
    set Gd random-normal invGd (invGd * invVar)
    set G0 random-normal invG0 (invG0 * invVar)
    set fecundity int random-normal invF (invF * invVar)
    set intrinsicM random-normal invIntrinsicM (invIntrinsicM * invVar)
    set maxH random-normal invMaxH (invMaxH * invVar)
    set reproH random-normal invReproH (invReproH * invVar)
    ]
  */

  /* show random-normal 10.1 5.2
   ;; prints a normally distributed random floating point
   ;; number with a mean of 10.1 and a standard deviation
   ;; of 5.2


  // set the random levels
  cout << "Var is: " << Var << endl;
  tree[treecounter][7]= randomnormal(Md,Md * Var);
  cout << "Md: "<< Md << " " <<tree[treecounter][7]<< endl;
  tree[treecounter][8]= randomnormal(M0,M0 * Var);
  cout << "M0: "<< M0 << " " <<tree[treecounter][8]<< endl;
  tree[treecounter][9]= randomnormal(Gd,Gd * Var);
  cout << "Gd: "<< Gd << " " <<tree[treecounter][9]<< endl;
  tree[treecounter][14]= randomnormal(F,F * Var);
  cout << "F: "<< F << " " <<tree[treecounter][14]<< endl;
  tree[treecounter][10]= randomnormal(IntrinsicM,IntrinsicM * Var);
  cout << "IntrinsicM: "<< IntrinsicM << " " <<tree[treecounter][10]<< endl;
  tree[treecounter][15]= randomnormal(MaxH,MaxH * Var);
  cout << "MaxH: "<< MaxH << " " <<tree[treecounter][15]<< endl;
  tree[treecounter][16]= randomnormal(ReproH,ReproH * Var);
  cout << "ReproH: "<< ReproH << " " <<tree[treecounter][16]<< endl;
*/


  //cout << "Var is: " << Var << endl;
  tree[treecounter][7]= Md;
  //cout << "Md: "<< Md << " " <<tree[treecounter][7]<< endl;
  tree[treecounter][8]= M0;
  //cout << "M0: "<< M0 << " " <<tree[treecounter][8]<< endl;
  tree[treecounter][9]= Gd;
  //cout << "Gd: "<< Gd << " " <<tree[treecounter][9]<< endl;
  tree[treecounter][14]= F;
  //cout << "F: "<< F << " " <<tree[treecounter][14]<< endl;
  tree[treecounter][10]= IntrinsicM;
  //cout << "IntrinsicM: "<< IntrinsicM << " " <<tree[treecounter][10]<< endl;
  tree[treecounter][15]= MaxH;
  //cout << "MaxH: "<< MaxH << " " <<tree[treecounter][15]<< endl;
  tree[treecounter][16]= ReproH;
  //cout << "ReproH: "<< ReproH << " " <<tree[treecounter][16]<< endl;
/*
if (treeType < 0.5)
  {
  resMd = Md;
  resM0 = M0;
  resGd = Gd;
  resF = F;
  resIntrinsicM = IntrinsicM;
  resMaxH = MaxH;
  resReproH = ReproH;
  resVar = Var;
  }
  else
  {
  invMd = Md;
  invM0 = M0;
  invGd = Gd;
  invF = F;
  invIntrinsicM = IntrinsicM;
  invMaxH = MaxH;
  invReproH = ReproH;
  invVar = Var;
  };
*/
}


// -----------------------------------------------------------------------------
//
void initSeedLocally(double treeType, double xcoordadult, double ycoordadult, double dispdistadult)
{
  double G0;
  double intrinsicM;
  double xdouble;
  double ydouble;
  double xcoord;
  double ycoord;
  double rnd;

/*
    if (treeType < 0.5) // 0 is resident and 1 is invader so <0.5 means resident (because of double)
    {
      G0 = resG0;
      intrinsicM = resIntrinsicM;
    }
    else
    {
      G0 = invG0;
      intrinsicM = invIntrinsicM;
    };

  /* Old nlogo function block
  set age 0
  set height 2
  set size 0.2
  set color blue
  set-traits
  while [Md <= 0 or (M0 <= 0) or (Gd <= 0) or (G0 <= 0) or (intrinsicM <= 0)]
    [set-traits]


  // get random x and y coord and store it in the local variables
  rnd = randomgenerator();
  if (rnd > 0.0)
    {
    xdouble = rnd;
    };
  rnd = randomgenerator();
  if (rnd > 0.0)
    {
    ydouble = rnd;
    };

  if (dispdistadult > 0.99)
    {
    dispdistadult = dispdistadult / 2;
    };
*/
  xdouble = randomgenerator();
  ydouble = randomgenerator();
  xcoord = xcoordadult * xdouble * dispdistadult;
  ycoord = ycoordadult * ydouble * dispdistadult;

  // this if clause is necessary to prevent accessing data outside of the field resulting in program crash
  if (xcoord < 0 || ycoord < 0 || xcoord > cellx || ycoord > celly)
    {
    xcoord = -1.0;
    ycoord = -1.0;
    }
    else
    {
    tree[treecounter][0] = xcoord;
    tree[treecounter][1] = ycoord;
    // increment the number tree in the cell file
    cell[int(xcoord)][int(ycoord)] = cell[int(xcoord)][int(ycoord)] + 1;

    // some basic settings of the seedlings
    tree[treecounter][2]=0.0; //age
    tree[treecounter][3]=200.0; //height
    tree[treecounter][4]=20.0; //size in 1/100 so we can use integer
    tree[treecounter][5]=1.0; //color blue = 1
    tree[treecounter][6]=treeType;

    // call a subroutine for the more complex settings
    setTraits(treeType);
    };
  // while (tree[treecounter][7] < 0.00001 || tree[treecounter][8] < 0.00001 || tree[treecounter][9] < 0.00001 || G0 < 0.00001 || intrinsicM < 0.00001)
  //setTraits(treeType);


}


// -----------------------------------------------------------------------------
//
void initSeed(double treeType)
{
   // local vars
   double xdouble = 0;
   double ydouble = 0;
   double xcoord = 0;
   double ycoord = 0;
   double intrinsicM = 0;

   if (treeType < 0.5)
     {
       intrinsicM= invIntrinsicM;
     }
   else
     {
       intrinsicM = resIntrinsicM;
     };

   tree[treecounter][2]=0.0; //age
   tree[treecounter][3]=200.0; //height
   tree[treecounter][4]=50.0; //size in  so we can use integer
   tree[treecounter][5]=0.0; //color white = 0.
   tree[treecounter][6]=treeType; //set initial seeds to resident = 0 species or 1 for invader
   setTraits(treeType);

/*
   // call setTraits if Md <=0, M0 <=0, Gd<=0, or intrinsicM <=0
   while (tree[treecounter][7] + 0.01 < 0 || tree[treecounter][8] + 0.01 < 0|| tree[treecounter][9] + 0.01 < 0 || intrinsicM + 0.01 < 0)
     {
     setTraits(treeType);
     };
*/

    //get random x and y coord
   xdouble = randomgenerator();
   ydouble = randomgenerator();

   // adapt numbers to grid
   xcoord = xdouble * cellx ;
   ycoord = ydouble * celly ;

   // error handling - there should never be a tree outside of the cell (x,y) possibilies except for the dead one which are on (-1,-1)
   if (int(xcoord) < 0.0 || int(xcoord) > cellx -1 || int(ycoord) < 0.0 || int(ycoord) > celly - 1)
     {
       cout << "There is an error in the grid!";
     };

   //set x,y position of the tree
   tree[treecounter][0] = xcoord;
   tree[treecounter][1] = ycoord;
   cell[int(xcoord)][int(ycoord)] = cell[int(xcoord)][int(ycoord)] + 1;
}

// -----------------------------------------------------------------------------
//
void createTrees(double treeType)
{
  // local vars
  double xdouble;
  double ydouble;
  double xcoord;
  double ycoord;
  double maxproduction;

  setTraits(treeType);

  if (treeType < 0.5)
    {
    firstRes = treecounter;
    maxproduction = resN;
    }
  else
    {
    firstInv = treecounter;
    maxproduction = invN;
    };

  // get random x and y coord and store it in the local variables
  xdouble = randomgenerator();
  ydouble = randomgenerator();
  // adapt numbers to grid so we get proper coordinates
  xcoord = xdouble * cellx ;
  ycoord = ydouble * celly ;

  // set x and y coordinates of the tree in the tree file
  // adding new trees to the set of trees
  //for (treecounter; treecounter < maxproduction; treecounter++)
  //{
  tree[treecounter][0] = xcoord;
  tree[treecounter][1] = ycoord;
  tree[treecounter][2] = 0.0;
  tree[treecounter][3] = 0.0;
  tree[treecounter][4] = 0.0;
  // increment the number tree in the cell file
  cell[int(xcoord)][int(ycoord)] = cell[int(xcoord)][int(ycoord)] + 1;

  //routine which creates the trees
  initSeed(treeType);




  // routine which sets more settings of the trees
  // -> there could be a error maybe we need a loop around this function..... double check!
  //
  //
  setTraits(treeType);
  //
  //
  //




  //};

}


// -----------------------------------------------------------------------------
//
void treesDie(void)
{
  // local variables
  int i;

  /* Original Function out of nlogo
   if height < maxH * 0.9 and (random-float 1 < M0 * exp(increment * (- Md)))
     [ifelse species = "resident"
       [set ddRes ddRes + 1]            --if case i.e. if resident count the residents.
       [set ddInv ddInv + age]          --else case and then die anyway!
     die
     ]
   if (random-float 1 < intrinsicM) ;0.3 / (1 + exp(4 - intrinsicM * age)))
     [
     ifelse species = "resident"
     [set lsRes lsRes + age
     set deathsRes deathsRes + 1]
     [set lsInv lsInv + age
     set deathsInv deathsInv + 1]
     die]
     ;if (age > 10 and (height < 100)) [die]
  */

  // just info: random-float 1 means that there should be a random number generated which is smaller than 1
  // this is done by the randomgenerator() which gives double values between 0 and 1


  //go through all trees - excluding the already dead ones
  for (i=0; i < treecounter; i++)
  {
    // removing the dead trees out of the list
    if (tree[i][0] < 0.0 || tree[i][1] < 0.0)
      {
	  // tree i is removed from this loop
      }
    else
      {
	  if (tree[i][3] < tree[i][15] * 0.90)
        {
        // if height < maxH * 0.9 and (random-float 1 < M0 * exp(increment * (- Md)))
        if (randomgenerator() < tree[i][8] * exp ( tree[i][10] * -1 * tree[i][7]))
          {
          // removing the dead trees out of the list
          if (cell[int(tree[i][0])][int(tree[i][1])] < 1)
            {
	        // tree i is removed from this loop
            }
            else
              {
	          // set the deathtoll selected by species up by onecout
              if (tree[i][6] < 0.5) //species identification
                {
                ddRes = ddRes + 1;
                };
              if (tree[i][6] > 0.5) //species identification
                {
                ddInv = ddInv + tree[i][2];
                };
	          // tree is killed by
	          // reduce the number of trees in the cell is is located
	          cell[int(tree[i][0])][int(tree[i][1])] = cell[int(tree[i][0])][int(tree[i][1])] - 1;
	          // remove the tree from the list by setting its (x,y) coordinates to (-1,-1)
	          tree[i][0] = -1;
	          tree[i][1] = -1;
              };
          };


      //if (random-float 1 < intrinsicM) ;0.3 / (1 + exp(4 - intrinsicM * age)))
      // removing the dead trees out of the list
      if (cell[int(tree[i][0])][int(tree[i][1])] < 1)
        {
	    // tree i is removed from this loop
        }
      else
        {
        if (tree[i][6] < 0.5) //means =0 - resident
          {
          if ( randomgenerator() < resIntrinsicM)
            {
	        // tree is killed by
	        // reduce the number of trees in the cell is is located
	        cell[int(tree[i][0])][int(tree[i][1])] = cell[int(tree[i][0])][int(tree[i][1])] - 1;
	        // remove the tree from the list by setting its (x,y) coordinates to (-1,-1)
	        tree[i][0] = -1;
	        tree[i][1] = -1;
       	    // count the death toll
	        lsRes = lsRes + tree[i][2];
            deathRes = deathRes + tree[i][2];
            };
          };
        };


      // removing the dead trees out of the list
      if (cell[int(tree[i][0])][int(tree[i][1])] < 1)
        {
        // tree i is removed from this loop
        }
      else
        {
        if (tree[i][6] > 0.5) //means =1 - invader
          {
          if ( randomgenerator() < invIntrinsicM)
	        {
	        // tree is killed by
	        // reduce the number of trees in the cell is is located
	        cell[int(tree[i][0])][int(tree[i][1])] = cell[int(tree[i][0])][int(tree[i][1])] - 1;
	        // remove the tree from the list by setting its (x,y) coordinates to (-1,-1)
	        tree[i][0] = -1;
	        tree[i][1] = -1;
            // count the death toll
	        lsInv = lsInv + tree[i][2];
            deathInv = deathInv + tree[i][2];
            };
          };
        };
      };
    };
  };
}


// -----------------------------------------------------------------------------
//
void treesDisperse(void)
{
  /* original nlogo function
  ask trees
  [if height >= reproH [
    hatch-trees fecundity
    [initSeedLocally
     compute-distance
     rt random-float 360 ; turn right
     fd dispdist; go fd
    ]
  ]]
  */


  // local variables
  int i;
  int j;

  // initialize so there can not be a memory miss
  computeDistance();

  for (i = 0; i < treecounter; i++)
  {
    // removing the dead trees out of the list
    if (tree[i][0] < 0 || tree[i][1] < 0)
      {
	// tree i is removed from this loop
      }
    else
      {
      // if treeheight is > reproH
      if (tree[i][3] > tree[i][16])
       {
       // hatch the trees with
       // fecundity is tree[i][14] meaning this is the number of new trees
       //cout << int(tree[i][14]) << endl;
       for (j = 0; j < int(tree[i][14]); j++)
        {
        // send the species, x,y coord, and disdist of the seeding tree
        computeDistance();
        initSeedLocally(tree[i][6],tree[i][0],tree[i][1],tree[i][13]);
        treecounter = treecounter + 1;
        //not sure about this setting!
        //cout << "I in loop" << j << endl;
        //rt = randomgenerator();
        //fd = dispdist;
        };
      };
    };
  };

}


// -----------------------------------------------------------------------------
void treesGrowheight(void)
{
  int i;
  double G0;
  double increment;

  for (i = 0; i < treecounter; i++)
  {
    // removing the dead trees out of the list
    if (tree[i][0] < 0 || tree[i][1] < 0)
      {
	  // tree i is removed from this loop
      }
    else
      {
      // set G0 for the species right
      if (tree[i][6] < 0.5) //we should never use = with double values!
        {
	    G0 = resG0;
        }
        else
        {
	    G0 = invG0;
        };

      // set increment

      // log is ln in C++ math lib

      // function from nlogo
      // [set increment ( G0 * ln (maxH / height) * height * resource )
      //  set height height + increment]
      increment = G0 * log (tree[i][15] / tree[i][3]) * tree[i][3] * tree[i][11];
      // set height
      //cout << "maxH is: " << tree[i][15] << endl;
      //cout << "height is: " << tree[i][3] << endl;
      //cout << "resource is: " << tree[i][11] << endl;
      //cout << "increment " << increment << endl;
      tree[i][3] = tree[i][3] + increment;
      };
  };
}


// -----------------------------------------------------------------------------
void treesDefineResource(void)
{
  int i = 0;
  int j = 0;
  double increment = 0.0;
  double countTreesHere = 0.0;
  double exponent = 0.0;
  double euler = 2.71828182845904523536028747135266249775724709369995;


  //    [set supComp (count trees-here with [height >= hcompRatio * ([height] of myself)]) * compRed
  //  set resource exp( (- supComp) * Gd) ]

  // we want to go through all existing trees so we have to adapt the list by removing those trees with (x,y) = (-1,-1)
  for (i = 0; i < treecounter; i++)
  {
    // removing the dead trees out of the list
    if (tree[i][0] < 0 || tree[i][1] < 0)
      {
	  // tree i is removed from this loop
      }
    else
      {
      countTreesHere = 0.0;
      // this loop is for the 'count trees-here' part
      for (j = 0; j < treecounter ; j ++)
        {
        // removing the dead trees out of the list
        if (tree[j][0] < 0 || tree[j][1] < 0)
          {
	      // tree is removed from this loop
          }
          else
          {
          if (tree[j][3] > hcompRatio * tree[i][3]) //ask for the specific height of the tree
	        {
	        // increase the number of trees by one if it fits the criteria
	        countTreesHere = countTreesHere + 1.0;
	        // adapt this sum by the compRed

	        //cout << countTreesHere << endl;
	        //cout << compRed << endl;
	        };
          //countTreesHere = countTreesHere + 1.0;
          };
        };
        tree[i][17] = countTreesHere * compRed;
        // tree[i][17] = countTreesHere > hcompRatio * tree[i][17] * compRed;
        exponent =  tree[i][17] * tree[i][9];
        tree[i][11] = pow(euler, -exponent);
        //cout << "17 is: " << tree[i][17] << endl;
        //cout << "9  is: " << tree[i][9] << endl;
        //cout << "resource is " << tree[i][11] << endl;
        //cout << "countTreesHere is: " << countTreesHere << endl;
        //cout << "exponent is: " << exponent << endl;
      };
    };


}


// -----------------------------------------------------------------------------
void treesGrowold(void)
{
  int i;
  for (i = 0; i < treecounter; i++)
  {
    // removing the dead trees out of the list
    if (tree[i][0] < 0 || tree[i][1] < 0)
      {
	  // tree i is removed from this loop
      }
    else
      {
      if (tree[i][3] > 1) //height
        {
        tree[i][2] = tree[i][2] + 1; //set the age plus
        tree[i][4] = 15; //size
        tree[i][5] = 2; // color 2 = blue
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
  };
}


// -----------------------------------------------------------------------------
//
void introTwoSpecies(double treetype)
{
  // local vars
  int i;
  int numberOfTrees;

  if (treetype < 0.5) //we should never use = with double values!
    {
	numberOfTrees = resN + treecounter;
    }
    else
    {
	numberOfTrees = invN + treecounter;
    };

  i = treecounter;
  for (i=0; i < numberOfTrees; i++)
    {
    treecounter = treecounter +1;
    createTrees(treetype);
    };
}


// -----------------------------------------------------------------------------
//
void disturbPatch(void)
{
  /* Original function from nlogo
  ask patches
  [ifelse random-float 1 < pDisturb ;k / (1 + exp(a - 0.01 * patchAge) ) ; sigmoid function see Li et al. 1997
        [ask (patches in-radius (random distRadius))
           [ask trees-here [die]
            set returninterval (returninterval + patchAge)
            set gaps gaps + 1
            set patchAge 0]]
         [set patchAge patchAge + 1]]
  */

  // Local variables
  int i;
  int j;
  int k;
  double rnd;

  // loop to go through the patches

  for (i = 0; i < cellx; i++)
  {
    for (j = 0; j < celly; j++)
    {

    rnd = randomgenerator();

    if (rnd < pDisturb)
      {
      //[ask (patches in-radius (random distRadius))
      //         [ask trees-here [die]                            = all trees here have to be killed
      //          set returninterval (returninterval + patchAge)
      //          set gaps gaps + 1
      //          set patchAge 0]]

      //kill every tree in this cell
      for (k = 0; k < treecounter; k++)
        {
        // remove the already dead trees from the loop
        if (tree[k][0] < 0.0 || tree[k][1] < 0.0)
          {
          // tree i is removed from this loop
          }
        else
          {
          // this complexity is necessary due to the double useage in the matrix
          if (tree[k][0] < (double(i)*1.1) && tree[k][0] > (double(i)*0.9))
            {
            if (tree[k][1] < (double(j)*1.1) && tree[k][1] > (double(j)*0.9))
              {
              // set the tree as dead by
              tree[k][0] = -1;
              tree[k][1] = -1;
              cell[i][j] = 0;
              returninterval = returninterval + 1; //?? don't know what this is for
              //if (patches[i][j] == 0)
              //  {
                gaps = gaps + 1;
              //  };
              // increment the patch age
              patches[i][j] = patches[i][j] + 1;// was patchAge = 0
              };
            };
          };
        };
      }
      else
      {
      //[set patchAge patchAge + 1]
      };

    };
  };

}


// -----------------------------------------------------------------------------
//
void changePatch(void)
{
  /* Originial function from nlogo
  ask patches
  [
    ifelse pDisturb > 0 [set pcolor (scale-color black patchAge (1 / pDisturb) 0)][set pcolor (scale-color black patchAge 100 0)]
        ;ifelse count trees-here with [age >= 15] >= 5 [set pcolor green] [set pcolor black]
    ]
  */
  if (pDisturb > 0)
    {
    //[set pcolor (scale-color black patchAge (1 / pDisturb) 0)]
    //patchAge = 1 / pDisturb;
    }
    else
    {
    //[set pcolor (scale-color black patchAge 100 0)]
    //patchAge = 100;
    };
}


// -----------------------------------------------------------------------------
// output of some data
// tree storage - max number of trees is set by treemax; the number of collums should be reduced to a min setting
// 0 xcoord,     1 ycoord,   2 age,      3 height,         4 seize,
// 5 color,      6 species,  7 Md,       8 M0,             9 Gd,         10 incrememtn,
// 11 resource, 12 infcomp, 13 dispdist, 14 fecundity,
// 15 maxH,     16 reproH,  17 supComp
void outputdata(int ticks,int k,char* fileName)
{
  int i;
  int j;
  long int numberOfTrees = 0;
  char store[25] = "";

  for (i = 0; i< 0; i++)
    {
    cout << "x: " << tree[i][0];
    cout << " y: " << tree[i][1];
    cout << " age: " << tree[i][2];
    cout << " height: " << tree[i][3];
    cout << " maxH: " << tree[i][15];
    cout << " seize: " << tree[i][4] << endl;
    };

  // output for the number of trees
  // fist we have to count all the trees in the cells - done in a prior function when we make trees
  // then we go throught all the cells we have and count the values in there
  if (k==0 || k==2)
  {
  for (i = 0 ; i < cellx ; i ++)
    {
      for (j = 0 ; j < celly ; j ++)
	    {
	    numberOfTrees = numberOfTrees + cell[i][j];
	    if (k==2)
	      {
          // open file for append the dateset to the list
          FILE *outputfile = fopen(fileName, "a"); // "a" stands for append

          // write out the data of this intervall/tick
          fputs("Cell: ",outputfile);
          fprintf(outputfile,"%d,",i);
          fprintf(outputfile,"%d ",j);
          fputs("has: ",outputfile);
          fprintf(outputfile, "%d ", cell[i][j]);
          fputs("trees \n",outputfile);

          // close file so we can append later on
          fclose(outputfile);
	      //cout << "Cell " << i << "," << j << " has " << cell[i][j] << " trees." << endl;
	      };
        };
    };

  // open file for append the dateset to the list
  FILE *outputfile = fopen(fileName, "a"); // "a" stands for append
/*
  numberOfTrees = 0;
  for (i = 0 ; i < treecounter ; i++)
    {
    if (tree[i][0] > -0.5 && tree[i][1] > -0.5)
      {
      //cout << "treeheight is" << tree[i][3] << endl;
      if (tree[i][3] > 100.0)
        {
        numberOfTrees = numberOfTrees + 1;
        };
      };
    };
*/

  // write out the data of this intervall/tick
  fputs("The number of trees is: ",outputfile);
  fprintf(outputfile,"%ld ",numberOfTrees);
  fputs("at the year: ",outputfile);
  fprintf(outputfile, "%d \n", ticks);

  // close file so we can append later on
  fclose(outputfile);
  };


  // send out the patches
  if (k==1)
    {
    cout << "number of gaps is: " << gaps << endl;
    for (i = 0; i < cellx ; i++)
      {
      for (j = 0; j < celly ; j++)
        {
        // a gap exists when it is 1 or bigger
        if (patches[i][j] > 0) // && patches[i][j] < ticksmax)
          {
          // open file for append the dateset to the list
          FILE *outputfile = fopen(fileName, "a"); // "a" stands for append

          // write out the data of this intervall/tick
          fputs("patch at x: ",outputfile);
          fprintf(outputfile,"%d ",i);
          fputs("and y: ",outputfile);
          fprintf(outputfile, "%d ", j);
          fputs("with age: ",outputfile);
          fprintf(outputfile, "%d \n", patches[i][j]);

          // close file so we can append later on
          fclose(outputfile);
          };
        };
      };
    };

}

// -----------------------------------------------------------------------------
// set up the field
void setup(void)
{
  system("CLS");

  //ask patches

  //change patches

//changePatch();

  //return 0;
}


// -----------------------------------------------------------------------------
// run the system
void runsystem(void)
{

  //names of the inputfile
  //good file: char fileInput[] = "filedatainput.txt";
  char fileInput[] = "input.txt";

  //names of the outputfiles
  char fileYear[] = "filedataoutputyear.txt";
  char fileCells[] = "filedataoutputcells.txt";
  char fileGaps[] = "filedataoutputgaps.txt";


  // get the setup of the variables from the inputfile
  fileGetSettings(fileInput);


  // initialize the system!
  // should be done to avoid problems!
  int i,j;
    for (i = 0 ; i < cellx ; i ++)
      {
      for (j = 0 ; j < celly ; j ++)
	    {
	    cell[i][j] = 0;
	    };
      };

    for (i = 0 ; i < cellx ; i ++)
      {
      for (j = 0 ; j < celly ; j ++)
	    {
	    patches[i][j] = 0;
	    };
      };

    for(i=0; i < treemax ; i++)
      {
      tree[i][0]= -1;
	  tree[i][1]= -1;
      };

    // treecounter = 1;

  // remove old files so we can append later
  fileremoveold(fileYear);
  fileremoveold(fileCells);
  fileremoveold(fileGaps);



  if (ticks > introT -1)
    if (ticks < introT + introP +1)
           {
      introTwoSpecies(0);
      introTwoSpecies(1);
      //outputdata(ticks);
      // ticksmax = 2;
           }

  for (ticks = 0; ticks < ticksmax; ticks++)
  {
      //treesDefineResource();
      //treesGrowold();
      //treesGrowheight();
      //treesDisperse();
      //treesDie();
      //disturbPatch();
      //changePatch();
      //cout << "tick :" << ticks;
      outputdata(ticks,0,fileYear); // ,0, stands for year
      //cout << "treecounter is: " << treecounter << endl;
  };

  //
  //};
  outputdata(ticks,1,fileGaps); //  1, stands for gaps
  outputdata(ticks,2,fileCells); // 2, stands for cells

  cout << "treecounter is now: " << treecounter << endl;
}

// -----------------------------------------------------------------------------
// main routine which calls all the other routines
int main(void)
{

  // run the system itself
  runsystem();



  return 0;
}



// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------














