//
//  m9.hpp
//  playground
//
//  Created by houguanhua on 2022/11/9.
//

#ifndef m9_hpp
#define m9_hpp


#include <memory>
#include <iostream>
        
struct B {
    virtual ~B() = default;
    virtual void bar() {
        std::cout << "B::bar\n";
    }
};

struct D: B {
    D() {
        std::cout << "D::D\n";
    }
    ~D() {
        std::cout << "D::~D\n";
    }
    void bar() override {
        std::cout << "D::bar\n";
    }
};

std::unique_ptr<D> pass_through(std::unique_ptr<D> p);

void close_file(std::FILE* fp);

struct List {
    struct Node {
        int data;
        std::unique_ptr<Node> next;
    };
    
    std::unique_ptr<Node> head;
    
    ~List() {
        while (head) {
            head = std::move(head->next);
        }
    }
    
    void push(int data) {
        head = std::unique_ptr<Node>(new Node{data, std::move(head)});
    }
    
};

#endif /* m9_hpp */
