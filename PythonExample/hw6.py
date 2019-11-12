#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Dec  5 18:04:57 2017

@author: joshuacanuel
"""

import numpy as np

def trap(n,func,a,b):
    total = 0
    incr = (b-a)/n
    a2 = a+incr
    for i in range(0,n):
        total = total + ((func(a)+func(a2))/2)*incr
        a = a2
        a2 = a2+incr
    print(total)
    
def simpsons(n,func,a,b):
    total = 0
    incr = (b-a)/n
    a2 = a+incr
    for i in range(0,n):
        total += (1/6)*((func(a)+4*func((a+a2)/2)+func(a2)))*incr
        a = a2
        a2 = a2+incr
    print(total)

def f1(x):
    return x*np.e**x**2
def f2(x):
    return x*np.sin(x**2)
def f3(x):
    return np.sin(x)*(np.e**np.cos(x))

print("trapezoidal")
trap(16,f1,0,1)
#0.8614679071317117
trap(32,f1,0,1)
#0.8597230482302871
print()
trap(16,f2,0,np.pi)
#0.891007482241
trap(32,f2,0,np.pi)
#0.93658211207
print()
trap(16,f3,0,1)
#1.00077717946
trap(32,f3,0,1)
#1.00151145671
print()
print("simpsons")
simpsons(16,f1,0,1)
#0.859141428596479
simpsons(32,f1,0,1)
#0.8591409464298769
print()
simpsons(16,f2,0,np.pi)
#0.951773655347
simpsons(32,f2,0,np.pi)
#0.951368613319
print()
simpsons(16,f3,0,1)
#1.0017562158
simpsons(32,f3,0,1)
#1.00175613434

#Exact for xe^(x^2)
#0.859140914229523

#Exact for xsin(x^2)
#0.95134268096654

#Exact for sin(x)e^(cos(x))
#1.00175612891014