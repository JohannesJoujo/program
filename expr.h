//
// Created by Johannes Joujo on 2024-02-16.
//

#ifndef PROGRAM_EXPR_H
#define PROGRAM_EXPR_H
#include <iostream>
#include <iomanip>
#include <vector>
#include "lexer.h"

struct op{
    virtual bool eval(it first, it last)=0;
    std::vector<op*> children;
    void add(op* child){
        if(child){
            children.push_back(child);
        }
    }
    virtual std::string id()=0;
    void print(int indent=0){
        std::cout<<std::setw(indent)<<" "<<id()<<"\n";
        for(auto child:children){
            child->print(indent+4);
        }
    }
};

struct char_op:op{
    char ch;
    char_op(char c):ch(c){}
    std::string id() override{
        return "char_op";
    }
    bool eval(it first, it last) override{
        if(*first==ch){
            return true;
        }
        return false;
    }
};

struct any_op:char_op{
    any_op(char c) : char_op(c) {} // Constructor accepting a char argument
    std::string id() override{
        return "any_op";
    }
    bool eval(it first, it last) override{
        if(first==last){
            return false;
        }
        return true;
    }
};

struct word:op{
    std::string id() override{
        return "word";
    }
    bool eval(it first, it last) override{
        auto result = children[0]->eval(first, last);
        if(result){
            return true;
        }
        if(children.size() > 1){
            return children[1]->eval(++first, last);
        }
        return result;
    }
};

struct counter:op{
    std::string id() override{
        return "counter";
    }
    int N = 0;
    counter(int c):N(c){}
    bool eval(it first, it last) override{
        auto result = children[0]->eval(first, last);
        if(result){
            return true;
        }

        if(children.size() > 1){
            return children[1]->eval(++first, last);
        }
        return result;

    }
};

struct group_op:op{
    std::string id() override{
        return "group_op";
    }
    bool eval(it first, it last) override{
        if(first == last)
            return false;
        auto result = children[0]->eval(first, last);
        return result;
    }
};

struct expr_op:op{
    std::string id() override{
        return "expr_op";
    }
    bool eval(it first, it last) override{
        if(first == last)
            return false;
        auto result = children[0]->eval(first, last);
        return result;
    }
};

struct multi_op:op{
    std::string id() override{
        return "multi";
    }
    bool eval(it first, it last) override{
        if(first==last){
            return false;
        }
        auto result = children[0]->eval(first, last);
        if(!result){
            return eval(first + 1, last);
        }
        return result;
        /*
        //auto result = children[0]->eval(first, last);
        if(!children[0]->eval(first, last)){
            //first=first++;
            return children[0]->eval(first,last);
        }
        std::cout<<*first;
        //first=first++;

        while(children[0]->eval(first,last)) {
            std::cout<<*first;
            first++;
            //return true;
        }
        return false;*/
    }

};

struct or_op:op{
    std::string id() override{
        return "or_op";
    }
    bool eval(it first, it last) override{
        auto result = children[0]->eval(first, last);

        return result && children[1]->eval(first, last);
    }
};

struct match_op:op{
    std::string id() override{
        return "match_op";
    }
    bool eval(it first, it last) override{
        if(first == last)
            return false;
        auto result = children[0]->eval(first, last);
        if(!result){
            return eval(first + 1, last);
        }
        return result;
    }
};

match_op* match(it first, it last, lexer lexer);
or_op* orOp(it first, it last,lexer lexer);
word* paserWord(it& first, it last,lexer lexer);

#endif //PROGRAM_EXPR_H
