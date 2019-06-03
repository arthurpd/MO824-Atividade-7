import subprocess


for x in ['ffd', 'nfd', 'bfd']:
	subprocess.run(f'g++ -O2 -o {x} {x}.cpp'.split())
	for i in range(10):
		subprocess.run(f"./{x}".split(), stdin=open(f'../bpp_instances/instance{i}.bpp', 'r'), stdout=open(f'log{x}-{i}.txt', 'w'))
