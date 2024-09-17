// project4.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

mutex my_mutex;
once_flag g_flag;  //系统定义标记

class MyClass
{
public:
	static void CreateIns()
	{
		m_instance = new MyClass();
		static CG cl;  //用来析构
	}


	static MyClass* Getinstance()
	{
		call_once(g_flag, CreateIns);
		return m_instance;
	}

	class CG  // 类中套类，用来释放对象
	{
	public:
		~CG()
		{
			if (MyClass::m_instance)
			{
				delete MyClass::m_instance;
				MyClass::m_instance = nullptr;
			}
		}

	};

	void func()
	{
		cout << "test" << endl;
	}

private:
	MyClass() {};
	MyClass(const MyClass& a);
	MyClass& operator=(const MyClass&); //私有化
	static MyClass* m_instance;  //静态成员变量
};

MyClass* MyClass::m_instance = nullptr; // 类静态成员初始化

void mythreadfun()
{
	cout << "线程开始执行..." << endl;
	MyClass* p_a = MyClass::Getinstance();
	cout << "线程执行结束了..." << endl;
	return;
}

int main()
{
	//初始化共享数据
	
    //单例模式
	MyClass* m_a = MyClass::Getinstance();
	MyClass* m_b = MyClass::Getinstance();  //相同对象
	m_a->func();
	MyClass::Getinstance()->func();

	//对于非主线程创建单例类对象，创建Getinstance需要实现互斥
	thread myt1(mythreadfun);
	thread myt2(mythreadfun);
	myt1.join();
	myt2.join();


	return 0;

	//std::call_once()
	//保证多线程中该函数只被调用一次，具备互斥量能力，更高效
	//std::once_flag标记，被设置成“已调用”，该函数就不会执行了
    
}


