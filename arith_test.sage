# ---------------------------------------------------
# Arith Function Test Sage Code
# ---------------------------------------------------
# date: @2023.11.01
# author: minji0022@kookmin.ac.kr
# ------------------- *Sage Code* -------------------
# Online Sage Inteface: https://sagecell.sagemath.org/
# ---------------------------------------------------

w=8; W=2^w

a = ZZ.random_element(W^10, W^20)
b = ZZ.random_element(W^10, W^20)

print("A = {}".format(hex(a)))
print("B = {}".format(hex(b)))

c = a+b
d = a-b

print("A+B = {}".format(hex(c)))
print("A-B = {}".format(hex(d)))