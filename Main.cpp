/**
*** Name - Brian Tran
*** NetID - BXT190005
*** Date - 11/5/2020
**/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <cmath>


#include "Auditorium.h"
#include "Seat.h"
#include "Node.h"


using namespace std;

struct Ticket{ // Ticket Struct

   int row; // variable that stores the row of the ticket
   char column; // variable that stores the column aka the seat letter of the ticket

   Ticket(int r, char c) // constructor of the ticket
   {
      row = r; // sets the row variable to row parameter
      column = c; // sets the column variable to the seat letter parameter
   }


};

struct Order{ // Order Struct

   int auditoriumNumber; // variable that stores auditorium number
   int adultTotal; // variable that stores the total amount of adult tickets
   int childTotal; // variable that stores the total amount of child tickets
   int seniorTotal; // variable that stores the total amount of senior tickets
   vector<Ticket> adultList; // vector that stores all adult Tickets of the order
   vector<Ticket> childList; // vector that stores all child Tickets of the order
   vector<Ticket> seniorList; // vector that stores all senior Tickets of the order
   int totalTickets;

   Order(int aTotal, int cTotal, int sTotal, int audiNum) // Order struct constructor
   {
      adultTotal = aTotal; // sets the adultTotal variable to the parameter of total adult tickets
      childTotal = cTotal; // sets the childTotal variable to the parameter of total child tickets
      seniorTotal = sTotal; // sets the seniorTotal variable to the parameter of total senior tickets
      auditoriumNumber = audiNum; // sets the auditoriumNumber variable to the parameter of auditorium number
   }

};

class User{ // User Class

   public:
      // constructors
      User(){username = " "; password = " ";} // default constructor
      User(string u, string p){username = u; password = p;} // overloaded constructor

      // accessors
      vector<Order>& getOrders(){return orders;} // accessor that returns order vector
      string getUsername(){return username;} // accessor that returns the username of user
      string getPassword(){return password;} // accessor that returns the password of user

      // mutators
      void setUsername(string u){username = u;} // accessor that returns the username of user
      void setPassword(string p){password = p;} // accessor that returns the password of user

   private:
      string username, password; // variable that stores username and password
      vector<Order> orders; // vector that stores the orders of the user

};


int cnt_rows(string fn) // counts the total number of rows in the file
{
   ifstream ifs; // inputfilestream
   int totalrow = 0; // variable to store the counter for total rows of auditorium
   string line = " "; // variable to store the line being read in from file

   ifs.open(fn); // open the file with filename fn

   while(getline(ifs, line)) // while can getline() from the file
   {
      totalrow++; // increment the counter by 1
   }
   ifs.close(); // close the file
   return totalrow; // return the total number of rows in the file
}

int cnt_seats(string fn) // counts the total number of seats/columns in the file
{
   ifstream ifs; // inputfilestream
   string line = " "; // variable that stores the line being read in from the file

   ifs.open(fn); // open the file with filename fn
   getline(ifs, line); // get the first line of the file
   ifs.close(); // close the file

   return (int)line.length(); // return the length of the line (which should be total number of seats of the auditorium
}

bool isNumber(string str) // function to check if the user input is a number
{
   for(long long unsigned int i = 0; i < str.length(); i++) // loops through each character in user input
   {
      if(!isdigit(str.at(i))) // if the character is not a digit, then return false
      {
         return false; // return false
      }
   }
   return true; // if all the characters are digits, return true
}

/** Reservations Functions **/

void displayAuditorium(Auditorium &theater, int totalRows, int totalSeats) // function to display theater on console
{
   Node<Seat*>*printablenode = theater.getfirst(); // pointer to head node
   int a = 0; // variable used for loop to move pointer down rows
   int b = 0; // variable used for loop to print out the seats
   int c = 0; // variable used for loop to display theater
   int ch = 65; // variable used to print out seat labels
   int rNum = 1; // variable that stores row number being displayed on auditorium

   cout << "  "; // prints out two spaces on console
   for(int i = 0; i < totalSeats; i++) // loops up till total number of seats in row
   {
      cout << char(ch); // prints out the ASCII -> character
      ch+=1; // increments the ASCII number of ch
   }
   cout << endl; // moves to next line on console

   while(c < totalRows) // loops up till the total number of rows in auditorium
   {
      if(c != 0) // if not the first row
      {
         a = 0; // sets the loop counter to 0
         printablenode = theater.getfirst(); // sets node pointer to point to head node
         while(a < c) // moves pointer down a row depending on if seats on previous row are done printing out
         {
            if(printablenode->getdown() != 0) // if there is another row !(nullptr == 0 address)
            {
                printablenode = printablenode->getdown(); // move pointer down the row
            }
            a++; // increment loop counter by 1
         }
      }

      b = 0; // resets b loop counter to 0
      cout << rNum << " "; // prints out the row number
      rNum++; // increments the row number
      while(b < totalSeats) // loop to display seats
      {
         if(printablenode->getpayload()->getTT() == 'A' || printablenode->getpayload()->getTT() == 'C' || printablenode->getpayload()->getTT() == 'S') // if seat already reserved
         {
            std::cout<< '#'; // print out a hashtag
         }
         else // else the seat is not reserved
         {
            std::cout<< '.'; // print out '.'
         }


         if(b != totalSeats-1) // if not the last seat in row
         {
             printablenode = printablenode->getright(); // move the pointer to right node
         }

         b++; // increment the b loop counter

      }

      std::cout << endl; // moves to next line
      c++; // increment c loop counter
   }
}

int bestAvailable(Auditorium& theater, int &row, int total_quantity, int totalRows, int totalSeats) // Returns best available starting seat
{
   int i = 1; // starting seat = i- 1
   int j = 0; // inner loop counter
   int k = 0; // is the iterator for the while loop that moves the pointer y down in the row loop
   char m; // stores starting seat of the section that is closest to middle
   int r = -1; // stores the row of the seat that is closest to middle
   bool n = false; // stores the boolean value that is used in the if statement to see if the seats checked are empty, true if empty, false if not empty
   double distance  = 999999999.0; // sets as high number, so that the if statement inside the outer loop will set the first distance equal to it (it is used for previousDistance calculated variable)
   double h = 0.0; // the current distance that is calculated (used to compare h with distance variable) distance variable is the previous distance calculated

   Node<Seat*>*y = theater.getfirst(); // pointer used to keep track of starting seat
   Node<Seat*>*yNext = nullptr; // pointer used to keep track and access seats for j loop
   for(int x = 0; x < totalRows; x++) // the loop that iterates each row
   {

      k = 0; // resets the iterator for while loop that move pointer y down in row loop
      while(k < x) // loop that moves the starting seat pointer down a row except for when it points to last row
      {
         if(k != totalRows- 1) // makes sure it does not go out of bounds
         {
            y = y->getdown(); // move down a row
         }

         k++; // increment iterator
      }

      for(i = 1; i < totalSeats - total_quantity + 1+1; i++) // loop through every seat in the row to see if there can be any reservations
      {
         n = true; // sets empty? checker to true
         yNext = y; // sets the position in grid of yNext to y, the starting seat
         for(j = 0; j < total_quantity; j++) // find way to save the middle seat aka the starting seat into variable maybe
         {
            if(yNext->getpayload()->getTT() != '.') // if one of the seats is not empty
            {
               n = false; // seats are not empty? false
               break; // break out of loop
            }

            yNext = yNext->getright(); // move pointer to seat to the right

         }


         if(n) // checks if all seats that are checked in inner loop are empty
         {
            h = fabs(sqrt((pow(((i + (total_quantity - 1)/2.0) - (totalSeats + 1)/2.0),2) + pow(((x + 1) - ((totalRows+1)/2.0)),2)))); // calculate distance of this starting seat (middle of selection) to center of auditorium
            if(h < distance) // h is the previous distance from before; if h is less than the previous distance calculated, set distance to new distance
            {
               m = y->getpayload()->getSeat(); // stores new starting seat letter
               r = y->getpayload()->getRow(); // stores new row number
               distance = h; // distance will save the smallest distance calculated
            }
            else if(h == distance) // if tie
            {
               if(abs(((totalRows+1)/2.0) - y->getpayload()->getRow()) < abs(((totalRows+1)/2.0) - r)) // if calculated distance is closer to the middle row
               {
                  m = y->getpayload()->getSeat(); // starting seat letter
                  r = y->getpayload()->getRow(); // row number
                  distance = h; // distance will save the smallest distance calculated
               }
               else if(abs(((totalRows+1)/2.0) - y->getpayload()->getRow()) == abs(((totalRows+1)/2.0) - r)) // both the calculated distance and the current small distance is same distance from middle, set the row to smallest number
               {
                  if(y->getpayload()->getRow() < r) // if the row of current row is less than the row of the saved distance row, then set the distance to the current distance
                  {
                     m = y->getpayload()->getSeat(); // starting seat letter
                     r = y->getpayload()->getRow(); // row number
                     distance = h; // distance will save the smallest distance calculated
                  }

               }
            }
         }
         y = y->getright(); // move one seat to right

      }
       y = theater.getfirst(); // sets to point to head of auditorium


   }
   if(distance != 999999999.0) // checks if there are seats available on the row
   {
      row = r; // sets row that was passed by reference to the new row r
      return int(m)-65; // returns the starting seat index
   }
   return -1; // if no seats are available on the row, then return -1
}

