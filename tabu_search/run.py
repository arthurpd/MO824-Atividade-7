import subprocess

x = 0
for i in range(10):
	subprocess.run(f"./a.out 600 {x}".split(), stdin=open(f'../bpp_instances/instance{i}.bpp', 'r'), stdout=open(f'log{x}-{i}.txt', 'w'))
