# -*- coding: UTF-8 -*-
import sys

op_add, op_sub, op_mul, op_div = ('+', '-', '*', '/')
seqs = [(0, 1, 2, 3), (0, 2, 1, 3), (1, 2, 0, 3), (0, 3, 1, 2), (1, 3, 0, 2), (2, 3, 0, 1)]


def has_op(e, op):
    if isinstance(e, tuple):
        _, o, _ = e
        return o == op
    return False


def add_mul(x1, x2, op, rev_op, f):
    v1, e1 = x1
    v2, e2 = x2
    if v1 > v2:
        v1, e1, v2, e2 = (v2, e2, v1, e1)
    if has_op(e1, rev_op) or has_op(e2, rev_op) or has_op(e1, op):
        return 0, ''
    if has_op(e2, op):
        e3, _, e4 = e2
        if not isinstance(e3, tuple) and e3 < v1 or not isinstance(e4, tuple) and e4 < v1:
            return 0, ''
    return f(v1, v2), (e1, op, e2)


def sub_div(x1, x2, op, rev_op, f):
    v1, e1 = x1
    v2, e2 = x2
    if v1 < v2:
        v1, e1, v2, e2 = (v2, e2, v1, e1)
    if has_op(e2, op) or has_op(e2, rev_op):
        return 0, ''
    return f(v1, v2), (e1, op, e2)


def calc(values):
    data1 = [(v, v) for v in values]
    ops = [lambda x1, x2: add_mul(x1, x2, op_add, op_sub, lambda v1, v2: v1 + v2),
           lambda x1, x2: sub_div(x1, x2, op_sub, op_add, lambda v1, v2: v1 - v2),
           lambda x1, x2: add_mul(x1, x2, op_mul, op_div, lambda v1, v2: v1 * v2),
           lambda x1, x2: sub_div(x1, x2, op_div, op_mul, lambda v1, v2: v1 // v2 if v1 % v2 == 0 else 0)]
    for (i1, i2, i3, i4) in seqs:
        for op1 in ops:
            x = op1(data1[i1], data1[i2])
            v, _ = x
            if v == 0:
                continue
            data2 = [data1[i3], data1[i4], x]
            for (j1, j2, j3, _) in seqs[:3]:
                for op2 in ops:
                    x = op2(data2[j1], data2[j2])
                    v, _ = x
                    if v == 0:
                        continue
                    for op3 in ops:
                        v, e = op3(data2[j3], x)
                        if v == 24:
                            yield e


def to_str(e):
    if not isinstance(e, tuple):
        return str(e)
    e1, op, e2 = e
    s1 = to_str(e1)
    s2 = to_str(e2)
    if (op == op_mul or op == op_div) and (has_op(e1, op_add) or has_op(e1, op_sub)):
        s1 = '(' + s1 + ')'
    if ((op == op_mul or op == op_div) and (has_op(e2, op_add) or has_op(e2, op_sub)
    )) or (op == op_sub and has_op(e2, op_add)) or (op == op_div and has_op(e2, op_mul)):
        s2 = '(' + s2 + ')'
    return s1 + op + s2


def fractional_calc(values):
    for (i1, i2, i3, i4) in seqs:
        v1, v2, v3, v4 = (values[i1], values[i2], values[i3], values[i4])
        if v1 > v2:
            v1, v2 = (v2, v1)
        if (v3 * v2 + v1) * v4 == 24 * v2:
            yield '(' + str(v3) + '+' + str(v1) + '/' + str(v2) + ')*' + str(v4)
        elif (v3 * v2 - v1) * v4 == 24 * v2:
            yield '(' + str(v3) + '-' + str(v1) + '/' + str(v2) + ')*' + str(v4)


def main(values):
    expressions = set(to_str(e) for e in calc(values))
    if not expressions:
        expressions = set(fractional_calc(values))
    for e in expressions:
        print(e)


if len(sys.argv) == 5:
    main([int(v) for v in sys.argv[1:]])
