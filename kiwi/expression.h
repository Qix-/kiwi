/*-----------------------------------------------------------------------------
| Copyright (c) 2013-2017, Nucleic Development Team.
|
| Distributed under the terms of the Modified BSD License.
|
| The full license is in the file LICENSE, distributed with this software.
|----------------------------------------------------------------------------*/
#pragma once
#include <vector>
#include "term.h"

namespace kiwi
{

template <typename TValue>
class BasicExpression
{

public:
    BasicExpression(TValue constant = 0.0) : m_constant(constant) {}

    BasicExpression(const BasicTerm<TValue> &term, TValue constant = 0.0) : m_terms(1, term), m_constant(constant) {}

    BasicExpression(std::vector<BasicTerm<TValue>> terms, TValue constant = 0.0) : m_terms(std::move(terms)), m_constant(constant) {}

    BasicExpression(const BasicExpression<TValue>&) = default;

    // Could be marked noexcept but for a bug in the GCC of the manylinux1 image
    BasicExpression(BasicExpression<TValue>&&) = default;

    ~BasicExpression() = default;

    const std::vector<BasicTerm<TValue>> &terms() const
    {
        return m_terms;
    }

    TValue constant() const
    {
        return m_constant;
    }

    TValue value() const
    {
        TValue result = m_constant;

        for (const BasicTerm<TValue> &term : m_terms)
            result += term.value();

        return result;
    }

    BasicExpression<TValue>& operator=(const BasicExpression<TValue>&) = default;

    // Could be marked noexcept but for a bug in the GCC of the manylinux1 image
    BasicExpression<TValue>& operator=(BasicExpression<TValue>&&) = default;

private:
    std::vector<BasicTerm<TValue>> m_terms;
    TValue m_constant;
};

using Expression = BasicExpression<double>;
using IntExpression = BasicExpression<long long>;

} // namespace kiwi
