import os
import argparse
import pandas as pd
import utils

""" compute mean, std, CI, for similar configuration name, save all in the same csv file """

# #### Global #### #

omnet_dir = "C:/omnetpp-5.7/CRAN2/"
src_dir = "csv_results/"
dest_dir = "xlsx_results_dir/"
alpha = 0.01  # 99% CI
repeat = 25


# #### Main #### #
def main():
    parser = argparse.ArgumentParser(description="compute mean, stdDev, CI")
    parser.add_argument("directory", help="insert the directory containing the result files")
    parser.add_argument("config_name", help="insert the config name to analyse")
    parser.add_argument("statistic", help="insert the name of the statistic to analyse")
    args = parser.parse_args()
    # print(args)
    config_name = args.config_name
    directory = args.directory
    statistic = args.statistic

    # filename = os.path.splitext(src_dir + filename_ext)[0]
    # print(filename)

    if not os.path.exists(omnet_dir + dest_dir):
        os.mkdir(omnet_dir + dest_dir)

    # options
    # data_rate = [1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2]
    # size = [600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800]
    # cells = [2, 3, 7, 10, 16, 20]
    # compression = [30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50]

    res_final = []
    values = []

    for j in range(2, 9, 2):

        df = pd.read_csv(omnet_dir + src_dir + directory + "/" + config_name + "-" + str(j) + "_"
                         + statistic + ".csv",
                         sep=';')

        for col in df.columns:
            if col.startswith('Unnamed'):
                values.extend(df[col].dropna().to_numpy())
            # elif col.startswith(statistic):
            #   time.extend(df[col].dropna().to_numpy())

        mean, stdDev, ci = utils.compute_mean_and_CI(values)

        # dict to save in csv
        # modify fields as needed
        res_final.append({"dr": "1.3",
                          "compression": str(j),
                          "mean": mean,
                          "stdDev": stdDev,
                          "ci": ci})

        values.clear()

    data = pd.DataFrame(data=res_final)
    data.to_csv(omnet_dir + dest_dir + config_name + "_" + statistic + ".csv", sep=';', index=False)


if __name__ == '__main__':
    main()
