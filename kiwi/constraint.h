/*-----------------------------------------------------------------------------
| Copyright (c) 2013-2017, Nucleic Development Team.
|
| Distributed under the terms of the Modified BSD License.
|
| The full license is in the file LICENSE, distributed with this software.
|----------------------------------------------------------------------------*/
#pragma once
#include <map>
#include <vector>
#include "expression.h"
#include "shareddata.h"
#include "strength.h"
#include "term.h"
#include "variable.h"
#include "util.h"

namespace kiwi
{

enum RelationalOperator
{
    OP_LE,
    OP_GE,
    OP_EQ
};

template <typename TValue>
class BasicConstraint
{

public:
    BasicConstraint() = default;

    BasicConstraint(const BasicExpression<TValue> &expr,
               RelationalOperator op,
               double strength = strength::required) : m_data(new ConstraintData(expr, op, strength)) {}

    BasicConstraint(const BasicConstraint<TValue> &other, double strength) : m_data(new ConstraintData(other, strength)) {}

    BasicConstraint(const BasicConstraint<TValue> &) = default;

    BasicConstraint(BasicConstraint<TValue> &&) noexcept = default;

    ~BasicConstraint() = default;

    const BasicExpression<TValue> &expression() const
    {
        return m_data->m_expression;
    }

    RelationalOperator op() const
    {
        return m_data->m_op;
    }

    double strength() const
    {
        return m_data->m_strength;
    }

    bool violated() const
    {
        switch (m_data->m_op)
        {
            case OP_EQ: return !impl::nearZero(m_data->m_expression.value());
            case OP_GE: return m_data->m_expression.value() < 0.0;
            case OP_LE: return m_data->m_expression.value() > 0.0;
        }

        std::abort();
    }

    bool operator!() const
    {
        return !m_data;
    }

    BasicConstraint<TValue>& operator=(const BasicConstraint<TValue> &) = default;

    BasicConstraint<TValue>& operator=(BasicConstraint<TValue> &&) noexcept = default;

private:
    static BasicExpression<TValue> reduce(const BasicExpression<TValue> &expr)
    {
        std::map<BasicVariable<TValue>, double> vars;
        for (const auto & term : expr.terms())
            vars[term.variable()] += term.coefficient();

        std::vector<BasicTerm<TValue>> terms(vars.begin(), vars.end());
        return BasicExpression<TValue>(std::move(terms), expr.constant());
    }

    class ConstraintData : public SharedData
    {

    public:
        ConstraintData(const BasicExpression<TValue> &expr,
                       RelationalOperator op,
                       double strength) : SharedData(),
                                          m_expression(reduce(expr)),
                                          m_strength(strength::clip(strength)),
                                          m_op(op) {}

        ConstraintData(const BasicConstraint<TValue> &other, double strength) : SharedData(),
                                                                   m_expression(other.expression()),
                                                                   m_strength(strength::clip(strength)),
                                                                   m_op(other.op()) {}

        ~ConstraintData() = default;

        BasicExpression<TValue> m_expression;
        double m_strength;
        RelationalOperator m_op;

    private:
        ConstraintData(const ConstraintData &other);

        ConstraintData &operator=(const ConstraintData &other);
    };

    SharedDataPtr<ConstraintData> m_data;

    friend bool operator<(const BasicConstraint<TValue> &lhs, const BasicConstraint<TValue> &rhs)
    {
        return lhs.m_data < rhs.m_data;
    }

    friend bool operator==(const BasicConstraint<TValue> &lhs, const BasicConstraint<TValue> &rhs)
    {
        return lhs.m_data == rhs.m_data;
    }

    friend bool operator!=(const BasicConstraint<TValue> &lhs, const BasicConstraint<TValue> &rhs)
    {
        return lhs.m_data != rhs.m_data;
    }
};

using Constraint = BasicConstraint<double>;
using IntConstraint = BasicConstraint<long long>;

} // namespace kiwi
