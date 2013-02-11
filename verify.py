from sys import argv
fn = argv[1] if len(argv) > 1 else '/dev/shm/read.log'
with open(fn) as f: ls = f.readlines()
ll = ls[-1]
ls = ls[6:-1]
js = [int(l.split(',')[0]) for l in ls]
diff = sorted(set(range(max(js) + 1)) - set(js))

print ll
if len(diff) > 100:
    print "First 100 missing:", diff[:100]
else:
    print "Missing:", diff
    
