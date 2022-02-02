/*-----------------------------------------------------------------------------
| Copyright (c) 2013-2017, Nucleic Development Team.
|
| Distributed under the terms of the Modified BSD License.
|
| The full license is in the file LICENSE, distributed with this software.
|----------------------------------------------------------------------------*/
#pragma once
#include <exception>
#include <string>
#include "constraint.h"
#include "variable.h"

namespace kiwi
{

template <typename TValue>
class UnsatisfiableConstraint : public std::exception
{

public:
    UnsatisfiableConstraint(BasicConstraint<TValue> constraint) : m_constraint(std::move(constraint)) {}

    ~UnsatisfiableConstraint() noexcept {}

    const char *what() const noexcept
    {
        return "The constraint can not be satisfied.";
    }

    const BasicConstraint<TValue> &constraint() const
    {
        return m_constraint;
    }

private:
    BasicConstraint<TValue> m_constraint;
};

template <typename TValue>
class UnknownConstraint : public std::exception
{

public:
    UnknownConstraint(BasicConstraint<TValue> constraint) : m_constraint(std::move(constraint)) {}

    ~UnknownConstraint() noexcept {}

    const char *what() const noexcept
    {
        return "The constraint has not been added to the solver.";
    }

    const BasicConstraint<TValue> &constraint() const
    {
        return m_constraint;
    }

private:
    BasicConstraint<TValue> m_constraint;
};

template <typename TValue>
class DuplicateConstraint : public std::exception
{

public:
    DuplicateConstraint(BasicConstraint<TValue> constraint) : m_constraint(std::move(constraint)) {}

    ~DuplicateConstraint() noexcept {}

    const char *what() const noexcept
    {
        return "The constraint has already been added to the solver.";
    }

    const BasicConstraint<TValue> &constraint() const
    {
        return m_constraint;
    }

private:
    BasicConstraint<TValue> m_constraint;
};

template <typename TValue>
class UnknownEditVariable : public std::exception
{

public:
    UnknownEditVariable(BasicVariable<TValue> variable) : m_variable(std::move(variable)) {}

    ~UnknownEditVariable() noexcept {}

    const char *what() const noexcept
    {
        return "The edit variable has not been added to the solver.";
    }

    const BasicVariable<TValue> &variable() const
    {
        return m_variable;
    }

private:
    BasicVariable<TValue> m_variable;
};

template <typename TValue>
class DuplicateEditVariable : public std::exception
{

public:
    DuplicateEditVariable(BasicVariable<TValue> variable) : m_variable(std::move(variable)) {}

    ~DuplicateEditVariable() noexcept {}

    const char *what() const noexcept
    {
        return "The edit variable has already been added to the solver.";
    }

    const BasicVariable<TValue> &variable() const
    {
        return m_variable;
    }

private:
    BasicVariable<TValue> m_variable;
};

class BadRequiredStrength : public std::exception
{

public:
    BadRequiredStrength() {}

    ~BadRequiredStrength() noexcept {}

    const char *what() const noexcept
    {
        return "A required strength cannot be used in this context.";
    }
};

class InternalSolverError : public std::exception
{

public:
    InternalSolverError() : m_msg("An internal solver error ocurred.") {}

    InternalSolverError(const char *msg) : m_msg(msg) {}

    InternalSolverError(std::string msg) : m_msg(std::move(msg)) {}

    ~InternalSolverError() noexcept {}

    const char *what() const noexcept
    {
        return m_msg.c_str();
    }

private:
    std::string m_msg;
};

} // namespace kiwi
