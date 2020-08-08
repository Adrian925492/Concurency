/* Waiting for events frm another thread - prmise and future object.

std::promise<T> <future> - creates object holding variable of type T, that is later (in unknown time) aqquired by future object. Promise is type of
object holding value, for which we expect to change in the future by other thread. Promise object can be used only once. Proise object can hold value, void (only state, 
may be used for synchronisation) or an excepton.

Each promise object is associated with some shared state and a result, which may be not yet evaluated, evaluated to a value (or void, or for an exception. 

Promise object is "push" object of promise - future communication way between threads. Promis is place, where we push the data, and future object is used to
get is (wait for it).

Promise can do 3 things with shared state:
-> make ready - promise object gets the value and marks it as ready to be read.
-> release - the promise gives up reference to the shared state. If this was the last such reference, the value is destroyed. Promise may not be used in the future.
-> abandon - the promise stores exception of type std::future_error with err code std::future_error::broken_promise. It akes the shared state ready and than released.

Methods:
constructor (including move and copy)
.swap(p) - swaps this promise with p promise obj
.operator=(p) - assigns shared state of other promise obj p passed as arg

.get_future() - returns future object of the promise
.set_value(x) - sets value x to the promise
.set_value_at_thread_exit(x) - sets value only when current thread is exiting.
.set_exeption(x) - stores exception x in the promise
.set_exception_at_thread_exit() - sets exception to the promise obj - only at thread exit.

std::future <future> - creates the object for accessing std::promise value and may be used to synchronize with promise state (unlock on ready state).
It can be also used with std::async and std::packaged_task objects. 

Methods:
constructor (including copy and move)
.operator=(f) - moves the future object
.share - transfers the shared state form this to a shared_future, returns shared_future object.
.get() - returns the result of associated object. Waits if result is not ready yet.
.wait() - waits for the results become available
.wait_for(x) - waits for result being avaliable with timeout x (chrono)
.wait_untill(x) - wait for the results being avaiable untill time x (chrono) is reached
.valid() - returns true if future object has a shared state () (ex. is associated with some not abandoned promise obj, package_task or async)

std::shared_future<T> - future object which can be used by multiple threads. Standard future object is usable only by one thread (has to be moved), but that one
can be used by multiple.


*/

#include <iostream>
#include <thread>
#include <future>

using namespace std;

void cv_fcn1(future<int> fut)
{
    int i = fut.get();  //Wait for value being available
    cout << "Thread 1: Read future object \n" << endl;
    cout << "Value: " << i << endl;
}

void cv_fcn2(promise<int> pr, promise<int> pr_shared)
{
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Thread 2: setting the promise \n";
    pr.set_value(2);    //Set promise value
    pr_shared.set_value(10);    //Set value for shared futures example
}

void sh_fcn1(shared_future<int> fut)   //Shared future usage: 1st thread read (uses)
{
    int i = fut.get();  //Wait for value being available
    cout << "Thread 1: Read future object \n" << endl;
    cout << "Value: " << i << endl;
}

void sh_fcn2(shared_future<int> fut)    //Shared future usage: 2nd thread read (uses)
{
    int i = fut.get();  //Wait for value being available
    cout << "Thread 1: Read future object \n" << endl;
    cout << "Value: " << i << endl;
}

void future_example()
{
    cout << "Promise - future example >>>>>>>>>>>>> \n\n";
    
    promise<int> variable;      //Create promise object
    future<int> fut = variable.get_future();    //Get future object of the variable

    promise<int> shared_varable;
    shared_future<int> shared_fut = shared_varable.get_future();

    thread t1;                  //Initialize empty thread object
    thread t2(cv_fcn2, std::move(variable), std::move(shared_varable));     //Initialize thread and assign function to it, run it at once, pass promise by move
    t1 = thread(cv_fcn1, std::move(fut));   //Assign thread to t1, run it, pass future object by move


    thread t3(sh_fcn1, shared_fut);
    thread t4(sh_fcn2, shared_fut);

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
    if (t4.joinable())
    {
        t4.join();
    }

    cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n";
}