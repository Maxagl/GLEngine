
## inheritance
1. C++ gives us the ability to explicitly choose which Base class constructor will be called! To do this, simply add a call to the Base class constructor in the member initializer list of the derived class
    ```c++
    class Derived: public Base
    {
    public:
        double m_cost {};

        Derived(double cost=0.0, int id=0)
            : Base{ id } // Call Base(int) constructor with value id!
            , m_cost{ cost }
        {
        }

        double getCost() const { return m_cost; }
    };
    ```
    Memory for derived is allocated.
    The Derived(double, int) constructor is called, where cost = 1.3, and id = 5.
    The compiler looks to see if we’ve asked for a particular Base class constructor. We have! So it calls Base(int) with id = 5.
    The base class constructor member initializer list sets m_id to 5.
    The base class constructor body executes, which does nothing.
    The base class constructor returns.
    The derived class constructor member initializer list sets m_cost to 1.3.
    The derived class constructor body executes, which does nothing.
    The derived class constructor returns.

## virtual function
1. Note that if a function is marked as virtual, all matching overrides in derived classes are also implicitly considered virtual, even if they are not explicitly marked as such.
2. Use the virtual keyword on virtual functions in a base class.
3. Use the override specifier (but not the virtual keyword) on override functions in derived classes. This includes virtual destructors.
``` c++
class A
{
public:
	virtual std::string_view getName1(int x) { return "A"; }
	virtual std::string_view getName2(int x) { return "A"; }
	virtual std::string_view getName3(int x) { return "A"; }
};

class B : public A
{
public:
	std::string_view getName1(short int x) override { return "B"; } // compile error, function is not an override
	std::string_view getName2(int x) const override { return "B"; } // compile error, function is not an override
	std::string_view getName3(int x) override { return "B"; } // okay, function is an override of A::getName3(int)

};

```
