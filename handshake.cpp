#include <iostream>
#include <mutex>
#include <condition_variable>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <string>
// include additional necessary headers

using namespace std;

mutex mut;
condition_variable synReady;
condition_variable ackReady;
bool initiate = true;


void query(int num) {
    // Should print: the print number (starting from 0), "SYN", and the three dots "..."
    for (int i = 0; i < num; i++) {
        unique_lock<mutex> ul(mut);
        synReady.wait(ul, [&](){return initiate;});
        cout << "[" << i << "] " << "SYN ... ";
        initiate = false;
        ul.unlock();
        ackReady.notify_one();
    }
}

void response(int num) {
    // Should print "ACK"
    for (int j = 0; j < num; j++) {
        unique_lock<mutex> ul(mut);
        ackReady.wait(ul, [&](){return !initiate;});
        cout << "ACK" << endl;
        initiate = true;
        ul.unlock();
        synReady.notify_one();
    }


}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: ./handshake <count>\n");
        exit(1);
    }

    /**
     * Steps to follow:
     * 1. Get the "count" from cmd args
     * 2. Create necessary synchronization primitive(s)
     * 3. Create two threads, one for "SYN" and the other for "ACK"
     * 4. Provide the threads with necessary args
     * 5. Update the "query" and "response" functions to synchronize the output
    */
 
    string countInput = argv[1];
    int count = stoi(countInput);

    condition_variable ackReady;
    condition_variable synReady;
    mutex mut;
 
    thread synThread(query, count);
    thread ackThread(response, count);
   
    synThread.join();
    ackThread.join();
    
    return 0;
}