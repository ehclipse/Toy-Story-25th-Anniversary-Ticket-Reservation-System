/**
***
***Name - Brian Tran
***NetId - BXT190005
***Date - 8/20/2020
***
**/



#ifndef NODE_H
#define NODE_H
#include "Seat.h"
#include <iostream>
template<typename T>
class Node{
   public:
      // default constructor
      Node();

      // overloaded constructor
      Node(T seat);

      // destructor
      ~Node();

      // accessors
      Node* getup(){return up;} // function to get up pointer
      Node* getdown(){return down;} // function to get down pointer
      Node* getleft(){return left;} // function to get left pointer
      Node* getright(){return right;} // function to get right pointer
      T getpayload(){return Payload;} // function to get payload of node

      // mutators
      void setup(Node* u){up = u;} // function to set up pointer
      void setdown(Node* d){down = d;} // function to set down pointer
      void setleft(Node* l){left = l;} // function to set left pointer
      void setright(Node* r){right = r; } // function to set right pointer
      void setpayload(T *Seat){Payload = *Seat;} // function to set payload

   private:
      Node *up, *down, *left, *right; // up,down,left,right pointers
      T Payload; //  payload variable


};

// default node constructor
template<typename T>
Node<T>::Node()
{
   up = nullptr;
   down = nullptr;
   left = nullptr;
   right = nullptr;
}

// overloaded node constructor
template<typename T>
Node<T>::Node(T seat)
{
   up = nullptr;
   down = nullptr;
   left = nullptr;
   right = nullptr;
   Payload = seat;
}

// destructor
template<typename T>
Node<T>::~Node()
{
   delete Payload;
}




#endif // NODE_H
