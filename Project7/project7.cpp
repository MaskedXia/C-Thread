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
    this_thread::sleep_for(dura); //休息2s
    cout << "mythread0 function end " << " threadid = " << this_thread::get_id() << endl;
    return 5;
}

int mythread(int arg)
{
    cout << "arg = " << arg << endl;
    cout << "mythread function start " << " threadid = " << this_thread::get_id() << endl;
    chrono::milliseconds dura(5000);
    this_thread::sleep_for(dura); //休息5s
    cout << "mythread function end " << " threadid = " << this_thread::get_id() << endl;
    return 5;
}

void mythread2(promise<int>& tmp, int calc)
{
    calc++;
    chrono::milliseconds dura(5000);
    this_thread::sleep_for(dura); //休息5s
    int res = calc;
    tmp.set_value(res);  //结果保存到tmp
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
    auto res2 = tmpf.get();  //shared_future可以get多次
    cout << "mythread4 function " << res << " " << res2 << endl;
    cout << "mythread4 function end " << " threadid = " << this_thread::get_id() << endl;
}

int main()
{
    // part 1
    //std::asnyc、std::future
    //希望线程返回结果
    //std::asnyc是个函数模板，启动一个异步任务，返回std::future对象
    cout << "main function " << " threadid = " << this_thread::get_id() << endl;
    //future<int> res = async(mythread, 7);  //创建了线程执行
    //cout << "future result: " << res.get() << endl;  //阻塞等待返回
    //res.wait(); //阻塞等待不返回

    // part 2
    //额外向async()传递一个launch类型参数
    //a)launch::deferred表示线程入口函数调用被延迟到future的wait或者get函数才执行(不创建新线程)
    //  wait或者get函数没被调用
    //b)launch::async表示async函数开始创建新线程
    //  async默认使用launch::async标记
    //future<int> res = async(launch::deferred, mythread, 7);
    //cout << "future result: " << res.get() << endl;

    // part 3
    //std::packaged_task打包任务
    //类模板
    //packaged_task<int(int)> mypt(mythread);  //mythread通过packaged_task包装
    //thread t1(ref(mypt), 1); //创建线程，线程开始执行，第二个参数作为线程入口函数参数
    //t1.join(); //等待线程执行结束
    //future<int> res = mypt.get_future(); //res保存mythread返回结果
    //cout << res.get() << endl;

    // part 4
    //std::promise类模板
    //某个线程赋值，从其他线程取值
    //通过promise保存一个值，在将来某个时刻通过future绑定对象到这个promise来得到绑定的值
    //promise<int> mypro;
    //thread t1(mythread2, ref(mypro), 180);
    //t1.join();
    //future<int> f1 = mypro.get_future();  //获取线程返回值
    //cout << f1.get() << endl;
    // 
    //thread t2(mythread3, ref(f1));
    //t2.join();

    // part 5  future_status 
    //future<int> res = async(mythread0);
    //cout << "continue..." << endl;

    ////cout << res.get() << endl; //卡在这等待线程结束
    ////枚举类型
    //future_status status = res.wait_for(chrono::seconds(3)); //等待6s

    //if (status == future_status::timeout)
    //{
    //    //超时，线程没执行完
    //    cout << "超时，线程没有执行结束" << endl;
    //}
    //else if (status == future_status::ready)
    //{
    //    //线程成功返回
    //    cout << "正常，线程成功返回，取到返回值：" << res.get() << endl;
    //}
    //else if (status == future_status::deferred)
    //{
    //    //延迟
    //    //当future<int> res = async(launch::deferred, mythread)成立;
    //    cout << "延迟，线程延迟执行" << res.get() << endl;

    //}



    // part 6 shared_future类模板
    //get函数复制数据

    //packaged_task<int(int)> mypt(mythread);
    //thread t1(ref(mypt), 1);
    //t1.join();

    //future<int> res = mypt.get_future(); //res保存mythread返回结果
    //shared_future<int> res_s(move(res));
    ////shared_future<int> res_s(res.share()); //与上同义
    //cout << res.valid() << endl; //是否有效值

    //thread t2(mythread4, ref(res_s));
    //t2.join();


    cout << "END" << endl;


    return 0;
}



