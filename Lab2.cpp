#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

using namespace std; 

#define Num_Threads 10

int counter;
void* Method1(int incr)
{
  using namespace std::chrono;
  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  counter++; 
  high_resolution_clock::time_point t2 = high_resolution_clock::now();

  duration<double> t = duration_cast<duration<double>>(t2-t1);
  cout << counter  <<"||" << incr << " Thread time creation = "
       << t.count()<< endl;
}

bool OnePerPerson[Num_Threads] = {false};//initialize the boolean variable 
                                         //set true once cell becomes true
mutex doorlock;
void* Method2(int incr)
{
  using namespace std::chrono;
  
  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  if (!OnePerPerson[incr]) {
    doorlock.unlock();
    //something needed to be done here, so it will be  more fair and it processes the next one 
    //only after the previous one has gone through
    OnePerPerson[incr] = true;
    counter++; 
    doorlock.unlock();
  }
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  duration<double> t = duration_cast<duration<double>>(t2-t1);
  cout << counter  <<"||" << incr << " Thread time creation = "
       << t.count()<< endl;
}

int now_serving = 0;
condition_variable cv;
void* Method3(int incr)
{
   using namespace std::chrono;
   //unique_lock<mutex> lck(doorlock);
   high_resolution_clock::time_point t1 = high_resolution_clock::now();
   //now serving should be the thread to be processed otherwise lock the access
   //incr serves like a thread id and we want this process to be perform by number
   bool notified = false;

  // while (!notified)
   { 
     // cv.wait(lck);
     
     // if(now_serving == incr)
      {
        doorlock.lock();
        counter++;
        now_serving++;
        doorlock.unlock();
        notified = true;  
       // cv.notify_all();
      }
   }

   high_resolution_clock::time_point t2 = high_resolution_clock::now();
   duration<double> t = duration_cast<duration<double>>(t2-t1);
   cout << counter  <<"||" << incr << " Thread time creation = "
        << t.count()<< endl;
 
}

int main()
{
  using namespace std::chrono;
  int choice;
  high_resolution_clock::time_point t1;
  high_resolution_clock::time_point t2;
  duration<double> t;

  //while (choice != 0)
{
  cout << "The Method to test out (1, 2, or 3) and 0 to terminate the program? ";
  //cin >> choice;
  choice = 3; 
 /*----------------------------------------------------------------------------*/
 /*----------------------------------------------------------------------------*/
 /*                                Method 1                                    */
 /*----------------------------------------------------------------------------*/
 /*----------------------------------------------------------------------------*/
 if (choice == 1){
  counter = 0;
  t1 = high_resolution_clock::now();
  thread Threads[Num_Threads];

  for(int i = 0; i < Num_Threads; i++)
   {
	Threads[i] = thread(Method1, i);
   }
    
   for (int i = 0; i < Num_Threads; i++)
   {
      Threads[i].join();
    }
  
  t2 = high_resolution_clock::now();
  t = duration_cast<duration<double>>(t2-t1);

  if (counter == Num_Threads)
  {
    cout << endl << "Total thread time creation = " << t.count() << endl;
    cout << endl << "Average thread time creation = " << t.count()/Num_Threads << endl;
  }

 }
 /*----------------------------------------------------------------------------*/
 /*----------------------------------------------------------------------------*/
 /*                                Method 2                                    */
 /*----------------------------------------------------------------------------*/
 /*----------------------------------------------------------------------------*/
 if (choice == 2){ 
  counter = 0;

  t1 = high_resolution_clock::now();

  thread Threads_2[Num_Threads];

  for(int i = 0; i < Num_Threads; i++)
   {
       Threads_2[i] = thread(Method2, i);
   }
  for (int i = 0; i < Num_Threads; i++)
   {    
       Threads_2[i].join();
   }
 
 
  if (counter == Num_Threads)
  {
     t2 = high_resolution_clock::now();

     t = duration_cast<duration<double>>(t2-t1);
     cout << endl << "Total thread time creation = " << t.count() << endl;
     cout << endl << "Average thread time creation = " << t.count()/Num_Threads << endl; 
  }
  
  for(int i = 0; i < Num_Threads; i++) //reset boolean so it will rum properly
  {
    OnePerPerson[i] = false;
  }
 }
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                Method 3                                   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
  if (choice == 3){ 
  counter = 0;
  now_serving = 0;
  t1 = high_resolution_clock::now();

  thread Threads_3[Num_Threads];

  for(int i = 0; i < Num_Threads; i++)
   {
      Threads_3[i] = thread(Method3, i);
      //cout << this_thread::get_id() << endl;
   } 
   
   for(int i = 0; i < Num_Threads; i++)   
   {
     Threads_3[now_serving].join();
     //ways to identify current output thread and increment now serving variable 
   }
   
    if (counter == Num_Threads)
    {
        t2 = high_resolution_clock::now();
        Threads_3[now_serving].join(); 
        t = duration_cast<duration<double>>(t2-t1);
        cout << endl << "Total thread time creation = " << t.count() << endl;
        cout << endl << "Average thread time creation = " << t.count()/Num_Threads;
    } 

 }

}

   return 0;
}

