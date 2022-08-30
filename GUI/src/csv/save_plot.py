import pandas as pd
import sys
import os


def main():
    print(f"Current path: {os.getcwd()}")
    # If keyword last is detected the last csv file added will be read
    if str(sys.argv[1]) == "last":
        # Get the list of all the csv file in the directory
        files = os.listdir()
        # Remove save_plot.py from the list
        files.remove(sys.argv[0])
        files.sort()
        file_name = (files[-1])
    # Otherwise the file name given will be selected
    else:
        file_name = str(sys.argv[1])
    df = pd.read_csv(file_name)
    initial_time = df.iloc[0]['Time [ms]']
    # if time is relative then convert to absolute
    if initial_time != 0:
        df['Time [ms]'] -= df.iloc[0]['Time [ms]']
    file_name = file_name.replace(".csv", "")
    df = df.dropna()
    # If there isn't a Power column it will be created
    if 'Power [W]' not in df.columns:
        df["Power [W]"] = df["Voltage [V]"] * \
            df["Current [A]"]
    # Split the directories name in DD_MM_YYYY and HH_MM_SS from the original file name
    directory_name = file_name.split("_")
    # Check whether plots folder exists, if not one will be created
    if not os.path.exists(f"../plots"):
        print("Creating plots dir")
        os.mkdir(f"../plots")
    # Check whether plot/directory_name[0] folder exists, if not one will be created
    if not os.path.exists(f"../plots/{directory_name[0]}"):
        os.mkdir(f"../plots/{directory_name[0]}")
        print(f"Creating plots dir based on date {directory_name[0]}")
    # Check whether plot/directory_name[0]/directory_name[1] folder exists, if not one will be created
    if not os.path.exists(f"../plots/{directory_name[0]}/{directory_name[1]}"):
        os.mkdir(f"../plots/{directory_name[0]}/{directory_name[1]}")
        print(f"Creating plots sub dir based on time {directory_name[1]}")
    # Whether plot/directory_name[0]/directory_name[1] exists it will be selected as current work path
    if os.path.exists(f"../plots/{directory_name[0]}/{directory_name[1]}"):
        os.chdir(f"../plots/{directory_name[0]}")
    print(f"Using path: {os.getcwd()}")
    # Save all sensor values in the same plot (using Time as x-axis)
    df.plot("Time [ms]").get_figure().savefig(
        f"{directory_name[1]}/All.pdf")
    # Plot and save separate values during Time
    # eg: Brake.pdf, Current.pdf etc
    for i in df.columns:
        if i != "Time [ms]" and i != "Sender":
            df.plot("Time [ms]", f"{i}").get_figure().savefig(
                f"{directory_name[1]}/{i}.pdf")
            df.plot("Time [ms]", f"{i}").get_figure().savefig(
                f"{directory_name[1]}/{i}.png", format="png", dpi=300)


if __name__ == "__main__":
    try:
        main()
        print("Done")
    except Exception as e:
        print(f"\nSome errors occured: {e}\r\n")
