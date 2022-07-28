/**
***
***Name - Brian Tran
***Date - 8/20/2020
***
**/



#ifndef AUDITORIUM_H
#define AUDITORIUM_H
#include "Node.h"
#include "Seat.h"
#include <string>

class Auditorium{
   public:
      // Default Constructor
      Auditorium(){ first = nullptr; totalRow = 0; totalSeats  = 0;}

      // Overloaded Constructor Prototype
      Auditorium(std::string, int);

      // Destructor
      ~Auditorium();

      // Accessor
      Node<Seat*>* getfirst(){return first;} // function get first pointer
      int gettotalRow(){return totalRow;} // function get total rows
      int gettotalSeats(){return totalSeats;} // function get total seats

      // Mutator
      void setfirst(Node<Seat*>* f){first = f;} // function set first pointer

   private:
      Node<Seat*>* first; // head ptr
      int totalRow, totalSeats; // variable to store total rows and total seats

};
#endif // AUDITORIUM_H
