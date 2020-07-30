#include <thread>

/* <thread> - the stadard library containing basic tools for operating with threads. 

Threads represents concurently executable functions. std::thread defines thread object, which is 
representation of single thead of execution. 

Thread is executed immidietly after construction. The <thread> provides basic api for low-level 
OS dependent multithreading mechanisms. Not all features provided by lower-level, OS specific threads
API are provided by standard API (for example, like ability of cancelling thread from other 
thread as provided for pthread lib.)

The function for thread is provided by constructor argumet. Thread can exit iself by finishing
thread function execution. 

Threads has 2 special abilities:
-> Joining - means, that other thread can wait for joined thread. The waiting thread will
do nothing, untill waited thread will finish its execution.
-> Detachong - means, that one thread will do not care about other thread, even if
the other thread has been created by the thread, and thread object (playing thread handler role)
would be lost. It permits the thread to execute independently.

Both, joining and detaching can be done only once on thread (by one thread).

.join() - wait for thread
.detach() - allow thread to run independently (rarely used, be careful! - detached thread
is not quaranteed to release all its resources when parent thread exits)

Thread has joinable() method, which informs, if the thread has already started its 
concurent execution and can be waited for or detached. Beforejoining the thread we should
check it, to prevent, that we will join unstarted (yet) thread.

.swap() - Allws to swap 2 thread objects each other

.operator=() - Allows to move thread object (assign) given by argument to our thread.

Thread id - each thread has its unique number (int) - id.
.get_id() - Returns thread's id.
.native_handle() - Returns underlaying (native API type) thread handler object

Thread initialization - constructors

-> thread()- initializes empty thread object
-> thread(thread) - copy thread passed by argument
-> template<F, Args> thread(f, args) - initializes thread object with f (of return type F) as 
    thread function (can be functor, function, method, lambda), and allows to pass arguments
    of the thread function by args (type of args we pass in template as Args).

! If You use threads with UNIX based system, the base API if std::thread is pthread. You have to add
-pthread flag to compilation to let it run concurently.

! In non realtime OS schedule of thread execution is undefined - nobody knows which thread will be running in time
and when contex switching wil occur.

*/

#include <thread>
#include <iostream>
#include <chrono>

using namespace std;

void thread_fcn1(void)
{
    cout << "Thread 1\n" << endl;
    this_thread::sleep_for(chrono::milliseconds(100));
}

void thread_fcn2(int param)
{
    cout << "Thread 2, param(int): " << param << "\n" << endl;
    this_thread::sleep_for(chrono::milliseconds(200));
}

void thread_example()
{
    cout << "Thread example >>>>>>>>>>>>> \n\n";
    
    thread t1;                  //Initialize empty thread object
    thread t2(thread_fcn2, 5);  //Initialize thread and assign function to it, run it at once
    t1 = thread(thread_fcn1);   //Assign thread to t1, run it
    thread t3 = thread([](){ cout << "Thread 3 - by lambda notation\n";});  //Create and assign thread 3 - by lambda

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


