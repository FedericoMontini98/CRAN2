import os
import pandas as pd
import matplotlib
import argparse

matplotlib.use('TkAgg')
from matplotlib import rcParams

rcParams['font.family'] = 'serif'
import matplotlib.pyplot as plt

""" calibrate the warmup time given a configuration filename """

omnet_dir = "C:/omnetpp-5.7/CRAN2/"
src_dir = "csv_results/"
dest_dir = "plots/"
alpha = 0.01  # 99% CI


def main():
    parser = argparse.ArgumentParser(description="warmup calibration")
    parser.add_argument("directory", help="insert the directory containing the result files")
    parser.add_argument("config_name", help="insert the configuration name to analyse")
    args = parser.parse_args()
    # print(args)
    directory = args.directory
    config_name = args.config_name

    # delay = []
    simTime = []
    values = []
    window = 5000

    df = pd.read_csv(omnet_dir + src_dir + directory + "/" + config_name + ".csv", sep=';')

    if not os.path.exists(omnet_dir + dest_dir):
        os.mkdir(omnet_dir + dest_dir)

    for col in df.columns:
        if col.startswith('Unnamed'):
            values.append(df[col].dropna().to_numpy())
        else:
            simTime.append(df[col].dropna().to_numpy())

    for x, y in zip(simTime, values):
        plt.plot(x, pd.Series(y).rolling(window, 1).mean(), linewidth=0.5)      # , marker='+', markersize=8

    plt.title("Warmup " + config_name)
    plt.xlabel('Simulation Time (s)')
    plt.ylabel('End-to-End Delay (s)')
    plt.ticklabel_format(axis='x', style='sci')
    plt.grid(linestyle='--')
    plt.savefig(omnet_dir + dest_dir + config_name + "_warmup.png")
    plt.show()


if __name__ == "__main__":
    main()
