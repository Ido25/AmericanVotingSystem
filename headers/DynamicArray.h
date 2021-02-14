#pragma once
#include <iostream>
#include <fstream>
#include "configs.h"

namespace election{
  
  template<class T>
  class DynamicArray{
	private:
	  void _resize(){
		  
		  _physical_size *= 2;
		  T *temp = new T[_physical_size];
		  for(int i = 0; i < _logical_size; i++)
			  temp[i] = _arr[i];
		  
		  delete[] _arr;
		  _arr = temp;
	  }
	  T *_arr;
	  int _logical_size;
	  int _physical_size;
	
	public:
	  DynamicArray(int size = 2) : _logical_size(0), _physical_size(size), _arr(new T[size]){}
	  DynamicArray(const DynamicArray &other) : _arr(nullptr){ *this = other; }
	  DynamicArray(std::ifstream &in){
		  
		  in.read(rcastc(&_physical_size), sizeof(_physical_size));
		  in.read(rcastc(&_logical_size), sizeof(_logical_size));
		  
		  _arr = new T[_physical_size];
		  try{
			  for(int i = 0; i < _logical_size; i++)
				  _arr[i].load(in);
		  }
		  catch(std::exception &ex){
			  if(_arr != nullptr)
				  delete[] _arr;
			  throw ex;
		  }
	  }
	  ~DynamicArray(){
		  
		  if(_arr != nullptr)
			  delete[] _arr;
	  }
	  const DynamicArray &operator=(const DynamicArray &other){
		  
		  if(this != &other){
			  _logical_size = other._logical_size;
			  _physical_size = other._physical_size;
			  delete[] _arr;
			  _arr = new T[_physical_size];
			  for(int i = 0; i < _logical_size; i++)
				  _arr[i] = other._arr[i];
		  }
		  return *this;
	  }
	  const T &operator[](int i) const{ return _arr[i]; }
	  T &operator[](int i){
		
		  if(i >= _logical_size)
			  throw std::out_of_range("Out of range");
		
		  return _arr[i];
	  }
	  void push_back(const T &value){
		
		  if(_logical_size == _physical_size)
			  _resize();
		  _arr[_logical_size++] = value;
	  }
	  void pop_back(){ _logical_size--; }
	  const T &front() const{ return _arr[0]; }
	  const T &back() const{ return _arr[_logical_size - 1]; }
	  T &front(){ return _arr[0]; }
	  T &back(){ return _arr[_logical_size - 1]; }
	  int size() const{ return _logical_size; }
	  int capacity() const{ return _physical_size; }
	  bool empty() const{ return _logical_size == 0; }
	  void clear(){ _logical_size = 0; }
	  
	  template<bool is_const>
	  class base_iterator{
		public:
		  using ds_type = std::conditional_t<is_const, const DynamicArray, DynamicArray>;
		  using iterator_category = std::bidirectional_iterator_tag;
		  using different_type = std::ptrdiff_t;
		  using value_type = std::conditional_t<is_const, const T, T>;
		  using pointer = value_type *;
		  using reference = value_type &;
		
		private:
		  ds_type *_da;
		  int _i;
		
		public:
		  base_iterator(ds_type &arr, int i) : _da(&arr), _i(i){}
		  base_iterator(const base_iterator &) = default;
		  constexpr base_iterator &operator=(const base_iterator &) = default;
		  
		  // we want to allow construction of const_iterator from iterator
		  friend class base_iterator<true>;
		  
		  template<bool _is_const = is_const, class = std::enable_if_t<_is_const>>
		  base_iterator(const base_iterator<false> &other) : _da(other._da), _i(other._i){}
		  bool operator==(const base_iterator &other) const{ return (_da == other._da) && (_i == other._i); }
		  bool operator!=(const base_iterator &other) const{ return !(*this == other); }
		  reference operator*(){ return _da->_arr[_i]; }
		  pointer operator->(){ return &_da->_arr[_i]; }
		  base_iterator &operator++(){
			  
			  ++_i;
			  return *this;
		  }
		  base_iterator operator++(int){
			  
			  base_iterator temp(*this);
			  ++_i;
			  return temp;
		  }
		  base_iterator &operator--(){
			
			  --_i;
			  return *this;
		  }
		  base_iterator operator--(int){
			
			  base_iterator temp(*this);
			  --_i;
			  return temp;
		  }
	  };
	  
