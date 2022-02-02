# -*- coding: utf-8 -*-
#------------------------------------------------------------------------------
# Copyright (c) 2014-2018, Nucleic Development Team.
#
# Distributed under the terms of the Modified BSD License.
#
# The full license is in the file LICENSE, distributed with this software.
#------------------------------------------------------------------------------
import math
import operator
import sys

import pytest

from .suites import suites
from kiwisolver import strength

@pytest.mark.parametrize('cls', suites)
class TestVariable(object):
    def test_variable_methods(self, cls):
        """Test the variable modification methods.

        """
        v = cls.Variable()
        assert v.name() == ""
        v.setName(u'γ')
        assert v.name() == 'γ'
        v.setName('foo')
        assert v.name() == 'foo'
        with pytest.raises(TypeError):
            v.setName(1)
        if sys.version_info >= (3,):
            with pytest.raises(TypeError):
                v.setName(b'r')

        assert v.value() == 0.0

        assert v.context() is None
        ctx = object()
        v.setContext(ctx)
        assert v.context() is ctx

        assert str(v) == 'foo'

        with pytest.raises(TypeError):
            cls.Variable(1)


    def test_variable_neg(self, cls):
        """Test neg on a variable.

        """
        v = cls.Variable("foo")

        neg = -v
        assert isinstance(neg, cls.Term)
        assert neg.variable() is v and neg.coefficient() == -1


    def test_variable_mul(self, cls):
        """Test variable multiplications.

        """
        v = cls.Variable("foo")
        v2 = cls.Variable('bar')

        for mul in (v * 2.0, 2 * v):
            assert isinstance(mul, cls.Term)
            assert mul.variable() is v and mul.coefficient() == 2

        with pytest.raises(TypeError):
            v * v2


    def test_variable_division(self, cls):
        """Test variable divisions.

        """
        v = cls.Variable("foo")
        v2 = cls.Variable('bar')

        div = v / 2.0
        assert isinstance(div, cls.Term)
        assert div.variable() is v and div.coefficient() == 0.5

        with pytest.raises(TypeError):
            v / v2

        with pytest.raises(ZeroDivisionError):
            v / 0


    def test_variable_addition(self, cls):
        """Test variable additions.

        """
        v = cls.Variable("foo")
        v2 = cls.Variable('bar')

        for add in (v + 2, 2.0 + v):
            assert isinstance(add, cls.Expression)
            assert add.constant() == 2
            terms = add.terms()
            assert (len(terms) == 1 and terms[0].variable() is v and
                    terms[0].coefficient() == 1)

        add2 = v + v2
        assert isinstance(add2, cls.Expression)
        assert add2.constant() == 0
        terms = add2.terms()
        assert (len(terms) == 2 and
                terms[0].variable() is v and terms[0].coefficient() == 1 and
                terms[1].variable() is v2 and terms[1].coefficient() == 1)

        with pytest.raises(TypeError):
            v + ''


    def test_variable_sub(self, cls):
        """Test variable substractions.

        """
        v = cls.Variable("foo")
        v2 = cls.Variable('bar')

        for sub, diff in zip((v - 2, 2 - v), (-2, 2)):
            assert isinstance(sub, cls.Expression)
            assert sub.constant() == diff
            terms = sub.terms()
            assert (len(terms) == 1 and terms[0].variable() is v and
                    terms[0].coefficient() == -math.copysign(1, diff))

        sub2 = v - v2
        assert isinstance(sub2, cls.Expression)
        assert sub2.constant() == 0
        terms = sub2.terms()
        assert (len(terms) == 2 and
                terms[0].variable() is v and terms[0].coefficient() == 1 and
                terms[1].variable() is v2 and terms[1].coefficient() == -1)


    def test_variable_rich_compare_operations(self, cls):
        """Test using comparison on variables.

        """
        v = cls.Variable('foo')
        v2 = cls.Variable(u'γ')

        for op, symbol in ((operator.le, '<='), (operator.eq, '=='),
                           (operator.ge, '>=')):
            c = op(v, v2 + 1)
            assert isinstance(c, cls.Constraint)
            e = c.expression()
            t = e.terms()
            assert len(t) == 2
            if t[0].variable() is not v:
                t = (t[1], t[0])
            assert (t[0].variable() is v and t[0].coefficient() == 1 and
                    t[1].variable() is v2 and t[1].coefficient() == -1)
            assert e.constant() == -1
            assert c.op() == symbol and c.strength() == strength.required

        for op in (operator.lt, operator.ne, operator.gt):
            with pytest.raises(TypeError):
                op(v, v2)
