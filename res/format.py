filename_list = ["com", "rmat1", "rmat2", "rmat3", "road", "soc", "web"]

for file in filename_list:
    filename = file + "_res.txt"
    with open(filename, "r") as f:
        content = f.readlines()

    sequencial = content[2]
    seq_list = sequencial.split("|")
    seq_str = seq_list[1].strip() + " " + seq_list[2].strip() + "\n"

    mteps_str = ""
    std_dv_str = ""

    for i in range(32):
        mteps = 0
        std_dv = 0
        for j in range(3):
            line = content[3 + i*5 + j + 2]
            data = line.split('|')
            mteps += float(data[1].strip())
            std_dv += float(data[2].strip())

        mteps /= 3
        std_dv /= 3
        mteps_str += str(round(mteps, 2)) + " "
        std_dv_str += str(round(std_dv, 2)) + " "

    mteps_str += "\n"
    std_dv_str += "\n"


    write_list = [seq_str, mteps_str, std_dv_str]

    with open("./format/" + filename, "w") as out:
        out.writelines(write_list)
