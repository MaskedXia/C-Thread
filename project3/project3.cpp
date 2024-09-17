// project3.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <string>

using namespace std;    

class A
{
public:
    int m_i;
    A(int i) : m_i(i){ cout << "A的构造函数执行了" << this << " thread_id = " << this_thread::get_id() << endl; }
    A(const A& a) : m_i(a.m_i) { cout << "A的拷贝构造函数执行了" << this << " thread_id = " << this_thread::get_id() << endl; }
    ~A() { cout << "A的析构函数执行了" << this << " thread_id = " << this_thread::get_id() << endl; }

    void threadWork(int num)
    {
        cout << "子线程threadWork执行了" << this << " thread_id = " << this_thread::get_id() << endl;
    }

    void operator()(int num)
    {
        cout << "子线程operator()执行了" << this << " thread_id = " << this_thread::get_id() << endl;
    }

};

void myPrint(const int& i, char* pbuf)
{
    cout << i << endl;  // i并不是mvar的引用，实际为值传递，那么即使主线程deteah，子线程仍然安全（不建议引用）
    cout << pbuf << endl;  // pbuf和mybuf同一地址，使用deteah有问题
}

void myPrint2(const int i, const string& pbuf)
{
    cout << i << endl;
    cout << pbuf << endl;
}

void myPrint3(const int i, const A& pbuf)
{  
    cout << "地址是" << &pbuf << endl;  // 打印地址 
}

void myPrint4(const A& pbuf)
{
    //pbuf.m_i = 100;
    cout << "子线程myPrint4参数地址是" << &pbuf << " thread_id = " << this_thread::get_id()  << endl;  // 打印地址 
}

void myPrint5(A& pbuf)
{
    pbuf.m_i = 100;
    cout << "子线程myPrint5参数地址是" << &pbuf << " thread_id = " << this_thread::get_id() << endl;  // 打印地址 
}

void myPrint6(unique_ptr<int> sp)
{
    cout << "子线程myPrint6参数地址是" << sp << " thread_id = " << this_thread::get_id() << endl;  // 打印地址 
}

int main()
{
    // 一：传递临时对象作为线程参数
    // 陷阱1：传指针有问题
    // 陷阱2：构造函数有问题
    // 建议（deteah）：int简单类型使用值传递；如果传递对象，避免隐式转换

    // 二：临时对象最为线程参数
    // 线程id：每个线程都有不同id 
    // std::this_thread::get_id()

    // 三：传递类对象、智能指针作为线程参数 
    // std::ref()（真正传引用）
    // std::move()
    // 使用join防止主线程释放内存

    // 四：用成员函数指针做线程函数

    // part 1 先构造，防止主线程退出，子线程还没有构造完（detach） 
    int mvar = 1;  // shift+f9快速监视
    int& mvary = mvar;
    char mybuf[] = "this is a test";
    // thread mytobj(myPrint1, mvary, mybuf)
    // thread mytobj(myPrint2, mvary, mybuf);  //存在mybuf被回收了，系统采用mybuf去转string
    // thread mytobj(myPrint2, mvary, string(mybuf));  // 直接转换成string，就可以保证稳定（deteah）


    // part 2 先构造，类验证
    int mvar2 = 12;
    //thread mytobj(myPrint3, mvar, mvar2);  // 希望mvar2构造成A类型对象（隐式转换）
                                                // 很可能主线程结束了，构造函数还没来得及构造
    //thread mytobj(myPrint3, mvar, A(mvar2));  // 构造、拷贝、析构都会执行，一定在主线程结束前构造完

    // part 3 先构造，防止隐式构造
    cout << "主线程id是：" << this_thread::get_id() << endl;
    //thread mytobj(myPrint4, mvar);
    //thread mytobj(myPrint4, A(mvar));

    // part 4 ref传引用，不再拷贝
     A mya(10);
     //thread mytobj(myPrint4, mya);
     //cout << mya.m_i << endl; //没有被修改

     //thread mytobj(myPrint5, ref(mya));  // 可以不用mutable，const
     //this_thread::sleep_for(chrono::milliseconds(1000));
     //cout << mya.m_i << endl; //已修改
    
    // part5 智能指针，move传引用
    unique_ptr<int> msp(new int(100));
    //thread mytobj(myPrint6, move(msp));

    // part6 成员函数
    A mya2(20);
    //thread mytobj(&A::threadWork, ref(mya2), 15);  // 使用自定义成员函数，参数（this, ...arg）
    thread mytobj(ref(mya2), 15);  // 使用operator()函数

    mytobj.join();
    // mytobj.detach();

    cout << "Hello, Cpp" << endl;




    return 0;
}