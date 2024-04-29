import pandas as pd
from collections import Counter

def read_csv_column_by_column(file_path):
    # Load the CSV file into a DataFrame
    df = pd.read_csv(file_path)
    
    # Iterate through each column in the DataFrame
    for column in df.columns:
        # Access the column data
        column_data = df[column]
        print(f"Column: {column}")
        print(column_data)
        print()


def count_flags_in_column(file_path, column_name):
    # Load the CSV file into a DataFrame
    df = pd.read_csv(file_path)
    
    # Check if the column exists in the DataFrame
    if column_name not in df.columns:
        print(f"Column {column_name} does not exist in the CSV file.")
        return
    
    # Concatenate all rows in the column into a single string
    # dropna() to avoid errors with NaN values
    all_flags = ' '.join(df[column_name].dropna())  
    
    # Split the string into a list of flags
    flags_list = all_flags.split()
    
    # Count occurrences of each flag using Counter
    flag_counts = Counter(flags_list)
    
    # Print the counts of each flag
    for flag, count in flag_counts.items():
        print(f"{flag}: {count}")


# path to CSV file
file_path = 'results.csv'

# this will print out the data column by column
read_csv_column_by_column(file_path)

# the name of column that contains the flags
column_name = 'Flags Used'  

# this will only look at flags 
count_flags_in_column(file_path, column_name)
