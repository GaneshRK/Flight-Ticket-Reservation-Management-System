✈️ Flight Ticket Reservation Management System
A robust, console-based application written in C (ISO C99) that simulates a flight ticket reservation system using a Queue implemented via a Linked List.
The system is designed to handle common booking tasks, manage unique ticket IDs, and persist data between sessions using a binary file.

✨ Features
Dynamic Queue Implementation: Uses a linked list for flexible memory management.
Unique Ticket IDs: Automatically assigns sequential ticket IDs starting from 1001.

Core Queue Operations:
Book Ticket (Enqueue): Adds a new passenger to the rear of the queue.
Serve Customer (Dequeue): Removes the passenger at the front of the queue.

Booking Management:
Cancel Ticket by ID: Allows removal of any booking (front, middle, or rear) using its unique ID.
Search by ID: Finds and displays details for a specific ticket.
Count Bookings: Displays the total number of active reservations.
Data Persistence: Automatically saves and loads booking data to/from a binary file (bookings.dat) upon startup and exit.

⚙️ Compilation and Execution
Prerequisites
A C compiler (e.g., GCC, Clang)
Standard C libraries (stdio.h, stdlib.h, string.h)

Compile
Use the following command to compile the source code (flight_reservation.c):
Bash
gcc -std=c99 -Wall -o flight_reservation flight_reservation.c
Run
Execute the compiled program from your terminal:

Bash

./flight_reservation
