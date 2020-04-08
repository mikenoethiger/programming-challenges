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


def a_k(a0, k, n2, g):
    return a0+k*n2/g


def b_k(b0, k, n1, g):
    return b0-k*n1/g


def solve(c1, n1, c2, n2, n):
    if n1 == n2 and n2 == n:
        print(n1, 0)
        return

    g, alpha, beta = euclid(n1, n2)

    if n % g != 0:
        print("failed")
        return

    # (a0,b0) := initial (but most likely not best) solution for a*n1 + b*n2 = n
    #            new solutions can be generated with the following relation:
    #            a_k = a0 + (k*n2)/g
    #            b_k = b0 - (k*n1)/g
    # m := slope of the cost function in relation to k: c(k)
    #      the initial cost function was c = a*c1 + b*c2
    #      b can be expressed as a, by transforming the other equation a*n1 + b*n2 = n
    #      this leads us to the cost function c(a) = a*(c1*n2-c2n1+c2n)/n2
    #      a can be expressed in terms of k by using the a_k equation above.
    #      this will give us the final c(k) = k*m + b function where m is the slope of this function.
    a0 = alpha * n / g
    b0 = beta * n / g
    m = (n2 * c1 - c2 * n1) / g

    # m > 0:
    #   - Costs decrease, as k decreases
    #   - a_k decreases, as k decreases
    #   - c(k) becomes minimal at first positive value of the a_k series
    # m < 0:
    #   - Costs decrease, as k increases
    #   - b_k decreases, as k increases
    #   - c(k) becomes minimal at first positive value of the b_k series
    if m > 0:
        # calculate k1, such that ak1 becomes the first positive (>=0) value in the a_k series
        k0 = int(-a0 * g / n2)
        # check if above calculation rounded wrong and shift k0 if necessary
        if a_k(a0, k0, n2, g) < 0:
            k0 += 1
    else:
        # calculate k1, such that bk1 becomes the first positive (>=0) value in the b_k series
        k0 = int(b0 * g / n1)
        # check if above calculation rounded wrong and shift k0 if necessary
        if b_k(b0, k0, n1, g) < 0:
            k0 -= 1

    ak = int(a_k(a0, k0, n2, g))
    bk = int(b_k(b0, k0, n1, g))

    # if now one of the components is still negative there doesn't exist a solution
    if ak < 0 or bk < 0:
        print('failed')
        return

    print(ak, bk)


line = input()

while line:
    n = int(line)
    if n == 0:
        break

    c1, n1 = input().split(" ")
    c2, n2 = input().split(" ")

    solve(int(c1), int(n1), int(c2), int(n2), n)
    line = input()
