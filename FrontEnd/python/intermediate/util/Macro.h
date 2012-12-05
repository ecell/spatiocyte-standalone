#ifndef _MACRO_H_
#define _MACRO_H_

#include <string>
#include <vector>
#include <iostream>

//#define __DEBUG__

using namespace std;

#define SAFE_DELETE(p) if(p){delete p; p=NULL;}
#define SAFE_DELETE_ARRAY(p) if(p){delete[] p; p=NULL;}

#ifdef __DEBUG__
	#define TRACE0(string) {cout << string << endl;}
#else
	#define TRACE0(string)
#endif

#endif