void reserveSeats(Auditorium& theater, int row, char seat, int adult_quantity, int senior_quantity, int child_quantity, int totalSeats) // reserves user chosen seats
{
   int A_count = adult_quantity; // total adult tickets
   int S_count = senior_quantity; // total senior tickets
   int C_count = child_quantity; // total child tickets
   int col = int(toupper(seat)) - 65; // converts char seat into the ASCII value of the char, then subtracts from the ASCII value of A to find the distance from column spot 0
   Node<Seat*>*r = theater.getfirst(); // pointer that currently points to head node
   int k = 0; // loop iterator to find correct row to reserve
   while(k < row-1) // positions at correct row
   {
      r = r->getdown(); // move pointer down
      k++; // increase iterator by 1
   }

   for(int i = 0; i < totalSeats; i++) // finds where starting seat is at
   {
      if(r->getpayload()->getSeat() == seat) // if seat located then break out of loop
      {
         break; // break out of loop
      }
      if(i != totalSeats - 1) // if not last seat in row
      {
         r = r->getright(); // move pointer to right
      }

   }

   for(int c = col; c < totalSeats; c++) // iterates through every seat after the starting seat
   {
      if(r->getpayload()->getTT() == '.') // if seat is empty
      {
         if(A_count > 0) // if there are still adult tickets to be reserved, reserve them
         {
           r->getpayload()->setTT('A');// reserves seat
           A_count -= 1; // decreases total adult tickets needed to be reserved by 1

         }
         else if(C_count > 0) // if there are still child tickets to be reserved, reserve them
         {
            r->getpayload()->setTT('C');; // reserves seat
            C_count -= 1; // decreases total child tickets needed to be reserved by 1
         }
         else if(S_count > 0)  // if there are still senior tickets to be reserved, reserve them
         {
            r->getpayload()->setTT('S'); // reserves seat
            S_count -= 1; // decreases total senior tickets needed to be reserved by 1
         }

      }
      if(c != totalSeats-1) // if not last seat in row
      {
         r = r->getright(); // move pointer to right node
      }

   }
}

bool checkAvailability(Auditorium& theater, int row, char seat, int total_quantity, int totalSeats) // checks if there are enough empty seats for total seats the user wants to reserve
{
   int col = int(seat) - 65; // converts char seat into the ASCII value of the char, then subtracts from the ASCII value of A to find the distance from column spot 0
   int counter = 0; // count how many available seats
   bool found = false; // if starting seat is found
   if(total_quantity > totalSeats) // if the total tickets exceeds the total seats in the row, return false
   {
      return false; // unable to reserve
   }
   Node<Seat*>*r = theater.getfirst(); // Node ptr that points to first seat node in auditorium
   Node<Seat*>*numCheck = nullptr; // pointer that will be used in a loop to check if there are enough seats after starting seat
   int k = 0; // loop iterator to position correct row of user wanting to reserve
   while(k < row-1) // positions at correct row
   {
      r = r->getdown(); // move pointer down
      k++; // increments iterator
   }
   for(int i = 0; i < totalSeats; i++) // finds where starting seat is at
   {
      if(r->getpayload()->getSeat() == seat) // if seat located then break out of loop
      {
         found = true; // starting seat found
         break; // break out of loop
      }
      if(i != totalSeats - 1) // if not last seat in row
      {
         r = r->getright(); // move pointer to right
      }

   }
   if(found) // if starting seat is found
   {
      numCheck = r; // points numCheck to location of starting seat object
      for(int d = col; d < totalSeats; d++) // makes sure there are enough seats after the starting seat for total tickets
      {
         if(numCheck->getpayload()->getTT() == '.') // if seat is empty
         {
            counter++; // increase counter
         }
         if(numCheck->getright() != 0) // if not last seat in row
         {
            numCheck = numCheck->getright(); // move pointer to right
         }

      }
      if(counter < total_quantity) // if there are not enough empty seats after starting seat
      {
         return false; // then unable to reserve requested seats
      }
      if(r->getpayload()->getTT() == '.') // checks if starting seat is reserved or not
      {
         for(int c = 0; c < total_quantity; c++) // loops through each seats after starting seat
         {
            if(r->getpayload()->getTT() != '.') // if one of them not empty
            {
               return false; // unable to reserve seats requested
            }
            if(r->getright() != 0) // if not last seat in row
            {
               r = r->getright(); // move pointer to the right
            }
         }
         return true; // able to reserve seats
      }

   }

   return false; // if not enough seats available return false

}

void writeFile(Auditorium& theater, int totalRows, int totalSeats, string outputfile) // function to write auditorium into file
{
   ofstream o; // output filestream
   o.open(outputfile); // open A1.txt output file
   if(!o.is_open()) // if unable to open file
   {
      cout << "unable to open output file" << endl; // print out unable to open file
   }
   Node<Seat*>*n = theater.getfirst(); // pointer that points to first node in auditorium
   int a = 0; // a loop iterator
   int b = 0; // b loop iterator
   int c = 0; // c loop iterator

   while(c < totalRows) // loop through each row in auditorium
   {
      if(c != 0) // if not first row
      {
         a = 0; // set a iterator to 0
         n = theater.getfirst(); // sets pointer to head node
         while(a < c) // loop until reach current row
         {
            if(n->getdown() != 0) // if bottom row exists
            {
                n = n->getdown(); // move pointer down
            }
            a++; // increment a iterator by 1
         }
      }

      b = 0; // set b iterator to 0
      while(b < totalSeats) // for every seat in auditorium
      {
         o << n->getpayload()->getTT(); // outputs seat into file
         if(b != totalSeats-1) // if not last seat in row
         {
             n = n->getright(); // move pointer to right node
         }

         b++; // increment b loop iterator by 1

      }
      c++; // increment c loop iterator by 1
      o << endl; // move to next line in file
   }
   o.close(); // close output file
}

/** Sort Ticket Vector **/
void sortTickets(vector<Ticket>& x) // Sorts the tickets in the ticket vectors for Adult, Child and Senior
{
   for(long long unsigned int z = 1; z < x.size(); z++) // loop through the vector
   {
      if(x.at(z).row < x.at(z-1).row) // if the row of the ticket that is before the current ticket is larger than the current ticket row
      {
         Ticket s(x.at(z).row, x.at(z).column); // sets item to temporary variable
         x.erase(x.begin() + z); // removes item not in order
         for(long long unsigned int m = 0; m < z; m ++) // loops through each each element in list before index of item checked and inserts item to appropriate place in list
         {
            if(x.at(m).row > s.row) // if the current Ticket's row is greater than the ticket row to be inserted
            {
               x.insert(x.begin()+ m, s); // insert it at that location
               break;
            }
            else if(x.at(m).row == s.row && x.at(m).column > s.column) // if they have the same row, but different seat letters where the current seat letter is greater than the inserted ticket seat letter
            {
               x.insert(x.begin()+ m, s); // insert the ticket at that spot
               break;
            }
         }

      }
      else if(x.at(z).row == x.at(z-1).row) // if the rows for the current and previous ticket is the same
      {
         if(x.at(z).column < x.at(z-1).column) // compare the columns to see if the column of the previous ticket is seat to right of current seat
         {
            Ticket s(x.at(z).row, x.at(z).column); // sets item to temporary variable
            x.erase(x.begin() + z); // removes item not in order
            for(long long unsigned int m = 0; m < z; m ++) // loops through each each element in list before index of item checked and inserts item to appropriate place in list
            {
               if((x.at(m).row == s.row) && (x.at(m).column > s.column)) // if the tickets are at the same row but the current ticket's seat letter is greater than the inserted ticket's seat letter
               {
                  x.insert(x.begin()+ m, s); // insert the ticket at that spot
                  break;
               }
               else if(x.at(m).row > s.row) // if the row of the current ticket is greater than the inserted ticket's row
               {
                  x.insert(x.begin()+ m, s); // insert the ticket at that spot
                  break;
               }
            }
         }
      }

   }

}

