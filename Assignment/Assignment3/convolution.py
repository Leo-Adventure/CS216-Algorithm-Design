import numpy as np

def convolution(v1, v2):
    len1 = len(v1)
    len2 = len(v2)

    if len2 > len1:
        tmp = v1
        v1 = v2
        v2 = tmp
        len1 = len(v1)
        len2 = len(v2)
    print(len1)
    print(len2)

    res = []
    dictionary = dict()
    for i in range(len1 + len2 - 1):
        dictionary[i] = 0
    for i in range(len1):
        for j in range(len2):
            dictionary[i + j] = dictionary[i + j] + v1[i]*v2[j]

    for i in range(len1 + len2 - 1):
        res.append((dictionary[i]))
    return res

def FFT(v1, v2):
    len1 = len(v1)
    len2 = len(v2)
    n = len1 + len2 - 1
    V1 = np.fft.fft(v1, n)
    V2 = np.fft.fft(v2, n)
    # print(V1)
    # print(V2)
    V3 =  V1 * V2
    res = np.fft.ifft(V3)[:n]
    return res





if __name__ == '__main__':
    v1 = [2, 3, 4, 5, 6]
    v2 = [1, 4, 7, 10]
    res = np.convolve(v1, v2) # 作为验证

    print(convolution(v1, v2))
    print(FFT(v1, v2))
    print(res)
