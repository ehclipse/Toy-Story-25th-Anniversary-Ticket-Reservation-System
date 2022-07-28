/**
***
*** Name - Brian Tran
*** Date - 8/20/2020
***
**/



#include "Auditorium.h"
#include "Seat.h"
#include <fstream>
#include <string>
#include <iostream>

// Destructor
Auditorium::~Auditorium()
{
   Node<Seat*>*deleter = first; //  sets the current pointer to delete to head seat first
   Node<Seat*>*nextdeleter = nullptr; // pointer that stores address of next node to be deleted
   int k = 0; // k loop iterator to move deleter ptr down
   int x = totalRow-1; // maximum rows deleter ptr can move down
   int y = 0; // y loop iterator to move deleter ptr to last seat of row

   for(int i = 0; i < totalRow; i++) // loop through every row and delete the seat nodes within them
   {
      k = 0; // resets k loop iterator
      y = 0; // resets y loop iterator
      deleter = first; // sets deleter to first seat
      nextdeleter = nullptr;
      while(k < x) // loop that moves the starting seat pointer down a row except for when it points to last row
      {
         if(deleter->getdown() != 0) // there is a row below
         {
            deleter = deleter->getdown(); // move down a row
         }

         k++; // increment iterator
      }
      while(y < totalSeats-1) // loop to move deleter ptr to last seat in row
      {
         deleter = deleter->getright(); // move deleter ptr to the right seat
         y++;
      }

      for(int j = 0; j < totalSeats; j++) // delete every seat in row
      {
         if(deleter->getleft() != 0) // if there is a left ptr
         {
            nextdeleter = deleter->getleft(); // sets nextdeleter pointer to the seat left to seat that is being deleted
         }
         else
         {
            nextdeleter = nullptr; // set next deleter ptr to null
         }
         delete deleter; // deletes the node that deleter is pointing to

         if(nextdeleter != 0) // if nextdeleter is not null
         {
            deleter = nextdeleter; // sets the current seat to be deleted to the next seat to be deleted
         }
      }
      x--; // move maximum ro


   }


}

