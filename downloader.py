import pandas as pd
import csv
from pymongo import MongoClient
 
to_fetch = str(input("Please select the figure you want to download: "))

# Making Connection
myclient = MongoClient("mongodb+srv://<user>:<password>@cluster0.3dssseg.mongodb.net/?retryWrites=true&w=majority")

# database
db = myclient["robot"]
  
# collection
collection = db[to_fetch]

df = pd.DataFrame(list(collection.find()), columns = None)
column_list = ["cmd", "x", "y", "z"]


with open('movements.csv','w') as file:
    file.write("SPEED,100" + "\n")
    
df[column_list].to_csv('movements.csv', mode = 'a', index = False, header = False)


with open('movements.csv','a') as file_a:
    file_a.write("END")
