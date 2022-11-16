import json
import time
from pymongo import MongoClient
  

timestr = time.strftime("%Y%m%d-%H%M%S") #Name of the json file
json_name = timestr + ".json"


with open("movements.csv", "r") as read_file, open("robot.csv","w") as write_file, open(json_name,"w") as outfile:
        
    flag = 0 # This flag is used to ignore 0 coordinates                
    robot_dict = []     #List of dictionaries to upload to mongodb
    
    r2 = {} 
    
    line = read_file.read().splitlines()
    write_file.write('SPEED,100' + "\n")
    
    for caracter in line:
        command = caracter.split(',')

        s0 = command[0]
        s1 = command[1]
            
        if command[2] == "1":
            flag = 1
            
        elif command[2] == "0" and flag == 1:    
            write_file.write("JAPPRO" + "," + s0+ "," + s1 + "," + "0" + "\n")
            r2["cmd"] = "JAPPRO"
            r2["x"] = s0
            r2["y"] = s1
            r2["z"] = "0"
            robot_dict.append(r2)  
            flag = 0
            
            
        elif command[2] == "0" and flag == 0:
            write_file.write("JMOVE" + "," + s0+ "," + s1 + "," + "10" + "\n")
            r2["cmd"] = "JMOVE"
            r2["x"] = s0
            r2["y"] = s1
            r2["z"] = "10"
            robot_dict.append(r2)
            
        r2={}
    write_file.write("END")

    json.dump(robot_dict,outfile)   #Create the json file with the dictionaries

# Making Connection
myclient = MongoClient("mongodb+srv://<user>:<password>@cluster0.3dssseg.mongodb.net/?retryWrites=true&w=majority")
  
# database
db = myclient["robot"]
  
# collection
Collection = db[timestr]
 
# Loading the json file
with open(json_name,"r") as file:
    file_data = json.load(file)


Collection.insert_many(file_data) 