/** View Orders **/
void viewOrders(unordered_map<string, User>  userData, string username) // Displays the user's orders in chronological order
{
   vector<Order> orders = userData.find(username)->second.getOrders(); // gets the orders vector
   if(orders.size() == 0) // if the orders vector is empty
   {
      cout << "No orders" << endl; // display "No orders" on console
   }
   else // else if vector is non-empty
   {
      vector<Ticket> printedoutTickets; // create a vector of tickets called printedoutTickets to put all the tickets from the adult, senior, child ticket vectors together
      for(long long unsigned int i = 0; i < orders.size(); i++) // goes through all orders, prints out seats and stuff
      {
         // Pushes every ticket into the vector from the order
         for(long long unsigned int j = 0; j < orders.at(i).adultList.size(); j++) // loop through the adult vector
         {
            printedoutTickets.push_back(Ticket(orders.at(i).adultList.at(j).row, orders.at(i).adultList.at(j).column)); // insert adult ticket to printedoutTickets
         }
         for(long long unsigned int k = 0; k < orders.at(i).childList.size(); k++) // loop through the child vector
         {
            printedoutTickets.push_back(Ticket(orders.at(i).childList.at(k).row, orders.at(i).childList.at(k).column)); // insert child ticket to printedoutTickets
         }
         for(long long unsigned int l = 0; l < orders.at(i).seniorList.size(); l++) // loop through the senior vector
         {
            printedoutTickets.push_back(Ticket(orders.at(i).seniorList.at(l).row, orders.at(i).seniorList.at(l).column)); // insert senior ticket to printedoutTickets
         }

         // Sort the printedoutTickets vector
         sortTickets(printedoutTickets); // sorts the vector of tickets to be printed out by row and seat order


         cout << "Auditorium " << orders.at(i).auditoriumNumber << ", "; // prints out auditorium number

         for(long long unsigned int h = 0; h < printedoutTickets.size(); h++) // loop through the vector
         {
            cout << printedoutTickets.at(h).row << printedoutTickets.at(h).column; // print the ticket's row and seat letter
            if((int)h != ((int)printedoutTickets.size())-1) // if not last ticket of vector
            {
               cout << ','; // print a comma after each ticket
            }
         }

         cout << endl; // move to next line
         cout << orders.at(i).adultList.size() << " adult, " << orders.at(i).childList.size() << " child, " << orders.at(i).seniorList.size() << " senior" << endl; // print out the total number of adult, child and senior tickets of the order
         cout << endl; // move to next line

         printedoutTickets.clear(); // clear the vector
      }
   }


}

/** Display Receipt **/
void displayReceipt(unordered_map<string, User>  userData, string username) // Display the receipt of the user's order in chronological order
{
   vector<Order> orders = userData.find(username)->second.getOrders(); // gets the orders vector
   if(orders.size() == 0) // if order vector is empty
   {
      cout << "No orders" << endl << "Customer Total: $0.00" << endl; // Display on console "No orders" and "Customer Total: $0.00"
   }
   else // else order vector is non-empty
   {
      float customerTotal = 0.00f; // variable to store the total amount of money the customer spends
      float orderTotal = 0.00f; // variable to store the total cost of each order
      long long unsigned int x = 0; // loop iterator
      long long unsigned int y = 0; // loop iterator
      long long unsigned int z = 0; // loop iterator
      for(long long unsigned int i = 0; i < orders.size(); i++) // goes through all orders, prints out seats and stuff
      {
         cout << "Auditorium " << orders.at(i).auditoriumNumber << ", "; // prints out auditorium number
         for(x = 0; x < orders.at(i).adultList.size(); x++) // prints out all adult ticket seats
         {
            cout << orders.at(i).adultList.at(x).row << orders.at(i).adultList.at(x).column; // prints out the ticket row number and seat letter
            if(x != orders.at(i).adultList.size() - 1) // if not at last ticket in vector
            {
               cout << ','; // display a , after every ticket information
            }
         }
         if(orders.at(i).childList.size() > 0 && x > 0) // if child tickets exist
         {
            cout << ','; // display a , after every ticket information
         }
         for(y = 0; y < orders.at(i).childList.size(); y++) // prints out all child ticket seats
         {
            cout << orders.at(i).childList.at(y).row << orders.at(i).childList.at(y).column; // prints out the ticket row number and seat letter
            if(y != orders.at(i).childList.size() - 1) // if not at last ticket in vector
            {
               cout << ','; // display a , after every ticket information
            }
         }
         if((x > 0 || y > 0) && (orders.at(i).seniorList.size() > 0)) // if senior tickets exist
         {
            cout << ','; // display a , after every ticket information
         }
         for(z = 0; z < orders.at(i).seniorList.size(); z++) // prints out all senior ticket seats
         {
            cout << orders.at(i).seniorList.at(z).row << orders.at(i).seniorList.at(z).column; // prints out the ticket row number and seat letter
            if(z != orders.at(i).seniorList.size() - 1) // if not at last ticket in vector
            {
               cout << ','; // display a , after every ticket information
            }
         }
         cout << endl; // move down a line
         cout << orders.at(i).adultList.size() << " adult, " << orders.at(i).childList.size() << " child, " << orders.at(i).seniorList.size() << " senior" << endl; // prints out total number of tickets, for adult, child and senior
         orderTotal = 10.00f*(float)orders.at(i).adultList.size() + 5.00f*(float)orders.at(i).childList.size() + 7.50f*(float)orders.at(i).seniorList.size(); // calculates and sets the total cost of the order
         cout << "Order Total: $" << fixed << setprecision(2) << orderTotal << endl << endl; // prints out the total cost for the order
         customerTotal += orderTotal; // adds the total cost of the current order to the total cost of the customer


      }
      cout << "Customer Total: $" << fixed << setprecision(2) << customerTotal << endl; // prints out the total cost of all the orders of the user for the customer
   }

}

