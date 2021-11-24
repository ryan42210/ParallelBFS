g++ main.cpp -o main -fopenmp -O2
./main edge_graph/web-Stanford.txt >./result/web_res.txt
./main edge_graph/soc-LiveJournal1.txt >./result/soc_res.txt
./main edge_graph/roadNet-CA.txt >./result/road_res.txt
./main edge_graph/com-orkut.txt >./result/com_res.txt

./main edge_graph/rmat1.txt -h >./result/rmat1_res.txt
./main edge_graph/rmat2.txt -h >./result/rmat2_res.txt
./main edge_graph/rmat3.txt -h >./result/rmat3_res.txt