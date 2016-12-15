import fileinput


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


def append_new(l, c):
    for x in l:
        if x == c:
            return False

    l.append(list(c))

    return True


def print_lists(l_i_temp, l_o_temp):
    print "%1.1f %1.1f %1.1f %1.1f %1.1f %1.1f %1.1f %1.1f %1.1f   %1.1f %1.1f %1.1f %1.1f %1.1f %1.1f %1.1f %1.1f %1.1f" % (l_i_temp[0], l_i_temp[1], l_i_temp[2],
                                                                                                                             l_i_temp[3], l_i_temp[4], l_i_temp[5],
                                                                                                                             l_i_temp[6], l_i_temp[7], l_i_temp[8],
                                                                                                                             l_o_temp[0], l_o_temp[1], l_o_temp[2],
                                                                                                                             l_o_temp[3], l_o_temp[4], l_o_temp[5],
                                                                                                                             l_o_temp[6], l_o_temp[7], l_o_temp[8])


l_input = []
l_output = []
l_input_unique = []

for line in fileinput.input():
    l_i_temp = []
    l_o_temp = []
    l_temp = l_i_temp
    i = 0
    for num in line.split():
        l_temp.append(float(num))
        if i == 8:
            l_temp = l_o_temp
        i += 1

    l_input.append(l_i_temp)
    l_output.append(l_o_temp)

for i in range(len(l_input)):
    l_i_temp = l_input[i]
    l_o_temp = l_output[i]

    if (append_new(l_input_unique, l_i_temp)):
        print_lists(l_i_temp, l_o_temp)

    l_i_temp_2 = rotate180(l_i_temp)
    l_o_temp_2 = rotate180(l_o_temp)
    if (append_new(l_input_unique, l_i_temp_2)):
        print_lists(l_i_temp_2, l_o_temp_2)

    # l_i_temp_2 = rotate90(l_i_temp)
    # l_o_temp_2 = rotate90(l_o_temp)
    # if (append_new(l_input_unique, l_i_temp_2)):
    #     print_lists(l_i_temp_2, l_o_temp_2)

    # l_i_temp_2 = rotate270(l_i_temp)
    # l_o_temp_2 = rotate270(l_o_temp)
    # if (append_new(l_input_unique, l_i_temp_2)):
    #     print_lists(l_i_temp_2, l_o_temp_2)

    # l_i_temp_2 = swap_asym(l_i_temp)
    # l_o_temp_2 = swap_asym(l_o_temp)
    # if (append_new(l_input_unique, l_i_temp_2)):
    #     print_lists(l_i_temp_2, l_o_temp_2)

    # l_i_temp_2 = swap_asym(rotate180(l_i_temp))
    # l_o_temp_2 = swap_asym(rotate180(l_o_temp))
    # if (append_new(l_input_unique, l_i_temp_2)):
    #     print_lists(l_i_temp_2, l_o_temp_2)

    # l_i_temp_2 = swap_asym(l_i_temp)
    # l_o_temp_2 = swap_asym(l_o_temp)
    # if (append_new(l_input_unique, l_i_temp_2)):
    #     print_lists(l_i_temp_2, l_o_temp_2)

    # l_i_temp_2 = mirror_v(l_i_temp)
    # l_o_temp_2 = mirror_h(l_o_temp)
    # if (append_new(l_input_unique, l_i_temp_2)):
    #     print_lists(l_i_temp_2, l_o_temp_2)
