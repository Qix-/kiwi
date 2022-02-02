# -*- coding: utf-8 -*-
#------------------------------------------------------------------------------
# Copyright (c) 2014-2018, Nucleic Development Team.
#
# Distributed under the terms of the Modified BSD License.
#
# The full license is in the file LICENSE, distributed with this software.
#------------------------------------------------------------------------------

import pytest

from kiwisolver import Constraint, Expression, Term, Variable, Solver

class Suite(object):
    def __init__(self, C, E, T, V, S):
        self.Constraint = C
        self.Expression = E
        self.Term = T
        self.Variable = V
        self.Solver = S

suites = [
    pytest.param(
        Suite(Constraint, Expression, Term, Variable, Solver),
        id='double'
    )
]
