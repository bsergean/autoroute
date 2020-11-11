'''Plot ascii charts'''
from __future__ import print_function

import os
import sys
import collections

from bar_chart import run


def asciiPlot(items):
    values = ['{} {}'.format(key, value) for key, value in items.items()]

    BarChartOptions = collections.namedtuple(
        'BarChartOptions',
        [
            'agg_value_key',
            'agg_key_value',
            'sort_keys',
            'sort_values',
            'reverse_sort',
            'numeric_sort',
            'percentage',
        ],
    )

    options = BarChartOptions(
        agg_value_key=False,
        agg_key_value=True,
        sort_keys=True,
        sort_values=True,
        reverse_sort=True,
        numeric_sort=False,
        percentage=False,
    )
    run(values, options)
    print()


if __name__ == '__main__':
    data = {
        'foo': 1000,
        'bar': 2000,
    }
    asciiPlot(data)