/** Print Report **/
void printReport(Auditorium& theater1, Auditorium& theater2, Auditorium& theater3, int totalRows1, int totalSeats1, int totalRows2, int totalSeats2, int totalRows3, int totalSeats3) // prints out the report for total sales/tickets sold etc.
{
   int totalOpen = 0; // variable to store total opens seats
   int totalReserved = 0; // variable to store total reserved seats
   int totalAdult = 0; // variable to store total reserved adult seats
   int totalChild = 0; // variable to store total reserved child seats
   int totalSenior = 0; // variable to store total reserved senior seats
   double totalAmountAll = 0.00; // variable to store total money made from all three auditoriums

   int open = 0; // variable to store total opens seats of auditorium
   int reserved = 0; // variable to store total reserved seats of auditorium
   int Adult_Sold = 0; // variable to store total reserved adult seats of auditorium
   int Child_Sold = 0;// variable to store total reserved child seats of auditorium
   int Senior_Sold = 0; // variable to store total reserved senior seats of auditorium
   double totalAmount = 0.00; // variable to store total money made from auditorium

   int c = 0; // loop iterator
   int b = 0; // loop iterator
   int a = 0; // loop iterator
   Node<Seat*>*n = theater1.getfirst(); // pointer to point to current seat being tracked

   // print report for Theater1
   while(c < totalRows1) // loops through every row in auditorium
   {
      if(c != 0) // if not first row
      {
         a = 0; // reset a loop iterator to 0
         n = theater1.getfirst(); // set pointer to point to head node
         while(a < c) // loops until current row is reached
         {
            if(n->getdown() != 0) // if there exists a bottom row; != nullptr instead of 0
            {
                n = n->getdown(); // move pointer down a row
            }
            a++; // increment a iterator
         }
      }

      b = 0; // reset b loop iterator to 0
      while(b < totalSeats1)
      {
         if(n->getpayload()->getTT() == 'A') // checks if seat is reserved by an adult
         {
            Adult_Sold += 1; // add 1 to adult tickets sold variable
            reserved += 1; // add 1 to total seats
         }
         else if(n->getpayload()->getTT() == 'C') // checks if seat is reserved by an child
         {
            Child_Sold += 1; // add 1 to child tickets sold variable
            reserved += 1; // add 1 to total seats
         }
         else if(n->getpayload()->getTT() == 'S') // checks if seat is reserved by an senior
         {
            Senior_Sold += 1; // add 1 to senior tickets sold variable
            reserved += 1; // add 1 to total seats
         }
         else if(n->getpayload()->getTT() == '.') // checks if seat is empty
         {
            open += 1; // add 1 to total seats
         }

         if(b != totalSeats1-1) // if not last seat in row
         {
             n = n->getright(); // move pointer to right
         }

         b++; // increase b loop iterator by 1

      }
      c++; // increase c loop iterator by 1

   }

   totalAmount = 10.00*Adult_Sold + 5.00*Child_Sold + 7.50*Senior_Sold; // calculates the current theater's total money made

   totalOpen += open; // adds the current theater's total open seats to the total open seats of all three auditorium
   totalReserved += reserved; // adds the current theater's total open seats to the total open seats of all three auditorium
   totalAdult += Adult_Sold; // adds the current theater's total reserved adult seats to the total reserved adult seats of all three auditorium
   totalChild += Child_Sold;// adds the current theater's total reserved child seats to the total reserved child seats of all three auditorium
   totalSenior += Senior_Sold; // adds the current theater's total reserved senior seats to the total reserved senior seats of all three auditorium
   totalAmountAll += totalAmount; // adds the current theater's total money made to the total money made from all three auditorium

   // Display Report for Auditorium 1
   cout << "Auditorium 1 " << '\t' << open << '\t' << reserved << '\t' << Adult_Sold << '\t' << Child_Sold << '\t' << Senior_Sold << '\t' << "$" << fixed << setprecision(2) << totalAmount << endl;

   open = 0; // resets the open variable
   reserved = 0; // resets the reserved variable
   Adult_Sold = 0; // resets the Adult_Sold variable
   Child_Sold = 0; // resets the Child_Sold variable
   Senior_Sold = 0; // resets the Senior_Sold variable
   totalAmount = 0.00f; // resets the totalAmount variable
   c = 0; // resets the loop iterator
   b = 0; // resets the loop iterator
   a = 0; // resets the loop iterator
   n = theater2.getfirst(); // pointer to point to current seat being tracked

   // print report for Theater2
   while(c < totalRows2) // loops through every row in auditorium
   {
      if(c != 0) // if not first row
      {
         a = 0; // reset a loop iterator to 0
         n = theater2.getfirst(); // set pointer to point to head node
         while(a < c) // loops until current row is reached
         {
            if(n->getdown() != 0) // if there exists a bottom row
            {
                n = n->getdown(); // move pointer down a row
            }
            a++; // increment a iterator
         }
      }

      b = 0; // reset b loop iterator to 0
      while(b < totalSeats2)
      {
         if(n->getpayload()->getTT() == 'A') // checks if seat is reserved by an adult
         {
            Adult_Sold += 1; // add 1 to adult tickets sold variable
            reserved += 1; // add 1 to total seats
         }
         else if(n->getpayload()->getTT() == 'C') // checks if seat is reserved by an child
         {
            Child_Sold += 1; // add 1 to child tickets sold variable
            reserved += 1; // add 1 to total seats
         }
         else if(n->getpayload()->getTT() == 'S') // checks if seat is reserved by an senior
         {
            Senior_Sold += 1; // add 1 to senior tickets sold variable
            reserved += 1; // add 1 to total seats
         }
         else if(n->getpayload()->getTT() == '.') // checks if seat is empty
         {
            open += 1; // add 1 to total seats
         }

         if(b != totalSeats2-1) // if not last seat in row
         {
             n = n->getright(); // move pointer to right
         }

         b++; // increase b loop iterator by 1

      }
      c++; // increase c loop iterator by 1

   }

   totalAmount = 10.00*Adult_Sold + 5.00*Child_Sold + 7.50*Senior_Sold; // calculates the current theater's total money made

   totalOpen += open; // adds the current theater's total open seats to the total open seats of all three auditorium
   totalReserved += reserved; // adds the current theater's total open seats to the total open seats of all three auditorium
   totalAdult += Adult_Sold; // adds the current theater's total reserved adult seats to the total reserved adult seats of all three auditorium
   totalChild += Child_Sold; // adds the current theater's total reserved child seats to the total reserved child seats of all three auditorium
   totalSenior += Senior_Sold; // adds the current theater's total reserved senior seats to the total reserved senior seats of all three auditorium
   totalAmountAll += totalAmount; // adds the current theater's total money made to the total money made from all three auditorium

   // Display Report for Auditorium 2
   cout << "Auditorium 2 " << '\t' << open << '\t' << reserved << '\t' << Adult_Sold << '\t' << Child_Sold << '\t' << Senior_Sold << '\t' << "$" << fixed << setprecision(2) << totalAmount << endl;

   open = 0; // resets the open variable
   reserved = 0; // resets the reserved variable
   Adult_Sold = 0; // resets the Adult_Sold variable
   Child_Sold = 0; // resets the Child_Sold variable
   Senior_Sold = 0; // resets the Senior_Sold variable
   totalAmount = 0.00f; // resets the totalAmount variable
   c = 0; // resets the loop iterator
   b = 0; // resets the loop iterator
   a = 0; // resets the loop iterator
   n = theater3.getfirst(); // pointer to point to current seat being tracked

   // print report for Theater3
   while(c < totalRows3) // loops through every row in auditorium
   {
      if(c != 0) // if not first row
      {
         a = 0; // reset a loop iterator to 0
         n = theater3.getfirst(); // set pointer to point to head node
         while(a < c) // loops until current row is reached
         {
            if(n->getdown() != 0) // if there exists a bottom row
            {
                n = n->getdown(); // move pointer down a row
            }
            a++; // increment a iterator
         }
      }

      b = 0; // reset b loop iterator to 0
      while(b < totalSeats3)
      {
         if(n->getpayload()->getTT() == 'A') // checks if seat is reserved by an adult
         {
            Adult_Sold += 1; // add 1 to adult tickets sold variable
            reserved += 1; // add 1 to total seats
         }
         else if(n->getpayload()->getTT() == 'C') // checks if seat is reserved by an child
         {
            Child_Sold += 1; // add 1 to child tickets sold variable
            reserved += 1; // add 1 to total seats
         }
         else if(n->getpayload()->getTT() == 'S') // checks if seat is reserved by an senior
         {
            Senior_Sold += 1; // add 1 to senior tickets sold variable
            reserved += 1; // add 1 to total seats
         }
         else if(n->getpayload()->getTT() == '.') // checks if seat is empty
         {
            open += 1; // add 1 to total seats
         }

         if(b != totalSeats3-1) // if not last seat in row
         {
             n = n->getright(); // move pointer to right
         }

         b++; // increase b loop iterator by 1

      }
      c++; // increase c loop iterator by 1

   }

   totalAmount = 10.00*Adult_Sold + 5.00*Child_Sold + 7.50*Senior_Sold; // calculates the current theater's total money made

   totalOpen += open; // adds the current theater's total open seats to the total open seats of all three auditorium
   totalReserved += reserved; // adds the current theater's total open seats to the total open seats of all three auditorium
   totalAdult += Adult_Sold; // adds the current theater's total reserved adult seats to the total reserved adult seats of all three auditorium
   totalChild += Child_Sold; // adds the current theater's total reserved child seats to the total reserved child seats of all three auditorium
   totalSenior += Senior_Sold; // adds the current theater's total reserved senior seats to the total reserved senior seats of all three auditorium
   totalAmountAll += totalAmount; // adds the current theater's total money made to the total money made from all three auditorium

   // Display Report for Auditorium 3
   cout << "Auditorium 3 " << '\t' << open << '\t' << reserved << '\t' << Adult_Sold << '\t' << Child_Sold << '\t' << Senior_Sold << '\t' << "$" << fixed << setprecision(2) << totalAmount << endl;

   cout << "Total " << "\t\t" << totalOpen << '\t' << totalReserved << '\t' << totalAdult << '\t' << totalChild << '\t' << totalSenior << '\t' << "$" << fixed << setprecision(2) << totalAmountAll << endl;


}

/** Unreserve Seat **/
void unreserveSeats(Auditorium& theater, int row, char seat, int adult_quantity, int senior_quantity, int child_quantity, int totalSeats) // unreserves user chosen seats
{
   int A_count = adult_quantity; // total adult tickets
   int S_count = senior_quantity; // total senior tickets
   int C_count = child_quantity; // total child tickets
   int col = int(toupper(seat)) - 65; // converts char seat into the ASCII value of the char, then subtracts from the ASCII value of A to find the distance from column spot 0
   Node<Seat*>*r = theater.getfirst(); // pointer that currently points to head node
   int k = 0; // loop iterator to find correct row to reserve
   while(k < row-1) // positions at correct row
   {
      r = r->getdown(); // move pointer down
      k++; // increase iterator by 1
   }

   for(int i = 0; i < totalSeats; i++) // finds where starting seat is at
   {
      if(r->getpayload()->getSeat() == seat) // if seat located then break out of loop
      {
         break; // break out of loop
      }
      if(i != totalSeats - 1) // if not last seat in row
      {
         r = r->getright(); // move pointer to right
      }

   }

   for(int c = col; c < totalSeats; c++) // iterates through every seat after the starting seat
   {
      if(r->getpayload()->getTT() != '.') // if seat is not empty
      {
         if(A_count > 0) // if there are still adult tickets to be unreserved, unreserve them
         {
           r->getpayload()->setTT('.');// unreserves seat
           A_count -= 1; // decreases total adult tickets needed to be unreserved by 1

         }
         else if(C_count > 0) // if there are still child tickets to be unreserved, unreserve them
         {
            r->getpayload()->setTT('.');; // unreserves seat
            C_count -= 1; // decreases total child tickets needed to be reserved by 1
         }
         else if(S_count > 0)  // if there are still senior tickets to be unreserved, unreserve them
         {
            r->getpayload()->setTT('.'); // unreserves seat
            S_count -= 1; // decreases total senior tickets needed to be unreserved by 1
         }

      }
      if(c != totalSeats-1) // if not last seat in row
      {
         r = r->getright(); // move pointer to right node
      }

   }
}

