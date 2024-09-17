#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <mutex>
#include <future>

using namespace std;

int mythread0()
{
    cout << "mythread0 function start " << " threadid = " << this_thread::get_id() << endl;
    chrono::milliseconds dura(2000);
    this_thread::sleep_for(dura); //��Ϣ2s
    cout << "mythread0 function end " << " threadid = " << this_thread::get_id() << endl;
    return 5;
}

int mythread(int arg)
{
    cout << "arg = " << arg << endl;
    cout << "mythread function start " << " threadid = " << this_thread::get_id() << endl;
    chrono::milliseconds dura(5000);
    this_thread::sleep_for(dura); //��Ϣ5s
    cout << "mythread function end " << " threadid = " << this_thread::get_id() << endl;
    return 5;
}

void mythread2(promise<int>& tmp, int calc)
{
    calc++;
    chrono::milliseconds dura(5000);
    this_thread::sleep_for(dura); //��Ϣ5s
    int res = calc;
    tmp.set_value(res);  //������浽tmp
    cout << "mythread2 function end " << " threadid = " << this_thread::get_id() << endl;
}

void mythread3(future<int>& tmpf)
{
    auto res = tmpf.get();
    cout << "mythread3 function " << res << endl;
    cout << "mythread3 function end " << " threadid = " << this_thread::get_id() << endl;
}

void mythread4(shared_future<int>& tmpf)
{
    cout << "mythread4 function start " << " threadid = " << this_thread::get_id() << endl;
    auto res = tmpf.get();
    auto res2 = tmpf.get();  //shared_future����get���
    cout << "mythread4 function " << res << " " << res2 << endl;
    cout << "mythread4 function end " << " threadid = " << this_thread::get_id() << endl;
}

int main()
{
    // part 1
    //std::asnyc��std::future
    //ϣ���̷߳��ؽ��
    //std::asnyc�Ǹ�����ģ�壬����һ���첽���񣬷���std::future����
    cout << "main function " << " threadid = " << this_thread::get_id() << endl;
    //future<int> res = async(mythread, 7);  //�������߳�ִ��
    //cout << "future result: " << res.get() << endl;  //�����ȴ�����
    //res.wait(); //�����ȴ�������

    // part 2
    //������async()����һ��launch���Ͳ���
    //a)launch::deferred��ʾ�߳���ں������ñ��ӳٵ�future��wait����get������ִ��(���������߳�)
    //  wait����get����û������
    //b)launch::async��ʾasync������ʼ�������߳�
    //  asyncĬ��ʹ��launch::async���
    //future<int> res = async(launch::deferred, mythread, 7);
    //cout << "future result: " << res.get() << endl;

    // part 3
    //std::packaged_task�������
    //��ģ��
    //packaged_task<int(int)> mypt(mythread);  //mythreadͨ��packaged_task��װ
    //thread t1(ref(mypt), 1); //�����̣߳��߳̿�ʼִ�У��ڶ���������Ϊ�߳���ں�������
    //t1.join(); //�ȴ��߳�ִ�н���
    //future<int> res = mypt.get_future(); //res����mythread���ؽ��
    //cout << res.get() << endl;

    // part 4
    //std::promise��ģ��
    //ĳ���̸߳�ֵ���������߳�ȡֵ
    //ͨ��promise����һ��ֵ���ڽ���ĳ��ʱ��ͨ��future�󶨶������promise���õ��󶨵�ֵ
    //promise<int> mypro;
    //thread t1(mythread2, ref(mypro), 180);
    //t1.join();
    //future<int> f1 = mypro.get_future();  //��ȡ�̷߳���ֵ
    //cout << f1.get() << endl;
    // 
    //thread t2(mythread3, ref(f1));
    //t2.join();

    // part 5  future_status 
    //future<int> res = async(mythread0);
    //cout << "continue..." << endl;

    ////cout << res.get() << endl; //������ȴ��߳̽���
    ////ö������
    //future_status status = res.wait_for(chrono::seconds(3)); //�ȴ�6s

    //if (status == future_status::timeout)
    //{
    //    //��ʱ���߳�ûִ����
    //    cout << "��ʱ���߳�û��ִ�н���" << endl;
    //}
    //else if (status == future_status::ready)
    //{
    //    //�̳߳ɹ�����
    //    cout << "�������̳߳ɹ����أ�ȡ������ֵ��" << res.get() << endl;
    //}
    //else if (status == future_status::deferred)
    //{
    //    //�ӳ�
    //    //��future<int> res = async(launch::deferred, mythread)����;
    //    cout << "�ӳ٣��߳��ӳ�ִ��" << res.get() << endl;

    //}



    // part 6 shared_future��ģ��
    //get������������

    //packaged_task<int(int)> mypt(mythread);
    //thread t1(ref(mypt), 1);
    //t1.join();

    //future<int> res = mypt.get_future(); //res����mythread���ؽ��
    //shared_future<int> res_s(move(res));
    ////shared_future<int> res_s(res.share()); //����ͬ��
    //cout << res.valid() << endl; //�Ƿ���Чֵ

    //thread t2(mythread4, ref(res_s));
    //t2.join();


    cout << "END" << endl;


    return 0;
}



