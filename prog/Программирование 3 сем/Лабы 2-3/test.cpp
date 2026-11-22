#include <iostream>
using namespace std;

class foo {
private:
  const int ID = 0;
public:
  int getID() const{
    return ID;
  }
};

class goo : public foo {
private:
  const int ID = 2;
public:
  int getID() const{
    return ID; 
  }
};

int main() {
  goo a;

  cout << a.getID() << endl;
}