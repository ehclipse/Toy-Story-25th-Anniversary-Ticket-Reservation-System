/**
***
***Name - Brian Tran
***NetId - BXT190005
***Date - 8/20/2020
***
**/



#ifndef SEAT_H
#define SEAT_H

class Seat{
   public:
      // default constructor
      Seat(){ row = 0; seat = ' '; ticket_type = ' ';}
      // overloaded constructor
      Seat(int r, char s, char tt){row = r; seat = s; ticket_type = tt;}

      // accessors
      int getRow(){return row;} // function to get row
      char getSeat(){return seat;} // function to get seat
      char getTT(){return ticket_type;} // function to get tickettype

      //mutators
      void setRow(int r){row = r;}  // function to set row
      void setSeat(char s){seat = s;}  // function to set seat
      void setTT(char tt){ticket_type = tt;}  // function to set tickettype

   private:
      int row; // stores row number
      char seat; // stores seat letter
      char ticket_type; // stores ticket type A,C,S and . if empty




};

#endif // SEAT_H
