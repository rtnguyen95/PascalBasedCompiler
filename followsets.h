#pragma once
#include <map>
#include <set>
#include <string>
using namespace std;

class FollowSets {
private:
    map<string, set<string>> setMap;
public:
    FollowSets() {

    }

    void addFollowSet(const string & name, set<string> & followSet) {
        setMap[name] = followSet;
    }

    void addFollowSet(const string & name, set<string> & followSet, set<string> & baseFollowSet) {
        setMap[name] = followSet;
        setMap[name].insert(baseFollowSet.begin(), baseFollowSet.end());
    }

    set<string> & getFollowSet(const string & name) {
        return setMap[name];
    }

    set<string> & operator[] (const string & name) {
        return setMap[name];
    }

    bool inFollowSet(const string & name, const string token) {
        set<string> & followSet = setMap[name];
        return followSet.find(token) != followSet.end();
    }
};