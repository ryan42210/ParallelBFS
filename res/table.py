filename = ["web", "road", "com", "soc", "rmat1", "rmat2", "rmat3"]

table = []
for i in range(32):
    th_line = str(i+1) + " & "
    table.append(th_line)

for i, name in enumerate(filename):
    full_name = "./format/" + name + "_res.txt"
    mteps = ""
    stddv = ""
    with open(full_name, 'r') as f:
        content = f.readlines()
        mteps_str = content[1]
        stddv_str = content[2]

    mteps_l =  mteps_str.split()
    dv_l =  stddv_str.split()
    for k in range(32):
        table[k] += mteps_l[k] + '/' + dv_l[k]
        if (i != 6):
            table[k] += " & "


    
for i in range(32):
    table[i] += "\\\\\n"

with open("output.txt", "w") as f:
    f.writelines(table)