#include <iostream>
#include <functional>

namespace function_pointer {

void FuncPointer1(int a) {
    std::cout << "Value is: " << a << std::endl;
}

int FuncPointer2(int a, int b) {
    std::cout << "Value is: " << a + b << std::endl;
    return a + b;
}

int Add(int a, int b) {
    return a + b;
}

int Subtract(int a, int b) {
    return a - b;
}
int DoOperation(int a, int b, int (*opfunc)(int, int)) {
    std::cout << "Operation Result: " << opfunc(a, b) << std::endl;
    return opfunc(a, b);
}

// Provided default function.
int DoAnotherOperation(int a, int b, int opfunc(int, int) = Add) {
    std::cout << "Operation Result: " << opfunc(a, b) << std::endl;
    return opfunc(a, b);
}

// Using type aliases for function.
using OpFunc = int(*)(int, int);
int DoDoOperation(int a, int b, OpFunc func = Add) {
    std::cout << "Operation Result: " << func(a, b) << std::endl;
    return func(a, b);
}

void FunctionPointer() {
    std::cout << "### In C++ each function has an address in memory." << std::endl;
    std::cout << "Normal Pointer: " << FuncPointer1 << std::endl;
    std::cout << "Forced Pointer: " << reinterpret_cast<void*>(FuncPointer1) << std::endl;

    std::cout << "### Declaring pointer to function." << std::endl;
    void (*func_ptr_1)(int) = 0;
    int (*func_ptr_2)(int, int) = 0;

    std::cout << "### Assigning function to function pointer." << std::endl;
    func_ptr_1 = &FuncPointer1;
    func_ptr_2 = FuncPointer2;
    std::cout << "Calling func_ptr_1: " << std::endl;
    (*func_ptr_1)(5);
    std::cout << "Calling func_ptr_2 via implicit dereference: \n" << func_ptr_2(5, 6) << std::endl;

    std::cout << "### Initializing function pointer." << std::endl;
    void (*func_ptr_3)(int){&FuncPointer1}; // Without '&', this also works.
    int (*func_ptr_4)(int, int) = FuncPointer2;
    std::cout << "Calling func_ptr_3: " << std::endl;
    func_ptr_3(10);
    std::cout << "Calling func_ptr_4: \n" << func_ptr_4(10, 11) << std::endl;

    std::cout << "### Passing function pointer to function ie, callback function." << std::endl;
    std::cout << "Calling via function passing: " << DoOperation(6, 7, Add) << std::endl;
    std::cout << "Implicit Function Type converted function: " << DoAnotherOperation(6, 7, Subtract) << std::endl;
    std::cout << "Function Pass using type aliases: " << DoDoOperation(6, 7, Subtract) << std::endl;

}
}

namespace function_object {

class FuncClass {
    int a_, b_;
  public:
    FuncClass(int a, int b) : a_(a), b_(b) {}
    int operator()(const int x, const int y) {
        return a_ * x + b_ * y;
    }
};

class Operation {
  public:
    virtual int operator()(const int a, const int b) = 0;
};

class Add: public Operation {
  public:
    int operator()(const int a, const int b) {
        return a + b;
    }
};

class Subtract: public Operation {
  public:
    int operator()(const int a, const int b) {
        return a - b;
    }
};

int DoOperation(int a, int b, Operation &op_func) {
    std::cout << "Result of operation: " << op_func(a, b) << std::endl;
    return op_func(a, b);
}

void FunctionObject() {
    FuncClass my_func_1(2, 3);
    FuncClass my_func_2{2, 3};
    std::cout << "### Calling Object as Function." << std::endl;
    std::cout << "Result of my_func_1: " << my_func_1(5, 6) << std::endl;
    std::cout << "Result of my_func_2: " << my_func_2(2, 4) << std::endl;

    std::cout << "### Passing function object to functions." << std::endl;
    Add add_func;
    Subtract subtract_func;
    std::cout << "Result of Subtraction: " << DoOperation(5, 6, add_func) << std::endl;
    std::cout << "Result of Add: " << DoOperation(5, 6, subtract_func) << std::endl;
}

}

namespace std_function_lambda {
// lambda function corresponds to something like this:
// [](){ code } will be converted to 
// struct XXX {
//   XXX() {}
//   void operator()const{
//     code
//   }
// };

void DoOperation(int a, int b, std::function<int(int, int)> func) {
    std::cout << "Result of Powerful DoOperation: " << func(a, b) << std::endl;
}

void StdFunctionLambda() {
    std::cout << "### Defining a std::function that takes two int input and outputs an int" << std::endl;
    int x = 2, y = 3;
    std::function<int(int, int)> add_func;
    add_func = [x, y](const int a, const int b) { return a * x + b * y; };
    auto subtract_func = [x, y](const int a, const int b) { return a * x - b * y; };
    std::cout << "Add function: " << add_func(5, 6) << std::endl;
    std::cout << "Subtract function: " << subtract_func(6, 7) << std::endl;

    std::cout << "### Calling by passing to a std::function." << std::endl;
    DoOperation(5, 6, add_func);
    function_object::FuncClass my_func_1(2, 3);
    DoOperation(5, 6, my_func_1);
    DoOperation(5, 6, function_pointer::Add);
}

}

int main() {
    // [Q] Why functions are not 1st class citizen in C++? Ans:
    // Although in C++ functions can be passed, returned or put in a container,
    // a locally declared function doesn't have all the local variable accessiblity.
    std::cout << "----------- FUNCTION POINTER -------------" << std::endl;
    function_pointer::FunctionPointer();
    std::cout << "----------- FUNCTION OBJECT -------------" << std::endl;
    function_object::FunctionObject();
    std::cout << "----------- FUNCTION LAMBDA -------------" << std::endl;
    std_function_lambda::StdFunctionLambda();
    return 0;
}