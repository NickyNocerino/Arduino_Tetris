#ifndef Node_h
#define Node_h
 
#include <Arduino.h>

class Node
{
  public:
    Node(bool t,bool p);
    void setTemp(bool t);
    void setPerm(bool p);
    bool getTemp();
    bool getPerm();
  private:
    bool temp;
    bool perm;
};
#endif
