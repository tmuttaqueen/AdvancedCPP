#include <iostream>
#include <unordered_map>
#include <memory>
#include <utility>
#include <vector>

class Person {
public:
    virtual int id() const = 0;
    virtual std::string name() const = 0;
    virtual std::string username() const = 0;
};

class Player: public Person {
public:
    Player(int id, std::string name, std::string username, int level) noexcept
        : id_(id), name_(name), username_(username), level_(level) {}
    Player() = delete;
    Player(const Player& p) = delete;
    Player& operator=(const Player& p) = delete;
    // Move constructor can't take const. Won't throw any exception.
     

    int id() const override {
        return id_;
    }
    std::string name() const override {
        return name_;
    }
    std::string username() const override {
        return username_;
    }
    int level() const {
        return level_;
    }
    
private:
    int id_;
    int level_;
    std::string name_;
    std::string username_;
};

class PersonDatabase {
public:
    bool Add(std::unique_ptr<Person> person) {
        if(db_.find(person->id()) != db_.end()) {
            std::cout << "Id=" << person->id() << " Already exists" << std::endl;
            return false;
        }
        db_.insert({person->id(), std::move(person)});
        return true;
    }
    void AddAll(std::vector<std::unique_ptr<Person>> persons) {
        for(auto& person: persons) {
            Add(std::move(person));
        }
    }
    std::tuple<bool, Person*> Value(int id) {
        if(db_.find(id) == db_.end()) {
            // nullptr is convertible to bool. ie. if(nullptr) is false.
            return std::make_tuple(true, nullptr);
        }
        return std::make_tuple(false, db_[id].get());
    }
private:
    std::unordered_map<int, std::unique_ptr<Person>> db_;
};

int main() {
    // Just a serious note, use unique_ptr only when you need to think about ownership. Otherwise use normal ptr.
    std::unique_ptr<Person> p1 = std::make_unique<Player>(1, "Tanveer", "tmuttaqueen", 10);
    std::unique_ptr<Person> p2 = std::make_unique<Player>(2, "Muttaqueen", "tanveer", 7);
    std::unique_ptr<Person> p3 = std::make_unique<Player>(3, "Alice", "alc", 15);
    std::unique_ptr<Person> p4 = std::make_unique<Player>(1, "Bob", "bb", 15);

    PersonDatabase db;
    bool err;
    Person* person;
    std::tie(err, person) = db.Value(1);
    std::cout << "Error: " << err << std::endl;
    if(person) {
        std::cout << "Id: " << person->id() << std::endl;
    }
    db.Add(std::move(p1));
    std::tie(err, person) = db.Value(1);
    std::cout << "Error: " << err << std::endl;
    if(person) {
        std::cout << "Id: " << person->id() << std::endl;
    }
    // Q: why vec{p1, p2, p3} give error because pf is_constructible()
    //  [with _ValueType = unique_ptr<Person>; _Tp = const unique_ptr<Person>&]'
    std::vector<std::unique_ptr<Person>> vec;
    // This is invalid because of db.Add(std::move(p1)) used p1 before.
    // vec.push_back(std::move(p1));
    vec.push_back(std::move(p2));
    vec.push_back(std::move(p3));
    vec.push_back(std::move(p4));
    db.AddAll(std::move(vec));
    std::tie(err, person) = db.Value(3);
    std::cout << "Error: " << err << std::endl;
    if(person) {
        std::cout << "Id: " << person->id() << std::endl;
    }
}