#pragma once

#include <string>
#include <iostream>

using namespace std;

class Slime {
private:
    string name_;
    int ability_count_;

public:
    Slime() : name_("Unknown"), ability_count_(0) {}
    Slime(const string& name, int ability_count) : name_(name), ability_count_(ability_count) {}

    Slime(const Slime& other) : name_(other.name_), ability_count_(other.ability_count_) {}

    Slime& operator=(const Slime& other) {
        if (this != &other) {
            name_ = other.name_;
            ability_count_ = other.ability_count_;
        }
        return *this;
    }

    const string& getName() const { return name_; }
    int getAbilityCount() const { return ability_count_; }

    void print() const {
        cout << "Person(Name: " << name_ << ", Abilities count: " << ability_count_ << ")\n";
    }
};