#!/usr/bin/python2.7
import pykst as kst
from numpy import *
from PyQt4 import QtCore, QtNetwork, QtGui

client = kst.Client()
print("kst.Client()")

colors = ["#00FFCC","#99FF00","#33EE33","#66CC33","#006600","#006666",
          "#FFFF00","#FF9900","#FF6600","#FF3300","#CC0000","#990066"]

f = open("pat","r")
f.readline()

s = f.readline().split(" ")

xvec = kst.EditableVector(client)
xvec.setFromList(array([float(s[1]),float(s[1])+float(s[2])]))
yvec = kst.EditableVector(client)
yvec.setFromList(array([float(s[2]),float(s[2])]))

eq = kst.NewCurve(client,xvec,yvec,False,False,False,False,False,False,colors[0],0,2,True,False,"","",False,"","",False,"","",False,False,True,False,True,True,False)

for line in f:
  s = line.split(" ")

  xvec = kst.EditableVector(client)
  xvec.setFromList(array([float(s[1]),float(s[1])+float(s[2])]))
  yvec = kst.EditableVector(client)
  yvec.setFromList(array([float(s[2]),float(s[2])]))

  eqvec = kst.NewCurve(client,xvec,yvec,False,False,False,False,False,False,colors[int(s[0])-1],0,2,True,False,"","",False,"","",False,"","",False,eq,False,False,True,True,False)
