# Original code taken here and modified for our needs:
# http://stackoverflow.com/questions/28712279/all-possible-tic-tac-toe-winning-combinations

unique_only = True


def won(c, n):
    if c[0] == n and c[1] == n and c[2] == n:
        return True
    if c[3] == n and c[4] == n and c[5] == n:
        return True
    if c[6] == n and c[7] == n and c[8] == n:
        return True

    if c[0] == n and c[3] == n and c[6] == n:
        return True
    if c[1] == n and c[4] == n and c[7] == n:
        return True
    if c[2] == n and c[5] == n and c[8] == n:
        return True

    if c[0] == n and c[4] == n and c[8] == n:
        return True
    if c[2] == n and c[4] == n and c[6] == n:
        return True

    return False


def num_pos(c, n):
    total = 0
    for i in range(9):
        if c[i] == n:
            total += 1

    return total


def rotate180(c):
    return list(reversed(c))


def rotate90(c):
    x = list(c)
    x[0] = c[6]
    x[1] = c[3]
    x[2] = c[0]
    x[3] = c[7]
    x[4] = c[4]
    x[5] = c[1]
    x[6] = c[8]
    x[7] = c[5]
    x[8] = c[2]

    return x


def rotate270(c):
    return rotate180(rotate90(c))


def swap_asym(c):
    x = list(c)
    x[0] = c[0]
    x[1] = c[3]
    x[2] = c[6]
    x[3] = c[1]
    x[4] = c[4]
    x[5] = c[7]
    x[6] = c[2]
    x[7] = c[5]
    x[8] = c[8]

    return x


def mirror_h(c):
    x = list(c)
    x[0] = c[2]
    x[1] = c[1]
    x[2] = c[0]
    x[3] = c[5]
    x[4] = c[4]
    x[5] = c[3]
    x[6] = c[8]
    x[7] = c[7]
    x[8] = c[6]

    return x


def mirror_v(c):
    return swap_asym(rotate90(c))


def append_unique(l, c):
    for x in l:
        if (x == rotate180(c)):
            return
        if (x == rotate90(c)):
            return
        if (x == rotate270(c)):
            return
        if (x == swap_asym(c)):
            return
        if (x == swap_asym(rotate180(c))):
            return
        if (x == mirror_v(c)):
            return
        if (x == mirror_h(c)):
            return

    l.append(list(c))


l = []
num = ['0.5', '1.0', '0.0']
pc = [' ', 'x', 'o']
c = [0] * 9
for c[0] in range(3):
    for c[1] in range(3):
        for c[2] in range(3):
            for c[3] in range(3):
                for c[4] in range(3):
                    for c[5] in range(3):
                        for c[6] in range(3):
                            for c[7] in range(3):
                                for c[8] in range(3):
                                    if (won(c, 2) and not won(c, 1) and num_pos(c, 2) == num_pos(c, 1)):
                                        if unique_only:
                                            append_unique(l, c)
                                        else:
                                            l.append(list(c))

i = 0
for x in l:
    print "%d: %s %s %s %s %s %s %s %s %s" % (
        i, num[x[0]], num[x[1]], num[x[2]],
        num[x[3]], num[x[4]], num[x[5]],
        num[x[6]], num[x[7]], num[x[8]])
    print " %s | %s | %s" % (pc[x[0]], pc[x[1]], pc[x[2]])
    print "---+---+---"
    print " %s | %s | %s" % (pc[x[3]], pc[x[4]], pc[x[5]])
    print "---+---+---"
    print " %s | %s | %s" % (pc[x[6]], pc[x[7]], pc[x[8]])
    print
    i += 1
