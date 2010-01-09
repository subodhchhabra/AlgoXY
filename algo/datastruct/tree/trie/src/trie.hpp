#ifndef _TRIE_
#define _TRIE_

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <iterator>
#include <functional> //for std::ptr_fun
#include <numeric> //for std::accumulate

// Definition
template<class Char, class Value>
struct Trie{
  typedef Trie<Char, Value> Self;
  typedef std::map<Char, Self*> Children;
  typedef Value ValueType;

  Trie():value(Value()){}

  virtual ~Trie(){
    for(typename Children::iterator it=children.begin();
        it!=children.end(); ++it)
      delete it->second;
  }

  Value value;
  Children children;
};

template<class Char, class Value, class Key>
Trie<Char, Value>* insert(Trie<Char, Value>* t, Key key, Value value=Value()){
  if(!t)
    t = new Trie<Char, Value>();

  Trie<Char, Value>* p(t);
  for(typename Key::iterator it=key.begin(); it!=key.end(); ++it){
    if(p->children.find(*it) == p->children.end())
      p->children[*it] = new Trie<Char, Value>();
    p = p->children[*it];
  }
  p->value = value;
  return t;
}

template<class T, class Key>
typename T::ValueType lookup(T* t, Key key){
  if(!t)
    return typename T::ValueType(); //or throw exception

  T* p(t);
  for(typename Key::iterator it=key.begin(); it!=key.end(); ++it){
    if(p->children.find(*it) == p->children.end())
      return typename T::ValueType(); //or throw exception
    p = p->children[*it];
  }
  return p->value;
}

// helper functions

// for C++ fold-left, std::accumulate
template<class T, class K>
T* insert_key(T* t, K key){
  return insert(t, key);
}

template<class T>
std::string trie_to_str(T* t, std::string prefix=""){
  std::ostringstream s;
  s<<"("<<prefix;
  if(t->value != typename T::ValueType())
    s<<":"<<t->value;
  for(typename T::Children::iterator it=t->children.begin();
      it!=t->children.end(); ++it)
    s<<", "<<trie_to_str(it->second, prefix+it->first);
  s<<")";
  return s.str();
}

class TrieTest{
public:
  void run(){
    std::cout<<"\ntest alphabetic trie\n";
    test_insert();
    test_lookup();
  }
private:
  void test_insert(){
    typedef Trie<char, std::string> TrieType;
    TrieType* t(0);
    const char* lst[] = {"a", "an", "another", "b", "bob", "bool", "home"};
    t = std::accumulate(lst, lst+sizeof(lst)/sizeof(char*), t,
			std::ptr_fun(insert_key<TrieType, std::string>));
    std::copy(lst, lst+sizeof(lst)/sizeof(char*),
	      std::ostream_iterator<std::string>(std::cout, ", "));
    std::cout<<"\n==>"<<trie_to_str(t)<<"\n";
    delete t;

    t=0;
    const char* keys[] = {"001", "100", "101"};
    const char* vals[] = {"y", "x", "z"};
    for(unsigned int i=0; i<sizeof(keys)/sizeof(char*); ++i)
      t = insert(t, std::string(keys[i]), std::string(vals[i]));
    std::copy(keys, keys+sizeof(keys)/sizeof(char*),
	      std::ostream_iterator<std::string>(std::cout, ", "));
    std::cout<<"==>"<<trie_to_str(t)<<"\n";
    delete t;
  }

  void test_lookup(){
    Trie<char, int>* t(0);
    const char* keys[] = {"a", "an", "another", "b", "bool", "bob", "home"};
    const int vals[] = {1, 2, 7, 1, 4, 3, 4};
    for(unsigned int i=0; i<sizeof(keys)/sizeof(char*); ++i)
      t = insert(t, std::string(keys[i]), vals[i]);
    std::cout<<"\nlookup another: "<<lookup(t, std::string("another"))
	     <<"\nlookup home: "<<lookup(t, std::string("home"))
	     <<"\nlookup the: "<<lookup(t, std::string("the"))<<"\n";
    delete t;
  }
};

#endif //_TRIE_

