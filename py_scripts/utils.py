import numpy as np
import math
from scipy.stats import norm
import csv

# ##### Global ##### #

alpha = 0.01    # 99% CI


def compute_weighted_avg_and_CI(values, weights):
    """
    Return the weighted average and standard deviation.

    values, weights -- Numpy ndarrays with the same shape.
    """
    average = np.average(values, weights=weights)
    # Fast and numerically precise:
    variance = np.average((values - average) ** 2, weights=weights)
    std = math.sqrt(variance)
    ci = norm.ppf(1 - alpha / 2) * std / math.sqrt(len(values))
    return average, std, ci


def compute_mean_and_CI(values):
    """
    Return the mean and the confidence interval

    values -- array of observations
    """
    mean = np.mean(values)
    # print("mean " + str(mean))
    std = np.std(values)
    ci = norm.ppf(1 - alpha / 2) * std / math.sqrt(len(values))
    return mean, std, ci


def write_csv_result(fields, results, dest_file, **kwargs):
    """ write to csv file the filter data to plot """

    with open(dest_file, 'w') as csvf:
        # print('open dest_file')

        writer = csv.DictWriter(csvf, fieldnames=fields)
        writer.writeheader()

        for res in results:
            writer.writerow(res)

