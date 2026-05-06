## C++对象模型和this指针
### 1. 成员变量和成员函数分开存储
在C++中，类内的成员变量和成员函数分开存储，只有非静态成员变量才属于类的对象上，静态成员变量和静态成员函数不属于类的对象上
```C++
class Person {
public:
	Person() {
		mA = 0;
	}
	//非静态成员变量占对象空间
	int mA;
	//静态成员变量不占对象空间
	static int mB; 
	//函数也不占对象空间，所有函数共享一个函数实例
	void func() {
		cout << "mA:" << this->mA << endl;
	}
	//静态成员函数也不占对象空间
	static void sfunc() {
	}
};
```

### 2. this指针概念
在C++中成员变量和成员函数是分开存储的，每一个非静态成员函数只会诞生一份函数实例，也就是说多个同类型的对象会共用一块代码\
那么问题是：这一块代码是如何区分那个对象调用自己的呢？\
c++通过提供特殊的对象指针，this指针，解决上述问题。*this指针指向被调用的成员函数所属的对象*
- this指针是隐含每一个非静态成员函数内的一种指针
- this指针不需要定义，直接使用即可
- this指针的用途：
    - 当形参和成员变量同名时，可用this指针来区分
    - 在类的非静态成员函数中返回对象本身，可使用return *this

### 3. 空指针访问成员函数   
C++中空指针也是可以调用成员函数的，但是也要注意有没有用到this指针，如果用到this指针，需要加以判断保证代码的健壮性
```C++
//空指针访问成员函数
class Person {
public:

	void ShowClassName() {
		cout << "我是Person类!" << endl;
	}

	void ShowPerson() {
		if (this == NULL) {
			return;
		}
		cout << mAge << endl;
	}

public:
	int mAge;
};

void test01()
{
	Person * p = NULL;
	p->ShowClassName(); //空指针，可以调用成员函数
	p->ShowPerson();  //但是如果成员函数中用到了this指针，就不可以了
}
```

### 4. const修饰成员函数
*常函数：*
- 成员函数后加const后我们称为这个函数为*常函数*
- 常函数内不可以修改成员属性
- 成员属性声明时加关键字mutable后，在常函数中依然可以修改
*常对象：*
- 声明对象前加const称该对象为常对象
- 常对象只能调用常函数
