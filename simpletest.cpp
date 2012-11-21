/*
 * Title: A simple test program for the character device module for the Linux kernel 3.6.2
 * Class: CPSC 457 Fall 2012
 * Author: Mea Wang
 * Copyright (c) 2012 Mea Wang. All rights reserved.
 */
 
#include <fcntl.h>          // for open() and close() system calls
#include <stdlib.h>         // for rand()
#include <iostream>         // for cout

using namespace std;

int main (int argc, char *argv[])
{
    // Try to open in write mode again. Should be successful.
    int writer1 = open("/dev/char_device", O_WRONLY);
    if (writer1 < 0)
        cout << "Writer 1 cannot open the device for writing." << endl;
    else
        cout << "Writer 1 opened the device for writing." << endl;
    
    // Generate a sequence of k characters, where k is a random number between 1 and 20
    srand(10);                          // Seed the random number generator
    const char* const a_to_z = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    char * buf = new char[20];
    for (int i = 0; i < 20; i++)
        buf[i] = (char) a_to_z[rand() % 52];
        
    // Write the sequence of character to the device
    int wBytes = write(writer1, buf, 20);
    cout << "Writer 1 wrote " << wBytes << " bytes to the device." << endl;
    cout << "Data: ";
    for (int i = 0; i < wBytes; i++)
        cout << buf[i];
    cout << endl;
    delete[] buf;
    
    // Try to open for reading.  Should be successful with the partilly developped module, 
    // but not with the fully developed module
    int reader1 = open("/dev/char_device", O_RDONLY);
    if (reader1 < 0)
        cout << "Reader 1 cannot open the device for reading." << endl;
    else
        cout << "Reader 1 opened the device for reading." << endl;
        
    // Close the device
    close(writer1);
    cout << "Writer 1 closed the device." << endl << endl;

    // Reader 1 reads the sequence of characters from the device
    buf = new char[20];
    int rBytes = read(reader1, buf, 0);
    cout << "Reader 1 read " << rBytes << " bytes from the device." << endl;
    cout << "Data: ";
    for (int i = 0; i < rBytes; i++)
        cout << buf[i];
    cout << endl;
    delete[] buf;
    
    close(reader1);
    cout << "Reader 1 closed the device." << endl;
}


