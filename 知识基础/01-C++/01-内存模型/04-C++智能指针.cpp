#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
using namespace std;

class Test{
public:
    Test(){
        cout << "Test()" << endl;
    }
    ~Test(){
        cout << "~Test()" << endl;
    }
    int getDebug(){
        return debug;
    }
    void doSomething(){
        cout << "doSomething()......" << endl;
    }
private:
    int debug = 20;
};

class DestructTest{
public:
    void operator()(Test *pt){
        pt->doSomething();
        delete pt;
    }
};

class Person{
public:
    Person(int v) {
        this->no = v;
        cout << "构造函数 \t no = " << this->no << endl;
    }
    ~Person(){
        cout << "析构函数 \t no = " << this->no << endl;
    }
private:
    int no;
};

class DestructPerson{
public:
    void operator()(Person *pt){
        cout << "DestructPerson......" <<endl;
        delete pt;
    }
};

class Girl;
class Boy{
public:
    Boy() {
        cout << "Boy构造函数" << endl;
    }
    ~Boy(){
        cout << "Boy析构函数" << endl;
    }
    void setGirlFriend(shared_ptr<Girl> _girlFriend){
        girlFriend = _girlFriend;
    }
private:
    // shared_ptr<Girl> girlFriend; // 循环引用，内存泄漏
    weak_ptr<Girl> girlFriend; // 弱引用，不会增加引用计数
};
class Girl{
public:
    Girl() {
        cout << "Girl构造函数" << endl;
    }
    ~Girl(){
        cout << "Girl析构函数" << endl;
    }
    void setBoyFriend(shared_ptr<Boy> _boyFriend){
        boyFriend = _boyFriend;
    }
private:
    shared_ptr<Boy> boyFriend;
};

void test_auto_ptr(){
    cout << " ========== 智能指针auto_ptr ========== " << endl;
    auto_ptr<Test> test(new Test());
    cout << "test->debug = " << test->getDebug() << endl;
    cout << "(*test).debug = " << (*test).getDebug() << endl;

    // Test *tmp = test.get();
    // cout << "tmp->debug = " << tmp->getDebug() << endl;

    // Test *tmp2 = test.release();
    // delete tmp2;

    // test.reset();
    // test.reset(new Test());

    // auto_ptr<Test> t1(new Test());
    // auto_ptr<Test> t2(new Test());
    // cout << "t1.get() = " << t1.get() << endl;
    // cout << "t2.get() = " << t2.get() << endl;
    // t1 = t2;
    // cout << "t1.get() = " << t1.get() << endl;
    // cout << "t2.get() = " << t2.get() << endl;

    // vector<auto_ptr<string>> vec;
    // auto_ptr<string> s1(new string("hello"));
    // auto_ptr<string> s2(new string("world"));
    // vec.push_back(move(s1));
    // vec.push_back(move(s2));
    // cout << "vec[0] = " << *vec.at(0) << endl;
    // cout << "vec[1] = " << *vec[1] << endl;
    // // 风险
    // vec[0] = vec[1];
    // cout << "vec[0] = " << *vec.at(0) << endl;
    // cout << "vec[1] = " << *vec[1] << endl; // 越界，访问空指针，会报错
}

void test_unique_ptr(){
    cout << " ========== 智能指针unique_ptr ========== " << endl;
    unique_ptr<string> p1(new string("hello"));
    unique_ptr<string> p2(new string("world"));

    cout << "p1.get() = " << p1.get() << endl;
    cout << "p2.get() = " << p2.get() << endl;

    // p1 = p2; // 禁止左赋值
    // unique_ptr<string> p3(p2); // 禁止左赋值构造

    // unique_ptr<string> p3(move(p1));
    // p1 = move(p2);
    // cout << "p1.get() = " << p1.get() << endl;
    // cout << "p2.get() = " << p2.get() << endl;

    // vector<unique_ptr<string>> vec;
    // vec.push_back(move(p1));
    // vec.push_back(move(p2));
    // cout << "vec[0] = " << *vec.at(0) << endl;
    // cout << "vec[1] = " << *vec[1] << endl;
    // // 风险
    // vec[0] = move(vec[1]);
    // cout << "vec[0] = " << *vec.at(0) << endl;
    // cout << "vec[1] = " << *vec[1] << endl; // 越界，访问空指针，会报错

    // // 支持对象数组管理，会自动调用delete[]去释放内存
    // unique_ptr<int[]> array(new int[5]);

    // unique_ptr<Test> t1;
    // unique_ptr<Test> t2(new Test());
    // unique_ptr<Test, DestructTest> t3;
    // unique_ptr<Test, DestructTest> t4(new Test());

    // // 主动释放内存
    // unique_ptr<Test> t5(new Test());
    // t5 = nullptr;
    // t5 = NULL;
    // t5.reset();

    // // 放弃控制权
    // unique_ptr<Test> t5(new Test());
    // Test *tmp = t5.release();
    // delete tmp;

    // // 重置
    // unique_ptr<Test> t5(new Test());
    // t5.reset(new Test());
    // cout << "t5->debug = " << t5->getDebug() << endl;

    // // auto_ptr 与 unique_ptr智能指针的内存管理陷阱
    // unique_ptr<string> p3;
    // string *str = new string("内存管理陷阱");
    // p3.reset(str);
    // {
    //     unique_ptr<string> p4;
    //     p4.reset(str);
    // }
    // // 此时p3为NULL
    // cout << "p3.get() = " << p3.get() << endl;
}

