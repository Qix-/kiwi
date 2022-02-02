/*-----------------------------------------------------------------------------
| Copyright (c) 2013-2017, Nucleic Development Team.
|
| Distributed under the terms of the Modified BSD License.
|
| The full license is in the file LICENSE, distributed with this software.
|----------------------------------------------------------------------------*/
#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include "constraint.h"
#include "solverimpl.h"
#include "term.h"

namespace kiwi
{

namespace impl
{

class DebugHelper
{

public:
    template <typename TValue>
    static void dump(const SolverImpl<TValue> &solver, std::ostream &out)
    {
        out << "Objective" << std::endl;
        out << "---------" << std::endl;
        dump(*solver.m_objective, out);
        out << std::endl;
        out << "Tableau" << std::endl;
        out << "-------" << std::endl;
        dump<TValue>(solver.m_rows, out);
        out << std::endl;
        out << "Infeasible" << std::endl;
        out << "----------" << std::endl;
        dump(solver.m_infeasible_rows, out);
        out << std::endl;
        out << "Variables" << std::endl;
        out << "---------" << std::endl;
        dump<TValue>(solver.m_vars, out);
        out << std::endl;
        out << "Edit Variables" << std::endl;
        out << "--------------" << std::endl;
        dump<TValue>(solver.m_edits, out);
        out << std::endl;
        out << "Constraints" << std::endl;
        out << "-----------" << std::endl;
        dump<TValue>(solver.m_cns, out);
        out << std::endl;
        out << std::endl;
    }

    template <typename TValue>
    static void dump(const typename SolverImpl<TValue>::RowMap &rows, std::ostream &out)
    {
        for (const auto &rowPair : rows)
        {
            dump(rowPair.first, out);
            out << " | ";
            dump(*rowPair.second, out);
        }
    }

    static void dump(const std::vector<Symbol> &symbols, std::ostream &out)
    {
        for (const auto &symbol : symbols)
        {
            dump(symbol, out);
            out << std::endl;
        }
    }

    template <typename TValue>
    static void dump(const typename SolverImpl<TValue>::VarMap &vars, std::ostream &out)
    {
        for (const auto &varPair : vars)
        {
            out << varPair.first.name() << " = ";
            dump(varPair.second, out);
            out << std::endl;
        }
    }

    template <typename TValue>
    static void dump(const typename SolverImpl<TValue>::CnMap &cns, std::ostream &out)
    {
        for (const auto &cnPair : cns)
            dump(cnPair.first, out);
    }

    template <typename TValue>
    static void dump(const typename SolverImpl<TValue>::EditMap &edits, std::ostream &out)
    {
        for (const auto &editPair : edits)
            out << editPair.first.name() << std::endl;
    }

    static void dump(const Row &row, std::ostream &out)
    {
        for (const auto &rowPair : row.cells())
        {
            out << " + " << rowPair.second << " * ";
            dump(rowPair.first, out);
        }
        out << std::endl;
    }

    static void dump(const Symbol &symbol, std::ostream &out)
    {
        switch (symbol.type())
        {
        case Symbol::Invalid:
            out << "i";
            break;
        case Symbol::External:
            out << "v";
            break;
        case Symbol::Slack:
            out << "s";
            break;
        case Symbol::Error:
            out << "e";
            break;
        case Symbol::Dummy:
            out << "d";
            break;
        default:
            break;
        }
        out << symbol.id();
    }

    template <typename TValue>
    static void dump(const BasicConstraint<TValue> &cn, std::ostream &out)
    {
        for (const auto &term : cn.expression().terms())
        {
            out << term.coefficient() << " * ";
            out << term.variable().name() << " + ";
        }
        out << cn.expression().constant();
        switch (cn.op())
        {
        case OP_LE:
            out << " <= 0 ";
            break;
        case OP_GE:
            out << " >= 0 ";
            break;
        case OP_EQ:
            out << " == 0 ";
            break;
        default:
            break;
        }
        out << " | strength = " << cn.strength() << std::endl;
    }
};

} // namespace impl

namespace debug
{

template <typename T>
void dump(const T &value)
{
    impl::DebugHelper::dump(value, std::cout);
}

template <typename T>
void dump(const T &value, std::ostream &out)
{
    impl::DebugHelper::dump(value, out);
}

template <typename T>
std::string dumps(const T &value)
{
    std::stringstream stream;
    impl::DebugHelper::dump(value, stream);
    return stream.str();
}

} // namespace debug

} // namespace kiwi
