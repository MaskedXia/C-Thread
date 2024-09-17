// project2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>

using namespace std;

// 创建线程从函数开始运行
void myPrint()
{
	cout << "我的线程开始执行了..." << endl;

	cout << "我的线程执行结束了..." << endl;

}

class CTest
{
public:
	CTest(int& i) : m_i(i)
	{
		cout << "CTest构造函数被执行了" << endl;
	}
	CTest(const CTest& ct) : m_i(ct.m_i)
	{
		cout << "CTest拷贝构造函数被执行了" << endl;
	}
	CTest(const CTest&& ct) : m_i(ct.m_i)
	{
		cout << "CTest移动构造函数被执行了" << endl;
	}
	CTest& operator=(const CTest& ct) 
	{
		this->m_i = ct.m_i;
		cout << "CTest拷贝赋值函数被执行了" << endl;
		return *this;
	}
	~CTest()
	{
		cout << "~CTest析构函数被执行了" << endl;
	}


	void operator()()
	{
		cout << "m_i的值为：" << m_i << endl;
	}
private:
	int &m_i;
};

int main()
{
	// 一：线程的开始和结束
	// 主函数从main()开始执行,创建线程要从一个函数开始运行，函数运行结束
	// 一般情况下主线程执行完毕，子线程还没有执行完毕，这些子线程会被操作系统强行终止
	// 需要保持子线程运行状态的话，要让主线程一直保持运行
	// 也存在例外情况，detach主线程不和子线程汇合，主线程先走一步了

	// #include <thread>头文件
	// myPrint初始函数

	// 创建了线程，线程执行起点myPrint，线程开始执行
	thread mytobj(myPrint);  // myPrint可调用对象

	this_thread::sleep_for(chrono::milliseconds(1000));

	// joinable判断是否可以成功使用join或detach
	if (mytobj.joinable())
	{
		cout << "join可用" << endl;
	}

	// 阻塞主线程，让主线程等待子线程执行完毕，然后子线程和主线程汇合
	mytobj.join();

	// 一旦detach，与这个主线程关联的thead对象就失去关联，子线程驻留后台运行
	// 相当于被C++运行时库接管（runtime library），当子线程执行完成，由运行时库负责清理线程相关资源（守护线程）
	// mytobj.detach();

	// 类
	int myint = 10;
	CTest ct(myint);
	thread mytobj2(ref(ct));
	mytobj2.join();
	// mytobj2.detach();  // 容易产生不可预料的结果，传引用myint被主线程释放
	//                  ct对象是被复制到了线程中，不存在问题

	// lambda函数
	auto myLambda = [] {
		cout << "线程3开始执行了" << endl;
	};
	thread mytobj3(myLambda);
	mytobj3.join();


	cout << "Hello cpp!" << endl;







	return 0;
}

