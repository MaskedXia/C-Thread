// project4.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <mutex>

using namespace std;

vector<int> gvec{ 1, 2, 3 }; //ȫ��

void myPrint(int inum)
{
    cout << "id = " << this_thread::get_id() << "���߳� ��ӡ��gvecֵ��"
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
            cout << "inMsgRecv()ִ�У� ����һ��Ԫ�� " << i << endl;
            msgRecv.push_back(i);
            //notify_one���ܻ�����Ч(û��wait����)
            my_cond.notify_one();  //���Ի���wait()�߳�
            //my_cond.notify_all();  //��������wait()�߳�
        }
    }

    void outMsgRecv()
    {
        int command = 0;
        while (true) {
            unique_lock<mutex> sguard1(myMutex);
            //wait(lock, �ɵ��ö���)
            //������󷵻���true��ֱ�ӷ���
            //������󷵻���false��wait���������������������У�ֱ������ĳ���̵߳���notify_one()��Ա����Ϊֹ
            //���wait�����ڵڶ���������Ĭ�ϵڶ������󷵻�false
            //��ʹ��notify_one����wait��wait����������
            // a��wait���ϳ��Ի�ȡ������������ȡ�������������wait����ȡ������ִ��b
            // b1��wait�еڶ������������ж�lambda���ʽ��������ʽΪfalse����wait�ֶԻ����������������ߵȴ�����     
            //                                       ������ʽΪtrue��wait�������أ�������������������������
            // b2��waitû�еڶ�����������wait���أ�����������
            my_cond.wait(sguard1, [this] {  //lambda�ɵ��ö���
                if (!msgRecv.empty())
                    return true;
                return false;
                });
            command = msgRecv.front();
            msgRecv.pop_front();
            cout << "outMsgRecv()ִ�У� ȡ��һ��Ԫ�� " << command << " thread_id:" << this_thread::get_id() <<  endl;
            sguard1.unlock();  //unique_lock����ʱ����
            //process(data);
        }
    }


private:
    list<int> msgRecv;
    mutex myMutex;
    condition_variable my_cond;  //������������
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

    //condition_variable��������
    //wait�ȴ�
    //notify_one����
    //notify_all()

    return 0;
}



