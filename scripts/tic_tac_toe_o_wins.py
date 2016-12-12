# Original code taken here and modified for our needs:
# http://stackoverflow.com/questions/28712279/all-possible-tic-tac-toe-winning-combinations


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


pc = [' ', 'x', 'o']
c = [0] * 9
i = 0
for c[0] in range(3):
    for c[1] in range(3):
        for c[2] in range(3):
            for c[3] in range(3):
                for c[4] in range(3):
                    for c[5] in range(3):
                        for c[6] in range(3):
                            for c[7] in range(3):
                                for c[8] in range(3):
                                    countx = sum([1 for x in c if x == 1])
                                    county = sum([1 for x in c if x == 2])
                                    if abs(countx - county) < 2:
                                        if (won(c, 2) and not won(c, 1) and num_pos(c, 2) == num_pos(c, 1)):
                                            print i
                                            print " %s | %s | %s" % (pc[c[0]], pc[c[1]], pc[c[2]])
                                            print "---+---+---"
                                            print " %s | %s | %s" % (pc[c[3]], pc[c[4]], pc[c[5]])
                                            print "---+---+---"
                                            print " %s | %s | %s" % (pc[c[6]], pc[c[7]], pc[c[8]])
                                            print
                                            i += 1
