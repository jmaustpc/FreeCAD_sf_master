
#include "PreCompiled.h"

#include "Base/Quantity.h"
#include "Base/Vector3D.h"

// inclusion of the generated files (generated out of QuantityPy.xml)
#include "QuantityPy.h"
#include "UnitPy.h"
#include "QuantityPy.cpp"

using namespace Base;

// returns a string which represents the object e.g. when printed in python
std::string QuantityPy::representation(void) const
{
	std::stringstream ret;
	ret << getQuantityPtr()->getValue() << " "; 
	ret << getQuantityPtr()->getUnit().getString();

	return ret.str();
}

PyObject *QuantityPy::PyMake(struct _typeobject *, PyObject *, PyObject *)  // Python wrapper
{
    // create a new instance of QuantityPy and the Twin object 
    return new QuantityPy(new Quantity);
}

// constructor method
int QuantityPy::PyInit(PyObject* args, PyObject* kwd)
{
    Quantity *self = getQuantityPtr();

    double f = DOUBLE_MAX;

    if (PyArg_ParseTuple(args, "|d", &f)) {
        if(f!=DOUBLE_MAX)
            *self = Quantity(f);
        return 0;
    }
    PyErr_Clear(); // set by PyArg_ParseTuple()

    PyObject *object;

    if (PyArg_ParseTuple(args,"O!",&(Base::QuantityPy::Type), &object)) {
        // Note: must be static_cast, not reinterpret_cast
        *self = *(static_cast<Base::QuantityPy*>(object)->getQuantityPtr());
        return 0;
    }
    PyErr_Clear(); // set by PyArg_ParseTuple()
    const char* string;
    if (PyArg_ParseTuple(args,"s", &string)) {
            
        *self = Quantity::parse(string);
        return 0;
    }

    PyErr_SetString(PyExc_TypeError, "Either three floats, tuple or Vector expected");
    return -1;
}


PyObject* QuantityPy::pow(PyObject * args)
{
    PyErr_SetString(PyExc_NotImplementedError, "Not yet implemented");
    return 0;
}

PyObject* QuantityPy::getUserPrefered(PyObject *args)
{
    PyErr_SetString(PyExc_NotImplementedError, "Not yet implemented");
    return 0;
}

PyObject* QuantityPy::number_add_handler(PyObject *self, PyObject *other)
{
    if (!PyObject_TypeCheck(self, &(QuantityPy::Type))) {
        PyErr_SetString(PyExc_TypeError, "First arg must be Quantity");
        return 0;
    }
    if (!PyObject_TypeCheck(other, &(QuantityPy::Type))) {
        PyErr_SetString(PyExc_TypeError, "Second arg must be Quantity");
        return 0;
    }
    Base::Quantity *a = static_cast<QuantityPy*>(self)->getQuantityPtr();
    Base::Quantity *b = static_cast<QuantityPy*>(other)->getQuantityPtr();
    return new QuantityPy(new Quantity(*a+*b) );
}

PyObject* QuantityPy::number_subtract_handler(PyObject *self, PyObject *other)
{
    if (!PyObject_TypeCheck(self, &(QuantityPy::Type))) {
        PyErr_SetString(PyExc_TypeError, "First arg must be Quantity");
        return 0;
    }
    if (!PyObject_TypeCheck(other, &(QuantityPy::Type))) {
        PyErr_SetString(PyExc_TypeError, "Second arg must be Quantity");
        return 0;
    }
    Base::Quantity *a = static_cast<QuantityPy*>(self)->getQuantityPtr();
    Base::Quantity *b = static_cast<QuantityPy*>(other)->getQuantityPtr();
    return new QuantityPy(new Quantity(*a-*b) );
}

PyObject* QuantityPy::number_multiply_handler(PyObject *self, PyObject *other)
{
    if (!PyObject_TypeCheck(self, &(QuantityPy::Type))) {
        PyErr_SetString(PyExc_TypeError, "First arg must be Quantity");
        return 0;
    }

    if (PyObject_TypeCheck(other, &(QuantityPy::Type))) {
        Base::Quantity *a = static_cast<QuantityPy*>(self) ->getQuantityPtr();
        Base::Quantity *b = static_cast<QuantityPy*>(other)->getQuantityPtr();
        
        return new QuantityPy(new Quantity(*a * *b) );
    }
    else if (PyFloat_Check(other)) {
        Base::Quantity *a = static_cast<QuantityPy*>(self) ->getQuantityPtr();
        double b = PyFloat_AsDouble(other);
        return new QuantityPy(new Quantity(*a*b) );
    }
    else {
        PyErr_SetString(PyExc_TypeError, "A Quantity can only be multiplied by Quantity or number");
        return 0;
    }
}


Py::Float QuantityPy::getValue(void) const
{
    return Py::Float(getQuantityPtr()->getValue());
}

void QuantityPy::setValue(Py::Float arg)
{
    getQuantityPtr()->setValue(arg);
}

Py::Object QuantityPy::getUnit(void) const
{
    return Py::Object(new UnitPy(new Unit(getQuantityPtr()->getUnit())));
}

void QuantityPy::setUnit(Py::Object arg)
{
    union PyType_Object pyType = {&(Base::UnitPy::Type)};
    Py::Type UnitType(pyType.o);
    if(!arg.isType(UnitType))
        throw Py::AttributeError("Not yet implemented");

    getQuantityPtr()->setUnit(*static_cast<Base::UnitPy*>((*arg))->getUnitPtr());
}


Py::String QuantityPy::getUserString(void) const
{
    //return Py::String();
    throw Py::AttributeError("Not yet implemented");
}

PyObject *QuantityPy::getCustomAttributes(const char* /*attr*/) const
{
    return 0;
}

int QuantityPy::setCustomAttributes(const char* /*attr*/, PyObject* /*obj*/)
{
    return 0; 
}


