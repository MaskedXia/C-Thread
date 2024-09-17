
#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <mutex>

using namespace std;

vector<int> gvec{ 1, 2, 3 }; //ȫ��

class A
{
public:
    void inMsgRecv()
    {
        for (int i = 0; i < 100; ++i)
        {
            
            unique_lock<mutex> sguard1(myMutex, try_to_lock);

            //unique_lock<mutex> sguard1(myMutex, defer_lock);  //��ʼ��û������myMutex
            //sguard1.lock(); //����unlock

            //sguard1.unlock();  // ����ǹ������
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
            // mutex* ptx = sguard1.release(); // ��ptx�ӹ�sguard1

            if (sguard1.owns_lock())
            {
                msgRecv.push_back(i);
                cout << "inMsgRecv()ִ�У� ����һ��Ԫ�� " << i << endl;
            }
            else
            {
                cout << "inMsgRecv()ִ�У�����ȡ��ʧ��... " << endl;
            }

        }
    }

    bool outMsgProc(int& command)
    {
        unique_lock<mutex> sguard1(myMutex);

        chrono::milliseconds durtime(100); //1s
        this_thread::sleep_for(durtime);  //��Ϣ1s�����µȴ�

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
                cout << "outMsgRecv()ִ�У�ȡ��һ��Ԫ�� " << command << endl;
            }
            else
            {
                cout << "outMsgRecv()ִ�У���Ŀǰ��Ϣ����Ϊ�� " << i << endl;
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
     // unique_lockȡ��lock_guard��������ֱ��ȡ����Ч�ʻ�ͣ�
     // 1��adopt_lock����ʾ����������Ѿ���lock����Ҫ��ǰlock��
     // 2��try_to_lock������lock()����mutex��û�����ɹ������������أ�����������owns_lock�ж�ȡ����
     // 3��defer_lock����ʼ��û�м�����mutex

    // unique_lock��Ա����
    // lock()
    // unlock()
    // try_lock()�����Լ������ɹ�true��ʧ��false
    // release()���������������mutex����ָ�룬���ͷ�����Ȩ��unique_lock��mutex������ϵ

    //����Ȩ����
    //��ֵ��Ҫmove()  ��ֵ�Զ�����

    A myobja;
    thread myOut(&A::outMsgRecv, &myobja);
    thread myIn(&A::inMsgRecv, &myobja);
    myOut.join();
    myIn.join();
}