import os
import pandas as pd
import matplotlib
import argparse

matplotlib.use('TkAgg')
from matplotlib import rcParams

rcParams['font.family'] = 'serif'
import matplotlib.pyplot as plt

""" script to calibrate the simulation time, using the sample variance in sliding window """

omnet_dir = "C:/omnetpp-5.7/CRAN2/"
src_dir = "csv_results/"
dest_dir = "plots/"
alpha = 0.01  # 99% CI
# repeat = 25


def main():
    parser = argparse.ArgumentParser(description="sim-time calibration")
    parser.add_argument("directory", help="insert the directory containing the result files")
    parser.add_argument("config_name", help="insert the configuration name to analyse")
    # parser.add_argument("statistic", help="insert the name of the statistic to analyse")
    args = parser.parse_args()
    # print(args)
    directory = args.directory
    config_name = args.config_name
    # statistic = args.statistic

    # delay = []
    simTime = []
    values = []
    window = 10000      # sliding window size

    df = pd.read_csv(omnet_dir + src_dir + directory + "/" + config_name + ".csv", sep=';')  # "_" + statistic
    # + ".csv", sep=';')

    if not os.path.exists(omnet_dir + dest_dir):
        os.mkdir(omnet_dir + dest_dir)

    for col in df.columns:
        if col.startswith('Unnamed'):
            values.append(df[col].dropna().to_numpy())
        else:
            simTime.append(df[col].dropna().to_numpy())

    # plt.figure(figsize=(45, 10), dpi=400)
    plt.figure(figsize=(20, 7), dpi=100)
    for x, y in zip(simTime, values):
        plt.plot(x, pd.Series(y).rolling(window, min_periods=1).var(), linewidth=0.5)  # , marker='+', markersize=8

    plt.title("Sim-Time Calibration - Exponential Distribution")
    plt.xlabel('Simulation Time (s)')
    plt.ylabel('Variance')
    plt.ticklabel_format(axis='x', style='sci')
    plt.grid(linestyle='--')
    plt.savefig(omnet_dir + dest_dir + config_name + ".png")
    plt.show()


if __name__ == "__main__":
    main()
