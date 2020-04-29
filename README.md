# CSV-Reader
Reads in a list of building assets with their quantity and compares them with a DIN276 list. If an equal asset is found it adds the quantity to the latter list.

Important in this first version is, that both files are provided in a certain format (Dataformat = CSV). 
For the asset list: - first line description
                    - first row = Asset name, third column = asset quantity
DIN276 norm: - first line description
             - first column Asset name
