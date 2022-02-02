/*-----------------------------------------------------------------------------
| Copyright (c) 2013-2017, Nucleic Development Team.
|
| Distributed under the terms of the Modified BSD License.
|
| The full license is in the file LICENSE, distributed with this software.
|----------------------------------------------------------------------------*/
#pragma once
#include <utility>
#include "variable.h"

namespace kiwi
{

template <typename TValue>
class BasicTerm
{

public:

	BasicTerm( BasicVariable<TValue> variable, double coefficient = 1.0 ) :
		m_variable( std::move(variable) ), m_coefficient( coefficient ) {}

	// to facilitate efficient map -> vector copies
	BasicTerm( const std::pair<const BasicVariable<TValue>, double>& pair ) :
		m_variable( pair.first ), m_coefficient( pair.second ) {}

	BasicTerm(const BasicTerm<TValue>&) = default;

	BasicTerm(BasicTerm<TValue>&&) noexcept = default;

	~BasicTerm() = default;

	const BasicVariable<TValue>& variable() const
	{
		return m_variable;
	}

	double coefficient() const
	{
		return m_coefficient;
	}

	TValue value() const
	{
		return m_coefficient * m_variable.value();
	}

	BasicTerm<TValue>& operator=(const BasicTerm<TValue>&) = default;

	BasicTerm<TValue>& operator=(BasicTerm<TValue>&&) noexcept = default;

private:

	BasicVariable<TValue> m_variable;
	double m_coefficient;
};

using Term = BasicTerm<double>;
using IntTerm = BasicTerm<long long>;

} // namespace kiwi
