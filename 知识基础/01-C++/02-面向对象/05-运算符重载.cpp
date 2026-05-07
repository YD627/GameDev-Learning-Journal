# include <iostream>
using namespace std;

class Person {
public:
    int m_a;
    int m_b;


    Person() {}
    Person(int a, int b) : m_a(a), m_b(b) {}
    // 加号运算符重载
    Person operator+(const Person& p) {
        return Person(m_a + p.m_a, m_b + p.m_b);
    }
    // 递增运算符重载
    Person& operator++() {
        m_a++;
        m_b++;
        return *this;
    }
    Person operator++(int) {
        Person temp(*this);
        m_a++;
        m_b++;
        return temp;
    }
    // 等号运算符重载
    bool operator == (const Person& p) {
        return m_a == p.m_a && m_b == p.m_b;
    }
    bool operator != (const Person& p) {
        return m_a != p.m_a || m_b != p.m_b;
    }
    // 关系运算符重载
    bool operator > (const Person& p) {
        return m_a > p.m_a && m_b > p.m_b;
    }
    bool operator < (const Person& p) {
        return m_a < p.m_a && m_b < p.m_b;
    }
    bool operator >= (const Person& p) {
        return m_a >= p.m_a && m_b >= p.m_b;
    }
    bool operator <= (const Person& p) {
        return m_a <= p.m_a && m_b <= p.m_b;
    }
    // 左移运算符重载
    friend ostream& operator<<(ostream& os, const Person& p) {
        os << "m_a: " << p.m_a << ", m_b: " << p.m_b ;
        return os;
    }
};

Person operator+(const Person& p, int val) {
    return Person(p.m_a + val, p.m_b + val);
}

void test() {
    Person p1(1, 2);
    Person p2(3, 4);
    Person p3 = p1 + p2;
    cout << p3.m_a << " " << p3.m_b << endl;
    Person p4 = p1 + 5;
    cout << p4 << endl;

    // 递增运算符重载
    cout << p1++ << endl;
    cout << p1 << endl;
    cout << ++p1 << endl;

    // 等号运算符重载
    cout << (p1 == p2) << endl;
    cout << (p1 != p2) << endl;
    // 关系运算符重载
    cout << (p1 > p2) << endl;
    cout << (p1 < p2) << endl;
    cout << (p1 >= p2) << endl;
    cout << (p1 <= p2) << endl;
}

int main() {
    test();
    return 0;
}