	  using iterator = base_iterator<false>;
	  using const_iterator = base_iterator<true>;
	  
	  iterator insert(const iterator &pos, const T &val){
		  
		  if(_logical_size == _physical_size)
			  _resize();
		  
		  iterator itrEnd = end();
		  iterator itrCurrent = itrEnd, itrPrev = --itrEnd;
		  while(itrCurrent != pos){
			  *itrCurrent = *itrPrev;
			  itrCurrent = itrPrev--;
		  }
		  
		  iterator p = pos;
		  *p = val;
		  ++_logical_size;
		  
		  return p;
	  }
	  iterator erase(const iterator &iter){
		  
		  iterator readIter = iter, writeIter = iter, endIter = this->end();
		  readIter++;
		  
		  while(writeIter != endIter){
			  *writeIter = *readIter;
			  writeIter++;
			  readIter++;
		  }
		  
		  _logical_size--;
		  iterator res = iter;
		  return res;
	  }
	  iterator erase(const iterator &first, const iterator &last){
		  
		  iterator readIter = last, writeIter = first, endIter = this->end();
		  bool in_range = true;
		  
		  while(readIter != endIter){
			  *writeIter = *readIter;
			  writeIter++;
			  readIter++;
		  }
		
		  writeIter = first;
		  while(writeIter++ != last)
			  _logical_size--;
		
		  iterator res = first;
		  return res;
	  }
	  iterator begin(){ return iterator(*this, 0); }
	  iterator end(){ return iterator(*this, _logical_size); }
	  const_iterator cbegin() const{ return const_iterator(*this, 0); }
	  const_iterator cend() const{ return const_iterator(*this, _logical_size); }
	
	  // iterator to cycle on array from end to start.
	  class reverse_iterator{
		public:
		  using ds_type = DynamicArray;
		  using iterator_category = std::bidirectional_iterator_tag;
		  using different_type = std::ptrdiff_t;
		  using value_type = T;
		  using pointer = value_type *;
		  using reference = value_type &;
	  
		protected:
		  ds_type *_da;
		  int _i;
	  
		public:
		  reverse_iterator(ds_type &arr, int i) : _da(&arr), _i(i){}
		  reverse_iterator(const reverse_iterator &) = default;
		  constexpr reverse_iterator &operator=(const reverse_iterator &) = default;
		  bool operator==(const reverse_iterator &other) const{ return (_da == other._da) && (_i == other._i); }
		  bool operator!=(const reverse_iterator &other) const{ return !(*this == other); }
		  reference operator*(){ return _da->_arr[_i]; }
		  pointer operator->(){ return &_da->_arr[_i]; }
		  reverse_iterator &operator++(){
			
			  --_i;
			  return *this;
		  }
		  reverse_iterator operator++(int){
			
			  reverse_iterator temp(*this);
			  --_i;
			  return temp;
		  }
		  reverse_iterator &operator--(){
			
			  ++_i;
			  return *this;
		  }
		  reverse_iterator operator--(int){
			
			  reverse_iterator temp(*this);
			  ++_i;
			  return temp;
		  }
	  };
	
	  // end of original array is begin of reversed array.
	  reverse_iterator rbegin(){ return reverse_iterator(*this, _logical_size - 1); }
	  reverse_iterator rend(){ return reverse_iterator(*this, -1); }
	  void print() const{
		
		  for(int i = 0; i < _logical_size; i++)
			  std::cout << _arr[i] << " ";
		  std::cout << std::endl;
	  }
	  void save(std::ofstream &out) const{
		
		  out.write(rcastcc(&_physical_size), sizeof(_physical_size));
		  out.write(rcastcc(&_logical_size), sizeof(_logical_size));
		  
		  for(int i = 0; i < _logical_size; i++)
			  _arr[i].save(out);
	  }
  };
}