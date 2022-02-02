#------------------------------------------------------------------------------
# Copyright (c) 2014-2018, Nucleic Development Team.
#
# Distributed under the terms of the Modified BSD License.
#
# The full license is in the file LICENSE, distributed with this software.
#------------------------------------------------------------------------------
import gc
import math
import operator

import pytest

from .suites import suites
from kiwisolver import strength

@pytest.mark.parametrize('cls', suites)
class TestExpression(object):
    def test_expression_creation(self, cls):
        """Test the cls.Term constructor.

        """
        v = cls.Variable('foo')
        v2 = cls.Variable('bar')
        v3 = cls.Variable('aux')
        e1 = cls.Expression((v*1, v2*2, v3*3))
        e2 = cls.Expression((v*1, v2*2, v3*3), 10)

        for e, val in ((e1, 0), (e2, 10)):
            t = e.terms()
            assert (len(t) == 3 and
                    t[0].variable() is v and t[0].coefficient() == 1 and
                    t[1].variable() is v2 and t[1].coefficient() == 2 and
                    t[2].variable() is v3 and t[2].coefficient() == 3)
            assert e.constant() == val

        assert str(e2) == '1 * foo + 2 * bar + 3 * aux + 10'

        with pytest.raises(TypeError) as excinfo:
            cls.Expression((1, v2*2, v3*3))
        assert 'Term' in excinfo.exconly()

        # ensure we test garbage collection.
        del e2
        gc.collect()


    @pytest.fixture()
    def expressions(self, cls):
        """Build expressions, terms and variables to test operations.

        """
        v = cls.Variable('foo')
        v2 = cls.Variable('bar')
        t = cls.Term(v, 10)
        t2 = cls.Term(v2)
        e = t + 5
        e2 = v2 - 10
        return e, e2, t, t2, v, v2


    def test_expression_neg(self, cls, expressions):
        """Test neg on an expression.

        """
        e, _, _, _, v, _ = expressions

        neg = -e
        assert isinstance(neg, cls.Expression)
        neg_t = neg.terms()
        assert (len(neg_t) == 1 and
                neg_t[0].variable() is v and neg_t[0].coefficient() == -10 and
                neg.constant() == -5)


    def test_expression_mul(self, cls, expressions):
        """Test expresion multiplication.

        """
        e, _, _, _, v, _ = expressions

        for mul in (e * 2.0, 2.0 * e):
            assert isinstance(mul, cls.Expression)
            mul_t = mul.terms()
            assert (len(mul_t) == 1 and
                    mul_t[0].variable() is v and mul_t[0].coefficient() == 20 and
                    mul.constant() == 10)

        with pytest.raises(TypeError):
            e * v


    def test_expression_div(self, cls, expressions):
        """Test expression divisions.

        """
        e, _, _, _, v, v2 = expressions

        div = e / 2
        assert isinstance(div, cls.Expression)
        div_t = div.terms()
        assert (len(div_t) == 1 and
                div_t[0].variable() is v and div_t[0].coefficient() == 5 and
                div.constant() == 2.5)

        with pytest.raises(TypeError):
            e / v2

        with pytest.raises(ZeroDivisionError):
            e / 0


    def test_expression_addition(self, cls, expressions):
        """Test expressions additions.

        """
        e, e2, _, t2, v, v2 = expressions

        for add in (e + 2, 2.0 + e):
            assert isinstance(add, cls.Expression)
            assert add.constant() == 7
            terms = add.terms()
            assert (len(terms) == 1 and terms[0].variable() is v and
                    terms[0].coefficient() == 10)

        add2 = e + v2
        assert isinstance(add2, cls.Expression)
        assert add2.constant() == 5
        terms = add2.terms()
        assert (len(terms) == 2 and
                terms[0].variable() is v and terms[0].coefficient() == 10 and
                terms[1].variable() is v2 and terms[1].coefficient() == 1)

        add3 = e + t2
        assert isinstance(add3, cls.Expression)
        assert add3.constant() == 5
        terms = add3.terms()
        assert (len(terms) == 2 and
                terms[0].variable() is v and terms[0].coefficient() == 10 and
                terms[1].variable() is v2 and terms[1].coefficient() == 1)

        add4 = e + e2
        assert isinstance(add4, cls.Expression)
        assert add4.constant() == -5
        terms = add4.terms()
        assert (len(terms) == 2 and
                terms[0].variable() is v and terms[0].coefficient() == 10 and
                terms[1].variable() is v2 and terms[1].coefficient() == 1)


    def test_expressions_substraction(self, cls, expressions):
        """Test expression substraction.

        """
        e, e2, _, t2, v, v2 = expressions

        for sub, diff in zip((e - 2, 2.0 - e), (3, -3)):
            assert isinstance(sub, cls.Expression)
            assert sub.constant() == diff
            terms = sub.terms()
            assert (len(terms) == 1 and terms[0].variable() is v and
                    terms[0].coefficient() == math.copysign(10, diff))

        for sub2, diff in zip((e - v2, v2 - e), (5, -5)):
            assert isinstance(sub2, cls.Expression)
            assert sub2.constant() == diff
            terms = sub2.terms()
            assert (len(terms) == 2 and
                    terms[0].variable() is v and
                    terms[0].coefficient() == math.copysign(10, diff) and
                    terms[1].variable() is v2 and
                     terms[1].coefficient() == -math.copysign(1, diff))

        for sub3, diff in zip((e - t2, t2 - e), (5, -5)):
            assert isinstance(sub3, cls.Expression)
            assert sub3.constant() == diff
            terms = sub3.terms()
            assert (len(terms) == 2 and
                    terms[0].variable() is v and
                    terms[0].coefficient() == math.copysign(10, diff) and
                    terms[1].variable() is v2 and
                    terms[1].coefficient() == -math.copysign(1, diff))

        sub4 = e - e2
        assert isinstance(sub3, cls.Expression)
        assert sub4.constant() == 15
        terms = sub4.terms()
        assert (len(terms) == 2 and
                terms[0].variable() is v and terms[0].coefficient() == 10 and
                terms[1].variable() is v2 and terms[1].coefficient() == -1)


    @pytest.mark.parametrize("op, symbol",
                             [(operator.le, '<='),
                              (operator.eq, '=='),
                              (operator.ge, '>='),
                              (operator.lt, None),
                              (operator.ne, None),
                              (operator.gt, None)])
    def test_expression_rich_compare_operations(self, cls, op, symbol):
        """Test using comparison on variables.

        """
        v1 = cls.Variable('foo')
        v2 = cls.Variable('bar')
        t1 = cls.Term(v1, 10)
        e1 = t1 + 5
        e2 = v2 - 10

        if symbol is not None:
            c = op(e1, e2)
            assert isinstance(c, cls.Constraint)
            e = c.expression()
            t = e.terms()
            assert len(t) == 2
            if t[0].variable() is not v1:
                t = (t[1], t[0])
            assert (t[0].variable() is v1 and t[0].coefficient() == 10 and
                    t[1].variable() is v2 and t[1].coefficient() == -1)
            assert e.constant() == 15
            assert c.op() == symbol and c.strength() == strength.required

        else:
            with pytest.raises(TypeError) as excinfo:
                op(e1, e2)
            assert "kiwisolver.Expression" in excinfo.exconly()