int main()
{
   ifstream ifs; // input file stream
   unordered_map<string, User> userMap; // hashmap that stores all the users
   string line = " "; // variable to store the line that is being read from the file
   string username = " "; // variable to store read in username
   string password = " "; // variable to store read in password
   int totalinputpass; // variable to store total attempts
   string menuoption = " "; // variable to store the menu option user inputs

   ofstream ofs; // outputfilestream
   string filename = " "; // variable to store auditorium user input filename
   int rowNum = 0; // row number of user input
   string strrowNum = " "; // user input rowNum in string form
   char startingSeat = ' '; // user input starting seat letter
   string startingSeatstr = " ";
   int numA = 0; // variable to store total requested adult tickets
   int numC = 0; // variable to store total requested child tickets
   int numS = 0; // variable to store total requested senior tickets
   int totalRows = 0; // variable to store totalRows
   int totalSeats = 0; // variable to store totalSeats

   string MainMenuSelect = " "; // Variable for which choice the user chooses (1) reserve seats (2) exit
   int totalTickets = 0; // Variable for total tickets user wants to reserve
   int startingSeatcol = -1; // the index of the starting seat char the user inputs
   int ssNum = 0; // starting seat number variable
   string promptYN = " "; // variable for response Y/N reserve best available seats
   string strnumA = " "; // string version of adult tickets requested by user
   string strnumC = " "; // string version of child tickets requested by user
   string strnumS = " "; // string version of senior tickets requested by user

   Auditorium *theater = nullptr; // ptr to point to current auditorium that is being used
   int auditoriumNumber = 0; // current auditorium number that is being used

   int adultcounter = 0; // variable to store the number of adult tickets
   int childcounter = 0; // variable to store the number of child tickets
   int seniorcounter = 0; // variable to store the number of senior tickets

   // Create Auditoriums
   Auditorium Auditorium1("A1.txt", cnt_rows("A1.txt")); // creates auditorium 1 from the auditorium 1 file
   Auditorium Auditorium2("A2.txt", cnt_rows("A2.txt")); // creates auditorium 2 from the auditorium 2 file
   Auditorium Auditorium3("A3.txt", cnt_rows("A3.txt")); // creates auditorium 3 from the auditorium 3 file



   // Log All Usernames and Passwords into Hashmap
   ifs.open("userdb.dat"); // open user data file

   for(int x = 0; x < cnt_rows("userdb.dat"); x++) // fill in hashmap with usernames and passwords
   {
      if(!ifs.eof()) // if not at end of file
      {
         getline(ifs, line); // get the line of the file
      }
      if(line.length() == 0) // if getline() gets empty line
      {
         break; // break out of loop
      }

      userMap.insert({line.substr(0, line.find(" ")) , User(line.substr(0, line.find(" ")), line.substr(line.find(" ") + 1))}); // insert the new username and user object into hashmap

   }

   ifs.close(); // close the file


   // STARTING POINT

   while(true) // loop for whole user interface
   {
      while(true) // loop for login
      {
         cout << "Username: " ; // prompt for username
         cin >> username; // prompt for username

         totalinputpass = 0;
         while(totalinputpass < 3) // if user has made less than 3 attempts
         {
            cout << "Password: "; // prompt for password
            cin >> password; // prompt for password

            if(userMap.find(username)->second.getPassword() == password) // if password matches inputted username's password
            {
               cout << "Welcome " << username << "!" << endl;
               break;
            }
            cout << "Invalid password" << endl; // If fails print "Invalid password"
            totalinputpass++; // increment counter for number of attempts

         }

         if(totalinputpass < 3) // checking if the user successfully logged in
         {
            break; // break out of loop
         }

      }

      if(username == "admin") // if user logged in as an admin -> admin interface
      {
         while(true) // admin main menu
         {
            while(true) // loop and ask admin main menu option
            {
               cout << "1. Print Report" << endl << "2. Logout" << endl << "3. Exit" << endl; // prompt for main menu option
               cin >> menuoption; // prompt for main menu option

               if(menuoption == "1" || menuoption == "2" || menuoption == "3") // if user enters valid input
               {
                  break; // break out of loop
               }

            }
            if(menuoption == "1") // Print Report
            {
               printReport(Auditorium1, Auditorium2, Auditorium3, cnt_rows("A1.txt"), cnt_seats("A1.txt"), cnt_rows("A2.txt"), cnt_seats("A2.txt"), cnt_rows("A3.txt"), cnt_seats("A3.txt"));
            }
            else if(menuoption == "3") // exit
            {
               theater = &Auditorium1; // set the theater ptr to point to auditorium 1
               writeFile(*theater, cnt_rows("A1.txt"), cnt_seats("A1.txt"), "A1Final.txt"); // write auditorium to file

               theater = &Auditorium2; // set the theater ptr to point to auditorium 2
               writeFile(*theater, cnt_rows("A2.txt"), cnt_seats("A2.txt"), "A2Final.txt"); // write auditorium to file

               theater = &Auditorium3; // set the theater ptr to point to auditorium 3
               writeFile(*theater, cnt_rows("A3.txt"), cnt_seats("A3.txt"), "A3Final.txt"); // write auditorium to file

               theater = nullptr; // set the theater ptr to point to nothing
               break; // exit the admin loop
            }
            else // logout
            {
               break; // exit the admin loop
            }
         }

         if(menuoption == "3") // exit the program
         {
            break; // exits the program
         }

      }
      else // the user logged in is a customer
      {
         while(true) // Customer main menu
         {
            while(true) // Prompt for main menu option
            {
               menuoption = ' '; // resets menu option variable
               cout << "1. Reserve Seats" << endl << "2. View Orders" << endl << "3. Update Order" << endl << "4. Display Receipt" << endl << "5. Log Out" << endl; // prompt customer menu option
               cin >> menuoption; // prompt customer menu option
               if(menuoption == "1" || menuoption == "2" || menuoption == "3" || menuoption == "4" || menuoption == "5") // if input is valid
               {
                  break; // break
               }
            }
            if(menuoption == "1") // Reserve Seats
            {
               string reservationtheateroption = " "; // variable that stores the option user chose for which theater to reserve in
               while(true) // prompt for auditorium
               {
                  reservationtheateroption = " "; // reset reservationtheateroption
                  cout << "1. Auditorium 1" << endl << "2. Auditorium 2" << endl << "3. Auditorium 3" << endl; // prompt for auditorium
                  cin >> reservationtheateroption; // prompt for auditorium
                  if(reservationtheateroption == "1" || reservationtheateroption == "2" || reservationtheateroption == "3") // if input is valid
                  {
                     break; // break
                  }
               }
               if(reservationtheateroption == "1") // if user picks auditorium 1
               {
                  ///DISPLAY THEATER
                  theater = &Auditorium1; // set the theater ptr to point to auditorium 1
                  totalRows = cnt_rows("A1.txt"); // set the total rows for auditorium 1
                  totalSeats = cnt_seats("A1.txt"); // set the total seats for auditorium 1
                  auditoriumNumber = 1; // set auditoriumNumber variable to 1
               }
               else if(reservationtheateroption == "2") // if user picks auditorium 2
               {
                  ///DISPLAY THEATER
                  theater = &Auditorium2; // set the theater ptr to point to auditorium 2
                  totalRows = cnt_rows("A2.txt"); // set the total rows for auditorium 2
                  totalSeats = cnt_seats("A2.txt"); // set the total seats for auditorium 2
                  auditoriumNumber = 2; // set auditoriumNumber variable to 2
               }
               else // if user picks auditorium 3
               {
                  ///DISPLAY THEATER
                  theater = &Auditorium3; // set the theater ptr to point to auditorium 3
                  totalRows = cnt_rows("A3.txt"); // set the total rows for auditorium 3
                  totalSeats = cnt_seats("A3.txt"); // set the total seats for auditorium 3
                  auditoriumNumber = 3; // set auditoriumNumber variable to 3
               }

               /// RESERVATION HERE!
               totalTickets = 0; // resets total tickets number to 0
               displayAuditorium(*theater, totalRows, totalSeats); // calls function to display theater
               //Prompts for row
               while(true) // loops until valid row number is inputted
               {
                  strrowNum = " ";
                  rowNum = 0; // resets the row number value
                  cout << "Row number: " << endl; // prompts row number
                  cin >> strrowNum; // prompts row number
                  // Validate Row
                  if(isdigit(strrowNum.at(0))) // if input is a digit
                  {
                     rowNum = stoi(strrowNum); //converts input to integer
                     if(rowNum <= totalRows && rowNum > 0) // checks if row number is within the auditorium, if so, break out of loop
                     {
                        break; // break out of loop
                     }
                  }
               }
               while(true) // loops until valid seat letter is inputted
               {
                  startingSeatstr = " ";
                  startingSeat = ' '; // sets starting seat variable to empty char
                  // Prompting Starting Seat
                  cout << "Starting Seat: " << endl; // prompts starting seat letter
                  cin >> startingSeatstr; // prompts user for starting seat letter

                  if(startingSeatstr.length()  == 1)
                  {
                     // Validate Seat
                     startingSeat = startingSeatstr.at(0);
                     startingSeatcol = int(toupper(startingSeat)) - 65; // My thought - this variable compares the ASCII value of the inputted startingSeat to the capital A ASCII value and subtracts them to get the index of the column that the user inputted
                     // This if statement checks if the startingSeat user input is larger or smaller than the number of seats/columns in the auditorium
                     if(startingSeatcol >= 0 && startingSeatcol < totalSeats) // if starting seat is within bounds of thater
                     {
                        break; // break out of loop
                     }

                  }

               }

               while(true)  // loops until valid adult tickets number is inputted
               {
                  strnumA = " "; // sets total number of inputted adult ticket variable to empty string
                  numA = 0; // resets number of adult tickets
                  // Prompt for number of adult tickets
                  cout << "Number of adult tickets: " << endl; // prompts number of requested adult tickets
                  cin >> strnumA; // prompts number of requested adult tickets
                  // Validate ticket number
                  if(isNumber(strnumA)) // if input is a number
                  {
                     numA = stoi(strnumA); // convert string to integer
                     if(numA >= 0 && numA <= totalSeats) // checks if number of adult tickets is not negative
                     {
                        break; // break out of loop

                     }
                  }

               }
               totalTickets += numA; // add total adult tickets requested to total tickets
               while(true)  // loops until valid child number is inputted
               {
                  strnumC = " "; // sets total number of inputted child ticket variable to empty string
                  numC = 0; // resets number of child tickets
                  // Prompt for number of child tickets
                  cout << "Number of child tickets: " << endl; // prompts number of requested child tickets
                  cin >> strnumC; // prompts number of requested child tickets
                  // Validate ticket number
                  if(isNumber(strnumC)) // if input is a number
                  {
                     numC = stoi(strnumC); // convert string to integer
                     if(numC >= 0 && (numC + totalTickets) <= totalSeats) // checks if number of child tickets is not negative
                     {
                        break; // break out loop
                     }
                  }
               }
               totalTickets += numC; // add total child tickets requested to total ticket
               while(true) // loops until valid senior number is inputted
               {
                  strnumS = " "; // sets total number of inputted senior ticket variable to empty string
                  numS = 0;// resets number of senior tickets
                  // Prompt for number of senior tickets
                  cout << "Number of senior tickets: "; // prompts number of requested senior tickets
                  cin >> strnumS; // prompts number of requested senior tickets
                  // Validate ticket number
                  if(isNumber(strnumS)) // if input is a number
                  {
                     numS = stoi(strnumS); // convert string to integer
                     if(numS >= 0 && (numS + totalTickets) <= totalSeats)// checks if number of senior tickets is not negative
                     {
                        break; // break out loop
                     }
                  }

               }
               totalTickets += numS; // add total senior tickets requested to total ticket
               // Checks if seats are available, if so then reserve the seats, else, call bestAvailable() and if possible ask user if they would like the best available seats
               if(checkAvailability(*theater, rowNum,startingSeat,totalTickets, totalSeats)) // checks if seats requested are available
               {
                  vector<Order>* userOrders = &(userMap.find(username)->second.getOrders()); // gets the orders vector
                  adultcounter = numA; // set total number of requested adult tickets to adult counter
                  childcounter = numC; // set total number of requested child tickets to child counter
                  seniorcounter = numS; // set total number of requested senior tickets to senior counter

                  userOrders->push_back(Order(numA, numC, numS, auditoriumNumber)); //  add the new order to userOrders vector
                  for(int h = 0; h < totalTickets; h++) // loop from 0 to totaltickets
                  {
                     if(adultcounter > 0) // if there is still adult tickets that have not been reserved
                     {
                        userOrders->at(userOrders->size() - 1).adultList.push_back(Ticket(rowNum, char(int(startingSeat) + h))); // reserve the seat and add it to adult vector
                        adultcounter--; // decrement adult counter
                     }
                     else if(childcounter > 0) // if there is still child tickets that have not been reserved
                     {
                        userOrders->at(userOrders->size() - 1).childList.push_back(Ticket(rowNum, char(int(startingSeat) + h))); // reserve the seat and add it to child vector
                        childcounter--; // decrement child counter
                     }
                     else if(seniorcounter > 0) // if there is still senior tickets that have not been reserved
                     {
                        userOrders->at(userOrders->size() - 1).seniorList.push_back(Ticket(rowNum, char(int(startingSeat) + h))); // reserve the seat and add it to senior vector
                        seniorcounter--; // decrement senior counter
                     }
                  }

                  reserveSeats(*theater, rowNum,startingSeat,numA,numS,numC, totalSeats); // if available, reserve those seats
               }
               else // if unable to reserve requested seats
               {
                  ssNum = bestAvailable(*theater, rowNum, totalTickets, totalRows, totalSeats); // calls the bestAvailable function to find best available seats
                  if(ssNum != -1) // checks if there are seats available
                  {
                     if(totalTickets > 1) // if the total tickets requested is greater than 1, print a range
                     {
                        cout << rowNum << char(ssNum + 65) << " - " << rowNum << char(ssNum + 65 - 1 + totalTickets) << endl; //  the char(ssNum + 65) converts the starting seat number to the letter of the seat; same thing with char(ssNum + 65 + total_tickets - 1) which is the end of range of the best available seats; -1 so not to reserve an extra seat since the starting seat is included already
                     }
                     else if(totalTickets == 1) // if the total tickets requested is 1, return a row and letter seat for best available
                     {
                        cout << rowNum << char(ssNum + 65) << endl; // prints out single best available seat
                     }
                     cout << "Reserve Seats? Y/N" << endl; // prompts user to reserve or not reserve seats
                     cin >> promptYN; // prompts user to reserve/not reserve the seats
                     if(promptYN.length() == 1 && promptYN == "Y") // if they reserve the seats
                     {
                        vector<Order>* userOrders = &(userMap.find(username)->second.getOrders()); // gets the orders vector
                        adultcounter = numA; // set total number of requested adult tickets to adult counter
                        childcounter = numC; // set total number of requested child tickets to child counter
                        seniorcounter = numS; // set total number of requested senior tickets to senior counter

                        userOrders->push_back(Order(numA, numC, numS, auditoriumNumber)); // add an order to user order vector
                        for(int h = 0; h < totalTickets; h++) // loop from 0 to totaltickets
                        {
                           if(adultcounter > 0) // if there is still adult tickets that have not been reserved
                           {
                              userOrders->at(userOrders->size() - 1).adultList.push_back(Ticket(rowNum, char(ssNum + h + 65))); // reserve the seat and add it to adult vector
                              adultcounter--; // decrement adult counter
                           }
                           else if(childcounter > 0) // if there is still child tickets that have not been reserved
                           {
                              userOrders->at(userOrders->size() - 1).childList.push_back(Ticket(rowNum, char(ssNum + h + 65))); // reserve the seat and add it to child vector
                              childcounter--; // decrement child counter
                           }
                           else if(seniorcounter > 0) // if there is still senior tickets that have not been reserved
                           {
                              userOrders->at(userOrders->size() - 1).seniorList.push_back(Ticket(rowNum, char(ssNum + h + 65))); // reserve the seat and add it to senior vector
                              seniorcounter--; // decrement senior counter
                           }
                        }

                        cout << "Reservation Successful!" << endl; // confirmation message

                        reserveSeats(*theater, rowNum,char(ssNum + 65),numA,numS,numC, totalSeats); // reserves best available seats

                     }
                  }
                  else // else, then no available seats
                  {
                     cout << "no seats available" << endl; // if no best seats available, print this statement
                  }
               }

            }
            else if(menuoption == "2") // View Orders
            {
               viewOrders(userMap, username); // call viewOrders function
            }
            else if(menuoption == "3") // Update Order
            {
               string orderIndexstr = " "; // variable to store orderIndex input
               int orderIndex = -1; // variable to store orderIndex user inputs in integer form
               string updateMenuoption = " "; // variable to store updateMenu input
               vector<Order> *orders = &(userMap.find(username)->second.getOrders()); // gets the orders vector
               while(true) // displays order menu
               {
                  if(orders->size() == 0) // if no orders in order vector
                  {
                     cout << "No orders" << endl; // display No orders
                     break; // break out of loop
                  }
                  else
                  {
                     vector<Ticket> printedoutTickets; // create printedoutTicket vector to store all the tickets
                     for(long long unsigned int i = 0; i < orders->size(); i++) // goes through all orders, prints out seats and stuff
                     {
                        cout << "Order " << i+1 << endl; // display "order" on console
                        cout << "Auditorium " << orders->at(i).auditoriumNumber << ", "; // prints out auditorium number
                        // Pushes every ticket into the vector from the order
                        for(long long unsigned int j = 0; j < orders->at(i).adultList.size(); j++) // loop through adult vector
                        {
                           printedoutTickets.push_back(Ticket(orders->at(i).adultList.at(j).row, orders->at(i).adultList.at(j).column)); // add ticket to adult vector
                        }
                        for(long long unsigned int k = 0; k < orders->at(i).childList.size(); k++) // loop through child vector
                        {
                           printedoutTickets.push_back(Ticket(orders->at(i).childList.at(k).row, orders->at(i).childList.at(k).column)); // add ticket to child vector
                        }
                        for(long long unsigned int l = 0; l < orders->at(i).seniorList.size(); l++) // loop through senior vector
                        {
                           printedoutTickets.push_back(Ticket(orders->at(i).seniorList.at(l).row, orders->at(i).seniorList.at(l).column)); // add ticket to senior vector
                        }

                        // Sort the printedoutTickets vector
                        sortTickets(printedoutTickets); // sort the tickets
                        for(long long unsigned int h = 0; h < printedoutTickets.size(); h++) // loop through tickets vector that will be pritned out
                        {
                           cout << printedoutTickets.at(h).row << printedoutTickets.at(h).column; // print ticket row and seat letter
                           if((int)h != (int)printedoutTickets.size()-1) // if not last item in vector
                           {
                              cout << ','; // add comma
                           }
                        }

                        cout << endl; // move to next line
                        cout << orders->at(i).adultList.size() << " adult, " << orders->at(i).childList.size() << " child, " << orders->at(i).seniorList.size() << " senior" << endl;


                        printedoutTickets.clear();
                     }
                     while(true) // validating orderIndex input
                     {
                        cin >> orderIndexstr; // prompt for order input
                        if(orderIndexstr.length() == 1 && isNumber(orderIndexstr)) // if size of order is 1
                        {
                           orderIndex = stoi(orderIndexstr); // set orderIndex to input from user
                        }
                        if(orderIndex > 0 && orderIndex <= (int)orders->size()) // if orderindex input is within range of valid orders
                        {
                           break; // break out of input orderIndex
                        }
                     }

                     if(orders->at(orderIndex-1).auditoriumNumber == 1) // if user picks auditorium 1
                     {
                        theater = &Auditorium1; // set the theater ptr to point to auditorium 1
                        totalSeats = cnt_seats("A1.txt"); // set the total seats for auditorium 1
                        totalRows = cnt_rows("A1.txt"); // set the total rows for auditorium 1
                        auditoriumNumber = 1;  // set auditoriumNumber variable to 1
                     }
                     else if(orders->at(orderIndex-1).auditoriumNumber == 2) // if user picks auditorium 2
                     {
                        theater = &Auditorium2; // set the theater ptr to point to auditorium 2
                        totalSeats = cnt_seats("A2.txt"); // set the total seats for auditorium 2
                        totalRows = cnt_rows("A2.txt"); // set the total rows for auditorium 2
                        auditoriumNumber = 2;  // set auditoriumNumber variable to 2
                     }
                     else if(orders->at(orderIndex-1).auditoriumNumber == 3) // if user picks auditorium 3
                     {
                        theater = &Auditorium3; // set the theater ptr to point to auditorium 3
                        totalSeats = cnt_seats("A3.txt"); // set the total seats for auditorium 3
                        totalRows = cnt_rows("A3.txt"); // set the total rows for auditorium 3
                        auditoriumNumber = 3;  // set auditoriumNumber variable to 3
                     }

                  }


                  break; // break out of the loop
               }

               while(true) // update menu option
               {
                  if(orders->size() == 0) // if no orders in order vector
                  {
                     break; // break out of loop
                  }
                  while(true) // prompt usermenuOption
                  {
                     updateMenuoption = " "; // clear string updateMenu
                     cout << "1. Add tickets to order" << endl << "2. Delete tickets from order" << endl << "3. Cancel Order" << endl; // prompt for  menuoption input
                     cin >> updateMenuoption; // prompt for  menuoption input

                     if(updateMenuoption == "1" || updateMenuoption == "2" || updateMenuoption == "3") // if menuoption input is valid
                     {
                        break; // break out of the loop
                     }
                  }

                  if(updateMenuoption == "1") // Add tickets to order
                  {

                     /// RESERVATION HERE!
                     rowNum = 0; // row number of user input
                     strrowNum = " "; // user input rowNum in string form
                     startingSeat = ' '; // user input starting seat letter
                     numA = 0; // variable to store total requested adult tickets
                     numC = 0; // variable to store total requested child tickets
                     numS = 0; // variable to store total requested senior tickets
                     startingSeatcol = 0; // the index of the starting seat char the user inputs
                     totalTickets = 0; // resets total tickets number to 0
                     displayAuditorium(*theater, totalRows, totalSeats); // calls function to display theater
                     //Prompts for row
                     while(true) // loops until valid row number is inputted
                     {
                        rowNum = 0; // resets the row number value
                        cout << "Row number: " << endl; // prompts row number
                        cin >> strrowNum; // prompts row number
                        // Validate Row
                        if(isdigit(strrowNum.at(0))) // if input is a digit
                        {
                           rowNum = stoi(strrowNum); //converts input to integer
                           if(rowNum <= totalRows && rowNum > 0) // checks if row number is within the auditorium, if so, break out of loop
                           {
                              break; // break out of loop
                           }
                        }
                     }
                     while(true) // loops until valid seat letter is inputted
                     {
                        startingSeatstr = " ";
                        startingSeat = ' '; // sets starting seat variable to empty char
                        // Prompting Starting Seat
                        cout << "Starting Seat: " << endl; // prompts starting seat letter
                        cin >> startingSeatstr; // prompts user for starting seat letter

                        if(startingSeatstr.length() == 1) // if starting seat input is length of 1
                        {
                           // Validate Seat
                           startingSeat = startingSeatstr.at(0); // if input is a digit
                           startingSeatcol = int(toupper(startingSeat)) - 65; // My thought - this variable compares the ASCII value of the inputted startingSeat to the capital A ASCII value and subtracts them to get the index of the column that the user inputted
                           // This if statement checks if the startingSeat user input is larger or smaller than the number of seats/columns in the auditorium
                           if(startingSeatcol >= 0 && startingSeatcol < totalSeats) // if starting seat is within bounds of thater
                           {
                              break; // break out of loop
                           }

                        }

                     }


                     while(true)  // loops until valid adult tickets number is inputted
                     {
                        strnumA = " "; // sets total number of inputted adult ticket variable to empty string
                        numA = 0; // resets number of adult tickets
                        // Prompt for number of adult tickets
                        cout << "Number of adult tickets: " << endl; // prompts number of requested adult tickets
                        cin >> strnumA; // prompts number of requested adult tickets
                        // Validate ticket number
                        if(isNumber(strnumA)) // if input is a number
                        {
                           numA = stoi(strnumA); // convert string to integer
                           if(numA >= 0 && numA <= totalSeats) // checks if number of adult tickets is not negative
                           {
                              break; // break out of loop

                           }
                        }

                     }
                     totalTickets += numA; // add total adult tickets requested to total tickets
                     while(true)  // loops until valid child number is inputted
                     {
                        strnumC = " "; // sets total number of inputted child ticket variable to empty string
                        numC = 0; // resets number of child tickets
                        // Prompt for number of child tickets
                        cout << "Number of child tickets: " << endl; // prompts number of requested child tickets
                        cin >> strnumC; // prompts number of requested child tickets
                        // Validate ticket number
                        if(isNumber(strnumC)) // if input is a number
                        {
                           numC = stoi(strnumC); // convert string to integer
                           if(numC >= 0 && (numC + totalTickets) <= totalSeats) // checks if number of child tickets is not negative
                           {
                              break; // break out loop
                           }
                        }
                     }
                     totalTickets += numC; // add total child tickets requested to total ticket
                     while(true) // loops until valid senior number is inputted
                     {
                        strnumS = " "; // sets total number of inputted senior ticket variable to empty string
                        numS = 0;// resets number of senior tickets
                        // Prompt for number of senior tickets
                        cout << "Number of senior tickets: "; // prompts number of requested senior tickets
                        cin >> strnumS; // prompts number of requested senior tickets
                        // Validate ticket number
                        if(isNumber(strnumS)) // if input is a number
                        {
                           numS = stoi(strnumS); // convert string to integer
                           if(numS >= 0 && (numS + totalTickets) <= totalSeats)// checks if number of senior tickets is not negative
                           {

                              break; // break out loop
                           }
                        }

                     }
                     totalTickets += numS; // add total senior tickets requested to total ticket

                     if(checkAvailability(*theater, rowNum,startingSeat,totalTickets, totalSeats)) // if requested seat is available
                     {
                        adultcounter = numA; // set adultcounter to total adult tickets requested
                        childcounter = numC; // set childcounter to total child tickets requested
                        seniorcounter = numS; // set seniorcounter to total senior tickets requested

                        for(int h = 0; h < totalTickets; h++)
                        {
                           if(adultcounter > 0) // if there are still adult tickets left to reserve
                           {
                              orders->at(orderIndex-1).adultList.push_back(Ticket(rowNum, char(int(startingSeat) + h))); // add ticket to adult ticket vector


                              adultcounter--; // decrement adult counter
                           }
                           else if(childcounter > 0) // if there are still child tickets left to reserve
                           {
                              orders->at(orderIndex-1).childList.push_back(Ticket(rowNum, char(int(startingSeat) + h)));  // add ticket to child ticket vector

                              childcounter--; // decrement child counter
                           }
                           else if(seniorcounter > 0) // if there are still senior tickets left to reserve
                           {
                              orders->at(orderIndex-1).seniorList.push_back(Ticket(rowNum, char(int(startingSeat) + h)));  // add ticket to senior ticket vector

                              seniorcounter--; // decrement senior counter
                           }
                        }
                        reserveSeats(*theater, rowNum,startingSeat,numA,numS,numC, totalSeats); // if available, reserve those seats
                        break;
                     }
                     else
                     {
                        cout << "no seats available" << endl; // Display no seats available
                     }


                  }
                  else if(updateMenuoption == "2") // Delete tickets from order
                  {
                     bool isvalid = false;
                     bool isAdult = false;
                     bool isChild = false;
                     int deleteIndex = -1;
                     //Prompts for row
                     while(true) // loops until valid row number is inputted
                     {
                        rowNum = 0; // resets the row number value
                        cout << "Row number: " << endl; // prompts row number
                        cin >> strrowNum; // prompts row number
                        // Validate Row
                        if(isdigit(strrowNum.at(0))) // if input is a digit
                        {
                           rowNum = stoi(strrowNum); //converts input to integer
                           if(rowNum <= totalRows && rowNum > 0) // checks if row number is within the auditorium, if so, break out of loop
                           {
                              break; // break out of loop
                           }
                        }
                     }
                     while(true) // loops until valid seat letter is inputted
                     {
                        startingSeatstr = " ";
                        startingSeat = ' '; // sets starting seat variable to empty char
                        // Prompting Starting Seat
                        cout << "Starting Seat: " << endl; // prompts starting seat letter
                        cin >> startingSeatstr; // prompts user for starting seat letter

                        if(startingSeatstr.length()  == 1) // if starting seat input is length of 1
                        {
                           // Validate Seat
                           startingSeat = startingSeatstr.at(0);
                           startingSeatcol = int(toupper(startingSeat)) - 65; // My thought - this variable compares the ASCII value of the inputted startingSeat to the capital A ASCII value and subtracts them to get the index of the column that the user inputted
                           // This if statement checks if the startingSeat user input is larger or smaller than the number of seats/columns in the auditorium
                           if(startingSeatcol >= 0 && startingSeatcol < totalSeats) // if starting seat is within bounds of thater
                           {
                              break; // break out of loop
                           }

                        }

                     }

                     for(long long unsigned int v = 0; v < orders->at(orderIndex-1).adultList.size(); v++) // loop through adult vector
                     {
                        if( orders->at(orderIndex-1).adultList.at(v).row == rowNum && orders->at(orderIndex-1).adultList.at(v).column == startingSeat) // if requested deleted ticket is found in adult vector
                        {
                           isvalid = true; // set isvalid to true
                           isAdult = true; // set isAdult to true
                           deleteIndex = (int)v; // set deleteIndex to index of ticket to delete in adult/senior/child vector
                           break; // break out of loop
                        }
                     }
                     if(!isvalid) // if requested delete ticket not valid
                     {
                        for(long long unsigned int v = 0; v < orders->at(orderIndex-1).childList.size(); v++) // loop through child vector
                        {
                           if( orders->at(orderIndex-1).childList.at(v).row == rowNum && orders->at(orderIndex-1).childList.at(v).column == startingSeat) // if requested deleted ticket is found in child vector
                           {
                              isvalid = true; // set isvalid to true
                              isChild = true; // set isChild to true
                              deleteIndex = (int)v; // set deleteIndex to index of ticket to delete in adult/senior/child vector
                              break; // break out of loop
                           }
                        }
                     }
                     if(!isvalid) // if requested delete ticket not valid
                     {
                        for(long long unsigned int v = 0; v < orders->at(orderIndex-1).seniorList.size(); v++) // loop through senior vector
                        {
                           if(orders->at(orderIndex-1).seniorList.at(v).row == rowNum && orders->at(orderIndex-1).seniorList.at(v).column == startingSeat) // if requested deleted ticket is found in senior vector
                           {
                              isvalid = true; // set isvalid to true
                              deleteIndex = (int)v; // set deleteIndex to index of ticket to delete in adult/senior/child vector
                              break; // break out of loop
                           }
                        }
                     }
                     if(isvalid) // if requested seat to delete is valid
                     {
                        unreserveSeats(*theater, rowNum,startingSeat,0,1,0, totalSeats); // unreserve the seat

                        if(isAdult) // if adult
                        {
                           orders->at(orderIndex-1).adultList.erase(orders->at(orderIndex-1).adultList.begin() + deleteIndex); // remove ticket from adult vector
                        }
                        else if(isChild) // if child
                        {
                           orders->at(orderIndex-1).childList.erase(orders->at(orderIndex-1).childList.begin() + deleteIndex); // remove ticket from child vector
                        }
                        else // if senior
                        {
                           orders->at(orderIndex-1).seniorList.erase(orders->at(orderIndex-1).seniorList.begin() + deleteIndex); // remove ticket from senior vector
                        }

                        if(orders->at(orderIndex-1).adultList.size() == 0 && orders->at(orderIndex-1).childList.size() == 0 && orders->at(orderIndex-1).seniorList.size() == 0 ) // if order has 0 adult 0 child and 0 senior
                        {
                           orders->erase(orders->begin() + orderIndex-1); // remove the order
                        }

                        break; // break out of update order loop
                     }





                  }
                  else if(updateMenuoption == "3")// Cancel Order
                  {

                     for(long long unsigned int x = 0; x < orders->at(orderIndex-1).adultList.size(); x++) // goes through every element of the adult vector and unreserves each seat
                     {
                        unreserveSeats(*theater, orders->at(orderIndex-1).adultList.at(x).row,orders->at(orderIndex-1).adultList.at(x).column,1,0,0, totalSeats); // unreserve the seats
                     }

                     for(long long unsigned int y = 0; y < orders->at(orderIndex-1).childList.size(); y++) // goes through every element of the child vector and unreserves each seat
                     {
                        unreserveSeats(*theater, orders->at(orderIndex-1).childList.at(y).row,orders->at(orderIndex-1).childList.at(y).column,0,0,1, totalSeats); // unreserve the seats
                     }

                     for(long long unsigned int z = 0; z < orders->at(orderIndex-1).seniorList.size(); z++)  // goes through every element of the senior vector and unreserves each seat
                     {
                        unreserveSeats(*theater, orders->at(orderIndex-1).seniorList.at(z).row,orders->at(orderIndex-1).seniorList.at(z).column,0,1,0, totalSeats); // unreserve the seats
                     }

                     orders->erase(orders->begin() + orderIndex-1); // erase the order from the vector


                     break; // break out of update order loop
                  }
               }
            }
            else if(menuoption == "4") // Display Receipt
            {
               displayReceipt(userMap, username); // call DisplayReceipt function
            }
            else // Log out
            {
               break; // break out of theater interface loop
            }

         }

      }

   }


   return 0;
}
