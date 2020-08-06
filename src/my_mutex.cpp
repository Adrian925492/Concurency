
/*
Mutex - simple mechanism for provideng thread-safety mechanism for resource access.
<mutex>

Mutex can be used to protect shared data being simultanously read by more than one thread.

If a thread wants to use protected resource, it takes the mutex. When mutex is taken, no other thread can access the resource.
When mutex owner finish using the resource, it releases the mutex, so other threads can use the resource.

std::mutex m;

Methods:
.lock() - Gets the mutex. If the mutex is not free, the thread will wait here untill mutex will be free.
.try_lock() - Tries to get the mutex. If mutex is free, the thread gets it and method returns true. If not, mutex is not locked, and method returns false.
.unlock() - Releases the mutex

.native_handle() - Returns unerlaying implementation of mutex object handler.

Specialities:

std::unique_lock(m) - Specifies unique lock object on mutex m. We can use the lock object later, swap it or change ownership without giving mutex itself.
    It is movable, but not copyable. Only owner object can use the lock, but it can be moved.
std::shared_lock(m) - Creates shared lock objeo on mutex m. Shared means, that shared ownership can be used (anu object can share the lock).It is not copyable but is movable.

Shared vs Unique
In both cases resource can be locked only once, but in shared it can be done by shared owners (more than one), and in unique - only one, unique owner.

Methods:
.lock(), .try_lock() and .unlock() - Has same behaviour as on mutex itself.
.lock_for(tieout) - Locks mutex for specified timeout (cnrono type)
.lock_untill(time) - Locks mutex untill specified time will be reached.

Deadlock problem:

When we have two threads, t1 and t2, and 2 mutex m1 and m2. T1 takes the mutex m1 and T2 takes mutex m2. However, t1 wants also mutex m2, so it has to wait,
beacouse it is already taken by thread t2. Similarly, thread t2 needs mutex m1 before it will release mutex m2. Such situation is called deadlock.

Deadlock prevention using lock objects.

If we have lock objects, we can use feature of paralell locking both (or more) mutex at once.

std::lock(m1, m2) <- This will lock both mutexes only, if both will be free. If not, it will wait (for both). Such lock will be released after going
out of scope, example:

std::try_lock(m1, m2, ...mn) <- This will try to lock all given mutexes at once, and will lock and return true only if all would be free. Otherwise, it returns
false and mo mutex would be locked.

{   //scope start
    unique_lock<mutex> l1(m1);
    unique_lock<mutex> l2(m2);

    lock(m1, m2);   //Here it will be locked

    ...

}   //and here both will be released (going out of scope).

std::lock_guard<mutex> lk(m) - Allows automaticallu lock mutex m. The lock is autoamtically released after going out of scope (like in above example with lock() function)

*/

#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>

using namespace std;

mutex m;
mutex m2;
int shared_variable = 1;

void mutex_fcn1(void)
{
    for (int i = 0; i < 20; ++i)
    {
        m.lock();   //Standard lock usage
        cout << "Thread 1: increment shared variable \n" << endl;
        ++shared_variable;
        m.unlock();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void mutex_fcn2(void)
{
    for (int i = 0; i < 20; ++i)
    {
        {   //Scope for lock guard
            lock_guard<mutex> lk(m);  //Locks mutex untill end of the scope
            cout << "Thread 2, decrement shared variable\n" << endl;
        }
            this_thread::sleep_for(chrono::milliseconds(200));
    }
}

void mutex_fcn3(void)
{
    for (int i = 0; i < 20; ++i)
    {
        {   //Scope for both lock
            unique_lock<mutex> l1(m);     //Unique_lock objs created - unique only for the thread
            unique_lock<mutex> l2(m2);

            if(try_lock(m, m2))  //Locks mutex untill end of the scope
            {
                cout << "Shared variable value: " << shared_variable << endl;
            }
        }
            this_thread::sleep_for(chrono::milliseconds(200));
    }
}

void mutex_example()
{
    cout << "Mutex example >>>>>>>>>>>>> \n\n";
    
    m2.lock();
    thread t1;                  //Initialize empty thread object
    thread t2(mutex_fcn2);     //Initialize thread and assign function to it, run it at once
    t1 = thread(mutex_fcn1);   //Assign thread to t1, run it
    thread t3 = thread(mutex_fcn3); //Create thread 3
    m2.unlock();


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