import os
import argparse
import pandas as pd
import utils

""" compute mean, stdDev, CI for simulation with more than one parameter changing """

# modify as needed

# #### Global #### #

omnet_dir = "C:/omnetpp-5.7/CRAN2/"     # base folder
src_dir = "csv_results/"                # source folder
dest_dir = "xlsx_results_dir/"          # destination folder
alpha = 0.01                            # 99% CI
repeat = 10                             # simulation repetitions


# #### Main #### #
def main():
    parser = argparse.ArgumentParser(description="compute mean, stdDev, CI")
    parser.add_argument("directory", help="insert the directory containing the simulation result files")
    parser.add_argument("config_name", help="insert the configuration name to analyse")
    parser.add_argument("statistic", help="insert the name of the statistic to analyse")
    args = parser.parse_args()
    # print(args)
    directory = args.directory
    config_name = args.config_name
    statistic = args.statistic

    # config_name = "calibration_speed"

    if not os.path.exists(omnet_dir + dest_dir):
        os.mkdir(omnet_dir + dest_dir)

    data_rate = [1.2, 1.25, 1.3, 1.35, 1.4]  # [1.4, 1.5, 1.6, 1.7, 1.8, 1.9]   # data_rate
    size = [1000, 1064, 1300, 1500]  # size
    # size_log = [6.97, 7, 7.17, 7.313]   # [6.55, 6.65, 6.75, 6.85, 6.95, 7.05, 7.15]
    # options = [2, 3, 7, 10, 16, 20] # cells
    # compression = [10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30]    # [30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50]

    res_final = []
    values = []
    dict_res = dict()

    for j in [1.35, 1.4]:
        for i in range(30, 35, 2):  # compression:
            df = pd.read_csv(omnet_dir + src_dir + directory + "/" + config_name + "-" + str(j) + "," + str(i) + "_"
                             + statistic + ".csv", sep=';')

            for col in df.columns:
                if col.startswith('Unnamed'):   # takes only column values
                    values.extend(df[col].dropna().to_numpy())

            mean, stdDev, ci = utils.compute_mean_and_CI(values)

            # modify as needed
            # dict to save in csv format
            res_final.append({"size_" + str(j): str(j),
                              "compression_" + str(j): str(i),
                              "mean_" + str(j): mean,
                              "stdDev_" + str(j): stdDev,
                              "ci_" + str(j): ci})

            values.clear()

    data = pd.DataFrame(data=res_final)
    data.to_csv(omnet_dir + dest_dir + config_name + "_" + statistic + "_1330.csv", sep=';', index=False)


if __name__ == '__main__':
    main()
