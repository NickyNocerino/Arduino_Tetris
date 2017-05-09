#include "Arduino.h"
#include "Node.h"

Node::Node(bool t,bool p)
{
  temp=t;
  perm=p;
}

void Node:: setTemp(bool t)
{
  temp=t;
}

void Node:: setPerm(bool p)
{
  perm=p;
}

bool Node::getTemp()
{
  return temp;
}

bool Node::getPerm()
{
  return perm;
}
