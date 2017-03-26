#ifndef FACTORY_H
#define FACTORY_H

#include <map>

template<class ID, class PRODUCT>
class Factory {
public:
    PRODUCT * create(const ID & id) const;

    template<class T>
    bool register_type(const ID & id);

    static Factory & instance()
    {
        static Factory f;
        return f;
    }

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

        virtual PRODUCT * operator()() = 0;
    };

    template<class T>
    class DefaultCreator : public BaseCreator {
    public:
        ~DefaultCreator() {}

        PRODUCT * operator()() override {
            return new T();
        }
    };

    std::map<ID, BaseCreator *> creators;
};

template<class ID, class PRODUCT>
Factory<ID, PRODUCT>::~Factory() {
 for (auto & value : creators) {
     delete value.second;
 }
}

template<class ID, class PRODUCT>
PRODUCT * Factory<ID, PRODUCT>::create(const ID & id) const {
    auto creator = creators.find(id);
    if (creator != creators.end()) {
        return creator->second->operator()();
    }
    return nullptr;
}

template<class ID, class PRODUCT>
template<class T>
bool Factory<ID, PRODUCT>::register_type(const ID & id) {
    BaseCreator * creator = new DefaultCreator<T>();
    creators.insert(std::make_pair(id, creator));
    return true;
};


#endif //FACTORY_H
