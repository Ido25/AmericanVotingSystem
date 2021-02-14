#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "DynamicArray.h"

using namespace std;

namespace election{
  
  inline const string SEPERATOR(){ return "---------------------------------------"; }
  inline const string CITIZEN_SEPERATOR(){ return "----------------CITIZEN----------------"; }
  inline const string DISTRICT_SEPERATOR(){ return "---------------DISTRICT---------------"; }
  inline const string PARTY_SEPERATOR(){ return "-----------------PARTY-----------------"; }
  inline const string PRESIDENT_SEPERATOR(){ return "---------------PRESIDENT---------------"; }
  inline const string WINNER_SEPERATOR(){ return "----------------WINNER-----------------"; }
  inline const string DELEGATORS_SEPERATOR(){ return "--------------DELEGATORS---------------"; }
  inline const string TOTAL_VOTES_SEPERATOR(){ return "-------------TOTAL VOTES---------------"; }
  #define rcastcc reinterpret_cast<const char*>
  #define rcastc reinterpret_cast<char*>
  
  class Party;
  
  class District;
  
  class Citizen;
  
  class CitizenMap;
  
  struct Representor;
  
  using Byte = unsigned char;
  using PrtArray = DynamicArray<Party>;
  using DstPtrArray = DynamicArray<District *>;
  using CitizenMapArray = DynamicArray<CitizenMap>;
  using RepresentorArrray = DynamicArray<Representor>;
  
  inline void readString(ifstream &in, string &dst){
	  
	  Byte len;
	  in.read(rcastc(&len), sizeof(len));
	
	  char *buffer = new char[len + 1];
	  in.read(rcastc(buffer), len);
	  buffer[len] = '\0';
	
	  dst = buffer;
	  delete[]buffer;
  }
  // this is the algorithm template that we decided to implement as requested.
  template<class T, class CMP_FUNC>
  void _Merge(T &res, T &arr1, T &arr2, CMP_FUNC &compare){
	  
	  auto curr1 = arr1.begin(), end1 = arr1.end();
	  auto curr2 = arr2.begin(), end2 = arr2.end();
	  
	  while(curr1 != end1 && curr2 != end2)
		  if(compare(*curr1, *curr2) > 0)
			  res.push_back(*(curr1++));
		  else
			  res.push_back(*(curr2++));
	  
	  while(curr1 != end1)
		  res.push_back(*(curr1++));
	  
	  while(curr2 != end2)
		  res.push_back(*(curr2++));
  }
  template<class T, class CMP_FUNC>
  void _MergeSort_helper(T &arr, CMP_FUNC &compare){
	  
	  if(arr.size() == 0 || arr.size() == 1) return;
	  
	  int half = arr.size() / 2;
	  auto iter = arr.begin();
	  T left(half), right(arr.size() - half);
	  
	  for(int i = 0; i < half; i++, iter++)
		  left.push_back(*iter);
	  for(int i = 0; i < arr.size() - half; i++, iter++)
		  right.push_back(*iter);
	  
	  _MergeSort_helper(left, compare);
	  _MergeSort_helper(right, compare);
	  
	  arr.clear();
	  _Merge(arr, left, right, compare);
  }
  template<class T, class ITER, class CMP_FUNC>
  void _MergeSort(ITER first, ITER last, CMP_FUNC &compare){
	  
	  T arr;
	  auto curr = first;
	  
	  while(curr != last)
		  arr.push_back(*(curr++));
	  
	  _MergeSort_helper(arr, compare);
	  
	  curr = first;
	  std::for_each(arr.begin(), arr.end(), [&](auto &item)->void{ *(curr++) = item; });
  }
}