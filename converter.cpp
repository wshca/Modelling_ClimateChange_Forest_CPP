#include <iostream>
#include <iomanip>
#include <stdlib.h>
using namespace std;
void c_to_f(void);
void f_to_c(void);

int main(void)
{
 int choice;
 char again;
 do
 {
  system("CLS");

  cout<<setw(10)<<" "<< "What conversion would you like to make?\n";// menu
  cout<<setw(20)<<" "<< "1. Celsius to Fahrenhei\n\n";
 // make a choice which functions to use.
  cout<<setw(20)<<" "<< "2. Fahrenheit to Celsius\n\n";
  cin>>choice;

   switch(choice)
 // go to chosen function
   {
    case 1:
     {
      c_to_f();
      break;
     }
    case 2:
     {
      f_to_c();
      break;
   }
  default:
  {
   cout<<setw(10)<<" "<< "Enter 1 ot 2 "<< endl;
 // validate and correct input of function choice.

  }

 }

 cout<<setw(10)<<" "<< "Do you wish to do another conversion? y for yes, n for no "; // return loop on y for yes.
 cin>> again;
 }while (again=='Y'||again=='y');
 return 0;
}

void c_to_f(void)
{

 system("CLS"); //clean screen for function data.
 int temp,fahrenheit;

 cout<< "\n\n\n ";
 cout<<setw(10)<<" "<< "Enter the temperature in whole degress Celsius: \a";
 cin>>temp;

 fahrenheit=((temp*9)/5)+32;
 cout<<endl<<setw(10)<<" "<<temp<<" degrees celsius is " <<fahrenheit<<"\a\n\n\n ";
}

void f_to_c(void)
{
 system("CLS"); // clear screen for function data.
 int temp, celsius;

 cout<< "\n\n\n ";
 cout<<setw(10)<<" "<< "Enter the temperature in whole degrees fahrenheit: \a";
 cin>>temp;
 celsius=((temp-32)*5)/9;

 cout<<endl<<setw(10)<<" "<<temp<<" degrees fahrenheit is "<<celsius<<" degrees celsius \a\n\n\n";
}