void test_shared_ptr(){
    cout << " ========== 智能指针shared_ptr ========== " << endl;
    shared_ptr<Person> sp1;
    shared_ptr<Person> sp2(new Person(2));

    // // 计数器
    // cout << "sp1.use_count() = " << sp1.use_count() << endl;
    // cout << "sp2.use_count() = " << sp2.use_count() << endl;

    // // 共享
    // sp1 = sp2;
    // cout << "sp1.use_count() = " << sp1.use_count() << endl;
    // cout << "sp2.use_count() = " << sp2.use_count() << endl;

    // shared_ptr<Person> sp3(sp1);
    // cout << "sp1.use_count() = " << sp1.use_count() << endl;
    // cout << "sp2.use_count() = " << sp2.use_count() << endl;
    // cout << "sp3.use_count() = " << sp3.use_count() << endl;

    // Person *person1 = new Person(1);
    // sp1.reset(person1); // 托管person1的内存，sp1会自动析构，释放person1的内存S

    // shared_ptr<Person[]> sp4;
    // shared_ptr<Person[]> sp5(new Person[5]{3,4,5,6,7});

    // shared_ptr<Person> sp6(NULL, DestructPerson());
    // shared_ptr<Person> sp7(new Person(7), DestructPerson());

    // // 初始化shared_ptr的几种方式
    // shared_ptr<int> up1(new int(100));
    // shared_ptr<int> up2(up1);
    // shared_ptr<int> up3 = make_shared<int>(3);
    // shared_ptr<string> up4 = make_shared<string>("hello");
    // shared_ptr<Person> up5 = make_shared<Person>(5);

    // // 赋值
    // shared_ptr<int> up1(new int(10));  // int(10) 的引用计数为1
    // shared_ptr<int> up2(new int(11));   // int(11) 的引用计数为1
    // up1 = up2;	// int(10) 的引用计数减1,计数归零内存释放，up2共享int(11)给up1, int(11)的引用计数为2

    // // 主动释放对象
    // shared_ptr<int> up1(new int(10));
    // up1 = nullptr;  // int(10) 的引用计数减1,计数归零内存释放 
    // up1 = NULL;

    // // 重置
    // shared_ptr<int> p(new int(10));
    // p.reset();    // 将p重置为空指针，所管理对象引用计数 减1
    // int *p1 = new int(11);
    // p.reset(p1);    // reset()里面的参数是一个指针，不是shared_ptr<int>类型

    // // 交换，原对象的计数器不变，只是交换sp1和sp2的管理对象
    // swap(sp1, sp2);
    // cout << "sp1.use_count() = " << sp1.use_count() << endl;
    // cout << "sp2.use_count() = " << sp2.use_count() << endl;

    // 使用陷阱
    shared_ptr<Boy> spBoy(new Boy());
    shared_ptr<Girl> spGirl(new Girl());
    spBoy->setGirlFriend(spGirl);
    spGirl->setBoyFriend(spBoy);
    // 此时spBoy和spGirl的引用计数都为2，会循环引用，内存泄漏，无法自动触发析构函数
    // 解决方法：使用弱引用，不会增加引用计数，不会循环引用
}

void test_weak_ptr(){
    cout << " ========== 智能指针weak_ptr ========== " << endl;
    shared_ptr<Girl> spGirl(new Girl());
    weak_ptr<Girl> wpGirl_1; // 定义弱的空指针
    weak_ptr<Girl> wpGirl_2(spGirl); // 使用共享指针构造
    wpGirl_1 = spGirl; // 允许共享指针赋值给弱指针

    // cout << "wpGirl_1.use_count() = " << wpGirl_1.use_count() << endl;
    // cout << "wpGirl_2.use_count() = " << wpGirl_2.use_count() << endl;
    // cout << "spGirl.use_count() = " << spGirl.use_count() << endl;

    // 弱指针不支持 * 和 -> 对指针的访问
    // wpGirl_1->setBoyFriend(spBoy);

    // 在必要的使用可以转换成共享指针 lock()
    shared_ptr<Girl> sp_girl;
    sp_girl = wpGirl_1.lock();
    // 使用完后，再将共享指针重置为空指针，避免内存泄漏，计数归零
    sp_girl = NULL;

    // expired函数的用法
    if(!wpGirl_1.expired()){
        cout << "还有共享指针在使用" << endl;
    }
    else{
        cout << "没有共享指针在使用" << endl;
    }
    spGirl = NULL;
    if(!wpGirl_1.expired()){
        cout << "还有共享指针在使用" << endl;
    }
    else{
        cout << "没有共享指针在使用" << endl;
    }
}
int main() {
    // test_auto_ptr();
    // test_unique_ptr();
    // test_shared_ptr();
    test_weak_ptr();
    return 0;
}