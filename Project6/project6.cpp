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
    cout << "id = " << this_thread::get_id() << "的线程 打印的gvec值是"
        << gvec[0] << gvec[1] << gvec[2] << endl;
}


class A
{
public:
    void inMsgRecv()
    {
        for (int i = 0; i < 1000; ++i)
        {
            unique_lock<mutex> sguard1(myMutex);
            cout << "inMsgRecv()执行， 插入一个元素 " << i << endl;
            msgRecv.push_back(i);
            //notify_one可能唤醒无效(没有wait堵塞)
            my_cond.notify_one();  //尝试唤醒wait()线程
            //my_cond.notify_all();  //唤醒所有wait()线程
        }
    }

    void outMsgRecv()
    {
        int command = 0;
        while (true) {
            unique_lock<mutex> sguard1(myMutex);
            //wait(lock, 可调用对象)
            //如果对象返回是true，直接返回
            //如果对象返回是false，wait解锁互斥量，并堵塞本行，直到其他某个线程调用notify_one()成员函数为止
            //如果wait不存在第二个参数，默认第二个对象返回false
            //当使用notify_one唤醒wait后，wait正常工作，
            // a）wait不断尝试获取互斥量锁，获取不到会继续卡在wait，获取到继续执行b
            // b1）wait有第二个参数，就判断lambda表达式，如果表达式为false，那wait又对互斥量解锁，又休眠等待唤醒     
            //                                       如果表达式为true，wait正常返回，流程走下来（互斥锁被锁）
            // b2）wait没有第二个参数，则wait返回，流程走下来
            my_cond.wait(sguard1, [this] {  //lambda可调用对象
                if (!msgRecv.empty())
                    return true;
                return false;
                });
            command = msgRecv.front();
            msgRecv.pop_front();
            cout << "outMsgRecv()执行， 取出一个元素 " << command << " thread_id:" << this_thread::get_id() <<  endl;
            sguard1.unlock();  //unique_lock可随时解锁
            //process(data);
        }
    }


private:
    list<int> msgRecv;
    mutex myMutex;
    condition_variable my_cond;  //生成条件对象
};


int main()
{
    A myobja;
    thread myOut(&A::outMsgRecv, &myobja);
    thread myOut2(&A::outMsgRecv, &myobja);
    thread myIn(&A::inMsgRecv, &myobja);
    myOut.join();
    myOut2.join();
    myIn.join();

    //condition_variable条件对象
    //wait等待
    //notify_one唤醒
    //notify_all()

    return 0;
}



