#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

#include "classes.h"

struct bad_type_exception : std::exception {
    char const * what() const throw() { return "No such class!"; }
};

typedef boost::shared_ptr<Object> ObjectPtr;

struct Factory {
    typedef std::map<std::string, boost::function<ObjectPtr()> > FactoryMap;
    FactoryMap f;
    Factory() {
        using boost::phoenix::new_;
        using boost::phoenix::construct;
        f["Constant"] = construct<ObjectPtr>(new_<Constant>());
        f["Multiply"] = construct<ObjectPtr>(new_<Multiply>());
        // ...
    }
    ObjectPtr operator()(std::string const & type) const {
        FactoryMap::const_iterator it = f.find(type);
        if (it == f.end()) throw bad_type_exception();
        return it->second();
    }
};

int main() {
    Factory factory;
    
    vector<ObjectPtr> nodes;
    vector<Connection*> connections;

    vector<Type> init;
    
    ObjectPtr const1 = factory("Constant");
    init.push_back(5.2);
    const1->init(init);
    init.clear();

    ObjectPtr const2 = factory("Constant");
    init.push_back(3.2);
    const2->init(init);
    init.clear();

    ObjectPtr mult = factory("Multiply");
    mult->init();
    
    Connection * conn1 = new Connection(&const1->output[0], &mult->input[0], sizeof(const1->output[0]));
    Connection * conn2 = new Connection(&const2->output[0], &mult->input[1], sizeof(const2->output[0]));
    
    nodes.push_back(const1);
    nodes.push_back(const2);
    connections.push_back(conn1);
    connections.push_back(conn2);
    nodes.push_back(mult);
    
    BOOST_FOREACH(Connection* connection, connections)
        connection->process();    

    BOOST_FOREACH(ObjectPtr node, nodes)
        node->process();
}

