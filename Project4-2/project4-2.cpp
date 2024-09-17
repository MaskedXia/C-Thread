
#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <mutex>

using namespace std;

vector<int> gvec{ 1, 2, 3 }; //全局

class A
{
public:
    void inMsgRecv()
    {
        for (int i = 0; i < 100; ++i)
        {
            
            unique_lock<mutex> sguard1(myMutex, try_to_lock);

            //unique_lock<mutex> sguard1(myMutex, defer_lock);  //初始化没加锁的myMutex
            //sguard1.lock(); //不用unlock

            //sguard1.unlock();  // 灵活处理非共享代码
            //...
            //sguard1.lock();

            //if (sguard1.try_lock() == true) 
            //{
            //    //...
            //}
            //else
            //{
            //    //...
            //}

            // unique_lock<mutex> sguard1(myMutex);
            // mutex* ptx = sguard1.release(); // 由ptx接管sguard1

            if (sguard1.owns_lock())
            {
                msgRecv.push_back(i);
                cout << "inMsgRecv()执行， 插入一个元素 " << i << endl;
            }
            else
            {
                cout << "inMsgRecv()执行，尝试取锁失败... " << endl;
            }

        }
    }

    bool outMsgProc(int& command)
    {
        unique_lock<mutex> sguard1(myMutex);

        chrono::milliseconds durtime(100); //1s
        this_thread::sleep_for(durtime);  //休息1s，导致等待

        if (!msgRecv.empty()) {
            command = msgRecv.front();
            msgRecv.pop_front();
            return true;
        }
        return false;
    }

    void outMsgRecv()
    {
        int command = 0;
        for (int i = 0; i < 100; ++i)
        {
            bool result = outMsgProc(command);
            if (result == true)
            {
                cout << "outMsgRecv()执行，取出一个元素 " << command << endl;
            }
            else
            {
                cout << "outMsgRecv()执行，但目前消息队列为空 " << i << endl;
            }
        }
        cout << "END" << endl;
    }

private:
    list<int> msgRecv;
    mutex myMutex;
};


int main()
{
     // unique_lock
     // unique_lock取代lock_guard，更灵活（可直接取代，效率会低）
     // 1、adopt_lock，表示这个互斥量已经被lock（需要提前lock）
     // 2、try_to_lock，尝试lock()锁定mutex，没锁定成功，会立即返回，不会阻塞（owns_lock判断取锁）
     // 3、defer_lock，初始化没有加锁的mutex

    // unique_lock成员函数
    // lock()
    // unlock()
    // try_lock()，尝试加锁，成功true，失败false
    // release()，返回他所管理的mutex对象指针，并释放所有权；unique_lock和mutex不再联系

    //所有权传递
    //左值需要move()  右值自动发生

    A myobja;
    thread myOut(&A::outMsgRecv, &myobja);
    thread myIn(&A::inMsgRecv, &myobja);
    myOut.join();
    myIn.join();
}