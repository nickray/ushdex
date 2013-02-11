with open('/dev/shm/read.log') as f: ls = f.readlines()
ll = ls[-1]
ls = ls[5:-1]
js = [int(l.split(',')[0]) for l in ls]
diff = sorted(set(range(max(js) + 1)) - set(js))

print ll
if len(diff) > 100:
    print "First 100 missing:", diff
else:
    print "Missing:", diff
    
