/*
 * Compile with 'g++ signal.cpp -std=c++11 -pthread
 */

#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>

using namespace std;
 
condition_variable CondVar;	// signalling variable
mutex Mtx;					// exclusion variable
int i = 0;					// global variable (protected by Mtx) 
bool done = false;			// global variable (protected by Mtx) 
 
void waits()
{
    unique_lock<mutex> Lck(Mtx);	// create a unique lock and lock the mutex
    cout << "Waiting... \n";
    CondVar.wait(Lck, []{return i == 1;});	// wait until CondVar is signaled, releases lock during waiting and relocks after signal arrived; lambda function to check if condition met (accessing protected variable)
    cout << "...finished waiting. i == 1\n";
    done = true;	// accessing protected variable
	Lck.unlock();	// releasing the mutex (not necessary, since lock is dectructed when out of scope)
}
 
void signals()
{
    this_thread::sleep_for(chrono::seconds(1)); // simple sleep
    cout << "Notifying falsely...\n";
    CondVar.notify_one();	// waiting thread is notified with i == 0.
							// CondVar.wait wakes up, checks i, and goes back to waiting
 
    unique_lock<mutex> Lck(Mtx);	// create a unique lock and lock the mutex
    i = 1;	// accessing protected variable
    while (!done) // accessing protected variable
    {
        cout << "Notifying true change...\n";
        Lck.unlock();
        CondVar.notify_one();	// waiting thread is notified with i == 1, CondVar.wait returns
								// notify does not need locking
        this_thread::sleep_for(chrono::seconds(1));
        Lck.lock();	// lock mutex (to access the protected variable)
    }
}
 
int main()
{
    thread t1(waits), t2(signals); // start threads
    t1.join(); 
    t2.join();
}