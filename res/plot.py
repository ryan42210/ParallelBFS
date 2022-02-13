import matplotlib.pyplot as plt

head = ["com", "rmat1", "rmat2", "rmat3", "road", "soc", "web"]
full_name = ["com-orkut", "RMAT1", "RMAT2", "RMAT3", "roadNet-CA", "soc-LiveJournal1", "web-Stanford"]
marker = ['o', 'v', '*', '+', 'x', '^', '1']

th_num = [i for i in range(1, 33)]

for i in range(7):
    filename = "./format/" + head[i] + "_res.txt"
    mteps = []
    with open(filename, "r") as f:
        content = f.readlines()
        mteps = content[1].split()

    mteps_num = []
    for j in mteps:
        mteps_num.append(float(j))

    plt.plot(th_num, [f/mteps_num[0] for f in mteps_num], marker=marker[i], label=full_name[i])

plt.plot(th_num[:16], th_num[:16], linestyle='--', label="linear")

plt.legend()

# plt.title("Benchmark result on 7 graphs")
plt.xlabel("thread number")
plt.ylabel("Speedup")

plt.xticks([2*(i+1) for i in range(16)])

# plt.savefig("benchmark.svg")
plt.show()