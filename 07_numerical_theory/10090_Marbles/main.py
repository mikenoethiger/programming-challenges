def euclid(a, b):
    '''Extended euclidean algorithm.
        Finds greatest common divisor (gcd) of a and b.

        Returns (g,f1,f2) where g=gcd(a,b) and f1*a + f2*b = g
    '''
    af = [1, 0]
    bf = [0, 1]
    while (a > 0) and (b > 0):
        if a < b:
            f = b // a
            bf[0] -= f * af[0]
            bf[1] -= f * af[1]
            b -= f*a
        else:
            f = a // b
            af[0] -= f * bf[0]
            af[1] -= f * bf[1]
            a -= f*b
    if a == 0:
        return b, bf[0], bf[1]
    else:
        return a, af[0], af[1]


def get_m_and_b(c1, n1, c2, n2, n, g, a0):
    '''Get slope `m` and y-intercept `b` of cost function `c(k) = m*k + b`'''
    m = (n2*c1-c2*n1)/g
    b = (a0*(n2*c1-c2*n1)+c2*n)/n2
    return m,b


def marbles(c1, n1, c2, n2, n):
    g, alpha, beta = euclid(n1, n2)
    a0 = alpha*n/g
    b0 = beta*n/g

    m, b = get_m_and_b(c1, n1, c2, n2, n, g, a0)

    return g, alpha, beta, a0, b0, m, b


def a_k(a0, k, n2, g):
    return a0+k*n2/g


def b_k(b0, k, n1, g):
    return b0-k*n1/g


def solve(c1, n1, c2, n2, n):
    if n1 == n2 and n2 == n:
        print(n1, 0)
        return

    g, alpha, beta, a0, b0, m, b = marbles(c1, n1, c2, n2, n)

    if n % g != 0:
        print("failed")
        return

    # k0 is the k for which the costs are equal to 0: C(k)=0
    k0 = -b/m
    # ak0 is the evaluated a at k0
    ak0 = a_k(a0, k0, n2, g)

    if ak0 < 0:
        # calculate k1, such that ak1 becomes the first positive (>=0) value in the a_k series
        k1 = int(-a0 * g / n2)
        if a_k(a0, k1, n2, g) < 0:
            k1 += 1
    else:
        # calculate k1, such that bk1 becomes the first positive (>=0) value in the b_k series
        k1 = int(b0 * g / n1)
        if b_k(b0, k1, n1, g) < 0:
            k1 -= 1

    ak = int(a_k(a0, k1, n2, g))
    bk = int(b_k(b0, k1, n1, g))

    if ak < 0 or bk < 0:
        print('failed')
        return

    print(ak, bk)

    # if m > 0:
    #     k0 = int(-g*a0/n2)
    # else:
    #     k0 = int(g*b0/n1)
    # print(int(a_k(a0, k0, n2, g)), int(b_k(b0, k0, n1, g)))

line = input()

while line:
    n = int(line)
    if n == 0:
        break

    c1, n1 = input().split(" ")
    c2, n2 = input().split(" ")

    solve(int(c1), int(n1), int(c2), int(n2), n)
    line = input()
