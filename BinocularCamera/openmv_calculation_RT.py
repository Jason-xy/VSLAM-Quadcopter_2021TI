
import numpy as np
import math
'''
内参
[fx  0  cx
 0  fy  cy
 0   0  1]
畸变参数
[k1, k2, k3, p1, p2]
'''
#相机内参
mtx = np.array([[386.86109179, 0., 151.45202906],
                [0., 385.56118791, 115.53383815],
                [0., 0., 1.]])

dist = np.array([-2.06745379e-01, -3.00545927e+00, 2.89347883e-04, 1.03638614e-03, 1.33154755e+01])

R=np.array( [[1.12903681e-01,5.87411076e-01,8.01374436e-01],
 [ 1.50978505e-01,7.87023829e-0,-5.98163008e-01],
 [-9.82068353e-01,1.88525120e-01,1.71441091e-04]])


T=np.array([[-0.19835846],
            [-1.03067159],
            [1.76784233]])



#计算像素点坐标对应的世界坐标
def uv2XY(u, v):
    print("旋转矩阵的逆矩阵：", R, "\n平移矩阵：", T)
    t = T[0]*R[2][0]+T[1]*R[2][1]+T[2]*R[2][2]
    ux = (u-mtx[0][2])/(mtx[0][0]*R[2][2])
    vy = (v-mtx[1][2])/(mtx[1][1]*R[2][2])
    x = t/(1/ux+R[2][0]+R[2][1]*vy/ux)
    y = t/(1/vy+R[2][0]*ux/vy+R[2][1])
    X = (R[0][0]-R[0][2]*R[2][0]/R[2][2])*(x-T[0])+(R[0][1]-R[0][2]*R[2][1]/R[2][2])*(y-T[1])
    Y = (R[1][0]-R[1][2]*R[2][0]/R[2][2])*(x-T[0])+(R[1][1]-R[1][2]*R[2][1]/R[2][2])*(y-T[1])
    return X, Y


