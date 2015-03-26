#pragma once

class IndexNotFoundException {
  public:
	IndexNotFoundException () 
	: message ("Index not found") { }
   
	const char * what () const { return message; }
  private:
	const char * message;
};