// Overloaded Constructor
Auditorium::Auditorium(std::string audifile, int rowTotal) // auditorium constructor that creates auditorium grid
{
   totalRow = rowTotal;
   std::fstream fs; // input filestream
   fs.open(audifile); // open theater file
   if(fs.is_open()) // checks if file is opened
   {
      //std::cout << "Auditorium File Successfully Opened" << std::endl; // prints out that open auditorium was successful
   }
   std::string line = " "; // sets line that stores line from getline() to empty string

   Node<Seat*>*headSeat =  nullptr; // head seat ptr to track first seat in auditorium
   Node<Seat*>*currSeat = nullptr; // curr seat ptr to track the location of current seat
   Node<Seat*>*nextSeat = nullptr; // next seat ptr to track the location of next seat
   Node<Seat*>*prevSeat = nullptr; // prev seat ptr to track the location of previous seat
   Node<Seat*>*topTracker = nullptr; // top seat ptr to track the location of above seat
   int m = 0; // tracks number to move down from head ptr
   for(int i = 0; i < rowTotal; i++) // row loop; loop up till the total number rows in auditorium
   {

      if(!fs.eof()) // if not end of file
      {
         getline(fs,line); // gets the next line
      }
      else // if end of file
      {
         break; // break out of loop
      }
      if(line.length() == 0) // if getline() gets an empty line
      {
         break; // break out of loop
      }

      for(long long unsigned int j = 0; j < line.length(); j++) // loop through every seat in each row
      {
         if(i == 0) // if first row
         {
            if(headSeat == nullptr) // if there is no headSeat
            {
               headSeat = new Node<Seat*>(new Seat(i+1, char(j + 65), line.at(j))); // i+1 because first row starts at 0 but is technically "row 1" of theater
               first = headSeat; // sets the head ptr to point to headseat
               topTracker = headSeat; // set top tracker to the location of the head node
               nextSeat = new Node<Seat*>(new Seat(i+1, char(j+65+1), line.at(j+1))); // creates new node for next seat
               headSeat->setright(nextSeat); // set the right pointer of head node to the next seat
               prevSeat = headSeat; // set the previous node to head node
               currSeat = nextSeat; // set current node to next node
               nextSeat = nullptr; // set next seat pointer to null

            }
            else // if there is a head seat
            {
               if(headSeat->getright() == nullptr) // if right pointer of head seat is null
               {
                  headSeat->setright(currSeat); // set right pointer of head seat to current seat
               }
               if(j != line.length() - 1) // if not the last seat
               {
                  nextSeat = new Node<Seat*>(new Seat(i+1, char(j+65+1), line.at(j+1))); // creates new node for next seat
                  currSeat->setright(nextSeat); // sets the right pointer to the nextSeat

               }
               currSeat->setleft(prevSeat); // set the left pointer to the previousSeat

               prevSeat = currSeat; // the current seat becomes the previous seat
               if(nextSeat != nullptr) // checks if there is a next seat
               {
                   currSeat = nextSeat; // set current seat pointer to next seat
               }
               nextSeat = nullptr; // next seat now points to nothing
            }

         }
         else if(i == rowTotal - 1) // if last row
         {
            if(j == 0 ) // if first seat
            {
               currSeat = new Node<Seat*>(new Seat(i+1, char(j+65), line.at(j))); // creates new node for the current seat
               nextSeat = new Node<Seat*>(new Seat(i+1, char(j+65+1), line.at(j+1))); // creates new node for next seat
               currSeat->setright(nextSeat); // set right pointer of current seat to next seat
               currSeat->setup(topTracker); // set the up pointer of current node to above node
               topTracker->setdown(currSeat);  // set the top tracker pointer to current node
               prevSeat = currSeat; // set the previous node to current node
               currSeat = nextSeat; // set current node to next node
               nextSeat = nullptr; // set next seat pointer to null

               topTracker = topTracker->getright(); // move to tracker to next node

            }
            else
            {
               if(prevSeat->getright() == nullptr) // if the beginning seat of the row does not have its right pointer pointing to a seat
               {
                  prevSeat->setright(currSeat); // sets the previous seat right pointer to current seat
               }
               if(j != line.length() - 1) // if not the last seat
               {
                  nextSeat = new Node<Seat*>(new Seat(i+1, char(j+65+1), line.at(j+1))); // creates new node for next seat
                  currSeat->setright(nextSeat); // sets the right pointer to the nextSeat

               }
               currSeat->setleft(prevSeat); // set the left pointer to the previousSeat
               prevSeat = currSeat; // set prevseat to current seat
               if(nextSeat != nullptr) // checks if there is a next seat
               {
                   currSeat = nextSeat; // set current seat to next seat
               }

               currSeat->setup(topTracker); // set the up pointer of current seat to top tracker
               topTracker->setdown(currSeat); // set top tracker down pointer to current seat
               if(topTracker != 0) // if not last seat in row j != line.length()-1 /// checks if next is not null (0)
               {
                  topTracker = topTracker->getright(); // move top tracker over by 1 seat to right
               }

               nextSeat = nullptr; // set nextseat ptr to null
            }

         }
         else // not first row and not last row
         {

            if(j == 0 ) // if first seat
            {
               currSeat = new Node<Seat*>(new Seat(i+1, char(j+65), line.at(j))); // creates new node for the current seat
               nextSeat = new Node<Seat*>(new Seat(i+1, char(j+65+1), line.at(j+1))); // creates new node for next seat
               currSeat->setright(nextSeat);
               currSeat->setup(topTracker); // set the up pointer of current node to above node
               topTracker->setdown(currSeat); // set the above node's down pointer to current node
               prevSeat = currSeat;
               currSeat = nextSeat;
               nextSeat = nullptr;
               topTracker = topTracker->getright();

            }
            else
            {
               if(prevSeat->getright() == nullptr) // if the beginning seat of the row does not have its right pointer pointing to a seat
               {
                  prevSeat->setright(currSeat); // set prevSeat right tracker to curr seat
               }
               if(j != line.length() - 1) // if not the last seat
               {
                  nextSeat = new Node<Seat*>(new Seat(i+1, char(j+65+1), line.at(j+1))); // creates new node for next seat
                  currSeat->setright(nextSeat); // sets the right pointer to the nextSeat

               }
               currSeat->setleft(prevSeat); // set the left pointer to the previousSeat
               prevSeat = currSeat; // set previous seat ptr to current seat
               if(nextSeat != nullptr) // checks if there is a next seat
               {
                   currSeat = nextSeat; // set current seat ptr to next seat
               }

               currSeat->setup(topTracker); // set up pointer of currseat to top tracker
               topTracker->setdown(currSeat); // set down ptr of top tracker to current seat
               if(j != line.length()-1) // if not last seat in row
               {
                  topTracker = topTracker->getright(); // move top tracker to right
               }

               nextSeat = nullptr; // set nextseat ptr to null
            }


         }


      }
      if(i != rowTotal-1 && i != 0) // if not last row and not first row
      {
         m = 0; // set m iterator to 0
         topTracker = first; // set toptracker to first node
         while(m < i) // loops until right row
         {
            topTracker = topTracker->getdown(); // move toptracker down 1 row

            m++; // increment m iterator by 1
         }

      }

   }
   totalSeats = (int)line.length();
   fs.close(); // closes theater file

}

