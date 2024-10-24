使用VS进行调试

[project2] 
一：线程的开始和结束
主函数从main()开始执行,创建线程要从一个函数开始运行，函数运行结束
一般情况下主线程执行完毕，子线程还没有执行完毕，这些子线程会被操作系统强行终止
需要保持子线程运行状态的话，要让主线程一直保持运行
也存在例外情况，detach主线程不和子线程汇合，主线程先走一步了 
一旦detach，与这个主线程关联的thead对象就失去关联，子线程驻留后台运行
相当于被C++运行时库接管（runtime library），当子线程执行完成，由运行时库负责清理线程相关资源（守护线程）


[project3] 
一：传递临时对象作为线程参数
陷阱1：传指针有问题
陷阱2：构造函数有问题
建议（deteah）：int简单类型使用值传递；如果传递对象，避免隐式转换

二：临时对象最为线程参数
线程id：每个线程都有不同id 
std::this_thread::get_id()

三：传递类对象、智能指针作为线程参数 
std::ref()（真正传引用）
std::move()
使用join防止主线程释放内存

四：用成员函数指针做线程函数


[project4] 
一：创建和等待多个线程
多个线程执行顺序是乱的，和操作系统内部调度机制有关
thread放到容器里，方便管理二：数据共享问题分析

二：数据共享问题分析
2.1只读数据是安全的
2.2有读有写（读的时候不能写，写的时候不能读）
互斥量

三：共享数据的保护案例
网络游戏服务器。一个线程收集玩家命令，并把命令写到队列
              另一个线程从队列取出命令解析，执行操作

保护共享数据，操作时，用代码把共享数据锁住，其他线程必须等待解锁
互斥量（mutex）的基本概念
互斥量是个类对象，多个线程尝试用lock()成员函数来加锁，只有一个线程能够锁定成功
成功的标志是lock()返回，如果没锁成功，流程卡在lock()尝试去锁
互斥量使用，保护少了没达到效果，多了影响效率

lock()和unlock()成对使用   
防止unlock()，引入lock_guard的类模板，可以直接取代lock()和unlock() RAII
lock_guard<mutex> sguard1(myMutex) 就不需要lock()和unlock()

死锁
至少两个互斥量，两个锁
线程A执行的时候，先锁住了锁1，lock()成功，去lock锁2
出现了上下文切换
线程B执行的时候，先锁住了锁2，lock()成功，去lock锁1
线程A锁不了锁2，流程走不下去；线程B也是一样
死锁一般解决方案：保持顺序一致
lock()函数模板，一次锁住两个或多个互斥量（谨慎使用），没锁住会阻塞
要么多个互斥量都锁住，要么都没锁着，只锁一个，另一个没锁成功，会立即把已经锁住的释放
lock(myMutex, myMutex2);
防止unlock，使用lock_guard的adopt_lock
unique_lock 取代 lock_guard


[project4-2]
unique_lock
unique_lock取代lock_guard，更灵活（可直接取代，效率会低）
1、adopt_lock，表示这个互斥量已经被lock（需要提前lock）
2、try_to_lock，尝试lock()锁定mutex，没锁定成功，会立即返回，不会阻塞（owns_lock判断取锁）
3、defer_lock，初始化没有加锁的mutex

unique_lock成员函数
lock()
unlock()
try_lock()，尝试加锁，成功true，失败false
release()，返回他所管理的mutex对象指针，并释放所有权；unique_lock和mutex不再联系

所有权传递
左值需要move()  右值自动发生


[project5]
单例模式
double check 
或
once_flag g_flag;  //系统定义标记
call_once(g_flag, CreateIns);

class CG  // 类中套类，用来释放对象


[project6]
condition_variable条件对象
wait等待
notify_one唤醒
notify_all()

wait(lock, 可调用对象)
如果对象返回是true，直接返回
如果对象返回是false，wait解锁互斥量，并堵塞本行，直到其他某个线程调用notify_one()成员函数为止
如果wait不存在第二个参数，默认第二个对象返回false
当使用notify_one唤醒wait后，wait正常工作，
 a）wait不断尝试获取互斥量锁，获取不到会继续卡在wait，获取到继续执行b
   b1）wait有第二个参数，就判断lambda表达式，如果表达式为false，那wait又对互斥量解锁，又休眠等待唤醒     
                                       如果表达式为true，wait正常返回，流程走下来（互斥锁被锁）
   b2）wait没有第二个参数，则wait返回，流程走下来


[project7]
part 1
std::asnyc、std::future
希望线程返回结果
std::asnyc是个函数模板，启动一个异步任务，返回std::future对象

part 2
额外向async()传递一个launch类型参数
a)launch::deferred表示线程入口函数调用被延迟到future的wait或者get函数才执行(不创建新线程)
  wait或者get函数没被调用
b)launch::async表示async函数开始创建新线程
  async默认使用launch::async标记

part 3
std::packaged_task打包任务
类模板

part 4
std::promise类模板
某个线程赋值，从其他线程取值
通过promise保存一个值，在将来某个时刻通过future绑定对象到这个promise来得到绑定的值

part 5
future_status 

part 6
shared_future类模板
get函数复制数据


[project8]
std::atomic，原子操作
无锁技术，比互斥量效率更高，针对一个变量




