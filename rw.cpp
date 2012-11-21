/*
 * Title: The incomplete solution for the classic Reader-Writer Problem
 * Class: CPSC 457 Fall 2012
 * Author: Mea Wang
 * Copyright (c) 2012 Mea Wang. All rights reserved.
 */

#include <iostream>         // for cout
#include <pthread.h>        // for threads
#include <stdlib.h>         // for rand()
#include <vector>
#include <fcntl.h>          // for open() and close() system calls


using namespace std;

// Constants from command-line arguments
int NUM_WRITERS;              // The number of writers
int NUM_READERS;               // The number of readers
int MAX_CHARS;                // The maximum length of the character sequence for each write operation
int MAX_TURNS;                // The maximum number of time that a writer repeats

void *writerThread(void *prt);
void *readerThread(void *prt);

int main(int argc, char *argv[])
{
    vector<pthread_t> writers;        // the producers
    vector<pthread_t> readers;        // the consumers
    pthread_attr_t attr;
    
    if (argc != 5) {
        cerr << "Usage: rw <M> <N>" << endl;
        exit(1);
    }
    
    // Parsing the command-line arguments
    NUM_READERS = atoi(argv[1]);
    NUM_WRITERS = atoi(argv[2]);
    MAX_CHARS = atoi(argv[3]);
    MAX_TURNS = atoi(argv[4]);
    if (NUM_WRITERS <= 0 || NUM_READERS <= 0 || MAX_CHARS <= 0 || MAX_TURNS <= 0) {
        cerr << "Arguments must be non-negative." << endl;
        exit(1);
    }
    
    // Initialize the thread attributes
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    // Creating N readers
    for(int i = 0; i < NUM_READERS; i++)
    {
        pthread_t r;
        int status = pthread_create(&r, &attr, readerThread, (void *) i);
        if (status < 0)
        {
            cerr << "Failed to create consumer threads, exiting." << endl;
            exit(1);
        }
        readers.push_back(r);
    }
    
    // Creating M writers
    srand(10);                          // Seed the random number generator
    for(int i = 0; i < NUM_WRITERS; i++)
    {
        pthread_t w;
        if( pthread_create(&w, &attr, writerThread, (void *) i) )
        {
            cerr << "Failed to create writer threads, exiting." << endl;
            exit(1);
        }
        writers.push_back(w);
    }
    
    // Wait for all writers
    for(int i = 0; i < NUM_WRITERS; i++)
        pthread_join(writers[i], NULL);
        
    // Terminate all readers
    for(int i = 0; i < NUM_READERS; i++)
        pthread_join(readers[i], NULL);
    
    // Cleanup
    writers.clear();
    readers.clear();
    
    return 0;
}

void *writerThread(void * n)
{
    int num = (int) ((unsigned long) n % 100);       // Reader ID
    int writer;             // The descriptor to the character device
    const char* const a_to_z = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    char * buf;             // The random sequence to be output to the device.
    int wBytes;             // The number of bytes successfully output to the device
    
    writer = open("/dev/char_device", O_WRONLY);
        
    // Generate a sequence of k characters, where k is a random number between 1 and 20
    srand(10);                          // Seed the random number generator
    buf = new char[20];
    for (int i = 0; i < 20; i++)
        buf[i] = (char) a_to_z[rand() % 52];

    // Write the sequence of character to the device
    wBytes = write(writer, buf, 20);
    delete[] buf;
                    
    // Close the device
    cout << "Writer " << num << ":      " << wBytes << endl;
    close(writer);
    sleep(1);
    
    pthread_exit(NULL);
}

void *readerThread(void * n)
{
    int num = (int) ((unsigned long) n % 100);       // Reader ID
    int reader;             // The descriptor to the character device
    char * buf;             // The random sequence read from the device.
    int k = 20;             // The number of bytes to read at a time
    int rBytes = 0;         // The number of bytes successfully read from the device
    

    reader = open("/dev/char_device", O_RDONLY);

    // Read the sequence of characters from the device
    buf = new char[k];
    rBytes = read(reader, buf, 0);
    delete[] buf;
        
    // Close the device
    cout << "Reader " << num << ":                  " << rBytes << endl;
    close(reader);
    sleep(1);
    
    pthread_exit(NULL);
}
