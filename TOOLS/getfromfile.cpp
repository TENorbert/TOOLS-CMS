#include <iostream>
#include <string>
using namespace std;

int main()
{
   ifstream file.open("smapledata.txt",ios::in);

   string line; int nl=0; int nc = 0; double temp=0;

   vector<vector<double> > matrix;

  // cin>>temp goes through names and ignores it since we don't need it
  string temp;
  cin >> temp;
  cin >> temp;
  cin >> temp;

  float timeval = 0;
  float volt = 0;
  float amp = 0;
  float timevalTemp = 0;
  float voltTemp = 0;
  float ampTemp = 0;

  while(cin >> timevalTemp >> voltTemp >> ampTemp)
    {
      if(voltTemp > volt)
	{
	  timeval = timevalTemp;
	  volt = voltTemp;
	  amp = ampTemp;
	}
    }
  cout << "Max Time: " << timeval << endl;
  cout << "Max Volt: " << volt << endl;
  cout << "Max Amp: "<< amp << endl;
  return 0;
}
