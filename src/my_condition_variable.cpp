/*
<condition_variable>

Condition variable - multithread synchronisation mechanism. Condition variable allows to synchronise 2 (or more) threads, but keeping control
on notifing thread.

It is synchronisation object, which can also pass value. Kind of semaphore (can be counting). 

Assume thread t1 has to be synchronised with thread t2 on condition variable. Thus, t2 waits on condition variable untill someone notifies it to be
freed. Condition variable, wnen notified, allows the wait threa dto go forward.

Methods:
.wait(unique_lock lk, pred) - blocks the current thread untill condition variable would be notified.
.wait_for(unique_lock lk, timeout, pred) - blocks the current thread untill cond variable will be notified or timeout will expir (chrono)
.wait_untill(unique_lock lk, time, pred) - blocks the current thread untill cond variable will be notified or time will be reached (chrono)

! All waits wait for some unique lock (ex. mutex). Predictate (pred) is functor, function or lambda defining what happend if we wnat the wait to
be continued. It shall return true, if wait should be continued.

.notify_one() - Allows to notify cond variable. Only one of waiting thread will be passed to go on.
.notify_all() - Allows all waiting threads to be passed to go on (notified).

.native_handle() - Returns native handler of the cond variable.

*/

#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std;

int var1 = 0;
mutex m1;
condition_variable cv;

void cv_fcn1(void)
{
    for (int i = 0; i < 20; ++i)
    {
        {
        lock_guard<mutex> lk(m1);
        cout << "Thread 1: increment shared variable \n" << endl;
        ++var1;
        }
        cv.notify_one();    //Notify condition variable
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void cv_fcn2(void)
{
    for (int i = 0; i < 20; ++i)
    {
        {
        unique_lock<mutex> lk(m1);
        cout << "T2 waiting...\n";
        cv.wait(lk, [](){return var1 % 2 == 0;});    //Wait for cond variable - only oven will be shown
        cout << "Thread 2, Reader\n" << endl;
        cout << "Value: " << var1 << "\n";
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}


void condition_variable_example()
{
    cout << "Condition variable example >>>>>>>>>>>>> \n\n";
    
    thread t1;                  //Initialize empty thread object
    thread t2(cv_fcn2);     //Initialize thread and assign function to it, run it at once
    t1 = thread(cv_fcn1);   //Assign thread to t1, run it


    if (t1.joinable())          //Check if threads are joinable and join all of them
    {
        t1.join();
    }
    if (t2.joinable())
    {
        t2.join();
    }

    cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n";
}

