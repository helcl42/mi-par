/* 
 * File:   Stack.h
 * Author: lubos
 *
 * Created on October 18, 2011, 2:40 AM
 */
#ifndef 	STACK_H
#define         STACK_H

#include <vector>
#include <iostream>
#include "Exception.h"


template <class T> class Stack;
template <class T> std::ostream &  	operator<<(std::ostream &, const Stack<T> &);
template <class T> int* 		operator/(const Stack<T> & stck, unsigned div);


template <typename T>
class Stack
{
private:
	void		     reallocStack();	

protected:
	T* 			stack;
	unsigned int 	size;
	unsigned int 	realSize;

public:
    	Stack(void);
    	Stack(const T& val);
	Stack(const Stack<T> &);
    	~Stack(void);

    	void             	push(const T &);
	T        		pop(void);

    	bool             	isEmpty(void) const;
    	T                	getTop() const;
    	unsigned int	getSize() const;
	unsigned int 	getRealSize() const;
	unsigned int	getSizeOfHalfStack() const;
	bool             	eraseTop(void);
    
    	T        		    operator[](unsigned int key) const;
    	Stack<T> &      	    operator=(const Stack<T> &);
    	friend int*     	    operator/<T>(const Stack<T> & stck, unsigned div);
    	friend std::ostream & operator<<<T> (std::ostream & os, const Stack<T> & sc);
};


template<class T>
Stack<T>::Stack(void)
{
	this->size = 0;
	this->realSize = 4;
	this->stack = new T[realSize];
}


template<class T>
Stack<T>::~Stack()
{
	delete [] stack;
}


template<class T>
unsigned int Stack<T>::getSize(void) const
{
	return size;
}


template<class T>
unsigned int Stack<T>::getRealSize(void) const 
{
	return realSize;
}


template <class T>
void Stack<T>::reallocStack()
{
//	std::cout << "realloc" << std::endl;
	realSize *= 2;
	T* tmpStack = new T[realSize];

	for(unsigned int i = 0; i < size; i++)
		tmpStack[i] = stack[i];

	delete [] stack;
	stack = tmpStack;
}


template<class T>
void Stack<T>::push(const T & value)
{
	if(size >= realSize)
		reallocStack();
	stack[size] = value;
	size++;
}


template<class T>
T Stack<T>::pop()
{
	T val = stack[--size];
	return val;
}


template<class T>
bool Stack<T>::eraseTop(void)
{
	if (isEmpty())
        	return false;

	pop();
   	return true;
}


template<class T>
bool Stack<T>::isEmpty(void) const
{
    return size == 0;
}


template <class T>
T Stack<T>::getTop() const
{
    	if(!isEmpty())
		return stack[size - 1];
    	else
	  	throw new Exception("Triing to get top of empty stack.");
}


template<class T>
std::ostream & operator<<(std::ostream & out, const Stack<T> & cs)
{
	out << "Size: " << cs.size << " realSize: " << cs.realSize << std::endl;
    	out << "[ ";
    	for(unsigned int i = 0; i < cs.size; i++)
		out << cs.stack[i] << " ";
    	out << "TOP]";
    	return out;
}


template<class T>
T Stack<T>::operator[](unsigned int key) const
{
	if(key >= size || key < 0)
		throw new Exception("Key is out of bounds.");

	return stack[key];
}



template<class T>
int* operator/(const Stack<T>& stack, unsigned int div)
{
	unsigned int ratio = stack.getSizeOfHalfStack();
	unsigned int size = stack.size;
//	std::cout << "size " << stack.size <<  " div " << div << " ratio: " << ratio << std::endl;

	if(size % 2 == 0 && !stack.isEmpty())
	{					
		int* data = new int[size - ratio];
		for(unsigned int j = ratio, i = 0; j < size; j++, i++)
			data[i] = stack[j];

		return data;
	}
	else
		throw new Exception("Stack size must be even or is empty.");
}


template<class T>
unsigned int Stack<T>::getSizeOfHalfStack() const
{
	unsigned int ratio = size / 2;
	
	if(ratio % 2 != 0)
		ratio--;

	return ratio;
}



template<class T>
Stack<T>::Stack(const Stack<T> & src)
{
	delete [] stack;
	realSize = src.size;
	size = 0;

	this->stack = new T[realSize];
    	for(unsigned int i = 0; i < realSize; i++, size++)
		stack[i] = src.stack[i];	    
}


template<class T>
Stack<T> & Stack<T>::operator=(const Stack & src)
{
    	if (&src != this)
    	{
		delete [] stack;
        	realSize = src.size;
		size = 0;
	
		this->stack = new T[realSize];
    		for(unsigned int i = 0; i < realSize; i++, size++)
			stack[i] = src.stack[i];	    
    	}
    	return (*this);
}

#endif