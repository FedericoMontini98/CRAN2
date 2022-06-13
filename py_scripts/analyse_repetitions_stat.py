import os
import argparse
import pandas as pd
from utils import compute_mean_and_CI

""" compute mean, stdDev, CI of each repetition individually and save all in one single file """
# modify as needed

# #### Global #### #

omnet_dir = "C:/omnetpp-5.7/CRAN2/"
src_dir = "csv_results/"
dest_dir = "xlsx_results_dir/"
alpha = 0.01  # 99% CI
repeat = 3


# #### Main #### #
def main():
    parser = argparse.ArgumentParser(description="compute mean, stdDev, CI of repetitions individually")
    parser.add_argument("directory", help="insert the directory containing the result files to analyse")
    parser.add_argument("filename", help="insert the config name to analyse")
    parser.add_argument("statistic", help="insert the name of statistic to analyse")
    # parser.add_argument("repeat", help="insert the number of repetitions")
    args = parser.parse_args()
    # print(args)
    directory = args.directory
    config_name = args.filename
    statistic = args.statistic

    if not os.path.exists(omnet_dir + dest_dir):
        os.mkdir(omnet_dir + dest_dir)

    values = []
    dict_mean_ci = []

    # for j in range(2):
    for i in range(repeat):
        df = pd.read_csv(omnet_dir + src_dir + directory + "/" + config_name + "_" + statistic + "_#" + str(i)
                         + ".csv", sep=';')
        for col in df.columns:
            if col.startswith('Unnamed'):
                values.extend(df[col].dropna().to_numpy())

        mean, stdDev, ci = compute_mean_and_CI(values)
        # print(mean, ci)
        dict_mean_ci.append({"repeat": str(i), "mean": mean, "stdDev": stdDev, "ci": ci})
        # print(dict_mean_ci)
        values.clear()

    data = pd.DataFrame(data=dict_mean_ci)
    data.to_csv(omnet_dir + dest_dir + config_name + "_" + statistic + ".csv", sep=';', index=False)


if __name__ == '__main__':
    main()
