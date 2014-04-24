/////////////////////////////////////////////////////////////////////
//
// String.h
// Efficient String buffer class (uses array doubling to resize)
// Version :1.4
//
//  Copyright (c)2003  Ben Woodhouse  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are 
//  met:
//  1. Redistributions of source code must retain the above copyright
//  notice, this list of conditions and the following disclaimer as
//  the first lines of this file unmodified.
//  2. Redistributions in binary form must reproduce the above copyright
//  notice, this list of conditions and the following disclaimer in the
//  documentation and/or other materials provided with the distribution.
//
//  THIS SOFTWARE IS PROVIDED BY BEN WOODHOUSE ``AS IS'' AND ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//  IN NO EVENT SHALL BEN WOODHOUSE BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
//  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
//  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "ArrayList.h"
#include <assert.h>

class String 
{
public:
	String();
	String(const char * src);
	String(const String &src);
	
	virtual ~String();

	const String& operator=(const char * src); //for null-terminated strings
	const String& operator=(const String& src);

	const String& operator+=(const String& appendix); //use for efficient concatenation 
	const String& operator+=(const char * appendix);  //for null-terminated strings

	const String operator+(const String& appendix); //For convenience, not efficiency

	bool operator==(const String& sb) const;
	bool operator!=(const String& sb) const;

	void append(const char * appendix, int appLength); 
	void append(const char * appendix);
	void append(const String &src);

	void printf(const char * format, ...);
	
	void copyFrom(const char * src, int srcLength);
	void copyFrom(const String &src);
	void copyFrom(const char * src);

	//returns the index of the text string (if found) or -1
	int search(const String &text) const						{return search(0,text);} 
	int search(int startOffset, const String &text) const; 

	//repeated search (position is maintained in pos)
	bool searchRep(int &pos, const String &text) const;

	bool writeToFile(FILE *file);
	bool readFromFile(FILE *file);

	bool readFile(FILE *file);
	bool writeFile(FILE *file);

	String trimStart(int offset) const							{return subString(offset,_length);}
	String trimEnd(int offset) const							{return subString(0,_length-offset);}

	const char * cStr() const									{return &buf.getConst(0);}

	int getLength() const										{return _length;} 
	int length() const											{return _length;} 

	String subString(int start, int end) const;				

	void insert(int pos, String& src);
	void remove(int pos, int len);
	void insertChar(int pos, char c);
	void removeChar(int pos);
	void insertCharAtEnd(char c)
	{
		resize(_length+1);
		buf[_length-1]=c; //write the character
	}
	void clear()	{resize(0);}
	
	int parseInt() const;

	String toUpper() const;
	String toLower() const;


	operator const char * ()	{return cStr();}
	char &operator[](int i)		{assert(i<_length && i>=0); return buf[i];}
	void resize(int newLength)	{buf.resize(newLength+1); _length=newLength; buf[newLength]='\0';} //resizes the buffer (making room for NULL character)

protected:

	ArrayList<char> buf;
	int _length;
};


