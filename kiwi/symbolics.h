/*-----------------------------------------------------------------------------
| Copyright (c) 2013-2017, Nucleic Development Team.
|
| Distributed under the terms of the Modified BSD License.
|
| The full license is in the file LICENSE, distributed with this software.
|----------------------------------------------------------------------------*/
#pragma once
#include <vector>
#include "constraint.h"
#include "expression.h"
#include "term.h"
#include "variable.h"


namespace kiwi
{

// Variable multiply, divide, and unary invert

template <typename TValue> inline
BasicTerm<TValue> operator*( const BasicVariable<TValue>& variable, TValue coefficient )
{
	return BasicTerm<TValue>( variable, coefficient );
}


template <typename TValue> inline
BasicTerm<TValue> operator/( const BasicVariable<TValue>& variable, TValue denominator )
{
	return BasicTerm<TValue>( variable, 1.0 / static_cast<double>(denominator) );
}


template <typename TValue> inline
BasicTerm<TValue> operator-( const BasicVariable<TValue>& variable )
{
	return variable * TValue{-1};
}


// Term multiply, divide, and unary invert

template <typename TValue> inline
BasicTerm<TValue> operator*( const BasicTerm<TValue>& term, TValue coefficient )
{
	return BasicTerm<TValue>( term.variable(), term.coefficient() * double(coefficient) );
}


template <typename TValue> inline
BasicTerm<TValue> operator/( const BasicTerm<TValue>& term, TValue denominator )
{
	return BasicTerm<TValue>( term.variable(), term.coefficient() * (1.0 / double(denominator)) );
}


template <typename TValue> inline
BasicTerm<TValue> operator-( const BasicTerm<TValue>& term )
{
	return term * TValue{-1};
}


// Expression multiply, divide, and unary invert

template <typename TValue> inline
BasicExpression<TValue> operator*( const BasicExpression<TValue>& expression, TValue coefficient )
{
	std::vector<BasicTerm<TValue>> terms;
	terms.reserve( expression.terms().size() );

	for (const BasicTerm<TValue> &term : expression.terms())
		terms.push_back(term * coefficient);

	return BasicExpression<TValue>( std::move(terms), expression.constant() * coefficient );
}


template <typename TValue> inline
BasicExpression<TValue> operator/( const BasicExpression<TValue>& expression, TValue denominator )
{
	std::vector<BasicTerm<TValue>> terms;
	terms.reserve( expression.terms().size() );

	for (const BasicTerm<TValue> &term : expression.terms())
		terms.push_back(term / denominator);

	return BasicExpression<TValue>( std::move(terms), expression.constant() / denominator );
}


template <typename TValue> inline
BasicExpression<TValue> operator-( const BasicExpression<TValue>& expression )
{
	return expression * TValue{-1};
}


// Scalar multiply

template <typename TValue> inline
BasicExpression<TValue> operator*( TValue coefficient, const BasicExpression<TValue>& expression )
{
	return expression * coefficient;
}


template <typename TValue> inline
BasicTerm<TValue> operator*( TValue coefficient, const BasicTerm<TValue>& term )
{
	return term * coefficient;
}


template <typename TValue> inline
BasicTerm<TValue> operator*( TValue coefficient, const BasicVariable<TValue>& variable )
{
	return variable * coefficient;
}


// Expression add and subtract

template <typename TValue> inline
BasicExpression<TValue> operator+( const BasicExpression<TValue>& first, const BasicExpression<TValue>& second )
{
	std::vector<BasicTerm<TValue>> terms;
	terms.reserve( first.terms().size() + second.terms().size() );
	terms.insert( terms.begin(), first.terms().begin(), first.terms().end() );
	terms.insert( terms.end(), second.terms().begin(), second.terms().end() );
	return BasicExpression<TValue>( std::move(terms), first.constant() + second.constant() );
}


template <typename TValue> inline
BasicExpression<TValue> operator+( const BasicExpression<TValue>& first, const BasicTerm<TValue>& second )
{
	std::vector<BasicTerm<TValue>> terms;
	terms.reserve( first.terms().size() + 1 );
	terms.insert( terms.begin(), first.terms().begin(), first.terms().end() );
	terms.push_back( second );
	return BasicExpression<TValue>( std::move(terms), first.constant() );
}


template <typename TValue> inline
BasicExpression<TValue> operator+( const BasicExpression<TValue>& expression, const BasicVariable<TValue>& variable )
{
	return expression + BasicTerm<TValue>( variable );
}


template <typename TValue> inline
BasicExpression<TValue> operator+( const BasicExpression<TValue>& expression, TValue constant )
{
	return BasicExpression<TValue>( expression.terms(), expression.constant() + constant );
}


template <typename TValue> inline
BasicExpression<TValue> operator-( const BasicExpression<TValue>& first, const BasicExpression<TValue>& second )
{
	return first + -second;
}


template <typename TValue> inline
BasicExpression<TValue> operator-( const BasicExpression<TValue>& expression, const BasicTerm<TValue>& term )
{
	return expression + -term;
}


template <typename TValue> inline
BasicExpression<TValue> operator-( const BasicExpression<TValue>& expression, const BasicVariable<TValue>& variable )
{
	return expression + -variable;
}


template <typename TValue> inline
BasicExpression<TValue> operator-( const BasicExpression<TValue>& expression, TValue constant )
{
	return expression + -constant;
}


// Term add and subtract

template <typename TValue> inline
BasicExpression<TValue> operator+( const BasicTerm<TValue>& term, const BasicExpression<TValue>& expression )
{
	return expression + term;
}


template <typename TValue> inline
BasicExpression<TValue> operator+( const BasicTerm<TValue>& first, const BasicTerm<TValue>& second )
{
	return BasicExpression<TValue>( { first, second } );
}


template <typename TValue> inline
BasicExpression<TValue> operator+( const BasicTerm<TValue>& term, const BasicVariable<TValue>& variable )
{
	return term + BasicTerm<TValue>( variable );
}


template <typename TValue> inline
BasicExpression<TValue> operator+( const BasicTerm<TValue>& term, TValue constant )
{
	return BasicExpression<TValue>( term, constant );
}


template <typename TValue> inline
BasicExpression<TValue> operator-( const BasicTerm<TValue>& term, const BasicExpression<TValue>& expression )
{
	return -expression + term;
}


template <typename TValue> inline
BasicExpression<TValue> operator-( const BasicTerm<TValue>& first, const BasicTerm<TValue>& second )
{
	return first + -second;
}


template <typename TValue> inline
BasicExpression<TValue> operator-( const BasicTerm<TValue>& term, const BasicVariable<TValue>& variable )
{
	return term + -variable;
}


template <typename TValue> inline
BasicExpression<TValue> operator-( const BasicTerm<TValue>& term, TValue constant )
{
	return term + -constant;
}


// Variable add and subtract

template <typename TValue> inline
BasicExpression<TValue> operator+( const BasicVariable<TValue>& variable, const BasicExpression<TValue>& expression )
{
	return expression + variable;
}


template <typename TValue> inline
BasicExpression<TValue> operator+( const BasicVariable<TValue>& variable, const BasicTerm<TValue>& term )
{
	return term + variable;
}


template <typename TValue> inline
BasicExpression<TValue> operator+( const BasicVariable<TValue>& first, const BasicVariable<TValue>& second )
{
	return BasicTerm<TValue>( first ) + second;
}


template <typename TValue> inline
BasicExpression<TValue> operator+( const BasicVariable<TValue>& variable, TValue constant )
{
	return BasicTerm<TValue>( variable ) + constant;
}


template <typename TValue> inline
BasicExpression<TValue> operator-( const BasicVariable<TValue>& variable, const BasicExpression<TValue>& expression )
{
	return variable + -expression;
}


template <typename TValue> inline
BasicExpression<TValue> operator-( const BasicVariable<TValue>& variable, const BasicTerm<TValue>& term )
{
	return variable + -term;
}


template <typename TValue> inline
BasicExpression<TValue> operator-( const BasicVariable<TValue>& first, const BasicVariable<TValue>& second )
{
	return first + -second;
}


template <typename TValue> inline
BasicExpression<TValue> operator-( const BasicVariable<TValue>& variable, TValue constant )
{
	return variable + -constant;
}


// Scalar add and subtract

template <typename TValue> inline
BasicExpression<TValue> operator+( TValue constant, const BasicExpression<TValue>& expression )
{
	return expression + constant;
}


template <typename TValue> inline
BasicExpression<TValue> operator+( TValue constant, const BasicTerm<TValue>& term )
{
	return term + constant;
}


template <typename TValue> inline
BasicExpression<TValue> operator+( TValue constant, const BasicVariable<TValue>& variable )
{
	return variable + constant;
}


template <typename TValue> inline
BasicExpression<TValue> operator-( TValue constant, const BasicExpression<TValue>& expression )
{
	return -expression + constant;
}


template <typename TValue> inline
BasicExpression<TValue> operator-( TValue constant, const BasicTerm<TValue>& term )
{
	return -term + constant;
}


template <typename TValue> inline
BasicExpression<TValue> operator-( TValue constant, const BasicVariable<TValue>& variable )
{
	return -variable + constant;
}


// Expression relations

template <typename TValue> inline
BasicConstraint<TValue> operator==( const BasicExpression<TValue>& first, const BasicExpression<TValue>& second )
{
	return BasicConstraint<TValue>( first - second, OP_EQ );
}


template <typename TValue> inline
BasicConstraint<TValue> operator==( const BasicExpression<TValue>& expression, const BasicTerm<TValue>& term )
{
	return expression == BasicExpression<TValue>( term );
}


template <typename TValue> inline
BasicConstraint<TValue> operator==( const BasicExpression<TValue>& expression, const BasicVariable<TValue>& variable )
{
	return expression == BasicTerm<TValue>( variable );
}


template <typename TValue> inline
BasicConstraint<TValue> operator==( const BasicExpression<TValue>& expression, TValue constant )
{
	return expression == BasicExpression<TValue>( constant );
}


template <typename TValue> inline
BasicConstraint<TValue> operator<=( const BasicExpression<TValue>& first, const BasicExpression<TValue>& second )
{
	return BasicConstraint<TValue>( first - second, OP_LE );
}


template <typename TValue> inline
BasicConstraint<TValue> operator<=( const BasicExpression<TValue>& expression, const BasicTerm<TValue>& term )
{
	return expression <= BasicExpression<TValue>( term );
}


template <typename TValue> inline
BasicConstraint<TValue> operator<=( const BasicExpression<TValue>& expression, const BasicVariable<TValue>& variable )
{
	return expression <= BasicTerm<TValue>( variable );
}


template <typename TValue> inline
BasicConstraint<TValue> operator<=( const BasicExpression<TValue>& expression, TValue constant )
{
	return expression <= BasicExpression<TValue>( constant );
}


template <typename TValue> inline
BasicConstraint<TValue> operator>=( const BasicExpression<TValue>& first, const BasicExpression<TValue>& second )
{
	return BasicConstraint<TValue>( first - second, OP_GE );
}


template <typename TValue> inline
BasicConstraint<TValue> operator>=( const BasicExpression<TValue>& expression, const BasicTerm<TValue>& term )
{
	return expression >= BasicExpression<TValue>( term );
}


template <typename TValue> inline
BasicConstraint<TValue> operator>=( const BasicExpression<TValue>& expression, const BasicVariable<TValue>& variable )
{
	return expression >= BasicTerm<TValue>( variable );
}


template <typename TValue> inline
BasicConstraint<TValue> operator>=( const BasicExpression<TValue>& expression, TValue constant )
{
	return expression >= BasicExpression<TValue>( constant );
}


// Term relations

template <typename TValue> inline
BasicConstraint<TValue> operator==( const BasicTerm<TValue>& term, const BasicExpression<TValue>& expression )
{
	return expression == term;
}


template <typename TValue> inline
BasicConstraint<TValue> operator==( const BasicTerm<TValue>& first, const BasicTerm<TValue>& second )
{
	return BasicExpression<TValue>( first ) == second;
}


template <typename TValue> inline
BasicConstraint<TValue> operator==( const BasicTerm<TValue>& term, const BasicVariable<TValue>& variable )
{
	return BasicExpression<TValue>( term ) == variable;
}


template <typename TValue> inline
BasicConstraint<TValue> operator==( const BasicTerm<TValue>& term, TValue constant )
{
	return BasicExpression<TValue>( term ) == constant;
}


template <typename TValue> inline
BasicConstraint<TValue> operator<=( const BasicTerm<TValue>& term, const BasicExpression<TValue>& expression )
{
	return expression >= term;
}


template <typename TValue> inline
BasicConstraint<TValue> operator<=( const BasicTerm<TValue>& first, const BasicTerm<TValue>& second )
{
	return BasicExpression<TValue>( first ) <= second;
}


template <typename TValue> inline
BasicConstraint<TValue> operator<=( const BasicTerm<TValue>& term, const BasicVariable<TValue>& variable )
{
	return BasicExpression<TValue>( term ) <= variable;
}


template <typename TValue> inline
BasicConstraint<TValue> operator<=( const BasicTerm<TValue>& term, TValue constant )
{
	return BasicExpression<TValue>( term ) <= constant;
}


template <typename TValue> inline
BasicConstraint<TValue> operator>=( const BasicTerm<TValue>& term, const BasicExpression<TValue>& expression )
{
	return expression <= term;
}


template <typename TValue> inline
BasicConstraint<TValue> operator>=( const BasicTerm<TValue>& first, const BasicTerm<TValue>& second )
{
	return BasicExpression<TValue>( first ) >= second;
}


template <typename TValue> inline
BasicConstraint<TValue> operator>=( const BasicTerm<TValue>& term, const BasicVariable<TValue>& variable )
{
	return BasicExpression<TValue>( term ) >= variable;
}


template <typename TValue> inline
BasicConstraint<TValue> operator>=( const BasicTerm<TValue>& term, TValue constant )
{
	return BasicExpression<TValue>( term ) >= constant;
}


// Variable relations
template <typename TValue> inline
BasicConstraint<TValue> operator==( const BasicVariable<TValue>& variable, const BasicExpression<TValue>& expression )
{
	return expression == variable;
}


template <typename TValue> inline
BasicConstraint<TValue> operator==( const BasicVariable<TValue>& variable, const BasicTerm<TValue>& term )
{
	return term == variable;
}


template <typename TValue> inline
BasicConstraint<TValue> operator==( const BasicVariable<TValue>& first, const BasicVariable<TValue>& second )
{
	return BasicTerm<TValue>( first ) == second;
}


template <typename TValue> inline
BasicConstraint<TValue> operator==( const BasicVariable<TValue>& variable, TValue constant )
{
	return BasicTerm<TValue>( variable ) == constant;
}


template <typename TValue> inline
BasicConstraint<TValue> operator<=( const BasicVariable<TValue>& variable, const BasicExpression<TValue>& expression )
{
	return expression >= variable;
}


template <typename TValue> inline
BasicConstraint<TValue> operator<=( const BasicVariable<TValue>& variable, const BasicTerm<TValue>& term )
{
	return term >= variable;
}


template <typename TValue> inline
BasicConstraint<TValue> operator<=( const BasicVariable<TValue>& first, const BasicVariable<TValue>& second )
{
	return BasicTerm<TValue>( first ) <= second;
}


template <typename TValue> inline
BasicConstraint<TValue> operator<=( const BasicVariable<TValue>& variable, TValue constant )
{
	return BasicTerm<TValue>( variable ) <= constant;
}


template <typename TValue> inline
BasicConstraint<TValue> operator>=( const BasicVariable<TValue>& variable, const BasicExpression<TValue>& expression )
{
	return expression <= variable;
}


template <typename TValue> inline
BasicConstraint<TValue> operator>=( const BasicVariable<TValue>& variable, const BasicTerm<TValue>& term )
{
	return term <= variable;
}


template <typename TValue> inline
BasicConstraint<TValue> operator>=( const BasicVariable<TValue>& first, const BasicVariable<TValue>& second )
{
	return BasicTerm<TValue>( first ) >= second;
}


template <typename TValue> inline
BasicConstraint<TValue> operator>=( const BasicVariable<TValue>& variable, TValue constant )
{
	return BasicTerm<TValue>( variable ) >= constant;
}


// Scalar relations

template <typename TValue> inline
BasicConstraint<TValue> operator==( TValue constant, const BasicExpression<TValue>& expression )
{
	return expression == constant;
}


template <typename TValue> inline
BasicConstraint<TValue> operator==( TValue constant, const BasicTerm<TValue>& term )
{
	return term == constant;
}


template <typename TValue> inline
BasicConstraint<TValue> operator==( TValue constant, const BasicVariable<TValue>& variable )
{
	return variable == constant;
}


template <typename TValue> inline
BasicConstraint<TValue> operator<=( TValue constant, const BasicExpression<TValue>& expression )
{
	return expression >= constant;
}


template <typename TValue> inline
BasicConstraint<TValue> operator<=( TValue constant, const BasicTerm<TValue>& term )
{
	return term >= constant;
}


template <typename TValue> inline
BasicConstraint<TValue> operator<=( TValue constant, const BasicVariable<TValue>& variable )
{
	return variable >= constant;
}


template <typename TValue> inline
BasicConstraint<TValue> operator>=( TValue constant, const BasicExpression<TValue>& expression )
{
	return expression <= constant;
}


template <typename TValue> inline
BasicConstraint<TValue> operator>=( TValue constant, const BasicTerm<TValue>& term )
{
	return term <= constant;
}


template <typename TValue> inline
BasicConstraint<TValue> operator>=( TValue constant, const BasicVariable<TValue>& variable )
{
	return variable <= constant;
}


// Constraint strength modifier

template <typename TValue> inline
BasicConstraint<TValue> operator|( const BasicConstraint<TValue>& constraint, double strength )
{
	return BasicConstraint<TValue>( constraint, strength );
}


template <typename TValue> inline
BasicConstraint<TValue> operator|( double strength, const BasicConstraint<TValue>& constraint )
{
	return constraint | strength;
}

} // namespace kiwi
