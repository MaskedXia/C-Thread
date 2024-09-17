// project4.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <mutex>

using namespace std;

vector<int> gvec{ 1, 2, 3 }; //全局

void myPrint(int inum)
{
    //cout << "线程开始执行了start" << inum << endl;
    //cout << "线程执行结束了over" << inum << endl;

    cout << "id = " << std::this_thread::get_id() << "的线程 打印的gvec值是"
        << gvec[0] << gvec[1] << gvec[2] << endl;

}

//注释：     先CTRL + K，然后CTRL + C
//取消注释： 先CTRL + K，然后CTRL + U

class A
{
public:
    void inMsgRecvQueue()
    {
        for (int i = 0; i < 1000; ++i)
        {
            cout << "inMsgRecv()执行， 插入一个元素 " << i << endl;
            //myMutex.lock();  // 实际过程中间存在其他代码，保护不同数据共享块
            //myMutex2.lock();
            lock(myMutex, myMutex2);
            lock_guard<mutex> sguard1(myMutex, adopt_lock);  // adopt_lock（结构体对象，起标记的作用）不构造lock()函数
            lock_guard<mutex> sguard2(myMutex2, adopt_lock);
            msgRecv.push_back(i);
            //myMutex.unlock();
            //myMutex2.unlock();
        }
    }

    bool outMsgProc(int& command)
    {
        //lock_guard<mutex> sguard(myMutex);
        // lock_guard构造函数执行mutex::lock()，析构函数执行mutex::unlock()
        //myMutex2.lock();
        //myMutex.lock();
        lock(myMutex, myMutex2);
        lock_guard<mutex> sguard1(myMutex, adopt_lock);
        lock_guard<mutex> sguard2(myMutex2, adopt_lock);
        if (!msgRecv.empty()) {
            command = msgRecv.front();
            msgRecv.pop_front();
            //myMutex.unlock();
            //myMutex2.unlock();
            return true;
        }
        //myMutex.unlock();
        //myMutex2.unlock();
        return false;
    }

    void outMsgRecvQueue()
    {
        int command = 0;
        for (int i = 0; i < 1000; ++i)
        {
            bool result = outMsgProc(command);
            if (result == true)
            {
                cout << "outMsgRecv()执行，取出一个元素 " << command << endl;
            }else
            {
                cout << "outMsgRecv()执行，但目前消息队列为空 " << i << endl;
            }
        }
        cout << "END" << endl;
    }

private:
    list<int> msgRecv;
    mutex myMutex;  // two lock验证死锁
    mutex myMutex2;
};

 
int main()
{
    // 一：创建和等待多个线程
    // 多个线程执行顺序是乱的，和操作系统内部调度机制有关
    // thread放到容器里，方便管理

    //vector<thread> mythreads;
    //for (int i = 0; i < 10; ++i)
    //{
    //    mythreads.push_back(thread(myPrint, i));
    //}
    //for (auto it = mythreads.begin(); it != mythreads.end(); ++it)
    //{
    //    it->join();
    //}
    //cout << "hello, cpp" << endl;

    // 二：数据共享问题分析
    // 2.1只读数据是安全的
    // 2.2有读有写（读的时候不能写，写的时候不能读）
    // 互斥量

    // 三：共享数据的保护案例
    // 网络游戏服务器。一个线程收集玩家命令，并把命令写到队列
    //               另一个线程从队列取出命令解析，执行操作
    //用成员函数作为线程函数
    A myobja;
    thread myOut(&A::outMsgRecvQueue, &myobja);
    thread myIn(&A::inMsgRecvQueue, &myobja);
    myOut.join();
    myIn.join();

    // unique_lock取代lock_guard，更灵活
    // 

    // 保护共享数据，操作时，用代码把共享数据锁住，其他线程必须等待解锁
    // 互斥量（mutex）的基本概念
    // 互斥量是个类对象，多个线程尝试用lock()成员函数来加锁，只有一个线程能够锁定成功
    // 成功的标志是lock()返回，如果没锁成功，流程卡在lock()尝试去锁
    // 互斥量使用，保护少了没达到效果，多了影响效率
    
    // lock()和unlock()成对使用   
    // 防止unlock()，引入lock_guard的类模板，可以直接取代lock()和unlock() RAII
    // lock_guard<mutex> sguard1(myMutex) 就不需要lock()和unlock()
    
    // 死锁
    // 至少两个互斥量，两个锁
    // 线程A执行的时候，先锁住了锁1，lock()成功，去lock锁2
    // 出现了上下文切换
    // 线程B执行的时候，先锁住了锁2，lock()成功，去lock锁1
    // 线程A锁不了锁2，流程走不下去；线程B也是一样
    // 死锁一般解决方案：保持顺序一致
    // lock()函数模板，一次锁住两个或多个互斥量（谨慎使用），没锁住会阻塞
    // 要么多个互斥量都锁住，要么都没锁着，只锁一个，另一个没锁成功，会立即把已经锁住的释放
    // lock(myMutex, myMutex2);
    // 防止unlock，使用lock_guard的adopt_lock
    

    // unique_lock 取代 lock_guard， 见project6



    return 0;
}



