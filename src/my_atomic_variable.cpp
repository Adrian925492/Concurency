/* <atomic> - atomic varible - in other words is variable safe in case of threads.
That means, that usage of thar variable will prevent us from data race.

Data race - if one hhread tries to read from variable when other thread writes to it.

Atomic variable has well defined behaviour - read thread will wain untill write thread
writes to the variable.

Atomic variable is good way to pass data between threads, but has nothing to thread synchronisation - the read thread will wait only
when write thread uses the variable. In other way - just stored data will
be read.

Atomic variable is not movable and copyable.

Atomic variable is template, so we can make variable of any type which is
copy constructible and copy asignable.

Methods:

.load() = get the value, it alreade written by other thread, wait and than het the value.
.store() or operator=() - stores the value to atomic variable
.exchange(x) - stores new value (X) TO VARIABLE AND RETURNS PREVIOUS ONE
.is_lock_free() - returns true, if atomic variable is not locked (nobody writes to it at the time)

Construction:

-> no argument construction: std::atomic<T> var();

Usage:

std::atomic<T> a; - creates atmic variable a for storing data type T.

Atomic variable has overloaded increment and decrement operators (++, --) and +=, -=, and bitwise (&=, |=, ^=)

*/

#include <thread>
#include <iostream>
#include <chrono>
#include <atomic>

using namespace std;

// SIMPLE EXAMPLE OF COUNTER BASED ON ATOMIC VARIABLE

atomic<int> var(0);     //Initialize atomic variable and assign value

void atomic_var_fcn1(void)
{
    for (int i = 0; i < 10; ++i)
    {
        cout << "Thread 1\n" << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
        var += 1;   //Increment atimic variable by 1
    }
}

void atomic_var_fcn2(int param)
{
    for (int i = 0; i < 10; ++i)
    {
        cout << "Thread 2, param(atomic variable): " << var.load() << "\n" << endl;
        this_thread::sleep_for(chrono::milliseconds(150));
    }
}

void atomic_example()
{
    cout << "Atomic example >>>>>>>>>>>>> \n\n";
    
    thread t1;                  //Initialize empty thread object
    thread t2(atomic_var_fcn2, 5);  //Initialize thread and assign function to it, run it at once
    t1 = thread(atomic_var_fcn1);   //Assign thread to t1, run it
    thread t3 = thread([]()
    { 
        cout << "Thread 3 - by lambda notation\n";
        this_thread::sleep_for(chrono::milliseconds(250));
        int a = var.exchange(0);        //Exchange value of atomic variable - line restart the counter
        cout << "atomic variable exchanged to 0, prev value: " << a << endl;
    
    });  //Create and assign thread 3 - by lambda

    if (t1.joinable())          //Check if threads are joinable and join all of them
    {
        t1.join();
    }
    if (t2.joinable())
    {
        t2.join();
    }
    if (t3.joinable())
    {
        t3.join();
    }

    cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n";
}



