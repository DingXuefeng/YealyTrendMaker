#ifndef Factory_H
#define Factory_H
#include <vector>
template<class T>
class Factory{
  public:
    static Factory *get() { if(!factory) factory = new Factory; return factory; }
    T *create();
    T *create(const std::string &var);
    ~Factory() { for(typename std::vector<T*>::iterator it = makers.begin(); it!=makers.end(); ++it) delete *it; }
  private:
    static Factory *factory;
    std::vector<T*> makers;
};
#ifdef TrendData_H
template<> Factory<TrendData> *Factory<TrendData>::factory = 0LL;
template<> TrendData *Factory<TrendData>::create(const std::string &var) {
  return new TrendDataImpl(var);
}
#endif
#ifdef TrendMaker_H
template<> Factory<TrendMaker> *Factory<TrendMaker>::factory = 0LL;
template<> TrendMaker *Factory<TrendMaker>::create(const std::string &var) {
  return new TrendMakerImpl(var);
}
#endif
#endif
