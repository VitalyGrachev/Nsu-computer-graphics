#ifndef FACTORY_H
#define FACTORY_H

#include <map>

template<class ID, class PRODUCT>
class Factory {
public:
    Factory & instance();

    PRODUCT * create(const ID & id);

    template<class T>
    void register_type(const ID & id);

private:
    Factory() {}

    Factory(const Factory &) = delete;

    Factory(const Factory &&) = delete;

    ~Factory();

    Factory & operator=(const Factory &) = delete;

    Factory & operator=(const Factory &&) = delete;

    class BaseCreator {
    public:
        virtual ~BaseCreator() {}

        virtual PRODUCT * operator()();
    };

    template<class T>
    class DefaultCreator : public BaseCreator {
    public:
        ~DefaultCreator() {}

        PRODUCT * operator()() override {
            return new T();
        }
    };

    std::map<ID, PRODUCT *> creators;
};

template<class ID, class PRODUCT>
Factory<ID, PRODUCT>::~Factory() {
 for (std::pair<ID, PRODUCT> & value : creators) {
     delete value.second;
 }
}

template<class ID, class PRODUCT>
Factory<ID, PRODUCT> & Factory<ID, PRODUCT>::instance() {
    static Factory factory;
    return factory;
}

template<class ID, class PRODUCT>
PRODUCT * Factory<ID, PRODUCT>::create(const ID & id) {
    auto creator = creators.find(id);
    if (creator != creators.end()) {
        return creator->second();
    }
    return nullptr;
}

template<class ID, class PRODUCT, class T>
void Factory<ID, PRODUCT>::register_type(const ID & id) {
    creators.insert(std::make_pair(id, new DefaultCreator<T>()));
};


#endif //FACTORY_H
