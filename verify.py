with open('/dev/shm/read.log') as f: ls = f.readlines()
ll = ls[-1]
ls = ls[5:-1]
js = [int(l.split(',')[0]) for l in ls]
diff = set(range(max(js) + 1)) - set(js)

print ll
print "First 100 missing:", diff
