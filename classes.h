#include <boost/variant.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

typedef boost::variant<int, float, double, void*, long unsigned int> Type;

/******************/
/* Base Class     */
/******************/
class Object
{
  public:
    Object(const Object&); //Kopieren verhindern
    Object& operator=(const Object&);

    Object();
    virtual ~Object();
    vector<Type> input;
    vector<Type> output;
    virtual void init();
    virtual void init(vector<Type> _init);
    virtual void process();
};

Object::Object()
{
}

Object::~Object()
{
}

void Object::init() {};
void Object::init(vector<Type> _init) {};
void Object::process() {};


/******************/
/* Multiply Class */
/******************/
class Multiply : public Object
{
  public:
    Multiply();
    ~Multiply();
    void init();
    void process();
};

Multiply::Multiply()
{
}

Multiply::~Multiply()
{
}

void Multiply::init()
{
  input.push_back(0.0);
  input.push_back(0.0);
  output.push_back(0.0);
}

void Multiply::process()
{
  cout << "process from Multiply()\n";
  output[0] = boost::get<double>(input[0]) * boost::get<double>(input[1]);
  cout << "output pin: " << output[0] << endl;
}

/******************/
/* Constant Class */
/******************/
class Constant : public Object
{
  public:
    Constant();
    ~Constant();
    void init(vector<Type> _init);
    void process();
};

Constant::Constant()
{
}

Constant::~Constant()
{
}

void Constant::init(vector<Type> _init)
{
  output.push_back(boost::get<double>(_init[0]));
}

void Constant::process()
{
  cout << "process from Constant()\n";
  cout << "output pin: " << output[0] << endl;
}

/********************/
/* Connection Class */
/********************/
class Connection : public Object
{
  public:
    Connection(void * _in, void * _out, unsigned long _size);
    ~Connection();
    void * in;
    void * out;
    unsigned long size;
    void process();
};

Connection::Connection(void * _in, void * _out, unsigned long _size)
{
  in = _in;
  out = _out;
  size = _size;
}

Connection::~Connection()
{
}

void Connection::process()
{
  cout << "process from Connection()\n";
  memcpy(out, in, size);
}
