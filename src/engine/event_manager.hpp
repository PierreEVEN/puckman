#pragma once

// SOURCE : https://github.com/PierreEVEN/CppUtils/
// This is a delegate system I've created for my C++ projects
// Usage : declare a delegate type using the appropriate macro : DECLARE_DELEGATE_TYPE(EventMyDelegate), then declare a property using this type.
// Ex :
//
// DECLARE_DELEGATE_MULTICAST(MyEvent, int, double); // Declare the type of the event
//
// MyEvent on_my_event; // Event instance
//
// struct Foo {
//   void foo(int a, double b) {} // a dummy method we want to be called when event is trigger
// }
//
// int main() {
//   Foo foo;
//   on_my_event.add_object(&foo, &Foo::foo);
//   on_my_event.add_lambda([&](int a, double b) {}); // same, but with a lambda method. Multiple objects or lambda can be bound to the same event
//
//   on_my_event.execute(1, 4.0); // trigger event
// }
//
// Inspired from Unreal Engine's system @EpicGame
// 

#include <memory>
#include <vector>

#define DECLARE_DELEGATE_MULTICAST(name, ...)                     using name = DelegateMultiCast<__VA_ARGS__>;

// Wrapper classes for lambda and methods
template <typename Return_T, typename... Args_T> class DelegateFunctionPtrWrapper
{
public:
    virtual Return_T execute(Args_T&...) = 0;
    virtual bool     operator==(const void*) const = 0;
};

template <typename... Args_T> class ILambdaClassStorage
{
public:
    ILambdaClassStorage() = default;
    virtual void execute(Args_T&&...in_args) = 0;
};

template <typename Lambda_T, typename... Args_T> class TLambdaClassStorage final : public ILambdaClassStorage<Args_T...>
{
public:
    TLambdaClassStorage(Lambda_T in_lambda)
        : lambda_expression(in_lambda)
    {
    }

    void execute(Args_T&&...in_args) override
    {
        lambda_expression(std::forward<Args_T>(in_args)...);
    }

private:
    Lambda_T lambda_expression;
};

// Reference to function and corresponding object instance are stored
template <typename ObjectClass_T, typename Return_T, typename... Args_T> class DelegateFunctionPtr final : public DelegateFunctionPtrWrapper<Return_T, Args_T...>
{
public:
    DelegateFunctionPtr(ObjectClass_T* objPtr, Return_T (ObjectClass_T::*funcPtr)(Args_T ...))
        : object_ptr(objPtr), function_ptr(funcPtr)
    {
    }

    Return_T execute(Args_T&...inArgs)
    {
        return (object_ptr->*function_ptr)(inArgs...);
    }

    bool operator==(const void* objPtr) const
    {
        return object_ptr == objPtr;
    }

private:
    ObjectClass_T*            object_ptr;
    Return_T (ObjectClass_T::*function_ptr)(Args_T ...);
};

// Main delegate class
template <typename... Args_T> class DelegateMultiCast final
{
public:
    ~DelegateMultiCast()
    {
        clear();
    }

    template <typename ObjectClass_T> void add_object(ObjectClass_T* inObjPtr, void (ObjectClass_T::*inFunc)(Args_T ...))
    {
        functions.push_back(std::make_shared<DelegateFunctionPtr<ObjectClass_T, void, Args_T...>>(inObjPtr, inFunc));
    }

    template <typename Lambda_T> void add_lambda(const Lambda_T& lambda)
    {
        lambda_expressions.emplace_back(std::make_shared<TLambdaClassStorage<Lambda_T, Args_T...>>(lambda));
    }

    void clear()
    {
        functions.clear();
        lambda_expressions.clear();
    }

    void clear_object(void* ObjPtr)
    {
        for (int64_t i = functions.size() - 1; i >= 0; --i)
            if (*functions[i] == ObjPtr)
                functions.erase(functions.begin() + i);
    }

    void execute(Args_T ...inArgs)
    {
        for (const auto& fct : functions)
            fct->execute(inArgs...);

        for (const auto& lambda : lambda_expressions)
            lambda->execute(std::forward<Args_T>(inArgs)...);
    }

private:
    std::vector<std::shared_ptr<DelegateFunctionPtrWrapper<void, Args_T...>>> functions;
    std::vector<std::shared_ptr<ILambdaClassStorage<Args_T...>>>              lambda_expressions;
};
