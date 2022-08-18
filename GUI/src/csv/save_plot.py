from fileinput import filename
import pandas as pd
import sys
import os


def main():
    print(f"Current path: {os.getcwd()}")
    file_name = str(sys.argv[1])
    df = pd.read_csv(file_name)
    file_name = file_name.replace(".csv", "")
    df = df.dropna()
    # If there isnìt a Power column it will be created
    if 'Power' not in df.columns:
        df["Power"] = df["Voltage"] * \
            df["Current"]
    directory_name = file_name.split("_")
    if not os.path.exists(f"../plots"):
        print("Creating plots dir")
        os.mkdir(f"../plots")
    if not os.path.exists(f"../plots/{directory_name[0]}"):
        os.mkdir(f"../plots/{directory_name[0]}")
        print(f"Creating plots dir based on date {directory_name[0]}")
    if not os.path.exists(f"../plots/{directory_name[0]}/{directory_name[1]}"):
        os.mkdir(f"../plots/{directory_name[0]}/{directory_name[1]}")
        print(f"Creating plots sub dir based on time {directory_name[1]}")

    if os.path.exists(f"../plots/{directory_name[0]}/{directory_name[1]}"):
        os.chdir(f"../plots/{directory_name[0]}")

    print(f"Using path: {os.getcwd()}")
    df.plot("Time").get_figure().savefig(
        f"{directory_name[1]}/All.pdf")

    if 'Brake' in df.columns:
        df.plot("Time", "Brake").get_figure().savefig(
            f"{directory_name[1]}/Brake.pdf")
    if 'Torque' in df.columns:
        df.plot("Time", "Torque").get_figure().savefig(
            f"{directory_name[1]}/Torque.pdf")
    if 'Voltage' in df.columns:
        df.plot("Time", "Voltage").get_figure().savefig(
            f"{directory_name[1]}/Voltage.pdf")
    if 'Current' in df.columns:
        df.plot("Time", "Current").get_figure().savefig(
            f"{directory_name[1]}/Current.pdf")
    if 'Power' in df.columns:
        df.plot("Time", "Power").get_figure().savefig(
            f"{directory_name[1]}/Power.pdf")


if __name__ == "__main__":
    try:
        main()
        print("Done")
    except Exception as e:
        print(f"\nSome errors occured: {e}\r\n")
