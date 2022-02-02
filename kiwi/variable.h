/*-----------------------------------------------------------------------------
| Copyright (c) 2013-2017, Nucleic Development Team.
|
| Distributed under the terms of the Modified BSD License.
|
| The full license is in the file LICENSE, distributed with this software.
|----------------------------------------------------------------------------*/
#pragma once
#include <memory>
#include <string>
#include "shareddata.h"

namespace kiwi
{

template <typename TValue>
class BasicVariable
{

public:
    class Context
    {
    public:
        Context() = default;
        virtual ~Context() {} // LCOV_EXCL_LINE
    };

    BasicVariable(Context *context = 0) : m_data(new VariableData("", context)) {}

    BasicVariable(std::string name, Context *context = 0) : m_data(new VariableData(std::move(name), context)) {}

    BasicVariable(const char *name, Context *context = 0) : m_data(new VariableData(name, context)) {}

    BasicVariable(const BasicVariable<TValue>&) = default;

    BasicVariable(BasicVariable<TValue>&&) noexcept = default;

    ~BasicVariable() = default;

    const std::string &name() const
    {
        return m_data->m_name;
    }

    void setName(const char *name)
    {
        m_data->m_name = name;
    }

    void setName(const std::string &name)
    {
        m_data->m_name = name;
    }

    Context *context() const
    {
        return m_data->m_context.get();
    }

    void setContext(Context *context)
    {
        m_data->m_context.reset(context);
    }

    TValue value() const
    {
        return m_data->m_value;
    }

    void setValue(TValue value)
    {
        m_data->m_value = value;
    }

    // operator== is used for symbolics
    bool equals(const BasicVariable<TValue> &other)
    {
        return m_data == other.m_data;
    }

    BasicVariable<TValue>& operator=(const BasicVariable<TValue>&) = default;

    BasicVariable<TValue>& operator=(BasicVariable<TValue>&&) noexcept = default;

private:
    class VariableData : public SharedData
    {

    public:
        VariableData(std::string name, Context *context) : SharedData(),
                                                                  m_name(std::move(name)),
                                                                  m_context(context),
                                                                  m_value(0.0) {}

        VariableData(const char *name, Context *context) : SharedData(),
                                                           m_name(name),
                                                           m_context(context),
                                                           m_value(0.0) {}

        ~VariableData() = default;

        std::string m_name;
        std::unique_ptr<Context> m_context;
        TValue m_value;

    private:
        VariableData(const VariableData &other);

        VariableData &operator=(const VariableData &other);
    };

    SharedDataPtr<VariableData> m_data;

    friend bool operator<(const BasicVariable<TValue> &lhs, const BasicVariable<TValue> &rhs)
    {
        return lhs.m_data < rhs.m_data;
    }
};

using Variable = BasicVariable<double>;
using IntVariable = BasicVariable<long long>;

} // namespace kiwi
