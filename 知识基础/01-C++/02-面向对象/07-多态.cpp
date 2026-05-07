#include <iostream>
using namespace std;

class Animal {
public:
    virtual void speak() {
        cout << "I am an animal" << endl;
    }
};

class Cat : public Animal {
public:
    void speak() override {
        cout << "I am a cat" << endl;
    }
};

class Dog : public Animal {
public:
    void speak() override {
        cout << "I am a dog" << endl;
    }
};

void Speak(Animal& animal) {
    animal.speak();
}

void test1() {
    Animal animal;
    Cat cat;
    Dog dog;
    Speak(animal);
    Speak(cat);
    Speak(dog);
}

int main() {
    test1();
    return 0;
}