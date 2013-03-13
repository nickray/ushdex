from numpy import diff
from sys import argv

inf = float('inf')

def replace(l, sgn, lax=False):
    if lax and 0. in l:
        l = l[:l.index(0.)]

    if sgn > 0:
        return [x if x != 0. else +inf for x in l]
    elif sgn < 0:
        return [x if x != 0. else -inf for x in l]
    else:
        return l

def assrt(s, g, l):
    print s, eval(s, g, l)
    assert(eval(s, g, l))

def check(s, lax=False):
    # Example:
    # "130313.095532.192580,18646645,0,5,0," +
    # "13028,0x1.972p+13,13029,0x1.9728p+13,1,47,...13025,0x1.9708p+13,13032,0x1.974p+13,169,134,0,0x0p+0,13033,0x1.9748p+13,0,1361363164932192670464 ERROR: Book entry (asks) out of order at index 4"
    split = s.split(',')
    N, n = map(int, split[3:5])
    #print split

    print "::", N, "bids and asks"
    split = split[5:]
    bids = replace(
        map(float.fromhex, [split[i] for i in range(1, 1 + N*6, 6)]),
        -1, lax)
    asks = replace(
        map(float.fromhex, [split[i] for i in range(3, 3 + N*6, 6)]),
        +1, lax)

    print bids
    print asks

    assrt("(diff(bids) < 0).all()", globals(), locals())
    assrt("(diff(asks) > 0).all()", globals(), locals())

    if n > 0:
        print "::", n, "implied bids and asks"
        split = split[6*N:]
        implied_bids = replace(
            map(float.fromhex, [split[i] for i in range(1, 1 + n*6, 6)]),
            -1, lax)
        implied_asks = replace(
            map(float.fromhex, [split[i] for i in range(3, 3 + n*6, 6)]),
            +1, lax)

        print implied_bids
        print implied_asks

        assrt("(diff(implied_bids) < 0).all()", globals(), locals())
        assrt("(diff(implied_asks) > 0).all()", globals(), locals())

def test_file(fn, lax=False):
    ls = [l for l in open(fn).readlines() if 'ERROR' in l]
    for l in ls: print; check(l, lax)

if __name__ == '__main__':
    fn = argv[1] if len(argv) == 2 else 'globconnect.log'
    test_file('globconnect.log', True)

