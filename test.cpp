/*
 * Title: A test program for the character device module for the Linux kernel 3.6.2
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

    // Try to open for reading.  Should be unsuccessful
    int reader1 = open("/dev/char_device", O_RDONLY);
    if (reader1 < 0)
        cout << "Reader 1 cannot open the device for reading." << endl;
    else
        cout << "Reader 1 opened the device for reading." << endl;
        
    // Try to open for writing.  Should be unsuccessful since writer 1 has openned the device already.
    int writer2 = open("/dev/char_device", O_WRONLY);
    if (writer2 < 0)
        cout << "Writer 2 cannot open the device for writing." << endl;
    else
        cout << "Writer 2 opened the device for writing." << endl;
    
    // Close the device
    close(writer1);
    cout << "Writer 1 closed the device." << endl << endl;

    // Open in read only mode. Should be successful.
    reader1 = open("/dev/char_device", O_RDONLY);
    if (reader1 < 0)
        cout << "Reader 1 cannot open the device for reading." << endl;
    else
        cout << "Reader 1 opened the device for reading." << endl;

    // Open in read only mode. Should be successful.
    int reader2 = open("/dev/char_device", O_RDONLY);
    if (reader2 < 0)
        cout << "Reader 2 cannot open the device for reading." << endl;
    else
        cout << "Reader 2 opened the device for reading." << endl;

    // Open in write mode. Should be unsuccessful.
    writer1 = open("/dev/char_device", O_WRONLY);
    if (writer1 < 0)
        cout << "Writer 1 cannot open the device for writing." << endl;
    else
        cout << "Writer 1 opened the device for writing." << endl;
    
    // Close the device
    close(reader1);
    cout << "Reader 1 closed the device." << endl;
    close(reader2);
    cout << "Reader 2 closed the device." << endl << endl;
    
    /*************  Test for writing and reading using a writer that **********/
    /*************  has both read and write acccess to the device *************/
    // Open in write mode. Should be successful.
    writer2 = open("/dev/char_device", O_WRONLY);
    if (writer2 < 0)
        cout << "Writer 2 cannot open the device for writing." << endl;
    else
        cout << "Writer 2 opened the device for writing." << endl;


    // Generate a sequence of k characters, where k is a random number between 1 and 20
    srand(10);                          // Seed the random number generator
    const char* const a_to_z = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int k = rand() % 20 + 1;
    char * buf = new char[k];
    for (int i = 0; i < k; i++)
        buf[i] = (char) a_to_z[rand() % 52];
    
    // Write the sequence of character to the device
    int wBytes = write(writer2, buf, k);
    cout << "Writer 2 wrote " << wBytes << " bytes to the device." << endl;
    cout << "Data: ";
    for (int i = 0; i < wBytes; i++)
        cout << buf[i];
    cout << endl;
    delete[] buf;
    
    close(writer2);
    cout << "Writer 2 closed the device." << endl << endl;
    
    // Open in read only mode. Should be successful.
    reader1 = open("/dev/char_device", O_RDONLY);
    if (reader1 < 0)
        cout << "Reader 1 cannot open the device for reading." << endl;
    else
        cout << "Reader 1 opened the device for reading." << endl;

    // Open a 2nd reader. Should be successful.
    reader2 = open("/dev/char_device", O_RDONLY);
    if (reader2 < 0)
        cout << "Reader 2 cannot open the device for reading." << endl;
    else
        cout << "Reader 2 opened the device for reading." << endl;

    // Reader 1 reads the sequence of characters from the device
    buf = new char[40];
    int rBytes = read(reader1, buf, 0);
    cout << "Reader 1 read " << rBytes << " bytes from the device." << endl;
    cout << "Data: ";
    for (int i = 0; i < rBytes; i++)
        cout << buf[i];
    cout << endl;
    delete[] buf;

    // Reader 2 reads the sequence of characters from the device
    buf = new char[40];
    rBytes = read(reader2, buf, 0);
    cout << "Reader 2 read " << rBytes << " bytes from the device." << endl;
    cout << "Data: ";
    for (int i = 0; i < rBytes; i++)
        cout << buf[i];
    cout << endl;
    delete[] buf;
    
    close(reader1);
    cout << "Reader 1 closed the device." << endl;
    close(reader2);
    cout << "Reader 2 closed the device." << endl << endl;
    

    // Open for writing again
    writer2 = open("/dev/char_device", O_WRONLY);
    if (writer2 < 0)
        cout << "Writer 2 cannot open the device for writing." << endl;
    else
        cout << "Writer 2 opened the device for writing." << endl;

    // Write a new sequent
    k = rand() % 20 + 1;
    buf = new char[k];
    for (int i = 0; i < k; i++)
        buf[i] = (char) a_to_z[rand() % 52];
    wBytes = write(writer2, buf, k);
    cout << "Writer 2 wrote " << wBytes << " bytes to the device." << endl;
    cout << "Data: ";
    for (int i = 0; i < wBytes; i++)
        cout << buf[i];
    cout << endl;
    delete[] buf;

    // Write again
    k = rand() % 20 + 1;
    buf = new char[k];
    for (int i = 0; i < k; i++)
        buf[i] = (char) a_to_z[rand() % 52];
    wBytes = write(writer2, buf, k);
    cout << "Writer 2 wrote " << wBytes << " bytes to the device." << endl;
    cout << "Data: ";
    for (int i = 0; i < wBytes; i++)
        cout << buf[i];
    cout << endl;
    delete[] buf;
    
    close(writer2);
    cout << "Writer 2 closed the device." << endl << endl;


    // Open in read only mode. Should be successful.
    reader1 = open("/dev/char_device", O_RDONLY);
    if (reader1 < 0)
        cout << "Reader 1 cannot open the device for reading." << endl;
    else
        cout << "Reader 1 opened the device for reading." << endl;

    // Reader 2 reads the sequence of characters from the device
    buf = new char[20];
    rBytes = read(reader1, buf, 0);
    cout << "Reader 1 read " << rBytes << " bytes from the device." << endl;
    cout << "Data: ";
    for (int i = 0; i < rBytes; i++)
        cout << buf[i];
    cout << endl;
    delete[] buf;
    
    // Read one more time
    buf = new char[20];
    rBytes = read(reader1, buf, rBytes);
    cout << "Reader 1 read " << rBytes << " bytes from the device." << endl;
    cout << "Data: ";
    for (int i = 0; i < rBytes; i++)
        cout << buf[i];
    cout << endl;
    delete[] buf;
    
    close(reader1);
    cout << "Reader 1 closed the device." << endl;
}


