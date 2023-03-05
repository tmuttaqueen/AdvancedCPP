#include <iostream>
#include <type_traits>

class Person {
public:
    virtual int id() const = 0;
    virtual std::string name() const = 0;
    virtual std::string username() const = 0;
};

class Player: public Person {
public:
    Player(int id, std::string name, std::string username, int level)
        : id_(id), name_(name), username_(username), level_(level) {}
    // default constructor, auto deleted after Player(...) defined.
    // Explicitely deleted default constructor.
    Player() = delete;
    // Assignemnt of Player deleted
    Player(const Player& p) = delete;
    // Copy Constructor of Player deleted. Can't pass this object to functions.
    Player& operator=(const Player& p) = delete;
    
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

int main() {
    Player p{1, "Tanveer", "tmuttaqueen", 10};
    std::cout << "Id: " << p.id() << "\nName: " << p.name() << "\nUsername: " << p.username() << "\nLevel: " << p.level() << std::endl;
    // std::is_abstract<Person>() is valid after C++14
    std::cout << "Is Person abstract: " << std::is_abstract<Person>::value << std::endl; 
    std::cout << "Is Person base of Player: " << std::is_base_of<Person, Player>::value << std::endl;
    std::cout << "Does Player has assignment: " << std::is_assignable<Player, Player>::value << std::endl;
    std::cout << "Does Player has copy constructor: " << std::is_copy_constructible<Player>::value << std::endl;
}