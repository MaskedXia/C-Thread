#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <mutex>
#include <future>

using namespace std;

atomic<int> g_value = 0;  //原子操作
atomic<bool> g_bool = false;
mutex g_mutex;



void mythread()
{
    for (int i = 0; i < 10000000; i++)
    {
        //g_mutex.lock();
        g_value++;   //atomic<int>不再需要互斥量
        //g_value = g_value + 1;  //值就不准确
        //g_mutex.unlock();
    }
}

int mythread2()
{
    cout << "mythread2 function start " << " threadid = " << this_thread::get_id() << endl;
    chrono::milliseconds dura(2000);
    while (!g_bool) {
        cout << "mythread2 function running " << " threadid = " << this_thread::get_id() << endl;
        this_thread::sleep_for(dura); //休息2s
    }
    cout << "mythread2 function end " << " threadid = " << this_thread::get_id() << endl;
    return 5;
}

int main()
{
    //std::atomic，原子操作
    //无锁技术，比互斥量效率更高，针对一个变量

    // part 1 atomic<int>
    //thread myobj1(mythread);
    //thread myobj2(mythread);
    //myobj1.join();
    //myobj2.join();
    //cout << "线程执行完毕，最终得到的g_value值为：" << g_value << endl;

    // part 2 atomic<bool>
    thread myobj1(mythread2);
    thread myobj2(mythread2);
    this_thread::sleep_for(chrono::milliseconds(5000));
    g_bool = true;
    myobj1.join();
    myobj2.join();

    

    return 0;
}



